#include <grpcpp/grpcpp.h>
#include "afs.grpc.pb.h"

using grpc::Channel;
using afs::FileSystemService;
using grpc::Status;
using afs::TestAuthResponse;

struct TestAuthReturnType
{
  Status status;
  TestAuthResponse response;
  TestAuthReturnType(Status status,
                    TestAuthResponse response) :
                    status(status),
                    response(response)
                    {}
}; 

class FileSystemClient {
  public:
    FileSystemClient(std::shared_ptr<Channel> channel);
    int Ping(int * round_trip_time);
    int GetFileStat(std::string path, struct stat *stbuf, char* root);
    int OpenFile(std::string path, std::string root);
    TestAuthReturnType TestAuth(std::string path, std::string root);

  private:
      std::unique_ptr<FileSystemService::Stub> stub_;
};