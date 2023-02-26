#include <grpcpp/grpcpp.h>
#include "afs.grpc.pb.h"
#include <fuse.h>
//#include <unreliablefs_ops.h>

using namespace std;
using grpc::Channel;
using afs::FileSystemService;
using grpc::Status;
using afs::TestAuthResponse;

struct TestAuthReturn
{
  Status status;
  TestAuthResponse response;
  TestAuthReturn(Status status, TestAuthResponse response) : status(status), response(response) {}
}; 

class FileSystemClient {
  public:
    FileSystemClient(std::shared_ptr<Channel> channel);
    int Ping(int * round_trip_time);
    int GetFileStat(std::string path, struct stat *buf, std::string root);
    int OpenFile(std::string path, std::string root);
    int CloseFile(int fd, std::string path, std::string root);
    int CreateFile(std::string abs_path, std::string root, mode_t mode, int flags);
    int DeleteFile(std::string abs_path, std::string root);
    int Access(std::string abs_path, int mode, std::string root);
    int MakeDir(std::string abs_path, std::string root, mode_t mode);
    int RemoveDir(std::string abs_path, std::string root);
    int ReadDir(std::string abs_path, std::string root, void *buf, fuse_fill_dir_t filler);
    int Rename(std:: string abs_path, std::string new_name, std::string root);

    int CloseFileUsingStream(int fd, std::string abs_path, std::string root);
    int OpenFileUsingStream(std::string abs_path, std::string root, int flags);

    TestAuthReturn TestAuth(std::string path, std::string root);

  private:
    std::unique_ptr<FileSystemService::Stub> stub_;
    unordered_map<std::string, struct timespec> open_map;
};
