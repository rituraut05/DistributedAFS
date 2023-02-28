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
    int GetFileStat(string path, struct stat *buf, string root);
    int OpenFile(string path, string root);
    int CloseFile(int fd, string path, string root);
    int CreateFile(string abs_path, string root, int mode, int flags);
    int DeleteFile(string abs_path, string root);
    int Access(string abs_path, int mode, string root);
    int MakeDir(string abs_path, string root, int mode);
    int RemoveDir(string abs_path, string root);
    int ReadDir(string abs_path, string root, void *buf, fuse_fill_dir_t filler);
    int Rename(std:: string abs_path, string new_name, string root);

    int CloseFileUsingStream(int fd, string abs_path, string root);
    int OpenFileUsingStream(string abs_path, string root, int flags);

    TestAuthReturn TestAuth(string path, string root);

  private:
    std::unique_ptr<FileSystemService::Stub> stub_;
    unordered_map<string, struct timespec> open_map;
};
