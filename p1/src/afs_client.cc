#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <unordered_map>
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
using afs::AccessRequest;
using afs::AccessResponse;
using afs::ReadDirRequest;
using afs::ReadDirResponse;
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
using std::vector;
using afs::CreateRequest;
using afs::CreateResponse;
using afs::RenameRequest;
using afs::RenameResponse;

#define CHUNK_SIZE 4096 // for streaming

string get_relative_path(string path, string root)
{
    string path_copy = path;
    path_copy.erase(0, root.length());
    return path_copy;
}

bool file_exists(std::string path)
{
    struct stat s;
    return (stat(path.c_str(), &s) == 0);
}

int GetModifyTime(std::string path, timespec* t)
{
    struct stat sb;
    if (stat(path.c_str(), &sb) == -1) {
        printf("[GetModifyTime]: Failed to stat file.\n");
        return -1;
    }
    printf("[GetModifyTime]: Function ended.\n");
    *t = sb.st_mtim;
    return 0;
}

int set_file_open_time(int fd, timespec t)
{
    struct timespec p[2] = { t, t };
    return futimens(fd, p);
}

int set_timings(string path, timespec t)
{
    struct timespec p[2] = { t, t };
    return utimensat(AT_FDCWD, path.c_str(), p, 0);
}

vector<string> tokenize_path(string path, char delim, bool is_file)
{
    vector<string> tokens;
    string temp = "";

    for (int i = 0; i < path.length(); i++) {
        if (path[i] == delim) {
            tokens.push_back(temp);
            temp = "";
        }
        else
            temp = temp + (path.c_str())[i];
    }

    if (!is_file)
        tokens.push_back(temp);

    return tokens;
}

int create_path(string relative_path, bool is_file, string root)
{
    string base_path = root;
    vector<string> tokens = tokenize_path(relative_path, '/', is_file);
    for (auto token : tokens) {
        base_path += token + "/";
        struct stat s;
        int r = stat(base_path.c_str(), &s);
        if (r != 0 && errno == 2) {
            printf("[create_path]: stat() ENOENT\n");
            if (mkdir(base_path.c_str(), S_IRWXU) != 0) {
                printf("[create_path]: mkdir() failed : %d\n", errno);
                return -1;
            }
        }
        else if (r != 0) {
            printf("[create_path]: stat() failed : %d\n", errno);
            return -1;
        }
    }
    printf("[create_path]: Function ended.\n");
    return 0;
}

string readFileIntoString(string path)
{
    ifstream input_file(path);
    if (!input_file.is_open()) {
        return string();
    }
    return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

int transform_rpc_err(grpc::StatusCode code)
{
    switch (code) {
    case StatusCode::OK:
        return 0;
    case StatusCode::INVALID_ARGUMENT:
        return EINVAL;
    case StatusCode::NOT_FOUND:
        return ENOENT;
    case StatusCode::DEADLINE_EXCEEDED:
    case StatusCode::UNAVAILABLE:
        return ETIMEDOUT;
    case StatusCode::ALREADY_EXISTS:
        return EEXIST;
    case StatusCode::PERMISSION_DENIED:
    case StatusCode::UNAUTHENTICATED:
        return EPERM;
    default:
        return EIO;
    }
}

extern "C" {
FileSystemClient::FileSystemClient(std::shared_ptr<Channel> channel)
    : stub_(FileSystemService::NewStub(channel))
{
    open_map.clear();
    std::cout << "-------------- Helloo --------------" << std::endl;
}

int FileSystemClient::Ping(int* round_trip_time)
{
    auto start = std::chrono::steady_clock::now();

    PingMessage request;
    PingMessage reply;
    Status status;

        ClientContext context;
        status = stub_->Ping(&context, request, &reply);

    if (status.ok()) {
        auto end = std::chrono::steady_clock::now();
        std::chrono::nanoseconds ns = end - start;
        *round_trip_time = ns.count();
        return 0;
    }
    else {

        return -1;
    }
}

int FileSystemClient::GetFileStat(std::string abs_path, struct stat* buf, std::string root)
{
    GetFileStatRequest req;
    GetFileStatResponse resp;
    Status status;
    std::string path = get_relative_path(abs_path, root);

    auto test_auth_result = TestAuth(abs_path, root);
    if (!test_auth_result.status.ok() || test_auth_result.response.file_changed()) {
        req.set_pathname(path);
            ClientContext context;
            resp.Clear();
            status = stub_->GetFileStat(&context, req, &resp);

        if (status.ok()) {
            int server_errno = resp.fs_errno();
            if (server_errno) {
                printf("[GetFileStat]: Error %d on server.\n", server_errno);
                printf("[GetFileStat]: Function ended due to server failure.\n");
                errno = server_errno;
                return -1;
            }

            buf->st_ino = resp.stat().ino();
            buf->st_mode = resp.stat().mode();
            buf->st_nlink = resp.stat().nlink();
            buf->st_uid = resp.stat().uid();
            buf->st_gid = resp.stat().gid();
            buf->st_size = resp.stat().size();
            buf->st_blksize = resp.stat().blksize();
            buf->st_blocks = resp.stat().blocks();
            buf->st_atime = resp.stat().atime();
            buf->st_mtime = resp.stat().mtime();
            buf->st_ctime = resp.stat().ctime();
            printf("[GetFileStat]: Function ended; ino = %d\n", buf->st_ino);
            return 0;
        }
        else {
            printf("[GetFileStat]: GetFileStat RPC failed.\n");
            printf("[GetFileStat]: Function ended due to RPC failure.\n");
            errno = transform_rpc_err(status.error_code());
            return -1;
        }
    }
    else {
        return lstat(abs_path.c_str(), buf);
    }
}

int FileSystemClient::OpenFile(std::string abs_path, std::string root)
{
    int file;
    FetchRequest req;
    FetchResponse resp;
    Status status;
    std::string path = get_relative_path(abs_path, root);

    auto test_auth_resp = TestAuth(abs_path, root);
    if (!test_auth_resp.status.ok() || test_auth_resp.response.file_changed()) {

        req.set_pathname(path);
            ClientContext context;
            resp.Clear();
            status = stub_->Fetch(&context, req, &resp);

        if (status.ok()) {
            int server_errno = resp.fs_errno();
            if (server_errno) {
                printf("[OpenFile] Error %d on server\n", server_errno);
                errno = server_errno;
                return -1;
            }

            if (create_path(path, true, root) != 0) {
                return -1;
            }

            file = open(abs_path.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0666);
            if (file == -1) {
                return -1;
            }

            if (write(file, resp.file_contents().c_str(), resp.file_contents().length()) == -1) {
                return -1;
            }

            if (fsync(file) == -1) {
                return -1;
            }

            struct timespec t;
            auto modified_time = resp.time_modified();
            t.tv_sec = modified_time.sec();
            t.tv_nsec = modified_time.nsec();
            if (set_file_open_time(file, t) == -1) {
                printf("[OpenFile]: Error (%d) setting file open and modified timings.\n", errno);
            }

            if (close(file) == -1) {
                return -1;
            }
            printf("[OpenFile]: Successfully wrote %d bytes to cache.\n", (int)resp.file_contents().length());
        }
        else {
            errno = transform_rpc_err(status.error_code());
            return -1;
        }
    }

    file = open(abs_path.c_str(), O_RDWR | O_CREAT, 0666);
    if (file == -1) {
        return -1;
    }

    return file;
}

TestAuthReturn FileSystemClient::TestAuth(std::string abs_path, std::string root)
{
    TestAuthRequest req;
    TestAuthResponse resp;
    Timestamp t;
    timespec fileModifiedTime;
    Status status;
    std::string path = get_relative_path(abs_path, root);

    if (!file_exists(abs_path)) {
        printf("[TestAuth]: File does not exist locally. Set TestAuth response to true.\n");
        resp.set_file_changed(true);
        return TestAuthReturn(status, resp);
    }

    if (GetModifyTime(abs_path, &fileModifiedTime) != 0) {
        printf("[TestAuth]: Failure to get local file modified time. Setting TestAuth response to true.\n");
        resp.set_file_changed(true);
        return TestAuthReturn(status, resp);
    }

    req.set_pathname(path);
    t.set_sec(fileModifiedTime.tv_sec);
    t.set_nsec(fileModifiedTime.tv_nsec);
    req.mutable_time_modified()->CopyFrom(t);

        ClientContext context;
        resp.Clear();
        status = stub_->TestAuth(&context, req, &resp);

    if (status.ok()) {
        printf("[TestAuth]: TestAuth RPC success.\n");
    }
    else {
        printf("[TestAuth]: TestAuth RPC failed.\n");
    }

    printf("[TestAuth]: Function ended successfully.\n");
    return TestAuthReturn(status, resp);
}

int FileSystemClient::Access(std::string abs_path, int mode, std::string root)
{
    AccessRequest req;
    AccessResponse resp;
    Status status;
    std::string path = get_relative_path(abs_path, root);

    req.set_pathname(path);
    req.set_mode(mode);

        ClientContext context;
        resp.Clear();
        status = stub_->Access(&context, req, &resp);

    if (status.ok()) {
        printf("[Access]: Access RPC success.\n");
        int server_errno = resp.fs_errno();
        if (server_errno) {
            printf("[Access]: Server returned error %d on Access call for %s.\n", server_errno, path);
            errno = server_errno;
            return -1;
        }
        return 0;
    }
    else {
        printf("[Access] Access RPC failed.\n");
        return -1;
    }
}

int FileSystemClient::MakeDir(std::string abs_path, std::string root, int mode)
{
    printf("MakeDir: Starting..\n");
    MakeDirRequest request;
    MakeDirResponse reply;
    Status status;

    std::string path = get_relative_path(abs_path, root);
    request.set_pathname(path);
    request.set_mode(mode);

        ClientContext context;
        reply.Clear();
        printf("MakdeDir: Path %s\n", path.c_str());
        status = stub_->MakeDir(&context, request, &reply);

    if (status.ok()) {
        printf("MakeDir: RPC Success\n");
        printf("MakeDir: Completed\n");
        uint server_errno = reply.fs_errno();
        if (server_errno) {
            printf("...but error %d on server\n", server_errno);
            printf("MakeDir: Completed\n");
            errno = server_errno;
            return -1;
        }

        create_path(path, false, root);
        return 0;
    }
    else {
        printf("MakeDir: RPC failure\n");
        printf("MakeDir: Completed\n");
        errno = transform_rpc_err(status.error_code());
        return -1;
    }
}

int FileSystemClient::RemoveDir(std::string abs_path, std::string root)
{
    printf("RemoveDir: Starting..\n");
    RemoveDirRequest request;
    RemoveDirResponse reply;
    Status status;

    std::string path = get_relative_path(abs_path, root);
    request.set_pathname(path);

        ClientContext context;
        reply.Clear();
        status = stub_->RemoveDir(&context, request, &reply);

    if (status.ok()) {
        printf("RemoveDir: RPC Success\n");
        printf("RemoveDir: Completed\n");

        uint server_errno = reply.fs_errno();
        if (server_errno) {
            printf("...but error %d on server\n", server_errno);
            printf("RemoveDir: Completed\n");
            errno = server_errno;
            return -1;
        }

        rmdir(abs_path.c_str());
        return 0;
    }
    else {
        printf("RemoveDir: RPC Failure\n");
        printf("RemoveDir: Completed\n");
        errno = transform_rpc_err(status.error_code());
        return -1;
    }
}

int FileSystemClient::ReadDir(std::string abs_path, std::string root, void* buf, fuse_fill_dir_t filler)
{
    printf("ReadDir: Starting..\n");
    ReadDirRequest request;
    ReadDirResponse reply;
    Status status;

    std::string path = get_relative_path(abs_path, root);
    request.set_pathname(path);

        ClientContext context;
        reply.Clear();
        status = stub_->ReadDir(&context, request, &reply);

    if (status.ok()) {
        printf("ReadDir: RPC Success\n");
        uint server_errno = reply.fs_errno();
        if (server_errno) {
            printf("...but error %d on server\n", server_errno);
            printf("ReadDir: Completed\n");
            errno = server_errno;
            return -1;
        }
    }
    else {
        printf("ReadDir: RPC Failure\n");
        errno = transform_rpc_err(status.error_code());
        return -1;
    }

    printf("ReadDir: Completed\n");
    for (auto itr = reply.entries().begin(); itr != reply.entries().end(); itr++) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = itr->size();
        st.st_mode = itr->mode();
        if (filler(buf, itr->file_name().c_str(), &st, 0))
            break;
    }
    return 0;
}

int FileSystemClient::CloseFile(int fd, std::string abs_path, std::string root)
{
    StoreRequest request;
    StoreResponse reply;
    Status status;
    std::string path = get_relative_path(abs_path, root);

    request.set_pathname(path);

    string content = readFileIntoString(abs_path);
    request.set_file_contents(content);

        ClientContext context;
        reply.Clear();
        status = stub_->Store(&context, request, &reply);

    if (status.ok()) {
        printf("CloseFile: RPC Success\n");

        uint server_errno = reply.fs_errno();
        if (server_errno) {
            printf("...but error %d on server\n", server_errno);
            printf("CloseFile: Completed\n");
            errno = server_errno;
            return -1;
        }
        auto timing = reply.time_modify();

        struct timespec t;
        t.tv_sec = timing.sec();
        t.tv_nsec = timing.nsec();

        if (set_timings(abs_path, t) == -1) {
            printf("CloseFile: error (%d) setting file timings\n", errno);
        }
        else {
            printf("CloseFile: updated file timings\n");
        }

        printf("CloseFile: Completed\n");
        return 0;
    }
    else {
        std::cout << status.error_message() << "\n";
        printf("CloseFile: RPC Failure %d.\n", status.error_code());
        printf("CloseFile: Completed\n");
        errno = transform_rpc_err(status.error_code());
        return -1;
    }
}

int FileSystemClient::CreateFile(std::string abs_path, std::string root, int mode, int flags)
{
    printf("CreateFile: Starting..\n");
    CreateRequest request;
    CreateResponse reply;
    Status status;
    std::string path = get_relative_path(abs_path, root);

    request.set_pathname(path);
    request.set_mode(mode);
    request.set_flags(flags);

        ClientContext context;
        reply.Clear();
        status = stub_->Create(&context, request, &reply);

    if (status.ok()) {
        printf("CreateFile: RPC Success\n");
        uint server_errno = reply.fs_errno();
        if (server_errno) {
            printf("...but error %d on server\n", server_errno);
            printf("CreateFile: Completed\n");
            errno = server_errno;
            return -1;
        }

        int ret = open(abs_path.c_str(), flags, mode);
        struct timespec ts;
        GetModifyTime(abs_path, &ts);
        open_map[abs_path] = ts;
        printf("[CreateFile]: Open time set to %d %d\n", open_map[abs_path].tv_sec, open_map[abs_path].tv_nsec);
        printf("[CreateFile]: %s, %d, %d\n", abs_path.c_str(), flags, mode);
        return ret;
    }
    else {
        printf("CreateFile: RPC Failure\n");
        printf("CreateFile: Completed\n");
        errno = transform_rpc_err(status.error_code());
        return -1;
    }
}

int FileSystemClient::DeleteFile(std::string abs_path, std::string root)
{
    RemoveRequest request;
    RemoveResponse reply;
    Status status;
    std::string path = get_relative_path(abs_path, root);

    request.set_pathname(path);

        ClientContext context;
        reply.Clear();
        status = stub_->Remove(&context, request, &reply);

    if (status.ok()) {
        printf("DeleteFile: RPC success\n");
        uint server_errno = reply.fs_errno();
        if (server_errno) {
            printf("...but error %d on server\n", server_errno);
            printf("DeleteFile: Completed\n");
            errno = server_errno;
            return -1;
        }

        printf("DeleteFile: Completed\n");

        if (file_exists(abs_path)) {
            unlink(abs_path.c_str());
        }
        if (open_map.find(abs_path) != open_map.end()) {
            open_map.erase(abs_path);
        }

        return 0;
    }
    else {
        printf("DeleteFile: RPC failure\n");
        printf("DeleteFile: Completed\n");
        errno = transform_rpc_err(status.error_code());
        return -1;
    }
}

int FileSystemClient::CloseFileUsingStream(int fd, std::string abs_path, std::string root)
{
    if (close(fd) == -1) {
        printf("[CloseFileUsingStream]: close() failed.\n");
        return -1;
    }
    StoreRequest request;
    StoreResponse reply;
    Status status;
    timespec file_modified_time;
    timespec file_opened_time = open_map[abs_path];
    std::string path = get_relative_path(abs_path, root);

    GetModifyTime(abs_path, &file_modified_time);
    if (open_map.find(abs_path) == open_map.end() || file_modified_time.tv_sec > file_opened_time.tv_sec || (file_modified_time.tv_sec == file_opened_time.tv_sec && file_modified_time.tv_nsec > file_opened_time.tv_nsec)) {
            ClientContext context;
            reply.Clear();

            struct stat st;
            stat(abs_path.c_str(), &st);
            int fileSize = st.st_size;
            int totalChunks = 0;
            totalChunks = fileSize / CHUNK_SIZE;
            bool aligned = true;
            int lastChunkSize = fileSize == 0 ? 0 : CHUNK_SIZE;
            if (fileSize % CHUNK_SIZE) {
                totalChunks++;
                aligned = false;
                lastChunkSize = fileSize % CHUNK_SIZE;
            }
            if (fileSize == 0) {
                printf("CloseFileUsingStream: Nothing to send.\n");
                return 0;
            }

            request.set_pathname(path);
            std::unique_ptr<ClientWriter<StoreRequest> > writer(
                stub_->StoreUsingStream(&context, &reply));

            std::ifstream fin(abs_path.c_str(), std::ios::binary);
            fin.clear();
            fin.seekg(0, ios::beg);

            unsigned long bytes = 0;
            unsigned long bytes_read = 0;
            for (size_t chunk = 0; chunk < totalChunks; chunk++) {
                size_t currentChunkSize = (chunk == totalChunks - 1 && !aligned) ? lastChunkSize : CHUNK_SIZE;

                char* buffer = new char[currentChunkSize];
                bytes += currentChunkSize;

                if (fin.read(buffer, currentChunkSize)) {
                    auto bcnt = fin.gcount();
                    bytes_read += bcnt;
                    request.set_pathname(path);
                    request.set_file_contents(buffer, bcnt);

                    if (!writer->Write(request)) {
                        printf("[CloseFileUsingStream]: Stream broke\n");
                        break;
                    }
                }
                else {
                    printf("[CloseFileUsingStream]: Failed to write chunk [iter %ld, total %ld B]\n", chunk, bytes);
                }
            }
            fin.close();
            writer->WritesDone();
            status = writer->Finish();


        if (status.ok()) {
            printf("[CloseFileUsingStream]: RPC Success\n");
            uint server_errno = reply.fs_errno();
            if (server_errno) {
                printf("[CloseFileUsingStream]: error %d on server\n", server_errno);
                printf("[CloseFileUsingStream]: Completed\n");
                errno = server_errno;
                return -1;
            }

            auto timing = reply.time_modify();

            struct timespec t;
            t.tv_sec = timing.sec();
            t.tv_nsec = timing.nsec();

            if (set_timings(abs_path, t) == -1) {
                printf("[CloseFileUsingStream]: error (%d) setting file timings\n", errno);
            }
            else {
                printf("[CloseFileUsingStream]: updated file timings\n");
            }

            printf("[CloseFileUsingStream]: Completed\n");
            if (open_map.find(abs_path) != open_map.end()) {
                open_map.erase(abs_path);
            }
            return 0;
        }
        else {
            printf("[CloseFileUsingStream]: RPC Failure\n");
            printf("[CloseFileUsingStream]: Completed\n");
            std::cout << "[CloseFileUsingStream]: Error msg: " << status.error_message() << "\n";
            errno = transform_rpc_err(status.error_code());
            if (open_map.find(abs_path) != open_map.end()) {
                open_map.erase(abs_path);
            }
            return -1;
        }
    }
    else {
        printf("[CloseFileUsingStream]: File not changed by client, skipping sending to server.\n");
        if (open_map.find(abs_path) != open_map.end()) {
            open_map.erase(abs_path);
        }
        return 0;
    }
}

int FileSystemClient::OpenFileUsingStream(std::string abs_path, std::string root, int flags)
{
    printf("OpenFileUsingStream: Inside function, path = %s\n", abs_path.c_str());
    int file;
    FetchRequest request;
    FetchResponse reply;
    Status status;
    ClientContext context;
    std::string path = get_relative_path(abs_path, root);

    auto test_auth_result = TestAuth(abs_path, root);
    if (!test_auth_result.status.ok() || test_auth_result.response.file_changed()) {
        if (test_auth_result.status.ok()) {
            printf("OpenFileUsingStream: TestAuth reports changed\n");
        }
        else {
            printf("OpenFileUsingStream: TestAuth RPC failed\n");
        }

        if (create_path(path, true, root) != 0) {
            printf("[OpenFileUsingStream]: create_path() call failed.\n");
            return -1;
        }

        
            request.set_pathname(path);


            std::unique_ptr<ClientReader<FetchResponse> > reader(
                stub_->FetchUsingStream(&context, request));

            std::ofstream file;
            file.open(abs_path, std::ios::binary);
            file.clear();
            file.seekp(0, ios::beg);

            while (reader->Read(&reply)) {
                file << reply.file_contents();
            }

            Status status = reader->Finish();
            file.close();
        

        if (status.ok()) {
            printf("OpenFileUsingStream: RPC Success\n");
            uint server_errno = reply.fs_errno();
            if (server_errno) {
                printf("OpenFileUsingStream: error %d on server\n", server_errno);
                errno = server_errno;
                return -1;
            }

            auto timing = reply.time_modified();

            struct timespec t;
            t.tv_sec = timing.sec();
            t.tv_nsec = timing.nsec();

            open_map[abs_path] = t;

            if (set_timings(abs_path, t) == -1) {
                printf("OpenFileUsingStream: error (%d) setting file timings\n", errno);
            }
            else {
                printf("OpenFileUsingStream: updated file timings\n");
            }
        }
        else {
            printf("OpenFileUsingStream: RPC Failure\n");
            errno = transform_rpc_err(status.error_code());
            return -1;
        }
    }
    else if (!test_auth_result.status.ok() || test_auth_result.response.file_group_locked()) {
        printf("OpenFileUsingStream: TestAuth reported file locked. Retry in some time!\n");
        return -1;
    }
    else {
        printf("OpenFileUsingStream: TestAuth reported no change.\n");
    }

    file = open(abs_path.c_str(), flags);
    if (file == -1) {
        printf("OpenFileUsingStream: open() failed\n");
        return -1;
    }
    printf("OpenFileUsingStream: Completed\n");
    return file;
}

int FileSystemClient::Rename(std::string abs_path, std::string new_name, std::string root)
{
    RenameRequest request;
    RenameResponse reply;
    Status status;

    std::string old_path = get_relative_path(abs_path, root);
    std::string new_path = get_relative_path(new_name, root);

    request.set_pathname(old_path);
    request.set_componentname(new_path);

        ClientContext context;
        reply.Clear();
        status = stub_->Rename(&context, request, &reply);

    if (status.ok()) {
        printf("Rename: RPC success\n");

        printf("Running rename\n");

        printf("%d\n", rename(abs_path.c_str(), new_name.c_str()));
        return 0;
    }
    else {
        printf("Rename: RPC failure\n");
        printf("Rename: Completed\n");
        errno = transform_rpc_err(status.error_code());
        return -1;
    }
}
};
