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
  TestAuthReturnType(Status status,
                    TestAuthResponse response) :
                    status(status),
                    response(response)
                    {}
}; 

// Globals
//string cache_root;

class FileSystemClient {
  public:
    FileSystemClient(std::shared_ptr<Channel> channel);
    int Ping(int * round_trip_time);
    int GetFileStat(std::string path, struct stat *stbuf, std::string root);
    int OpenFile(std::string path, std::string root);
    int CloseFile(int fd, std::string path, std::string root);
    TestAuthReturnType TestAuth(std::string path, std::string root);
    
    int MakeDir(std::string abs_path, std::string root, mode_t mode);
    int RemoveDir(std::string abs_path, std::string root);
    int ReadDir(std::string abs_path, std::string root, void *buf, fuse_fill_dir_t filler);
    int CreateFile(std::string abs_path, std::string root, mode_t mode, dev_t rdev);
    int DeleteFile(std::string abs_path, std::string root);

/*   
    string get_cache_path(string relative_path) {
        return cache_root + relative_path;           
   }
*/

  private:
      std::unique_ptr<FileSystemService::Stub> stub_;
/*
      int create_path(string relative_path, bool is_file)
            {
                printf("create_path: Entering function\n");
                vector<string> tokens = tokenize_path(relative_path, '/', is_file);
                string base_path = cache_root;
                for (auto token : tokens)
                {
                    base_path += token + "/";

                    struct stat s;
                    int r = stat(base_path.c_str(), &s);
                    if (r != 0 && errno == 2)
                    {
                        printf("create_path: stat() ENOENT\n");
                        if (mkdir(base_path.c_str(), S_IRWXU) != 0)
                        {
                            printf("create_path: mkdir() failed : %d\n", errno);
                            return -1;
                        }
                    }
                    else if (r != 0)
                    {
                        printf("create_path: stat() failed : %d\n", errno);
                        return -1;
                    }

                }
                printf("create_path: Exiting function\n");
                return 0;
            }

      vector<string> tokenize_path(string path, char delim, bool is_file)
            {
                vector<string> tokens;
                string temp = "";

                for(int i = 0; i < path.length(); i++)
                {
                    if(path[i] == delim){
                        tokens.push_back(temp);
                        temp = "";
                    }
                    else
                        temp = temp + (path.c_str())[i];
                }

                // if path is dir not file, create all dirs in path
                if (!is_file)
                    tokens.push_back(temp);

                return tokens;
            }
*/
};
