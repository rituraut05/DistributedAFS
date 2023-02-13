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


#ifdef __cplusplus
}
#endif
#endif
