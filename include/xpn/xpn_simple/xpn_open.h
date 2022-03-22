#ifndef _XPN_OPEN_H
#define _XPN_OPEN_H

#include "xpn.h"
#include "xpn_file.h"
#include "xpn_policy_open.h"
#include "xpn_policy_cwd.h"
#include "xpn_init.h"
#include "xpn_rw.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


int xpn_preload(const char *virtual_path, const char *storage_path);
int xpn_flush(const char *virtual_path, const char *storage_path);
	    
int xpn_creat(const char *path, mode_t perm);

int xpn_open(const char *path, int flags, ...);

int xpn_close(int fd);

int xpn_unlink(const char *path);

int xpn_rename(const char *path, const char *newpath);

int xpn_fstat(int fd, struct stat *sb);

int xpn_stat(const char *path, struct stat *sb);

int xpn_chown(const char *path, uid_t owner, gid_t group);

int xpn_fchown(int fd, uid_t owner, gid_t group);

int xpn_chmod(const char *path, mode_t mode);

int xpn_fchmod(int fd, mode_t mode);

int xpn_truncate(const char *path, off_t length);

int xpn_ftruncate(int fd, off_t length);

int xpn_dup(int fd);

int xpn_dup2(int fd, int fd2);

FILE *xpn_fopencookie(const char *path, const char *mode);

FILE *xpn_fopen(const char *filename, const char *mode);

int xpn_fclose(FILE *stream);


 #ifdef  __cplusplus
     }
 #endif


#endif

