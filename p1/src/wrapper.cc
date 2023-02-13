#include "afs_client.hh"
#include "wrapper.h"

#include "afs.grpc.pb.h"
#include "afs.pb.h"

extern "C" {
	WFileSystemClient create_FileSystemClient(char* server_addr) {
		return (FileSystemClient*) new FileSystemClient(grpc::CreateChannel(server_addr, grpc::InsecureChannelCredentials()));
	}

	int ping_FileSystemClient(WFileSystemClient v, int* i) {
		return ((FileSystemClient*)v)->Ping(i);
	}

	int getFileStat_FileSystemClient(WFileSystemClient v, char* path, struct stat *stbuf, char* root) {
		return ((FileSystemClient*)v)->GetFileStat(path, stbuf, root);
	}

	int open_FileSystemClient(WFileSystemClient v, char* path, char* root) {
		return ((FileSystemClient*)v)->OpenFile(path, root);
	}
	
	int makeDir_FileSystemClient(WFileSystemClient v, char* abs_path, char* root, mode_t mode) {
		return ((FileSystemClient*)v)->MakeDir(abs_path, root, mode);
	}

	int removeDir_FileSystemClient(WFileSystemClient v, char* abs_path, char* root) {
                return ((FileSystemClient*)v)->RemoveDir(abs_path, root);
        }

	int readDir_FileSystemClient(WFileSystemClient v, char* abs_path, char* root, void *buf, int* filler) {
                return ((FileSystemClient*)v)->ReadDir(abs_path, root, buf, (fuse_fill_dir_t) filler);
        }
	
}
