#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
// grpc code here

#include <grpcpp/grpcpp.h>
#include <string>
#include <chrono>
#include <ctime>
#include "afs.grpc.pb.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <utime.h>
#include <fstream>
#include <sstream>

// #include <grpc/grpc.h>
// #include <grpcpp/channel.h>
// #include <grpcpp/client_context.h>
// #include <grpcpp/create_channel.h>
// #include <grpcpp/security/credentials.h>
// #ifdef BAZEL_BUILD
// #include "examples/protos/route_guide.grpc.pb.h"
// #else
// #include "route_guide.grpc.pb.h"
// #endif

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
#define RETRY_TIME_START      1                                     // in seconds
#define RETRY_TIME_MULTIPLIER 2                                     // for rpc retry w backoff
#define MAX_RETRY             5                                     // rpc retry

class FileSystemClient {
 public:
//  RouteGuideClient(std::shared_ptr<Channel> channel, const std::string& db)
//       : stub_(RouteGuide::NewStub(channel)) {
//     routeguide::ParseDb(db, &feature_list_);
//   }
  FileSystemClient(std::shared_ptr<Channel> channel)
      : stub_(FileSystemService::NewStub(channel)) {

        std::cout << "-------------- Helloo --------------" << std::endl;
      }

    FileSystemClient* initGRPC() {
        FileSystemClient* client = new FileSystemClient(
            grpc::CreateChannel("localhost:50051",
                                grpc::InsecureChannelCredentials()));

        std::cout << "-------------- Ping --------------" << std::endl;
        std::chrono::nanoseconds ping_time;
        client->Ping(&ping_time);
        

        return client;
    }


    int Ping(std::chrono::nanoseconds * round_trip_time) {
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
            *round_trip_time = end-start;
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

    private:
      std::unique_ptr<FileSystemService::Stub> stub_;
};

// FileSystemClient* initGRPC() {
// FileSystemClient* client = new FileSystemClient(
//       grpc::CreateChannel("localhost:50051",
//                           grpc::InsecureChannelCredentials()));

//   std::cout << "-------------- Ping --------------" << std::endl;
//   std::chrono::nanoseconds ping_time;
//   client->Ping(&ping_time);
  

//   return client;
// }

// int main(int argc, char** argv) {
//   FileSystemClient client(
//       grpc::CreateChannel("localhost:50051",
//                           grpc::InsecureChannelCredentials()));

//   std::cout << "-------------- Ping --------------" << std::endl;
//   std::chrono::nanoseconds ping_time;
//   client.Ping(&ping_time);
  

//   return 0;
// }


// g++ -std=c++17 afs_client.cc afs.pb.cc afs.grpc.pb.cc -o client `pkg-config --libs --cflags protobuf grpc_cpp_plugin`

// g++ client.o -L/usr/local/lib `pkg-config --libs --static protobuf grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl afs_client.cc

// g++ -std=c++11 -I ./ -I ./ afs_client.cc afs.pb.cc afs.grpc.pb.cc -L libprotobuf -lprotobuf -L grpc++ -lgrpc++ -lgrpc -o afs_client.o
