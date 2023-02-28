#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <filesystem>
#include <shared_mutex>
#include <vector>
#include <algorithm>
#include <cstddef>

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

#define CHUNK_SIZE 4096

namespace fs = std::filesystem;
using afs::AccessRequest;
using afs::AccessResponse;
using afs::CreateRequest;
using afs::CreateResponse;
using afs::DirectoryEntry;
using afs::FetchRequest;
using afs::FetchResponse;
using afs::FileMode;
using afs::FileStat;
using afs::FileSystemService;
using afs::GetFileStatRequest;
using afs::GetFileStatResponse;
using afs::MakeDirRequest;
using afs::MakeDirResponse;
using afs::PingMessage;
using afs::ReadDirRequest;
using afs::ReadDirResponse;
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

static const string TEMP_FILE_EXT = ".afs_tmp";
const std::vector<string> atomicFilesGroup = {};

class BindingException : public std::runtime_error
{
    StatusCode code;

public:
    BindingException(const char *msg, StatusCode code) : std::runtime_error(msg), code(code) {}

    StatusCode get_code() const
    {
        return code;
    }
};

class FSException : public std::runtime_error
{
    uint fs_errno;

public:
    FSException(uint fs_errno) : std::runtime_error("Filesystem call error"), fs_errno(fs_errno) {}

    uint get_fs_errno() const
    {
        return fs_errno;
    }
};

class AFSImpl final : public FileSystemService::Service
{
    path root;
    int groupLocked = 0;
    std::shared_mutex mutex;

    // Sets groupLocked value to v
    void setFileGroupLocked(int v)
    {
        mutex.lock();
        groupLocked = v;
        mutex.unlock();
    }

    bool checkFileInGroup(path filepath)
    {
        std::string fp_string = filepath.u8string();
        std::size_t parDirPos = fp_string.find_last_of("/");
        // get directory
        std::string dir = fp_string.substr(0, parDirPos);
        // get file
        std::string file = fp_string.substr(parDirPos, fp_string.length());
        printf("Checking if the file is a part of the Atomic Group: %s\n", file.c_str());
        return std::find(atomicFilesGroup.begin(), atomicFilesGroup.end(), file) != atomicFilesGroup.end();
    }

    path getPath(string relativePath)
    {
        path normalPath = (root / relativePath).lexically_normal();
        auto [a, b] = std::mismatch(root.begin(), root.end(), normalPath.begin());
        if (a != root.end())
        {
            throw BindingException("Normalized path is outside server root.", StatusCode::INVALID_ARGUMENT);
        }

        return normalPath;
    }

    Timestamp getModifiedTime(path filepath)
    {
        struct stat sb;
        if (stat(filepath.c_str(), &sb) == -1)
        {
            throw FSException(errno);
        }
        Timestamp t;
        t.set_sec(sb.st_mtim.tv_sec);
        t.set_nsec(sb.st_mtim.tv_nsec);
        return t;
    }

    string readFile(path filepath)
    {
        uint fileErr;
        std::error_code ec;
        auto status = fs::status(filepath, ec);

        if (ec)
        {
            fileErr = ec.value();
        }
        switch (status.type())
        {
        case fs::file_type::regular:
            fileErr = 0;
        case fs::file_type::directory:
            fileErr = EISDIR;
        default:
            fileErr = EPERM;
        }
        if (fileErr != 0)
        {
            throw FSException(fileErr);
        }

        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        std::ostringstream buffer;
        buffer << file.rdbuf();

        if (file.fail())
        {
            throw BindingException("Error reading file", StatusCode::UNKNOWN);
        }

        return buffer.str();
    }

    uint isValidDest(path filepath)
    {
        std::error_code ec;
        auto status = fs::status(filepath, ec);
        switch (status.type())
        {
        case fs::file_type::regular:
            return 0;
        case fs::file_type::directory:
            return EISDIR;
        case fs::file_type::not_found:
            break;
        default:
            return EPERM;
        }

        switch (ec.value())
        {
        case ENOENT:
            break;
        default:
            return ec.value();
        }

        if (!fs::is_directory(filepath.parent_path(), ec))
        {
            return ENOENT;
        }

        return 0;
    }

    void writeFile(path filepath, string content)
    {
        std::ofstream file;

        uint pre_err = isValidDest(filepath);
        if (pre_err != 0)
        {
            throw FSException(pre_err);
        }

        path temppath = path(filepath.string() + TEMP_FILE_EXT);

        // Write to temp path
        file.open(temppath, std::ios::binary);
        file << content;
        file.close();

        if (file.fail())
        {
            throw BindingException("Error writing temp file", StatusCode::UNKNOWN);
        }

        // Overwrite dest file with temp file
        if (rename(temppath.c_str(), filepath.c_str()) == -1)
        {
            throw BindingException("Error committing temp file", StatusCode::UNKNOWN);
        }
    }

    FileStat readStat(path filepath)
    {
        struct stat sb;
        FileStat ret;

        if (stat(filepath.c_str(), &sb) == -1)
        {
            throw FSException(errno);
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

public:
    explicit AFSImpl(path root) : root(root) {}

    Status Ping(ServerContext *context, const PingMessage *req, PingMessage *resp) override
    {
        return Status::OK;
    }

    Status GetFileStat(ServerContext *context, const GetFileStatRequest *req, GetFileStatResponse *resp) override
    {
        try
        {
            path filepath = getPath(req->pathname());

            auto stat = readStat(filepath);
            resp->mutable_stat()->CopyFrom(stat);
            return Status::OK;
        }
        catch (const BindingException &e)
        {
            printf("{GetFileStat}: Binding Exception: %d %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        }
        catch (const FSException &e)
        {
            printf("{GetFileStat}: System Exception: %d\n", e.get_fs_errno());
            resp->set_fs_errno(e.get_fs_errno());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            printf("{GetFileStat}: Unexpected Exception %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status Access(ServerContext *context, const AccessRequest *req, AccessResponse *resp) override
    {
        try
        {
            path filepath = getPath(req->pathname());
            int mode = req->mode();

            int ret = access(filepath.c_str(), mode);
            if (ret != 0)
            {
                resp->set_fs_errno(errno);
            }
            return Status::OK;
        }
        catch (const FSException &e)
        {
            printf("{Access}: System Exception: %d\n", e.get_fs_errno());
            resp->set_fs_errno(e.get_fs_errno());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            printf("{GetFileStat}: Unexpected Exception %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status Fetch(ServerContext *context, const FetchRequest *req, FetchResponse *resp) override
    {
        path filepath = getPath(req->pathname());
        try
        {
            auto content = readFile(filepath);
            resp->mutable_time_modified()->CopyFrom(getModifiedTime(filepath));
            resp->set_file_contents(content);
            return Status::OK;
        }
        catch (const BindingException &e)
        {
            printf("{Fetch}: Binding Exception: %d: %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        }
        catch (const FSException &e)
        {
            printf("{Fetch}: System Exception: %d\n", e.get_fs_errno());
            resp->set_fs_errno(e.get_fs_errno());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            printf("{Fetch}: Unexpected Exception: %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status TestAuth(ServerContext *context, const TestAuthRequest *req, TestAuthResponse *resp) override
    {
        try
        {
            path filepath = getPath(req->pathname());

            auto ts_server = getModifiedTime(filepath);
            auto ts_client = req->time_modified();
            bool file_changed = (ts_server.sec() > ts_client.sec()) ||
                                (ts_server.sec() == ts_client.sec() && ts_server.nsec() > ts_client.nsec());
            bool file_group_locked = checkFileInGroup(filepath.c_str()) && groupLocked; // return true if file is locked

            resp->set_file_changed(file_changed);
            resp->set_file_group_locked(file_group_locked);
            return Status::OK;
        }
        catch (const BindingException &e)
        {
            printf("{TestAuth}: Binding Exception: %d %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        }
        catch (const FSException &e)
        {
            printf("{TestAuth}: System Exception: %d\n", e.get_fs_errno());
            resp->set_file_changed(true);
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            printf("{TestAuth}: Unexpected Exception %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status MakeDir(ServerContext *context, const MakeDirRequest *request, MakeDirResponse *reply) override
    {
        try
        {
            path filepath = getPath(request->pathname());
            printf("MakeDir: filepath = %s\n", filepath.c_str());
            if (mkdir(filepath.c_str(), request->mode()) == -1)
            {
                throw FSException(errno);
            }
            // make_dir(filepath, request->mode());
            return Status::OK;
        }
        catch (const BindingException &e)
        {
            printf("{Binding Exception: %d] %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        }
        catch (const FSException &e)
        {
            printf("{System Exception: %d]\n", e.get_fs_errno());
            reply->set_fs_errno(e.get_fs_errno());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            printf("ERROR: [Unexpected Exception] %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status RemoveDir(ServerContext *context, const RemoveDirRequest *request, RemoveDirResponse *reply) override
    {
        try
        {
            path filepath = getPath(request->pathname());
            printf("RemoveDir: filepath = %s\n", filepath.c_str());
            if (rmdir(filepath.c_str()) == -1)
            {
                throw FSException(errno);
            }
            // remove_dir(filepath);
            return Status::OK;
        }
        catch (const BindingException &e)
        {
            printf("{Binding Exception: %d] %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        }
        catch (const FSException &e)
        {
            printf("{System Exception: %d]\n", e.get_fs_errno());
            reply->set_fs_errno(e.get_fs_errno());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            printf("ERROR: [Unexpected Exception] %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status ReadDir(ServerContext *context, const ReadDirRequest *request, ReadDirResponse *reply) override
    {
        printf("ReadDir: Entering function\n");
        try
        {
            path filepath = getPath(request->pathname());
            printf("ReadDir: filepath = %s\n", filepath.c_str());
            for (const auto &entry : fs::directory_iterator(filepath))
            {
                DirectoryEntry *msg = reply->add_entries();
                msg->set_file_name(entry.path().filename());
                msg->set_mode(is_regular_file(entry.status()) ? FileMode::REG : is_directory(entry.status()) ? FileMode::DIR
                                                                                                             : FileMode::UNSUPPORTED);
                msg->set_size(is_regular_file(entry.status()) ? file_size(entry) : 0);
            }
            // list_dir(filepath, reply);
            return Status::OK;
        }
        catch (const BindingException &e)
        {
            printf("{Binding Exception: %d] %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        }
        catch (const FSException &e)
        {
            printf("{System Exception: %d]\n", e.get_fs_errno());
            reply->set_fs_errno(e.get_fs_errno());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            printf("ERROR: [Unexpected Exception] %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status Store(ServerContext *context, const StoreRequest *request, StoreResponse *reply) override
    {
        try
        {
            path filepath = getPath(request->pathname());
            printf("Store: filepath = %s\n", filepath.c_str());

            writeFile(filepath, request->file_contents());

            reply->mutable_time_modify()->CopyFrom(getModifiedTime(filepath));
            return Status::OK;
        }
        catch (const BindingException &e)
        {
            printf("{Binding Exception: %d] %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        }
        catch (const FSException &e)
        {
            printf("{System Exception: %d]\n", e.get_fs_errno());
            reply->set_fs_errno(e.get_fs_errno());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            printf("ERROR: [Unexpected Exception] %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status Create(ServerContext *context, const CreateRequest *request, CreateResponse *reply) override
    {
        try
        {
            path filepath = getPath(request->pathname());
            printf("Create: filepath = %s\n", filepath.c_str());

            int fd = open(filepath.c_str(), request->flags(), request->mode());
            close(fd);
            return Status::OK;
        }
        catch (const BindingException &e)
        {
            printf("Create: [Binding Exception: %d] %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        }
        catch (const FSException &e)
        {
            printf("Create: [System Exception: %d]\n", e.get_fs_errno());
            reply->set_fs_errno(e.get_fs_errno());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            printf("ERROR: Create: Unexpected Exception] %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status Remove(ServerContext *context, const RemoveRequest *request, RemoveResponse *reply) override
    {
        try
        {
            path filepath = getPath(request->pathname());
            cout << "Remove: filepath = " << filepath << endl;

            if (unlink(filepath.c_str()) == -1)
            {
                throw FSException(errno);
            }
            return Status::OK;
        }
        catch (const BindingException &e)
        {
            printf("{Binding Exception: %d] %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        }
        catch (const FSException &e)
        {
            printf("{System Exception: %d]\n", e.get_fs_errno());
            reply->set_fs_errno(e.get_fs_errno());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            printf("ERROR: [Unexpected Exception] %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status StoreUsingStream(ServerContext *context, ServerReader<StoreRequest> *reader, StoreResponse *reply) override
    {
        StoreRequest request;
        std::ofstream file;
        path filepath;
        int i = 0;
        try
        {
            reader->Read(&request);
            filepath = getPath(request.pathname());
            printf("StoreUsingStream: filepath = %s\n", filepath.c_str());

            uint pre_err = isValidDest(filepath);
            if (pre_err != 0)
            {
                throw FSException(pre_err);
            }

            path temppath = path(filepath.string() + TEMP_FILE_EXT);

            file.open(temppath, std::ios::binary);

            unsigned long iter = 0;
            unsigned long bytes = 0;

            string chunk;

            do
            {
                chunk = request.file_contents();
                file << chunk;
                iter++;
                bytes += chunk.length();
            } while (reader->Read(&request));

            file.close();

            if (rename(temppath.c_str(), filepath.c_str()) == -1)
            {
                throw BindingException("Error committing temp file", StatusCode::UNKNOWN);
            }

            auto time = getModifiedTime(filepath);
            reply->mutable_time_modify()->CopyFrom(time);

            if (checkFileInGroup(filepath.c_str()))
            {
                setFileGroupLocked(0);
                printf("{StoreUsingStream]File: %s was a part of Atomic Lock Group. \n", filepath.c_str());
            }

            return Status::OK;
        }
        catch (const BindingException &e)
        {
            printf("StoreUsingStream: [Binding Exception: %d] %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        }
        catch (const FSException &e)
        {
            printf("StoreUsingStream: [System Exception: %d]\n", e.get_fs_errno());
            reply->set_fs_errno(e.get_fs_errno());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            printf("StoreUsingStream: [Unexpected Exception] %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status FetchUsingStream(ServerContext *context, const FetchRequest *request, ServerWriter<FetchResponse> *writer) override
    {
        try
        {
            path filepath = getPath(request->pathname());
            printf("FetchUsingStream: filepath = %s\n", filepath.c_str());
            FetchResponse reply;
            // Set Group Lock
            if (checkFileInGroup(filepath.c_str()))
            {
                printf("{FetchUsingStream]File: %s is a part of Atomic Lock Group. \n", filepath.c_str());
                setFileGroupLocked(1);
            }

            reply.mutable_time_modified()->CopyFrom(getModifiedTime(filepath));

            std::ifstream fin(filepath.c_str(), std::ios::binary);

            struct stat st;
            stat(filepath.c_str(), &st);
            int fileSize = st.st_size;
            int totalChunks = 0;
            totalChunks = fileSize / CHUNK_SIZE;
            bool aligned = true;
            int lastChunkSize = fileSize == 0 ? 0 : CHUNK_SIZE;
            if (fileSize % CHUNK_SIZE)
            {
                totalChunks++;
                aligned = false;
                lastChunkSize = fileSize % CHUNK_SIZE;
            }

            unsigned long bytes = 0;
            unsigned long bytes_read = 0;
            for (size_t chunk = 0; chunk < totalChunks; chunk++)
            {
                size_t currentChunkSize = (chunk == totalChunks - 1 && !aligned) ? lastChunkSize : CHUNK_SIZE;

                char *buffer = new char[currentChunkSize];
                if (fin.read(buffer, currentChunkSize))
                {
                    auto bcnt = fin.gcount();
                    bytes += currentChunkSize;
                    bytes_read += bcnt;

                    reply.set_file_contents(buffer, bcnt);

                    writer->Write(reply);
                }
            }

            fin.close();
            return Status::OK;
        }
        catch (const BindingException &e)
        {
            printf("FetchUsingStream [Binding Exception: %d] %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        }
        catch (const std::exception &e)
        {
            printf("FetchUsingStream [Unexpected Exception] %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }

    Status Rename(ServerContext *context, const RenameRequest *request, RenameResponse *reply) override
    {
        try
        {
            path srcpath = getPath(request->pathname());
            path dstpath = getPath(request->componentname());

            printf("Rename: srcpath = %s, dstpath = %s\n", srcpath.c_str(), dstpath.c_str());
            if (rename(srcpath.c_str(), dstpath.c_str()) == -1)
            {
                printf("move_file: Exiting function with error %d\n", errno);
                throw FSException(errno);
            }

            return Status::OK;
        }
        catch (const BindingException &e)
        {
            printf("{Binding Exception: %d] %s\n", e.get_code(), e.what());
            return Status(e.get_code(), e.what());
        }
        catch (const FSException &e)
        {
            printf("{System Exception: %d]\n", e.get_fs_errno());
            reply->set_fs_errno(e.get_fs_errno());
            return Status::OK;
        }
        catch (const std::exception &e)
        {
            printf("ERROR: [Unexpected Exception] %s\n", e.what());
            return Status(StatusCode::UNKNOWN, e.what());
        }
    }
};

void RunServer(path root)
{
    std::string server_address("0.0.0.0:50057");
    AFSImpl service(root);

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stdout, "Usage: ./afs_server root_folder\n");
        return -1;
    }

    auto root = fs::canonical(argv[1]);
    RunServer(root);
    return 0;
}
