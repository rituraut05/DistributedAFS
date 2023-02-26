#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <filesystem>
#include <shared_mutex>
#include <vector>
#include <algorithm>
#include <cstddef>  

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#ifdef BAZEL_BUILD
#include "afs.grpc.pb.h"
#else
#include "afs.grpc.pb.h"
#endif

#define DEBUG 1
#define debugprintf(...)       \
    if (DEBUG) {             \
        printf(__VA_ARGS__); \
    }
#define errprintf(...) \
    { printf(__VA_ARGS__); }
#define CHUNK_SIZE 4096																	// for streaming 

namespace fs = std::filesystem;
using fs::path;
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
using afs::CreateRequest;
using afs::CreateResponse;
using afs::RemoveRequest;
using afs::RemoveResponse;
using afs::RenameRequest;
using afs::RenameResponse;

static const string TEMP_FILE_EXT = ".afs_tmp";
// const std::vector<string> atomicFilesGroup = {"/acc1", "/acc2"};
const std::vector<string> atomicFilesGroup = {};

class ProtocolException : public std::runtime_error {
   StatusCode code;

   public:
    ProtocolException(const char* msg, StatusCode code) : std::runtime_error(msg), code(code) {}

    StatusCode get_code() const {
        return code;
    }
};

class FileSystemException : public std::runtime_error {
    uint fs_errno;
    
    public:
    FileSystemException(uint fs_errno) : std::runtime_error("Error in filesystem call"), fs_errno(fs_errno) {}
    
    uint get_fs_errno() const {
        return fs_errno;
    }
};

class AFSImpl final : public FileSystemService::Service {
  path root;
  int groupLocked = 0;
  std::shared_mutex mutex;

  // Sets groupLocked value to v
  void setFileGroupLocked(int v) {
    mutex.lock();
    groupLocked = v;
    mutex.unlock();
  }

  bool checkFileInGroup(path filepath) {
    std::string fp_string = filepath.u8string();
    std::size_t parDirPos = fp_string.find_last_of("/");
    // get directory
    std::string dir = fp_string.substr(0, parDirPos);
    // get file
    std::string file = fp_string.substr(parDirPos, fp_string.length());
    debugprintf("Checking if the file is a part of the Atomic Group: %s\n", file.c_str());
    return std::find(atomicFilesGroup.begin(), atomicFilesGroup.end(), file) != atomicFilesGroup.end();
  }

  path getPath(string relativePath) {
    path normalPath = (root / relativePath).lexically_normal();
    // debugprintf("[getPath]: root = %s\n", root.c_str());
    // debugprintf("[getPath]: relative path = %s\n", relativePath.c_str());
    // debugprintf("[getPath]: Normalised path = %s\n", normalPath.c_str());
    
    // error checking 
    auto [a, b] = std::mismatch(root.begin(), root.end(), normalPath.begin());
    if (a != root.end()) {
      throw ProtocolException("Normalized path is outside server root.", StatusCode::INVALID_ARGUMENT);
    }

    return normalPath;
  }

  Timestamp readModifyTime(path filepath) {
    struct stat sb;
    if (stat(filepath.c_str(), &sb) == -1) {
      throw FileSystemException(errno);
    }
    Timestamp t;
    t.set_sec(sb.st_mtim.tv_sec);
    t.set_nsec(sb.st_mtim.tv_nsec);
    return t;
  }

  uint checkReadLocation(path filepath) {
    std::error_code ec;
    auto status = fs::status(filepath, ec);
    
    // Error locating file in filepath
    if(ec) {
      return ec.value();
    } 
    switch (status.type()) {
      case fs::file_type::regular: 
        return 0;
      case fs::file_type::directory: // filepath is a directory
        return EISDIR;
      default:
        return EPERM; // Other types of file, we throw a permission error
    }
  }

  string readFile(path filepath) {
    // Check that path exists and is a file before proceeding
    uint chkFileErr = checkReadLocation(filepath);
    if(chkFileErr != 0) {
        throw FileSystemException(chkFileErr);
    }
    
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    std::ostringstream buffer;
    buffer << file.rdbuf();

    if (file.fail()) {
      debugprintf("[readFile]: Function ended on file read failure.\n");
      throw ProtocolException("Error reading file", StatusCode::UNKNOWN);
    }

    return buffer.str();
  }

  uint check_valid_write_destination(path filepath) {
        std::error_code ec;
        auto status = fs::status(filepath, ec);
        switch (status.type()) {
            case fs::file_type::regular:
                return 0;
            case fs::file_type::directory:
                return EISDIR;
                //throw ProtocolException("Attempting to write file to location of directory", StatusCode::FAILED_PRECONDITION);
            case fs::file_type::not_found:
                break;
            default:
                return EPERM;
                //throw ProtocolException("Attempting to write file to location of non-file item", StatusCode::FAILED_PRECONDITION);
        }

        // type was not_found, so check the error code
        switch (ec.value()) {
            case ENOENT:
                break;
            // case ENOTDIR:
            //     return ENOTDIR;
            //     throw ProtocolException("Non-directory in path prefix", StatusCode::FAILED_PRECONDITION);
            default:
                return ec.value();
                // throw ProtocolException("Error checking file status", StatusCode::UNKNOWN);
        }

        // error code was ENOENT, so check that parent directory exists
        if (!fs::is_directory(filepath.parent_path(), ec)) {
            return ENOENT;
            // throw ProtocolException("Parent directory does not exist", StatusCode::NOT_FOUND);
        }

        // result (b)
        return 0;
  }

  path get_tempfile_path(path filepath) {
      return path(filepath.string() + TEMP_FILE_EXT);
  }

  void writeFile(path filepath, string content) {
        debugprintf("writeFile: Entering function\n");

        std::ofstream file;

        // Check that this is a valid destination
        uint pre_err = check_valid_write_destination(filepath);
        if(pre_err != 0) {
          throw FileSystemException(pre_err);
        }

        path temppath = get_tempfile_path(filepath);

        // Write to temp path
        file.open(temppath, std::ios::binary);
        file << content;
        file.close();

        // Typical failure cases should be handled by the validity check above,
        // so failure to write the temp file means something unexpected has happened.
        if (file.fail()) {
            debugprintf("writeFile: Exiting function\n");
            throw ProtocolException("Error writing temp file", StatusCode::UNKNOWN);
        }

        // Overwrite dest file with temp file
        if (rename(temppath.c_str(), filepath.c_str()) == -1) {
            debugprintf("writeFile: Exiting function\n");
            throw ProtocolException("Error committing temp file", StatusCode::UNKNOWN);
        }

        debugprintf("write_file: Exiting function\n");
    }

  FileStat readStat(path filepath) {
    struct stat sb;
    FileStat ret;

    if (stat(filepath.c_str(), &sb) == -1) {
      debugprintf("[readStat]: Reading file stat failed with errno=%d\n", errno);
      throw FileSystemException(errno);
    }

    ret.set_ino(sb.st_ino);
    ret.set_mode(sb.st_mode);
    ret.set_nlink(sb.st_nlink);
    ret.set_uid(sb.st_uid);
    ret.set_gid(sb.st_gid);
    ret.set_size(sb.st_size);
    ret.set_blksize(sb.st_blksize);
    ret.set_blocks(sb.st_blocks);
    ret.set_atime(sb.st_atime);
    ret.set_mtime(sb.st_mtime);
    ret.set_ctime(sb.st_ctime);
    return ret;
  }

  void make_dir(path filepath, int mode) {
	debugprintf("make_dir: Entering function\n");
        if (mkdir(filepath.c_str(), mode) == -1) {
            debugprintf("make_dir: Exiting function\n");
            throw FileSystemException(errno);
        }
        debugprintf("make_dir: Exiting function\n");
    }

    void remove_dir(path filepath) {
        if (rmdir(filepath.c_str()) == -1) {
            throw FileSystemException(errno);
        }
    }

    void list_dir(path filepath, ListDirResponse* reply) {
        debugprintf("list_dir: Entered function\n");
        for (const auto& entry : fs::directory_iterator(filepath)) {
            DirectoryEntry* msg = reply->add_entries();
            msg->set_file_name(entry.path().filename());
            msg->set_mode(is_regular_file(entry.status()) ? FileMode::REG : is_directory(entry.status()) ? FileMode::DIR
                                                                                         : FileMode::UNSUPPORTED);
            msg->set_size(is_regular_file(entry.status()) ? file_size(entry) : 0);
        }
        debugprintf("list_dir: Exiting function\n");
    }

    void move_file(path srcpath, path dstpath) {
        debugprintf("move_file: Entering function\n");


        if (rename(srcpath.c_str(), dstpath.c_str()) == -1) {
            debugprintf("move_file: Exiting function with error %d\n", errno);
            throw FileSystemException(errno);
        }

        debugprintf("move_file: Exiting function\n");
    }

 public:
    explicit AFSImpl(path root) : root(root) {}

    Status Ping(ServerContext* context, const PingMessage* req, PingMessage* resp) override {
        debugprintf("[Ping]: Returning Success.\n");
        return Status::OK;
    }

    Status GetFileStat(ServerContext* context, const GetFileStatRequest* req, GetFileStatResponse* resp) override {
      // debugprintf("[GetFileStat]: Function entered.\n");
      try {
        path filepath = getPath(req->pathname());
        // debugprintf("[GetFileStat]: filepath = %s\n", filepath.c_str());

        auto stat = readStat(filepath);
        resp->mutable_stat()->CopyFrom(stat);
        // debugprintf("[GetFileStat]: Function ended.\n");
        return Status::OK;
      } catch (const ProtocolException& e) {
        debugprintf("[GetFileStat]: Protocol Exception: %d %s\n", e.get_code(), e.what());
        return Status(e.get_code(), e.what());
      } catch(const FileSystemException& e) {
        debugprintf("[GetFileStat]: System Exception: %d\n", e.get_fs_errno());
        resp -> set_fs_errno(e.get_fs_errno());
        return Status::OK;
      } catch (const std::exception& e) {
        debugprintf("[GetFileStat]: Unexpected Exception %s\n", e.what());
        return Status(StatusCode::UNKNOWN, e.what());
      }
    }

    Status Access(ServerContext* context, const AccessRequest* req, AccessResponse* resp) override {
      debugprintf("[Access]: Function entered.\n");
      try {
        path filepath = getPath(req->pathname());
        int mode = req->mode();
        debugprintf("[Access]: filepath = %s\n", filepath.c_str());

        int ret = access(filepath.c_str(), mode);
        if(ret != 0) {
          resp->set_fs_errno(errno);
        }
        debugprintf("[Access]: Function ended.\n");
        return Status::OK;
      } catch (const FileSystemException& e) {
        debugprintf("[Access]: System Exception: %d\n", e.get_fs_errno());
        resp->set_fs_errno(e.get_fs_errno());
        return Status::OK;
      } catch (const std::exception& e) {
        debugprintf("[GetFileStat]: Unexpected Exception %s\n", e.what());
        return Status(StatusCode::UNKNOWN, e.what());
      }
    }

    Status Fetch(ServerContext* context, const FetchRequest* req, FetchResponse* resp) override {
      debugprintf("[Fetch] Function entered.\n");
      path filepath = getPath(req->pathname());
      try {
        auto content = readFile(filepath);
        resp->mutable_time_modified()->CopyFrom(readModifyTime(filepath));
        resp->set_file_contents(content);
        debugprintf("[Fetch] Function ended.\n");
        return Status::OK;
      } catch (const ProtocolException& e) {
        debugprintf("[Fetch]: Protocol Exception: %d: %s\n", e.get_code(), e.what());
        return Status(e.get_code(), e.what());
      } catch(const FileSystemException& e) {
        debugprintf("[Fetch]: System Exception: %d\n", e.get_fs_errno());
        resp -> set_fs_errno(e.get_fs_errno());
        return Status::OK;
      } catch (const std::exception& e) {
        debugprintf("[Fetch]: Unexpected Exception: %s\n", e.what());
        return Status(StatusCode::UNKNOWN, e.what());
      }
    }

    Status TestAuth(ServerContext* context, const TestAuthRequest* req, TestAuthResponse* resp) override {
      // debugprintf("[TestAuth]: Function entered.\n");
      try {
        path filepath = getPath(req->pathname());
        // debugprintf("[TestAuth]: filepath = %s\n", filepath.c_str());
        
        auto ts_server = readModifyTime(filepath);
        auto ts_client = req->time_modified();
        bool file_changed = (ts_server.sec() > ts_client.sec()) || 
          (ts_server.sec() == ts_client.sec() && ts_server.nsec() > ts_client.nsec());
        printf("[TestAuth]: Checking group lock for file: %s\n", filepath.c_str());
        bool file_group_locked = checkFileInGroup(filepath.c_str()) && groupLocked; //return true if file is locked

        resp->set_file_changed(file_changed);
        resp->set_file_group_locked(file_group_locked);
        // debugprintf("[TestAuth]: Function ended.\n");
        return Status::OK;
      } catch (const ProtocolException& e) {
        debugprintf("[TestAuth]: Protocol Exception: %d %s\n", e.get_code(), e.what());
        return Status(e.get_code(), e.what());
      } catch(const FileSystemException& e) {
        debugprintf("[TestAuth]: System Exception: %d\n", e.get_fs_errno());
        resp -> set_file_changed(true); 
        return Status::OK;
      } catch (const std::exception& e) {
        debugprintf("[TestAuth]: Unexpected Exception %s\n", e.what());
        return Status(StatusCode::UNKNOWN, e.what());
      }
    }
    
    Status MakeDir(ServerContext* context, const MakeDirRequest* request, MakeDirResponse* reply) override {
      printf("MakeDir: Entering function\n");

      try {
        path filepath = getPath(request->pathname());
        printf("MakeDir: filepath = %s\n", filepath.c_str());
        make_dir(filepath, request->mode());
        printf("MakeDir: Exiting function on Success path\n");
        return Status::OK;
      } catch (const ProtocolException& e) {
        printf("[Protocol Exception: %d] %s\n", e.get_code(), e.what());
        printf("MakeDir: Exiting function on ProtocolException path\n");
        return Status(e.get_code(), e.what());
      } catch(const FileSystemException& e) {
        printf("[System Exception: %d]\n", e.get_fs_errno());
        reply -> set_fs_errno(e.get_fs_errno());
        return Status::OK;
      } catch (const std::exception& e) {
        errprintf("[Unexpected Exception] %s\n", e.what());
        printf("MakeDir: Exiting function on Exception path\n");
        return Status(StatusCode::UNKNOWN, e.what());
      }
    }

    Status RemoveDir(ServerContext* context, const RemoveDirRequest* request, RemoveDirResponse* reply) override {
      printf("RemoveDir: Entering function\n");
      try {
        path filepath = getPath(request->pathname());
        printf("RemoveDir: filepath = %s\n", filepath.c_str());
        remove_dir(filepath);
        printf("RemoveDir: Exiting function on Success path\n");
        return Status::OK;
      } catch (const ProtocolException& e) {
        printf("[Protocol Exception: %d] %s\n", e.get_code(), e.what());
        printf("RemoveDir: Exiting function on ProtocolException path\n");
        return Status(e.get_code(), e.what());
      } catch(const FileSystemException& e) {
        printf("[System Exception: %d]\n", e.get_fs_errno());
        reply -> set_fs_errno(e.get_fs_errno());
        return Status::OK;
      } catch (const std::exception& e) {
        errprintf("[Unexpected Exception] %s\n", e.what());
        printf("RemoveDir: Exiting function on Exception path\n");
        return Status(StatusCode::UNKNOWN, e.what());
      }
    }

    Status ReadDir(ServerContext* context, const ListDirRequest* request, ListDirResponse* reply) override {
      debugprintf("ListDir: Entering function\n");
      try {
        path filepath = getPath(request->pathname());
        printf("ListDir: filepath = %s\n", filepath.c_str());
        list_dir(filepath, reply);
        printf("ListDir: Exiting function on Success path\n");
        return Status::OK;
      } catch (const ProtocolException& e) {
        printf("[Protocol Exception: %d] %s\n", e.get_code(), e.what());
        printf("ListDir: Exiting function on ProtocolException path\n");
        return Status(e.get_code(), e.what());
      } catch(const FileSystemException& e) {
        printf("[System Exception: %d]\n", e.get_fs_errno());
        reply -> set_fs_errno(e.get_fs_errno());
        return Status::OK;
      } catch (const std::exception& e) {
        errprintf("[Unexpected Exception] %s\n", e.what());
        printf("ListDir: Exiting function on Exception path\n");
        return Status(StatusCode::UNKNOWN, e.what());
      }
    }


    Timestamp convert_timestamp(timespec raw) {
        Timestamp a;
        a.set_sec(raw.tv_sec);
        a.set_nsec(raw.tv_nsec);
        return a;
    }

    Status Store(ServerContext* context, const StoreRequest* request, StoreResponse* reply) override {
        debugprintf("Store: Entering function\n");
        try {
            path filepath = getPath(request->pathname());
            debugprintf("Store: filepath = %s\n", filepath.c_str());

            writeFile(filepath, request->file_contents());
            
            reply->mutable_time_modify()->CopyFrom(readModifyTime(filepath));
            debugprintf("Store: Exiting function on Success path\n");
            return Status::OK;

        } catch (const ProtocolException& e) {
            debugprintf("[Protocol Exception: %d] %s\n", e.get_code(), e.what());
            debugprintf("Store: Exiting function on ProtocolException path\n");
            return Status(e.get_code(), e.what());
        } catch(const FileSystemException& e) {
            debugprintf("[System Exception: %d]\n", e.get_fs_errno());
            reply -> set_fs_errno(e.get_fs_errno());
            return Status::OK;
        } catch (const std::exception& e) {
            errprintf("[Unexpected Exception] %s\n", e.what());
            debugprintf("Store: Exiting function on Exception path\n");
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }
    Status Create(ServerContext* context, const CreateRequest* request, CreateResponse* reply) override {
        debugprintf("Create: Entering function\n");

        try {
            path filepath = getPath(request->pathname());
            debugprintf("Create: filepath = %s\n", filepath.c_str());
            
            int ret = open(filepath.c_str(), request->flags(), request->mode());
            debugprintf("Create: Exiting function.\n");
            return Status::OK;
        } catch (const ProtocolException& e) {
            debugprintf("Create: [Protocol Exception: %d] %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        } catch(const FileSystemException& e) {
            debugprintf("Create: [System Exception: %d]\n", e.get_fs_errno());
            reply -> set_fs_errno(e.get_fs_errno());
            return Status::OK;
        } catch (const std::exception& e) {
            errprintf("Create: Unexpected Exception] %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }
    void delete_file(path filepath) {
        debugprintf("delete_file: Entering function\n");
        if (unlink(filepath.c_str()) == -1) {
            debugprintf("delete_file: Exiting function\n");
            throw FileSystemException(errno);
        }
        debugprintf("delete_file: Exiting function\n");
    }
    Status Remove(ServerContext* context, const RemoveRequest* request, RemoveResponse* reply) override {
        debugprintf("Remove: Entering function\n");
        try {
            path filepath = getPath(request->pathname());
            cout << "Remove: filepath = " << filepath << endl;

            // auto lock = locks.GetWriteLock(filepath.string());
            
            delete_file(filepath);

            debugprintf("Remove: Exiting function on Success path\n");
            return Status::OK;
        } catch (const ProtocolException& e) {
            debugprintf("[Protocol Exception: %d] %s\n", e.get_code(), e.what());
            debugprintf("Remove: Exiting function on ProtocolException path\n");
            return Status(e.get_code(), e.what());
        } catch(const FileSystemException& e) {
            debugprintf("[System Exception: %d]\n", e.get_fs_errno());
            reply -> set_fs_errno(e.get_fs_errno());
            return Status::OK;
        } catch (const std::exception& e) {
            errprintf("[Unexpected Exception] %s\n", e.what());
            debugprintf("Remove: Exiting function on Exception path\n");
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status StoreUsingStream(ServerContext* context, ServerReader<StoreRequest> * reader, StoreResponse* reply) override {
      debugprintf("StoreUsingStream: Entering function\n");
      StoreRequest request;
      std::ofstream file;
      path filepath;
      int i = 0;
      try {
        reader -> Read(&request);
        // Read first request in stream
        // if(!reader -> Read(&request)) {
        //   throw ProtocolException("Empty stream", StatusCode::CANCELLED);
        // }
        
        // Note: this assumes all requests in the stream are to the same pathname
        filepath = getPath(request.pathname());
        debugprintf("StoreUsingStream: filepath = %s\n", filepath.c_str());
        
        // Check that this is a valid destination
        uint pre_err = check_valid_write_destination(filepath);
        if(pre_err != 0) {
          throw FileSystemException(pre_err);
        }

        path temppath = get_tempfile_path(filepath);
        
        file.open(temppath, std::ios::binary);

        unsigned long iter = 0;
        unsigned long bytes = 0;
        
        string chunk;
        
        do {
          chunk = request.file_contents();
          file << chunk;
          iter++;
          bytes += chunk.length();
          // debugprintf("StoreUsingStream: Read message [iter %ld, total %ld B]\n",iter,bytes);
        } while (reader->Read(&request));

        file.close();

        // Overwrite dest file with temp file
        if (rename(temppath.c_str(), filepath.c_str()) == -1) {
          debugprintf("StoreUsingStream: Exiting function on error committng temp file\n");
          throw ProtocolException("Error committing temp file", StatusCode::UNKNOWN);
        }

        auto time = readModifyTime(filepath);
        reply->mutable_time_modify()->CopyFrom(time);
        debugprintf("StoreUsingStream: Exiting function\n");
        
        // Release Group Lock
        if(checkFileInGroup(filepath.c_str())){
          setFileGroupLocked(0);
          printf("[StoreUsingStream]File: %s was a part of Atomic Lock Group. \n", filepath.c_str());
        }

        return Status::OK; 
      } catch (const ProtocolException& e) {
        debugprintf("StoreUsingStream: [Protocol Exception: %d] %s\n", e.get_code(), e.what());
        return Status(e.get_code(), e.what());
      } catch(const FileSystemException& e) {
        debugprintf("StoreUsingStream: [System Exception: %d]\n", e.get_fs_errno());
        reply -> set_fs_errno(e.get_fs_errno());
        return Status::OK;
      } catch (const std::exception& e) {
        debugprintf("StoreUsingStream: [Unexpected Exception] %s\n", e.what());
        return Status(StatusCode::UNKNOWN, e.what());
      }
    }

    Status FetchUsingStream(ServerContext* context, const FetchRequest* request, ServerWriter<FetchResponse>* writer) override {
      try {
        path filepath = getPath(request->pathname());
        debugprintf("FetchUsingStream: filepath = %s\n", filepath.c_str());
        FetchResponse reply;
        //Set Group Lock
        if(checkFileInGroup(filepath.c_str())){
          printf("[FetchUsingStream]File: %s is a part of Atomic Lock Group. \n", filepath.c_str());
          setFileGroupLocked(1);
        }
        
        // Set response
        reply.mutable_time_modified()->CopyFrom(readModifyTime(filepath));
        
        // Get total chunks
        std::ifstream fin(filepath.c_str(), std::ios::binary);

        struct stat st;
        stat(filepath.c_str(), &st);
        int fileSize = st.st_size;
        int totalChunks = 0;
        totalChunks = fileSize / CHUNK_SIZE;
        bool aligned = true;
        int lastChunkSize = CHUNK_SIZE;
        if (fileSize % CHUNK_SIZE) {
          totalChunks++;
          aligned = false;
          lastChunkSize = fileSize % CHUNK_SIZE;
        }
            
        debugprintf("FetchUsingStream: fileSize = %d\n", fileSize);
        debugprintf("FetchUsingStream: totalChunks = %d\n", totalChunks);
        debugprintf("FetchUsingStream: lastChunkSize = %d\n", lastChunkSize);
            
        unsigned long bytes=0;
        unsigned long bytes_read = 0;
        for (size_t chunk = 0; chunk < totalChunks; chunk++) {
          size_t currentChunkSize = (chunk == totalChunks - 1 && !aligned) ? 
                                        lastChunkSize : CHUNK_SIZE;
              
          char * buffer = new char [currentChunkSize];
          if (fin.read(buffer, currentChunkSize)) {
            auto bcnt = fin.gcount();
            bytes += currentChunkSize;
            bytes_read += bcnt;
            
            reply.set_file_contents(buffer, bcnt);
            
            // debugprintf("FetchUsingStream: Read chunk [iter %ld, expect %ld B, read %ld B]\n",chunk, bytes, bytes_read);
            writer->Write(reply);
          }
        }

        fin.close();
        return Status::OK;
      } catch (const ProtocolException& e) {
        debugprintf("FetchUsingStream [Protocol Exception: %d] %s\n", e.get_code(), e.what());
        return Status(e.get_code(), e.what());
      } catch (const std::exception& e) {
        debugprintf("FetchUsingStream [Unexpected Exception] %s\n", e.what());
        return Status(StatusCode::UNKNOWN, e.what());
      }
    }

    Status Rename(ServerContext* context, const RenameRequest* request, RenameResponse* reply) override {
        debugprintf("Rename: Entering function\n");
        try {
            path srcpath = getPath(request->pathname());
            path dstpath = getPath(request->componentname());

            debugprintf("Rename: srcpath = %s, dstpath = %s\n", srcpath.c_str(), dstpath.c_str());

            move_file(srcpath, dstpath);

            debugprintf("Rename: Exiting function on Success path\n");
            return Status::OK;
        } catch (const ProtocolException& e) {
            debugprintf("[Protocol Exception: %d] %s\n", e.get_code(), e.what());
            debugprintf("Rename: Exiting function on ProtocolException path\n");
            return Status(e.get_code(), e.what());
        } catch(const FileSystemException& e) {
            debugprintf("[System Exception: %d]\n", e.get_fs_errno());
            reply -> set_fs_errno(e.get_fs_errno());
            return Status::OK;
        } catch (const std::exception& e) {
            errprintf("[Unexpected Exception] %s\n", e.what());
            debugprintf("Rename: Exiting function on Exception path\n");
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }
};

void RunServer(path root) {
  std::string server_address("0.0.0.0:50052");
  AFSImpl service(root);

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stdout, "Usage: ./afs_server root_folder\n");
    return -1;
  }

  auto root = fs::canonical(argv[1]);
  RunServer(root);
  return 0;
}
