#ifndef _XPN_OPEN_H
#define _XPN_OPEN_H

#include "xpn.h"
#include "xpn_file.h"
#include "xpn_policy_open.h"
#include "xpn_policy_cwd.h"
#include "xpn_init.h"
#include "xpn_rw.h"
#include "base/workers.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


int   xpn_open(const char *path, int flags, ...);
FILE *xpn_fopencookie(const char *path, const char *mode);

int   xpn_simple_creat(const char *path, mode_t perm) ;
FILE *xpn_simple_fopen(const char *filename, const char *mode) ;
int   xpn_simple_close(int fd) ;
int   xpn_simple_unlink(const char *path) ;
int   xpn_simple_rename(const char *path, const char *newpath) ;
int   xpn_simple_fstat(int fd, struct stat *sb) ;
int   xpn_simple_fclose(FILE *fp) ;
int   xpn_simple_stat(const char *path, struct stat *sb) ;
int   xpn_simple_chown( const char *path,  uid_t owner,  gid_t group) ;
int   xpn_simple_fchown(int  fd,  uid_t owner,  gid_t group) ;
int   xpn_simple_chmod( const char *path,  mode_t mode) ;
int   xpn_simple_fchmod( int fd,  mode_t mode) ;
int   xpn_simple_truncate( const char *path,  off_t length) ;
int   xpn_simple_ftruncate( int fd, off_t length) ;

int   xpn_simple_dup(int fd) ;
int   xpn_simple_dup2(int fd, int fd2) ;

int   xpn_simple_flush(const char *virtual_path, const char *storage_path) ;
int   xpn_simple_preload(const char *virtual_path, const char *storage_path) ;


 #ifdef  __cplusplus
     }
 #endif


#endif

