#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>


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
using afs::MknodRequest;
using afs::MknodResponse;
using afs::RemoveDirRequest;
using afs::RemoveDirResponse;
using afs::RemoveRequest;
using afs::RemoveResponse;
using afs::RenameRequest;
using afs::RenameResponse;
using afs::StoreRequest;
using afs::StoreResponse;
using afs::TestAuthRequest;
using afs::TestAuthResponse;
using afs::Timestamp;


class AFSImpl final : public FileSystemService::Service {
 public:
    explicit AFSImpl(const std::string& db) {
        // afs::ParseDb(db, &feature_list_);
    }

    Status Ping(ServerContext* context, const PingMessage* request, PingMessage* reply) override {
        return Status::OK;
    }
};

void RunServer(const std::string& db_path) {
  std::string server_address("0.0.0.0:50051");
  AFSImpl service(db_path);

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer("hello");

  return 0;
}
