#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

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

#include "afs.grpc.pb.h"
#include "afs_client.hh"

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
};


// g++ -std=c++17 afs_client.cc afs.pb.cc afs.grpc.pb.cc -o client `pkg-config --libs --cflags protobuf grpc_cpp_plugin`

// g++ client.o -L/usr/local/lib `pkg-config --libs --static protobuf grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl afs_client.cc

// g++ -std=c++11 -I ./ -I ./ afs_client.cc afs.pb.cc afs.grpc.pb.cc -L libprotobuf -lprotobuf -L grpc++ -lgrpc++ -lgrpc -o afs_client.o
