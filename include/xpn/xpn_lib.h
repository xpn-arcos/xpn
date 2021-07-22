#ifndef _XPN_LIB_H_
#define _XPN_LIB_H_

#include "xpn_simple/xpn_simple_lib.h"


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

int 		xpn_dup2(int fd, int fd2);
**********************************************/
#endif
