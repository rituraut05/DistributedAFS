#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <unordered_map>
#include <grpcpp/grpcpp.h>
#include <string>
#include <chrono>
#include <ctime>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <utime.h>
#include <fstream>
#include <sstream>

//#include <fuse.h>
#include "afs.grpc.pb.h"
#include "afs_client.hh"
//#include <unreliablefs_ops.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using grpc::StatusCode;
using std::cout;
using std::endl;
using std::string;
using afs::DirectoryEntry;
using afs::PingMessage;
using afs::FetchRequest;
using afs::FetchResponse;
using afs::FileMode;
using afs::FileStat;
using afs::FileSystemService;
using afs::GetFileStatRequest;
using afs::GetFileStatResponse;
using afs::AccessRequest;
using afs::AccessResponse;
using afs::ListDirRequest;
using afs::ListDirResponse;
using afs::MakeDirRequest;
using afs::MakeDirResponse;
using afs::RemoveDirRequest;
using afs::RemoveDirResponse;
using afs::RemoveRequest;
using afs::RemoveResponse;
using afs::StoreRequest;
using afs::StoreResponse;
using afs::TestAuthRequest;
using afs::TestAuthResponse;
using afs::Timestamp;
using std::vector;
using afs::CreateRequest;
using afs::CreateResponse;
using afs::RenameRequest;
using afs::RenameResponse;

#define DEBUG                 1                                     // for debugging
#define debugprintf(...)        if (DEBUG) { printf(__VA_ARGS__); }   // for debugging
#define RETRY_TIME_START      1                                     // in seconds
#define RETRY_TIME_MULTIPLIER 2                                     // for rpc 
#define MAX_RETRIES           5                                     // rpc retry
#define CHUNK_SIZE            4096																	// for streaming 

enum fuse_readdir_flags {
         FUSE_READDIR_PLUS = (1 << 0),
};

enum fuse_fill_dir_flags {
         FUSE_FILL_DIR_PLUS = (1 << 1),
};

string get_relative_path(string path, string root) {
	string path_copy = path;
	path_copy.erase(0, root.length());
	return path_copy;
}

bool file_exists(std::string path) {
	struct stat s;   
	return (stat(path.c_str(), &s) == 0); 
}

int GetModifyTime(std::string path, timespec * t) {
	debugprintf("[GetModifyTime]: Function entered.\n");
	struct stat sb;
	if (stat(path.c_str(), &sb) == -1) {
		debugprintf("[GetModifyTime]: Failed to stat file.\n");
		return -1;
	}
	debugprintf("[GetModifyTime]: Function ended.\n");
	*t = sb.st_mtim;
	return 0;
}

int set_file_open_time(int fd, timespec t) {
	struct timespec p[2] = {t,t};
	return futimens(fd,p);
}

int set_timings(string path, timespec t) {
	struct timespec p[2] = {t,t};
	return utimensat(AT_FDCWD, path.c_str(), p, 0);
}

vector<string> tokenize_path(string path, char delim, bool is_file) {
	vector<string> tokens;
	string temp = "";
	
	for(int i = 0; i < path.length(); i++)
	{
		if(path[i] == delim){
			tokens.push_back(temp);
			temp = "";
		}
		else
			temp = temp + (path.c_str())[i];           
	}

	// if path is dir not file, create all dirs in path
	if (!is_file)
		tokens.push_back(temp);

	return tokens;
}

int create_path(string relative_path, bool is_file, string root) {
	debugprintf("[create_path]: Function entered.\n");
	string base_path = root;
	vector<string> tokens = tokenize_path(relative_path, '/', is_file);
	for (auto token : tokens)
	{
		base_path += token + "/";
		struct stat s;
		int r = stat(base_path.c_str(), &s);
		if (r != 0 && errno == 2) 
		{
				debugprintf("[create_path]: stat() ENOENT\n");
				if (mkdir(base_path.c_str(), S_IRWXU) != 0)
				{
						debugprintf("[create_path]: mkdir() failed : %d\n", errno);
						return -1;
				}
		}
		else if (r != 0)
		{
				debugprintf("[create_path]: stat() failed : %d\n", errno);
				return -1;
		}		
	}
	debugprintf("[create_path]: Function ended.\n");
	return 0;
}

string readFileIntoString(string path) {
	ifstream input_file(path);
	if (!input_file.is_open()) 
	{
		debugprintf("readFileIntoString(): failed\n");
		return string();
	}
	return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

int transform_rpc_err(grpc::StatusCode code) {
switch(code) {
	case StatusCode::OK:
		return 0;
	case StatusCode::INVALID_ARGUMENT:
		return EINVAL;
	case StatusCode::NOT_FOUND:
		return ENOENT;
	case StatusCode::DEADLINE_EXCEEDED:
	case StatusCode::UNAVAILABLE:
		return ETIMEDOUT;
	case StatusCode::ALREADY_EXISTS:
		return EEXIST;
	case StatusCode::PERMISSION_DENIED:
	case StatusCode::UNAUTHENTICATED:
		return EPERM;
	default:
		return EIO;
	}
}

extern "C" {
	FileSystemClient::FileSystemClient(std::shared_ptr<Channel> channel)
		: stub_(FileSystemService::NewStub(channel)) {
			open_map.clear();
			std::cout << "-------------- Helloo --------------" << std::endl;
	}

  int FileSystemClient::Ping(int * round_trip_time) {
			auto start = std::chrono::steady_clock::now();
			
			PingMessage request;
			PingMessage reply;
			Status status;
			uint32_t retryCount = 0;
			
			// Retry w backoff
			do 
			{
				ClientContext context;
				printf("Ping: Invoking RPC\n");
				sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
				status = stub_->Ping(&context, request, &reply);
				retryCount++;
			} while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE);

			// Checking RPC Status
			if (status.ok()) 
			{
				printf("Ping: RPC Success\n");
				auto end = std::chrono::steady_clock::now();
				std::chrono::nanoseconds ns = end-start;
				*round_trip_time = ns.count();
				#if DEBUG
				std::chrono::duration<double,std::ratio<1,1>> seconds = end-start;
				printf("Ping: Exiting function (took %fs)\n",seconds.count());
				#endif
				return 0;
			}
			else
			{
				printf("Ping: RPC failure\n");
				printf("Ping: Exiting function\n");
				return -1;
			}
    }

	int FileSystemClient::GetFileStat(std::string abs_path, struct stat *buf, std::string root) {
			debugprintf("[GetFileStat]: Function entered.\n");
			GetFileStatRequest req;
			GetFileStatResponse resp;
			Status status;
			int retryCount = 0;
			std::string path = get_relative_path(abs_path, root);

			auto test_auth_result = TestAuth(abs_path, root);
			if(!test_auth_result.status.ok() || test_auth_result.response.file_changed()) {
				// Make RPC & retry
				req.set_pathname(path);
				do {
					debugprintf("[GetFileStat]: Invoking GetFileStat RPC.\n");
					ClientContext context;
					resp.Clear();
					sleep(retryCount * RETRY_TIME_START * RETRY_TIME_MULTIPLIER);
					retryCount++;
					status = stub_->GetFileStat(&context, req, &resp);
				} while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE);

				// Checking RPC Status
				if (status.ok()) 
				{
					debugprintf("[GetFileStat]: GetFileStat RPC success.\n");
					int server_errno = resp.fs_errno();
					if(server_errno) {
						debugprintf("[GetFileStat]: Error %d on server.\n", server_errno);
						debugprintf("[GetFileStat]: Function ended due to server failure.\n"); 
						errno = server_errno;
						return -1;
					}
					
					buf->st_ino = resp.stat().ino();
					buf->st_mode = resp.stat().mode();
					buf->st_nlink = resp.stat().nlink();
					buf->st_uid = resp.stat().uid();
					buf->st_gid = resp.stat().gid();
					buf->st_size = resp.stat().size();
					buf->st_blksize = resp.stat().blksize();
					buf->st_blocks = resp.stat().blocks();
					buf->st_atime = resp.stat().atime();
					buf->st_mtime = resp.stat().mtime();
					buf->st_ctime = resp.stat().ctime();
					debugprintf("[GetFileStat]: Function ended; ino = %d\n", buf->st_ino);
					return 0;
				} 
				else {
					debugprintf("[GetFileStat]: GetFileStat RPC failed.\n");
					debugprintf("[GetFileStat]: Function ended due to RPC failure.\n");
					errno = transform_rpc_err(status.error_code());
					return -1;
				}
			} else {
				debugprintf("[GetFileStat]: No change in file, getting stat from client local.\n");
				return lstat(abs_path.c_str(), buf);
			}
		}

	int FileSystemClient::OpenFile(std::string abs_path, std::string root) {
			debugprintf("[OpenFile]: Function entered.\n");
			int file;
			int retryCount = 0;
			FetchRequest req;
			FetchResponse resp;
			Status status;
			std::string path = get_relative_path(abs_path, root);
	
			auto test_auth_resp = TestAuth(abs_path, root);			
			if (!test_auth_resp.status.ok() || test_auth_resp.response.file_changed()) {  
				if(test_auth_resp.status.ok()) {
					debugprintf("[OpenFile]: TestAuth returns file changed. Fetching file from server.\n");
				} else {
					debugprintf("[OpenFile]: Failure invoking TestAuth RPC. Fetching file from server.\n");
				}
					
				req.set_pathname(path);
				do {
					debugprintf("[OpenFile]: Invoking Fetch RPC.\n");
					ClientContext context;
					resp.Clear();
					sleep(retryCount * RETRY_TIME_START * RETRY_TIME_MULTIPLIER);
					retryCount++;
					status = stub_->Fetch(&context, req, &resp);
				} while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE);
	
				// Checking RPC Status
				if (status.ok()) {
					debugprintf("[OpenFile]: Fetch RPC success.\n");
					int server_errno = resp.fs_errno();
					if(server_errno) {
						debugprintf("[OpenFile] Error %d on server\n", server_errno);
						debugprintf("[OpenFile]: Function ended in failure.\n");
						errno = server_errno;
						return -1;
					}

					// create directory tree if not exists, as it exists on the server
					if (create_path(path, true, root) != 0) {
						debugprintf("[OpenFile]: create_path() call failed.\n");
						return -1;
					}

					// opens file if it already exists, else creates the file at the path 
					file = open(abs_path.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0666);
					if (file == -1) {
						debugprintf("[OpenFile]: open() to copy file contents failed.\n");
						return -1;
					}

					// Writes the contents fetched from server to the file 
					if (write(file, resp.file_contents().c_str(), resp.file_contents().length()) == -1) {
						debugprintf("[OpenFile]: write() to file in client cache failed.\n");
						return -1;
					}

					// flush contents to file in client
					if (fsync(file) == -1) {
						debugprintf("[OpenFile]: fsync() to file in client cache failed.\n");
						return -1;
					}

					// set file open and modified times according to what server sends
					struct timespec t;
					auto modified_time = resp.time_modified();
					t.tv_sec = modified_time.sec();
					t.tv_nsec = modified_time.nsec();	
					if(set_file_open_time(file, t) == -1) {
						debugprintf("[OpenFile]: Error (%d) setting file open and modified timings.\n", errno);
					}
							
					// close the file on local client
					if (close(file) == -1) {
						debugprintf("[OpenFile]: close() on file in client cache failed.\n");
						return -1;
					}
					debugprintf("[OpenFile]: Successfully wrote %d bytes to cache.\n", (int)resp.file_contents().length()); 
				} 
				else 
				{
					debugprintf("[OpenFile]: Fetch RPC Failed.\n");
					errno = transform_rpc_err(status.error_code());
					return -1;
				}
			} else {
				debugprintf("[OpenFile]: TestAuth reported no change in file.\n");
			}

			file = open(abs_path.c_str(), O_RDWR | O_CREAT, 0666); 
			if (file == -1)
			{
				debugprintf("[OpenFile]: open() on client copy failed.\n");
				return -1;
			}
			
			debugprintf("[OpenFile]: Function ended.\n");
			return file;
		}

	TestAuthReturn FileSystemClient::TestAuth(std::string abs_path, std::string root) {
			debugprintf("[TestAuth]: Function entered.\n");
			TestAuthRequest req;
			TestAuthResponse resp;
			Timestamp t;
			timespec fileModifiedTime;
			Status status;
			int retryCount = 0;
			std::string path = get_relative_path(abs_path, root);

			// Check if local file exists
			// Set TestAuth response to true if local file doesn't exist
			if (!file_exists(abs_path)) {
				debugprintf("[TestAuth]: File does not exist locally. Set TestAuth response to true.\n");
				resp.set_file_changed(true);
				return TestAuthReturn(status, resp);
			}
			
			// Get local modified time
			// Set TestAuth response to true if getting local file modified time fails
			if (GetModifyTime(abs_path, &fileModifiedTime) != 0) {
				debugprintf("[TestAuth]: Failure to get local file modified time. Setting TestAuth response to true.\n");
				resp.set_file_changed(true); 
				return TestAuthReturn(status, resp);
			}

			// Set Request
			req.set_pathname(path);
			t.set_sec(fileModifiedTime.tv_sec);
			t.set_nsec(fileModifiedTime.tv_nsec);
			req.mutable_time_modified()->CopyFrom(t);

			// Make RPC and retry 
			do {
				debugprintf("[TestAuth]: Invoking TestAuth RPC.\n");
				ClientContext context;
				resp.Clear();
				sleep(retryCount * RETRY_TIME_START * RETRY_TIME_MULTIPLIER);
				retryCount++;
				status = stub_->TestAuth(&context, req, &resp);
			} while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE);

			if (status.ok()) {
				debugprintf("[TestAuth]: TestAuth RPC success.\n");
			}
			else {
				debugprintf("[TestAuth]: TestAuth RPC failed.\n");
			}

			debugprintf("[TestAuth]: Function ended successfully.\n");
			return TestAuthReturn(status, resp);
		}

	int FileSystemClient::Access(std::string abs_path, int mode, std::string root) {
			debugprintf("[Access]: Function entered.\n");
			AccessRequest req;
			AccessResponse resp;
			Status status;
			int retryCount = 0;
			std::string path = get_relative_path(abs_path, root);

			req.set_pathname(path);
			req.set_mode(mode);

			do {
				debugprintf("[Access] Invoking Access RPC.\n");
				ClientContext context;
				resp.Clear();
				sleep(retryCount * RETRY_TIME_START * RETRY_TIME_MULTIPLIER);
				retryCount++;
				status = stub_->Access(&context, req, &resp);
			} while(retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE);

			if(status.ok()) {
				debugprintf("[Access]: Access RPC success.\n");
				int server_errno = resp.fs_errno();
				if(server_errno) {
					debugprintf("[Access]: Server returned error %d on Access call for %s.\n", server_errno, path);
					errno = server_errno;
					return -1;
				}
				return 0;
			} else {
				debugprintf("[Access] Access RPC failed.\n");
				return -1;
			}
		}
		
  int FileSystemClient::MakeDir(std::string abs_path, std::string root, mode_t mode) {
			printf("MakeDir: Entering function\n");
			MakeDirRequest request;
			MakeDirResponse reply;
			Status status;
			int retryCount = 0;

			std::string path = get_relative_path(abs_path, root);
			request.set_pathname(path);
			request.set_mode(mode);

			// Make RPC & retry
                
			do         
			{
				ClientContext context;
				reply.Clear();
				printf("MakeDir: Invoking RPC\n");
				printf("MakdeDir: Path %s\n", path.c_str());
				sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
				status = stub_->MakeDir(&context, request, &reply);
				retryCount++;
			} while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE);
   	  
      // Checking RPC Status	
			if (status.ok()) {
				printf("MakeDir: RPC Success\n");
				printf("MakeDir: Exiting function\n");
				uint server_errno = reply.fs_errno();
				if(server_errno) {
					printf("...but error %d on server\n", server_errno);
					printf("MakeDir: Exiting function\n"); 
					errno = server_errno;
					return -1;
				}
                    
				// making the dir in cache directory, hierarchical if necessary
				create_path(path, false, root);
				return 0;
			}
			else
			{
				printf("MakeDir: RPC failure\n");
				printf("MakeDir: Exiting function\n");
				errno = transform_rpc_err(status.error_code());
				return -1;
			}
    }

  int FileSystemClient::RemoveDir(std::string abs_path, std::string root) {
        printf("RemoveDir: Entering function\n");
			RemoveDirRequest request;
			RemoveDirResponse reply;
			Status status;
			uint32_t retryCount = 0;

			std::string path = get_relative_path(abs_path, root);
			request.set_pathname(path);

			// Make RPC
			// Retry w backoff
						
			do
			{
					ClientContext context;
							reply.Clear();
					printf("RemoveDir: Invoking RPC\n");
					sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
					status = stub_->RemoveDir(&context, request, &reply);
					retryCount++;
			} while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE);

						// Checking RPC Status

			if (status.ok())
			{
					printf("RemoveDir: RPC Success\n");
					printf("RemoveDir: Exiting function\n");

								uint server_errno = reply.fs_errno();
					if(server_errno) {
				printf("...but error %d on server\n", server_errno);
						printf("RemoveDir: Exiting function\n");
				errno = server_errno;
				return -1;
					}

								// removing directory from cache
					// rmdir(get_cache_path(path).c_str());
					rmdir(abs_path.c_str());
					return 0;
			}
			else
			{
					printf("RemoveDir: RPC Failure\n");
					printf("RemoveDir: Exiting function\n");
					errno = transform_rpc_err(status.error_code());
					return -1;
			}
  }

  int FileSystemClient::ReadDir(std::string abs_path, std::string root, void *buf, fuse_fill_dir_t filler) {
        printf("ListDir: Entering function\n");
			ListDirRequest request;
			ListDirResponse reply;
			Status status;
			uint32_t retryCount = 0;

			std::string path = get_relative_path(abs_path, root);
			request.set_pathname(path);

			// Make RPC
			// Retry w backoff
		
			do
			{
					ClientContext context;
								reply.Clear();
							printf("ListDir: Invoking RPC\n");
							sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
							status = stub_->ReadDir(&context, request, &reply);
							retryCount++;
					} while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE);

				
			// std::cout << "count = " << reply.entries().size() << std::endl;

			// Checking RPC Status
			if (status.ok()) {
								printf("ListDir: RPC Success\n");       
							uint server_errno = reply.fs_errno();
						if(server_errno) {
								printf("...but error %d on server\n", server_errno);
										printf("ListDir: Exiting function\n");          
						errno = server_errno;
										return -1;
					}
						
			}
						else
						{
								// std::cout << status.error_code() << ": " << status.error_message()
								//           << std::endl;
					//PrintErrorMessage(status.error_code(), status.error_message(), "ListDir");
					printf("ListDir: RPC Failure\n");
								errno = transform_rpc_err(status.error_code());
							return -1;
					}

					printf("ListDir: Exiting function\n");
					for (auto itr = reply.entries().begin(); itr != reply.entries().end(); itr++) {
								struct stat st;
								memset(&st, 0, sizeof(st));
								st.st_ino = itr->size();
								st.st_mode = itr->mode();
								if (filler(buf, itr->file_name().c_str() , &st, 0))
										break;
				}
				return 0;
  }

	int FileSystemClient::CloseFile(int fd, std::string abs_path, std::string root)
	{
		debugprintf("CloseFile: Entered function\n");
		StoreRequest request;
		StoreResponse reply;
		Status status;
		uint32_t retryCount = 0;
		std::string path = get_relative_path(abs_path, root);
		

		// Set request
		// const string cache_path = get_cache_path(path);
		
		request.set_pathname(path);

		string content = readFileIntoString(abs_path);
		request.set_file_contents(content);

		// Make RPC
		// Retry with backoff
		do 
		{
			ClientContext context;
			reply.Clear();
			debugprintf("CloseFile: Invoking RPC\n");
			sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
			status = stub_->Store(&context, request, &reply);
			retryCount++;
		} while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE);

		// Checking RPC Status
		if (status.ok()) 
		{
			debugprintf("CloseFile: RPC Success\n");

			uint server_errno = reply.fs_errno();
			if(server_errno) {
				debugprintf("...but error %d on server\n", server_errno);
				debugprintf("CloseFile: Exiting function\n"); 
				errno = server_errno;
					return -1;
			}
			auto timing = reply.time_modify();
			
			struct timespec t;
			t.tv_sec = timing.sec();
			t.tv_nsec = timing.nsec();
			
			if(set_timings(abs_path,t) == -1) {
				debugprintf("CloseFile: error (%d) setting file timings\n",errno);
			} else {
				debugprintf("CloseFile: updated file timings\n");
			}
			
			debugprintf("CloseFile: Exiting function\n");
			return 0;
		} 
		else 
		{
			std::cout << status.error_message() << "\n";
			debugprintf("CloseFile: RPC Failure %d.\n", status.error_code());
			debugprintf("CloseFile: Exiting function\n");
			errno = transform_rpc_err(status.error_code());
				return -1;
		}
	}

	int FileSystemClient::CreateFile(std::string abs_path, std::string root, mode_t mode, int flags)
	{
		debugprintf("CreateFile: Entering function\n");
		CreateRequest request;
		CreateResponse reply;
		Status status;
		uint32_t retryCount = 0;
		std::string path = get_relative_path(abs_path, root);

		request.set_pathname(path);
		request.set_mode(mode);
		request.set_flags(flags);

		// Make RPC and retry
		do 
		{
			ClientContext context;
			reply.Clear();
			debugprintf("CreateFile: Invoking RPC\n");
			sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
			status = stub_->Create(&context, request, &reply);
			retryCount++;
		} while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE);

		// Checking RPC Status
		if (status.ok()) 
		{
			debugprintf("CreateFile: RPC Success\n");
			uint server_errno = reply.fs_errno();
			if(server_errno) {
				debugprintf("...but error %d on server\n", server_errno);
				debugprintf("CreateFile: Exiting function\n"); 
				errno = server_errno;
					return -1;
			}

			int ret = open(abs_path.c_str(), flags, mode);
			debugprintf("CreateFile: %s, %d, %d\n", abs_path.c_str(), flags, mode);
			struct timespec curr_time;
			timespec_get(&curr_time, TIME_UTC);
			curr_time.tv_sec--;
			open_map[abs_path] = curr_time;
			return ret;
		} 
		else 
		{
			debugprintf("CreateFile: RPC Failure\n");
			debugprintf("CreateFile: Exiting function\n");
			errno = transform_rpc_err(status.error_code());
				return -1;
		}
	}

	int FileSystemClient::DeleteFile(std::string abs_path, std::string root) 
	{
		debugprintf("DeleteFile: Entered function\n");
		RemoveRequest request;
		RemoveResponse reply;
		Status status;
		uint32_t retryCount = 0;
		std::string path = get_relative_path(abs_path, root);

		request.set_pathname(path);
		
		// Make RPC 
		// Retry with backoff
		do 
		{
			ClientContext context;
			reply.Clear();
			debugprintf("DeleteFile: Invoking RPC\n");
			sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
			status = stub_->Remove(&context, request, &reply);
			retryCount++;
		} while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE );
		

		// Checking RPC Status 
		if (status.ok()) 
		{
			debugprintf("DeleteFile: RPC success\n");
			uint server_errno = reply.fs_errno();
			if(server_errno) {
				debugprintf("...but error %d on server\n", server_errno);
				debugprintf("DeleteFile: Exiting function\n"); 
				errno = server_errno;
					return -1;
			}
			
			debugprintf("DeleteFile: Exiting function\n");

			// remove from local cache
			if (file_exists(abs_path))
			{
				unlink(abs_path.c_str());
			}
			if(open_map.find(abs_path) != open_map.end())
			{
				open_map.erase(abs_path);
			}

			return 0;
		} 
		else
		{
			debugprintf("DeleteFile: RPC failure\n");
			debugprintf("DeleteFile: Exiting function\n");
			errno = transform_rpc_err(status.error_code());
				return -1;
		}                
	}

	int FileSystemClient::CloseFileUsingStream(int fd, std::string abs_path, std::string root) {
		debugprintf("[CloseFileUsingStream]: Function entered. %s\n", abs_path.c_str());
		if (close(fd) == -1) {
				debugprintf("[CloseFileUsingStream]: close() failed.\n");
				return -1;
		}
		StoreRequest request;
		StoreResponse reply;
		Status status;
		int retryCount = 0;
		timespec file_modified_time;
		timespec file_opened_time = open_map[abs_path];
		std::string path = get_relative_path(abs_path, root);

		GetModifyTime(abs_path, &file_modified_time);
		// std::cout << abs_path << " m:" << file_modified_time.tv_sec << " o:" << file_opened_time.tv_sec << "\n";
		// std::cout << abs_path << " m:" << file_modified_time.tv_nsec << " o:" << file_opened_time.tv_nsec << "\n";
		if(open_map.find(abs_path) == open_map.end() || 
			file_modified_time.tv_sec > file_opened_time.tv_sec ||
			(file_modified_time.tv_sec == file_opened_time.tv_sec && file_modified_time.tv_nsec > file_opened_time.tv_nsec)
			)
		{
			do {
				debugprintf("[CloseFileUsingStream]: Invoking Close RPC.\n");
				ClientContext context;
				reply.Clear();
				sleep(retryCount * RETRY_TIME_START * RETRY_TIME_MULTIPLIER);
				// std::cout << "Checking paths; abs_path=" << abs_path << " path=" << path << "\n";

				struct stat st;
				stat(abs_path.c_str(), &st);
				int fileSize = st.st_size;
				int totalChunks = 0;
				totalChunks = fileSize / CHUNK_SIZE;
				bool aligned = true;
				int lastChunkSize = fileSize == 0 ? 0 : CHUNK_SIZE;
				if (fileSize % CHUNK_SIZE) {
					totalChunks++;
					aligned = false;
					lastChunkSize = fileSize % CHUNK_SIZE;
				}
				debugprintf("[CloseFileUsingStream]: fileSize = %d\n", fileSize);
				debugprintf("[CloseFileUsingStream]: totalChunks = %d\n", totalChunks);
				debugprintf("[CloseFileUsingStream]: lastChunkSize = %d\n", lastChunkSize);

				if(fileSize == 0) {
					debugprintf("CloseFileUsingStream: Nothing to send.\n");
					return 0;
				}

				request.set_pathname(path);
				std::unique_ptr<ClientWriter<StoreRequest>> writer(
				stub_->StoreUsingStream(&context, &reply));

				std::ifstream fin(abs_path.c_str(), std::ios::binary);
				fin.clear();
				fin.seekg(0, ios::beg);

				unsigned long bytes = 0;
				unsigned long bytes_read = 0;
				for (size_t chunk = 0; chunk < totalChunks; chunk++) {
					size_t currentChunkSize = (chunk == totalChunks - 1 && !aligned) ? 
						lastChunkSize : CHUNK_SIZE;

					char * buffer = new char[currentChunkSize];	
					bytes += currentChunkSize;
		
					if (fin.read(buffer, currentChunkSize)) 
					{
						auto bcnt = fin.gcount();
						bytes_read += bcnt;
						request.set_pathname(path);
						request.set_file_contents(buffer, bcnt);
						// debugprintf("[CloseFileUsingStream]: Read chunk [iter %ld, expect %ld B, read %ld B]\n",chunk, bytes, bytes_read);

						if (!writer->Write(request)) 
						{
							debugprintf("[CloseFileUsingStream]: Stream broke\n");
							break; 
						}
					} else {
						debugprintf("[CloseFileUsingStream]: Failed to write chunk [iter %ld, total %ld B]\n", chunk, bytes);
					}
				}
				fin.close();
				writer->WritesDone();
				status = writer->Finish();

				retryCount++;
			} while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE);

			if (status.ok()) {
				debugprintf("[CloseFileUsingStream]: RPC Success\n");
				uint server_errno = reply.fs_errno();
				if(server_errno) {
					debugprintf("[CloseFileUsingStream]: error %d on server\n", server_errno);
					debugprintf("[CloseFileUsingStream]: Exiting function\n"); 
					errno = server_errno;
					return -1;
				}
				
				auto timing = reply.time_modify();
				
				struct timespec t;
				t.tv_sec = timing.sec();
				t.tv_nsec = timing.nsec();
				
				if(set_timings(abs_path, t) == -1) {
						debugprintf("[CloseFileUsingStream]: error (%d) setting file timings\n", errno);
				} else {
						debugprintf("[CloseFileUsingStream]: updated file timings\n");
				}
				
				debugprintf("[CloseFileUsingStream]: Exiting function\n");
				if(open_map.find(abs_path) != open_map.end())
				{
					open_map.erase(abs_path);
				}
				return 0;
			} else {
				debugprintf("[CloseFileUsingStream]: RPC Failure\n");
				debugprintf("[CloseFileUsingStream]: Exiting function\n");
				std::cout << "[CloseFileUsingStream]: Error msg: " << status.error_message() << "\n";
				errno = transform_rpc_err(status.error_code());
				if(open_map.find(abs_path) != open_map.end())
				{
					open_map.erase(abs_path);
				}
				return -1;
			}
		} else {
			debugprintf("[CloseFileUsingStream]: File not changed by client, skipping sending to server.\n");
			if(open_map.find(abs_path) != open_map.end())
			{
				open_map.erase(abs_path);
			}
			return 0;
		}
	}

  int FileSystemClient::OpenFileUsingStream(std::string abs_path, std::string root, int flags) {
		debugprintf("OpenFileUsingStream: Inside function, path = %s\n", abs_path.c_str());
		int file;
		FetchRequest request;
		FetchResponse reply;
		Status status;
		ClientContext context;
		int retryCount = 0;
		std::string path = get_relative_path(abs_path, root);

		auto test_auth_result = TestAuth(abs_path, root);
		if (!test_auth_result.status.ok() || test_auth_result.response.file_changed()) {  
			if(test_auth_result.status.ok()) {
				debugprintf("OpenFileUsingStream: TestAuth reports changed\n");
			} else {
				debugprintf("OpenFileUsingStream: TestAuth RPC failed\n");
			}

			if (create_path(path, true, root) != 0) {
				debugprintf("[OpenFileUsingStream]: create_path() call failed.\n");
				return -1;
			}
			
			do {
				debugprintf("OpenFileUsingStream: Invoking RPC\n");
				request.set_pathname(path);
				
				sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
				
				std::unique_ptr<ClientReader<FetchResponse>> reader(
						stub_->FetchUsingStream(&context, request));
				
				std::ofstream file;
				file.open(abs_path, std::ios::binary);
				file.clear();
				file.seekp(0, ios::beg);

				while (reader->Read(&reply))
				{
					file << reply.file_contents();
				}

				Status status = reader->Finish();
				file.close();
				retryCount++;
			} while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE);

			if (status.ok()) {
				debugprintf("OpenFileUsingStream: RPC Success\n");
				uint server_errno = reply.fs_errno();
				if(server_errno) {
					debugprintf("OpenFileUsingStream: error %d on server\n", server_errno);
					errno = server_errno;
					return -1;
				}
				
				auto timing = reply.time_modified();
				
				struct timespec t;
				t.tv_sec = timing.sec();
				t.tv_nsec = timing.nsec();
				
				if(set_timings(abs_path,t) == -1) {
					debugprintf("OpenFileUsingStream: error (%d) setting file timings\n",errno);
				} else {
					debugprintf("OpenFileUsingStream: updated file timings\n");
				}
					
			} else {
				debugprintf("OpenFileUsingStream: RPC Failure\n");
				errno = transform_rpc_err(status.error_code());
				return -1;
			}
		} else if(!test_auth_result.status.ok() || test_auth_result.response.file_group_locked()) {
			debugprintf("OpenFileUsingStream: TestAuth reported file locked. Retry in some time!\n");
			return -1;
		} else {
			debugprintf("OpenFileUsingStream: TestAuth reported no change.\n");
		}
		
		file = open(abs_path.c_str(), flags);
		if (file == -1) {
			debugprintf("OpenFileUsingStream: open() failed\n");
			return -1;
		}
		struct timespec curr_time;
		timespec_get(&curr_time, TIME_UTC);
		curr_time.tv_sec--;
		open_map[abs_path] = curr_time;
		debugprintf("OpenFileUsingStream: Exiting function\n");
		return file;
	}

  int FileSystemClient::Rename(std::string abs_path, std::string new_name, std::string root)
        {
                debugprintf("Rename: Entered function\n");
                RenameRequest request;
                RenameResponse reply;
                Status status;
                uint32_t retryCount = 0;

                std::string old_path = get_relative_path(abs_path, root);
		std::string new_path = get_relative_path(new_name, root);


                request.set_pathname(old_path);
								request.set_componentname(new_path);

                // Make RPC
                do
                {
                        ClientContext context;
                        reply.Clear();
                        debugprintf("Rename: Invoking RPC\n");
                        sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
                        status = stub_->Rename(&context, request, &reply);
                        retryCount++;
                } while (retryCount < MAX_RETRIES && status.error_code() == StatusCode::UNAVAILABLE );


                // Checking RPC Status
                if (status.ok())
                {
                        debugprintf("Rename: RPC success\n");
                        // uint server_errno = reply.fs_errno();
                        // if(server_errno) {
                        //         debugprintf("...but error %d on server\n", server_errno);
                        //         debugprintf("Rename: Exiting function\n");
                        //         errno = server_errno;
                        //                 return -1;
                        // }

                        // debugprintf("Rename: Exiting function\n");

						debugprintf("Running rename\n");
                        
                        debugprintf("%d\n", rename(abs_path.c_str(), new_name.c_str()));
                        return 0;
                }
                else
                {
                        debugprintf("Rename: RPC failure\n");
                        debugprintf("Rename: Exiting function\n");
                        errno = transform_rpc_err(status.error_code());
                                return -1;
                }
        }
};


// g++ -std=c++17 afs_client.cc afs.pb.cc afs.grpc.pb.cc -o client `pkg-config --libs --cflags protobuf grpc_cpp_plugin`

// g++ client.o -L/usr/local/lib `pkg-config --libs --static protobuf grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl afs_client.cc

// g++ -std=c++11 -I ./ -I ./ afs_client.cc afs.pb.cc afs.grpc.pb.cc -L libprotobuf -lprotobuf -L grpc++ -lgrpc++ -lgrpc -o afs_client.o
