#ifndef _XPN_BYPASS_H_
#define _XPN_BYPASS_H_


//
// Includes
//

#define _GNU_SOURCE

#include <dlfcn.h>
#include <sys/stat.h>
#include <stdarg.h>

#include "xpn.h"
#include "syscall_proxies.h"

#include <dirent.h>
#include <string.h>
#include "mpi.h"

//#include<pthread.h> //Mutex


//
// Const
//


#ifndef _STAT_VER
#define _STAT_VER 0
#endif


//#define RTLD_NEXT ((void *) -1l)
#define MAX_FDS   10000
#define MAX_DIRS  10000
#define PLUSXPN   1000

#undef __USE_FILE_OFFSET64
#undef __USE_LARGEFILE64


#define FD_FREE 0
#define FD_SYS  1
#define FD_XPN  2


//
// Types
//

struct __dirstream
{
  int fd;                     /* File descriptor.  */
  //__libc_lock_define (, lock) /* Mutex lock for this structure.  */ //TODO
  size_t allocation;          /* Space allocated for the block.  */
  size_t size;                /* Total valid data in the block.  */
  size_t offset;              /* Current offset into the block.  */
  off_t  filepos;              /* Position of next entry to read.  */
  /* Directory block.  */
  char data[0] __attribute__ ((aligned (__alignof__ (void*))));

  char * path;
};


struct generic_fd{
  int type;
  int real_fd;
  int is_file;
};


//
// API
//


// File API

int open       (const char *path, int flags, ...);
int open64     (const char *path, int flags, ...);
int __open_2   (const char *path, int flags, ...);
int creat      (const char *path, mode_t mode);
int close      (int fd);

int ftruncate  (int fildes, off_t length);

ssize_t read   (int fildes,       void *buf, size_t nbyte);
ssize_t write  (int fildes, const void *buf, size_t nbyte);

off_t   lseek  (int fildes, off_t offset, int whence);

int __lxstat64 (int ver, const char *path, struct stat64 *buf);
int __xstat64  (int ver, const char *path, struct stat64 *buf);
int __fxstat64 (int ver,       int fildes, struct stat64 *buf);
int __lxstat   (int ver, const char *path, struct stat   *buf);
int __xstat    (int ver, const char *path, struct stat   *buf);
int __fxstat   (int ver,           int fd, struct stat   *buf);
int   fstatat64(int dfd, const char *path, struct stat64 *buf, int flags);
int newfstatat (int dfd, const char *path, struct stat64 *buf, int flags);

int rename     (const char *old_path, const char *new_path);
int unlink     (const char *path);


// Directory API

int   mkdir  (const char *path, mode_t mode);
DIR * opendir(const char *dirname);

struct dirent   *readdir  (DIR *dirp);
struct dirent64 *readdir64(DIR *dirp);

int closedir(DIR *dirp);
int rmdir   (const char *path);


// Proccess API

int fork (void);
int dup  (int fildes);
int dup2 (int fildes, int fildes2);
void exit(int status) ;


// Manager API

int   chdir    (const char *path);
int   chmod    (const char *path, mode_t mode);
int   fchmod   (      int fildes, mode_t mode);
int   chown    (const char *path, uid_t owner, gid_t group);
int   fcntl    (int fd, int cmd, long arg);
int   access   (const char *path, int mode);
char *realpath (const char *restrict path, char *restrict resolved_path);
int   fsync    (int fd);


#endif
