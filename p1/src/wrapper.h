#ifndef __WRAPPER_H
#define __WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif
#include <sys/types.h>
typedef void * WFileSystemClient;

WFileSystemClient create_FileSystemClient(char*);
int ping_FileSystemClient(WFileSystemClient, int*);
int getFileStat_FileSystemClient(WFileSystemClient, char*, struct stat *, char*);
int open_FileSystemClient(WFileSystemClient, char*, char*);
int access_FileSystemClient(WFileSystemClient, char*, int, char*);
int close_FileSystemClient(WFileSystemClient v, int fd, char* path, char* root);

int makeDir_FileSystemClient(WFileSystemClient, char*, char*, mode_t);
int removeDir_FileSystemClient(WFileSystemClient, char*, char*);
int readDir_FileSystemClient(WFileSystemClient, char*, char*, void *, int*);

int createFile_FileSystemClient(WFileSystemClient v, char* , char* , mode_t, int);
int deleteFile_FileSystemClient(WFileSystemClient v, char* abs_path, char* root);

#ifdef __cplusplus
}
#endif
#endif
