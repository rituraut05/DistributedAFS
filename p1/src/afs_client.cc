#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <vector>

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

#define DEBUG                 1                                     // for debugging
#define dbgprintf(...)        if (DEBUG) { printf(__VA_ARGS__); }   // for debugging
#define RETRY_TIME_START      1                                     // in seconds
#define RETRY_TIME_MULTIPLIER 2                                     // for rpc retry w backoff
#define MAX_RETRY             5                                     // rpc retry

enum fuse_readdir_flags {
         FUSE_READDIR_PLUS = (1 << 0),
};

enum fuse_fill_dir_flags {
         FUSE_FILL_DIR_PLUS = (1 << 1),
};

string get_relative_path(string path, string root) {
	string path_copy = path;
	path_copy.erase(0, root.length()+1);
	return path_copy;
}

bool FileExists(std::string path) {
	struct stat s;   
	return (stat(path.c_str(), &s) == 0); 
}

uint32_t GetModifyTime(std::string filepath, timespec * t) {
	dbgprintf("GetModifyTime: Entering function\n");
	struct stat sb;
	if (stat(filepath.c_str(), &sb) == -1) 
	{
		dbgprintf("GetModifyTime: Failed\n");
		return -1;
	}
	dbgprintf("GetModifyTime: Exiting function\n");
	*t = sb.st_mtim;
	return 0;
}

int set_timings_opened(int fd, timespec t) {
	struct timespec p[2] = {t,t};
	return futimens(fd,p);
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
	dbgprintf("create_path: Entering function\n");
	vector<string> tokens = tokenize_path(relative_path, '/', is_file);

	// Change base path later
	string base_path = root;
	for (auto token : tokens)
	{
		base_path += token + "/";

		struct stat s;
		int r = stat(base_path.c_str(), &s);
		if (r != 0 && errno == 2) 
		{
				dbgprintf("create_path: stat() ENOENT\n");
				if (mkdir(base_path.c_str(), S_IRWXU) != 0)
				{
						dbgprintf("create_path: mkdir() failed : %d\n", errno);
						return -1;
				}
		}
		else if (r != 0)
		{
				dbgprintf("create_path: stat() failed : %d\n", errno);
				return -1;
		}		
	}
	dbgprintf("create_path: Exiting function\n");
	return 0;
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
		return EIO; // fall back to IO err for unexpected issues
	}
}

extern "C" {
    FileSystemClient::FileSystemClient(std::shared_ptr<Channel> channel)
      : stub_(FileSystemService::NewStub(channel)) {

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
			} while (retryCount < MAX_RETRY && status.error_code() == StatusCode::UNAVAILABLE);

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

		int FileSystemClient::GetFileStat(std::string abs_path, struct stat *stbuf, std::string root) {
			dbgprintf("GetFileStat: Entering function\n");
			GetFileStatRequest request;
			GetFileStatResponse reply;
			Status status;
			uint32_t retryCount = 0;

			std::string path = get_relative_path(abs_path, root);

//			dbgprintf("Path is: %s\n", path);

			request.set_pathname(path);
			// Make RPC
			// Retry w backoff
			do 
			{
				ClientContext context;
				reply.Clear();
				dbgprintf("GetFileStat: Invoking RPC\n");
				sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
				status = stub_->GetFileStat(&context, request, &reply);
				retryCount++;
			} while (retryCount < MAX_RETRY && status.error_code() == StatusCode::UNAVAILABLE);

			// Checking RPC Status
			if (status.ok()) 
			{
				dbgprintf("GetFileStat: RPC Success\n");
				uint server_errno = reply.fs_errno();
				if(server_errno) {
						dbgprintf("...but error %d on server\n", server_errno);
						dbgprintf("GetFileStat: Exiting function\n"); 
						errno = server_errno;
								return -1;
				}
				
				stbuf->st_ino = reply.status().ino();
				stbuf->st_mode = reply.status().mode();
				stbuf->st_nlink = reply.status().nlink();
				stbuf->st_uid = reply.status().uid();
				stbuf->st_gid = reply.status().gid();
				stbuf->st_size = reply.status().size();
				stbuf->st_blksize = reply.status().blksize();
				stbuf->st_blocks = reply.status().blocks();
				stbuf->st_atime = reply.status().atime();
				stbuf->st_mtime = reply.status().mtime();
				stbuf->st_ctime = reply.status().ctime();
				dbgprintf("GetFileStat: Exiting function\n");
				return 0;
			} 
			else
			{
				dbgprintf("GetFileStat: RPC Failed\n");
				dbgprintf("GetFileStat: Exiting function\n");
				errno = transform_rpc_err(status.error_code());
						return -1;
			}
		}

		int FileSystemClient::OpenFile(std::string abs_path, std::string root) {
			dbgprintf("OpenFile: Inside function\n");
			int file;
			FetchRequest request;
			FetchResponse reply;
			Status status;

			uint32_t retryCount = 0;

			std::string path = get_relative_path(abs_path, root);
	
			auto test_auth_result = TestAuth(abs_path, root);
			
			if (!test_auth_result.status.ok() || test_auth_result.response.has_changed())
			{  
					#if DEBUG
					if(test_auth_result.status.ok()) {
							dbgprintf("OpenFile: TestAuth reports changed\n");
					} else {
							dbgprintf("OpenFile: TestAuth RPC failed\n");
					}
					#endif
					
					request.set_pathname(path);

					// Make RPC
					// Retry with backoff
					do 
					{
							ClientContext context;
							reply.Clear();
							dbgprintf("OpenFile: Invoking RPC\n");
							sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
							status = stub_->Fetch(&context, request, &reply);
							retryCount++;
					} while (retryCount < MAX_RETRY && status.error_code() == StatusCode::UNAVAILABLE );
	
					// Checking RPC Status
					if (status.ok()) 
					{
							dbgprintf("OpenFile: RPC Success\n");
							uint server_errno = reply.fs_errno();
							if(server_errno) {
									dbgprintf("...but error %d on server\n", server_errno);
									dbgprintf("OpenFile: Exiting function\n");
									errno = server_errno;
									return -1;
							}
							

							// create directory tree if not exists, as it exists on the server
							if (create_path(path, true, root) != 0)
							{
									dbgprintf("create_path: failed\n");
									return -1;
							}

							dbgprintf("OpenFile: create_path() Success\n");

							file = open(abs_path.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0666);
							if (file == -1)
							{
									dbgprintf("OpenFile: open() failed\n");
									return -1;
							}
					
							//dbgprintf("OpenFile: reply.file_contents().length() = %ld\n", reply.file_contents().length());
					
							if (write(file, reply.file_contents().c_str(), reply.file_contents().length()) == -1)
							{
									dbgprintf("OpenFile: write() failed\n");
									return -1;
							}
									
							auto timing = reply.time_modify();
							
							struct timespec t;
							t.tv_sec = timing.sec();
							t.tv_nsec = timing.nsec();
							
							if(set_timings_opened(file,t) == -1) {
									dbgprintf("OpenFile: error (%d) setting file timings\n",errno);
							} else {
									dbgprintf("OpenFile: updated file timings\n");
							}
							
							if (fsync(file) == -1)
							{
									dbgprintf("OpenFile: fsync() failed\n");
									return -1;
							}
							
							if (close(file) == -1)
							{
									dbgprintf("OpenFile: close() failed\n");
									return -1;
							}
							
							dbgprintf("OpenFile: successfully wrote %d bytes to cache\n", (int)reply.file_contents().length()); 
					} 
					else 
					{
							dbgprintf("OpenFile: RPC Failure\n");
							errno = transform_rpc_err(status.error_code());
							return -1;
					}
			} else {
					dbgprintf("OpenFile: TestAuth reports no change\n");
			}

			file = open(abs_path.c_str(), O_RDWR | O_CREAT, 0666); 
			if (file == -1)
			{
					dbgprintf("OpenFile: open() failed\n");
					return -1;
			}
			
			dbgprintf("OpenFile: Exiting function\n");
			return file;
		}

		TestAuthReturnType FileSystemClient::TestAuth(std::string abs_path, std::string root) {
			dbgprintf("TestAuth: Entering function\n");
			TestAuthRequest request;
			TestAuthResponse reply;
			Timestamp t;
			timespec modifyTime;
			Status status;
			uint32_t retryCount = 0;

			std::string path = get_relative_path(abs_path, root);

			// Check if local file exists
			if (!FileExists(abs_path))
			{
				dbgprintf("TestAuth: Local file does not exist\n");
				reply.set_has_changed(true);
				return TestAuthReturnType(status, reply);
			}
			
			// Get local modified time
			if (GetModifyTime(abs_path, &modifyTime) != 0)
			{
				dbgprintf("TestAuth: Exiting function\n");
				reply.set_has_changed(false); // TO CHECK: Should we return true or false in this case?
				return TestAuthReturnType(status, reply);
			}

			// Set Request
			request.set_pathname(path);
			t.set_sec(modifyTime.tv_sec);
			t.set_nsec(modifyTime.tv_nsec);
			request.mutable_time_modify()->CopyFrom(t);

			// Make RPC
			// Retry w backoff
			do 
			{
				ClientContext context;
				reply.Clear();
				dbgprintf("TestAuth: Invoking RPC\n");
				sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
				status = stub_->TestAuth(&context, request, &reply);
				retryCount++;
			} while (retryCount < MAX_RETRY && status.error_code() == StatusCode::UNAVAILABLE);

			if (status.ok()) 
			{
				dbgprintf("TestAuth: RPC Success\n");
			}
			else
			{
				dbgprintf("TestAuth: RPC Failure\n");
			}

			dbgprintf("TestAuth: Exiting function\n");
			return TestAuthReturnType(status, reply);
		}	

		
    int FileSystemClient::MakeDir(std::string abs_path, std::string root, mode_t mode) {
        printf("MakeDir: Entering function\n");
	MakeDirRequest request;
	MakeDirResponse reply;
	Status status;
	uint32_t retryCount = 0;

	std::string path = get_relative_path(abs_path, root);
	request.set_pathname(path);
	request.set_mode(mode);

	// Make RPC
	// Retry w backoff
                
	do         
	{
	    ClientContext context;
    	    reply.Clear();
	    printf("MakeDir: Invoking RPC\n");
	    sleep(RETRY_TIME_START * retryCount * RETRY_TIME_MULTIPLIER);
	    status = stub_->MakeDir(&context, request, &reply);
	    retryCount++;
	} while (retryCount < MAX_RETRY && status.error_code() == StatusCode::UNAVAILABLE);
   	  
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
	} while (retryCount < MAX_RETRY && status.error_code() == StatusCode::UNAVAILABLE);

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
	    rmdir(path.c_str());
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
    	} while (retryCount < MAX_RETRY && status.error_code() == StatusCode::UNAVAILABLE);

    
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
       	    // if (filler(buf, itr->file_name().c_str() , &st, 0, static_cast<fuse_fill_dir_flags>(0)))
       	    if (filler(buf, itr->file_name().c_str() , &st, 0))
                break;
   	}
   	return 0;
    }

};


// g++ -std=c++17 afs_client.cc afs.pb.cc afs.grpc.pb.cc -o client `pkg-config --libs --cflags protobuf grpc_cpp_plugin`

// g++ client.o -L/usr/local/lib `pkg-config --libs --static protobuf grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl afs_client.cc

// g++ -std=c++11 -I ./ -I ./ afs_client.cc afs.pb.cc afs.grpc.pb.cc -L libprotobuf -lprotobuf -L grpc++ -lgrpc++ -lgrpc -o afs_client.o
