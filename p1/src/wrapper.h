#ifndef __WRAPPER_H
#define __WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void * WFileSystemClient;

WFileSystemClient create_FileSystemClient(char*);
int ping_FileSystemClient(WFileSystemClient, int*);


#ifdef __cplusplus
}
#endif
#endif
