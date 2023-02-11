#include <grpcpp/grpcpp.h>
#include "afs.grpc.pb.h"

using grpc::Channel;
using afs::FileSystemService;

class FileSystemClient {
  public:
    FileSystemClient(std::shared_ptr<Channel> channel);
    int Ping(int * round_trip_time);

  private:
      std::unique_ptr<FileSystemService::Stub> stub_;
};