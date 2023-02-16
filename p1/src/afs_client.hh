#include <grpcpp/grpcpp.h>
#include "afs.grpc.pb.h"
#include <fuse.h>
//#include <unreliablefs_ops.h>

using namespace std;
using grpc::Channel;
using afs::FileSystemService;
using grpc::Status;
using afs::TestAuthResponse;

struct TestAuthReturnType
{
  Status status;
  TestAuthResponse response;
  TestAuthReturnType(Status status, TestAuthResponse response) : status(status), response(response) {}
}; 

class FileSystemClient {
  public:
    FileSystemClient(std::shared_ptr<Channel> channel);
    int Ping(int * round_trip_time);
    int GetFileStat(std::string path, struct stat *stbuf, std::string root);
    int OpenFile(std::string path, std::string root);
    TestAuthReturnType TestAuth(std::string path, std::string root);
    
    int MakeDir(std::string abs_path, std::string root, mode_t mode);
    int RemoveDir(std::string abs_path, std::string root);
    int ReadDir(std::string abs_path, std::string root, void *buf, fuse_fill_dir_t filler);

  private:
    std::unique_ptr<FileSystemService::Stub> stub_;
};
