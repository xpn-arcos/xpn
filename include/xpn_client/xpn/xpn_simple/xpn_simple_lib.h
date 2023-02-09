#ifndef _XPN_SIMPLE_LIB_H_
#define _XPN_SIMPLE_LIB_H_

#include "xpn_cwd.h"
#include "xpn_file.h"
#include "xpn_opendir.h"
#include "xpn_policy_init.h"     
#include "xpn_policy_rw.h"
#include "xpn_dir.h"
#include "xpn_init.h" 
#include "xpn_open.h"
#include "xpn_policy_opendir.h"
#include "xpn_rw.h"
#include "xpn_err.h"
#include "xpn_policy_cwd.h"
#include "xpn_policy_open.h"


/******************************************

#define xpn_getcwd	xpn_xxx_getcwd	
#define xpn_mkdir	xpn_xxx_mkdir	
#define xpn_rmdir	xpn_xxx_rmdir

#define xpn_read	xpn_xxx_read
#define xpn_write	xpn_xxx_write
#define xpn_pread	xpn_xxx_pread
#define xpn_pwrite	xpn_xxx_pwrite
#define xpn_writev	xpn_xxx_writev
#define xpn_readv	xpn_xxx_readv
#define xpn_lseek	xpn_xxx_lseek

#define xpn_creat	xpn_xxx_creat
#define xpn_open	xpn_xxx_open
#define xpn_close	xpn_xxx_close
#define xpn_unlink	xpn_xxx_unlink
#define xpn_rename	xpn_xxx_rename
#define xpn_fstat	xpn_xxx_fstat
#define xpn_stat	xpn_xxx_stat
#define xpn_chown	xpn_xxx_chown
#define xpn_fchown	xpn_xxx_fchown
#define xpn_chmod	xpn_xxx_chmod
#define xpn_fchmod	xpn_xxx_fchmod
#define xpn_truncate	xpn_xxx_truncate
#define xpn_ftruncate	xpn_xxx_ftruncate
#define xpn_dup		xpn_xxx_dup
#define xpn_dup2	xpn_xxx_dup2

#define xpn_opendir	xpn_xxx_opendir
#define xpn_closedir	xpn_xxx_closedir
#define xpn_readdir	xpn_xxx_readdir
********************************************/





/*******************************
char* 		xpn_getcwd	(char *path, size_t size);
int 		xpn_mkdir	(const char *path, mode_t perm) ;
int 		xpn_rmdir	(const char *path) ;

ssize_t 	xpn_read	(int fd, void *buffer, size_t size);
ssize_t 	xpn_write	(int fd, const void *buffer, size_t size);
ssize_t 	xpn_pread	(int fd, void *buffer, size_t size, off_t offset);
ssize_t 	xpn_pwrite	(int fd, const void *buffer, size_t size, off_t offset);
ssize_t 	xpn_writev	(int fildes, const struct iovec *iov, int iovcnt);
ssize_t 	xpn_readv	(int fildes, const struct iovec *iov, int iovcnt);
off_t 		xpn_lseek	(int fd, off_t offset, int flag);

int 		xpn_creat	(const char *path, mode_t perm);
int 		xpn_open	(const char *path, int flags , ...);
int 		xpn_close	(int fd);
int 		xpn_unlink	(const char *path);
int 		xpn_rename	(const char *path, const char *newpath);
int 		xpn_fstat	(int fd, struct stat *sb);
int 		xpn_stat	(const char *path, struct stat *sb);
int 		xpn_chown	(const char *path, uid_t owner, gid_t group);
int 		xpn_fchown	(int fd, uid_t owner, gid_t group);
int 		xpn_chmod	(const char *path, mode_t mode);
int 		xpn_fchmod	(int fd, mode_t mode);
int 		xpn_truncate	(const char *path, off_t length);
int 		xpn_ftruncate	(int fd, off_t length);
int 		xpn_dup		(int fd);
int 		xpn_dup2	(int fd, int fd2);

DIR* 		xpn_opendir	(const char *path);
int 		xpn_closedir	(DIR *dirp);
struct dirent* 	xpn_readdir	(DIR *dirp);
void 		xpn_rewinddir	(DIR *dirp);
**********************************************/
#endif
