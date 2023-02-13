#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <sys/stat.h>
#include <filesystem>

//#include "locks.h"
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

/******************************************************************************
 * MACROS
 *****************************************************************************/
#define DEBUG 1
#define debugprintf(...)       \
    if (DEBUG) {             \
        printf(__VA_ARGS__); \
    }
#define dbgprintf(...)       \
    if (DEBUG) {             \
        printf(__VA_ARGS__); \
    }
#define errprintf(...) \
    { printf(__VA_ARGS__); }

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

/******************************************************************************
 * GLOBALS
 *****************************************************************************/
// For Performance
// stores filepath, timestamp, content of insertion
// TODO: Add reader/writer locks
std::map<fs::path, std::pair<time_t, std::string>> mem_map; 
static const string TEMP_FILE_EXT = ".afs_tmp";

/******************************************************************************
 * EXCEPTION HANDLER
 *****************************************************************************/
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

/******************************************************************************
 * gRPC SYNC SERVER IMPLEMENTATION
 *****************************************************************************/
class AFSImpl final : public FileSystemService::Service {
//  MutexMap locks;
//
  path root;

  path getPath(string relativePath) {
    path normalPath = (root / relativePath).lexically_normal();
    debugprintf("getPath: root = %s\n", root.c_str());
    debugprintf("getPath: relative path = %s\n", relativePath.c_str());
    debugprintf("getPath: Normalised path = %s\n", normalPath.c_str());
    
    // error checking 
    auto [a, b] = std::mismatch(root.begin(), root.end(), normalPath.begin());
    if (a != root.end()) {
      throw ProtocolException("Normalized path is outside storage root", StatusCode::INVALID_ARGUMENT);
    }

    return normalPath;
  }

  path to_storage_path(string relative) {
        dbgprintf("to_storage_path: root = %s\n", root.c_str());
        dbgprintf("to_storage_path: relative = %s\n", relative.c_str());
        path normalized = (root / relative).lexically_normal();
        dbgprintf("to_storage_path: normalized = %s\n", normalized.c_str());
        // Check that this path is under our storage root
        auto [a, b] = std::mismatch(root.begin(), root.end(), normalized.begin());
        if (a != root.end()) {
            throw ProtocolException("Normalized path is outside storage root", StatusCode::INVALID_ARGUMENT);
        }

        if (normalized.extension() == TEMP_FILE_EXT) {
            throw ProtocolException("Attempting to access file with a reserved extension", StatusCode::INVALID_ARGUMENT);
        }

        return normalized;  
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
      case fs::file_type::directory: // filepath has directory
        return EISDIR;
      default:
        return EPERM; // Other types of file, we throw a permission error
    }
  }

  string readFile(path filepath) {
    debugprintf("readFile: Entering function\n");

    // Check that path exists and is a file before proceeding
    uint chkFileErr = checkReadLocation(filepath);
    if(chkFileErr != 0) {
        throw FileSystemException(chkFileErr);
    }
    
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    std::ostringstream buffer;
    buffer << file.rdbuf();

    if (file.fail()) {
      debugprintf("readFile: Exiting function with failure\n");
      throw ProtocolException("Error reading file", StatusCode::UNKNOWN);
    }

    debugprintf("readFile: Exiting function\n");
    return buffer.str();
  }

  FileStat read_stat(path filepath) {
    struct stat sb;
    FileStat ret;

    if (stat(filepath.c_str(), &sb) == -1) {
      debugprintf("read_stat: failed\n");
      debugprintf("read_stat: errno = %d\n", errno);
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
        dbgprintf("make_dir: Entering function\n");
        if (mkdir(filepath.c_str(), mode) == -1) {
            dbgprintf("make_dir: Exiting function\n");
            throw FileSystemException(errno);
            // switch (errno) {
            //     case EEXIST:
            //         throw ProtocolException("Path already exists", StatusCode::ALREADY_EXISTS);
            //     case ENOENT:
            //         throw ProtocolException("Missing directory in path prefix", StatusCode::NOT_FOUND);
            //     case ENOTDIR:
            //         throw ProtocolException("Non-directory in path prefix", StatusCode::FAILED_PRECONDITION);
            //     default:
            //         throw ProtocolException("Error in call to mkdir", StatusCode::UNKNOWN);
            // }
        }
        dbgprintf("make_dir: Exiting function\n");
    }

    void remove_dir(path filepath) {
        if (rmdir(filepath.c_str()) == -1) {
            throw FileSystemException(errno);
            // switch (errno) {
            //     case EEXIST:
            //     case ENOTEMPTY:
            //         throw ProtocolException("Attempting to remove non-empty directory", StatusCode::FAILED_PRECONDITION);
            //     case EINVAL:
            //         throw ProtocolException("Invalid directory name", StatusCode::INVALID_ARGUMENT);
            //     case ENOENT:
            //         throw ProtocolException("Missing directory in path", StatusCode::NOT_FOUND);
            //     case ENOTDIR:
            //         throw ProtocolException("Non-directory in path", StatusCode::FAILED_PRECONDITION);
            //     default:
            //         throw ProtocolException("Error in call to mkdir", StatusCode::UNKNOWN);
            // }
        }
    }

    void list_dir(path filepath, ListDirResponse* reply) {
        dbgprintf("list_dir: Entered function\n");
        // TODO catch errors from directory_iterator
        for (const auto& entry : fs::directory_iterator(filepath)) {
            DirectoryEntry* msg = reply->add_entries();
            msg->set_file_name(entry.path().filename());
            msg->set_mode(is_regular_file(entry.status()) ? FileMode::REG : is_directory(entry.status()) ? FileMode::DIR
                                                                                         : FileMode::UNSUPPORTED);
            msg->set_size(is_regular_file(entry.status()) ? file_size(entry) : 0);
        }
        dbgprintf("list_dir: Exiting function\n");
    }

 public:
    explicit AFSImpl(path root) : root(root) {}

    Status Ping(ServerContext* context, const PingMessage* request, PingMessage* reply) override {
        return Status::OK;
    }

    Status GetFileStat(ServerContext* context, const GetFileStatRequest* request, GetFileStatResponse* reply) override {
      debugprintf("GetFileStat: Entering function\n");
      try {
        path filepath = getPath(request->pathname());
        debugprintf("GetFileStat: filepath = %s\n", filepath.c_str());

        auto status = read_stat(filepath);
        reply->mutable_status()->CopyFrom(status);
        return Status::OK;
      } catch (const ProtocolException& e) {
        debugprintf("[Protocol Exception: %d] %s\n", e.get_code(), e.what());
        return Status(e.get_code(), e.what());
      } catch(const FileSystemException& e) {
        debugprintf("[System Exception: %d]\n", e.get_fs_errno());
        reply -> set_fs_errno(e.get_fs_errno());
        return Status::OK;
      } catch (const std::exception& e) {
        debugprintf("[Unexpected Exception] %s\n", e.what());
        return Status(StatusCode::UNKNOWN, e.what());
      }
    }

    Status Fetch(ServerContext* context, const FetchRequest* request, FetchResponse* reply) override {
      path filepath = getPath(request->pathname());
      try {
        auto content = readFile(filepath);
        reply->set_file_contents(content);
        reply->mutable_time_modify()->CopyFrom(readModifyTime(filepath));
        return Status::OK;
      } catch (const ProtocolException& e) {
        debugprintf("Protocol Exception: %d %s\n", e.get_code(), e.what());
        return Status(e.get_code(), e.what());
      } catch(const FileSystemException& e) {
        debugprintf("System Exception: %d\n", e.get_fs_errno());
        reply -> set_fs_errno(e.get_fs_errno());
        return Status::OK;
      } catch (const std::exception& e) {
        debugprintf("Unexpected Exception: %s\n", e.what());
        return Status(StatusCode::UNKNOWN, e.what());
      }
    }

    Status TestAuth(ServerContext* context, const TestAuthRequest* request, TestAuthResponse* reply) override {
      debugprintf("TestAuth: Entering function\n");
      try {
        path filepath = getPath(request->pathname());
        debugprintf("TestAuth: filepath = %s\n", filepath.c_str());
        
        auto ts0 = readModifyTime(filepath);
        auto ts1 = request->time_modify();
        bool changed = (ts0.sec() != ts1.sec()) || (ts0.nsec() != ts1.nsec());

        reply->set_has_changed(changed);
        debugprintf("TestAuth: Exiting function on Success path\n");
        return Status::OK;
      } catch (const ProtocolException& e) {
        debugprintf("[Protocol Exception: %d] %s\n", e.get_code(), e.what());
        debugprintf("TestAuth: Exiting function on ProtocolException path\n");
        return Status(e.get_code(), e.what());
      } catch(const FileSystemException& e) {
        debugprintf("[System Exception: %d]\n", e.get_fs_errno());
        // Instead of reporting filesystem error, invalidate the cache entry
        reply -> set_has_changed(true); 
        return Status::OK;
      } catch (const std::exception& e) {
        debugprintf("[Unexpected Exception] %s\n", e.what());
        debugprintf("TestAuth: Exiting function on Exception path\n");
        return Status(StatusCode::UNKNOWN, e.what());
      }
    }
    
    Status MakeDir(ServerContext* context, const MakeDirRequest* request, MakeDirResponse* reply) override {
        printf("MakeDir: Entering function\n");

        try {
            path filepath = to_storage_path(request->pathname());
            printf("TestAuth: filepath = %s\n", filepath.c_str());

     //       auto lock = locks.GetWriteLock(filepath.string());
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
            path filepath = to_storage_path(request->pathname());
            printf("RemoveDir: filepath = %s\n", filepath.c_str());

            // NOTE: this doesn't wait for any locks in child paths.
            // This would be an issue in the edge case where we receive a request
            // to delete the last file in a folder immediately before a request to delete
            // the folder itself; in this case, the folder might not be deleted as it should be.
   //         auto lock = locks.GetWriteLock(filepath.string());
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
        dbgprintf("ListDir: Entering function\n");
        try {
            path filepath = to_storage_path(request->pathname());
            printf("ListDir: filepath = %s\n", filepath.c_str());

            // NOTE: this doesn't wait for any locks in child paths.
            // This might cause problems if child files are being created/deleted
            // during iteration.
 //           auto lock = locks.GetReadLock(filepath.string());

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
};

void RunServer(path root) {
  std::string server_address("0.0.0.0:50051");
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
