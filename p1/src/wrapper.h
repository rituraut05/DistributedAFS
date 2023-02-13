#ifndef __WRAPPER_H
#define __WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void * WFileSystemClient;

WFileSystemClient create_FileSystemClient(char*);
int ping_FileSystemClient(WFileSystemClient, int*);
int open_FileSystemClient(WFileSystemClient, char*, char*);
int getFileStat_FileSystemClient(WFileSystemClient, char*, struct stat *, char*);

int makeDir_FileSystemClient(WFileSystemClient, char*, char*, mode_t);
int removeDir_FileSystemClient(WFileSystemClient, char*, char*);
int readDir_FileSystemClient(WFileSystemClient, char*, char*, void *, int*);


#ifdef __cplusplus
}
#endif
#endif
