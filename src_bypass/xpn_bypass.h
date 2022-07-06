#ifndef _XPN_LIB_H_
#define _XPN_LIB_H_


#define _GNU_SOURCE

#include <dlfcn.h>
#include <sys/stat.h>

#include "xpn.h"
#include "syscall_proxies.h"

#include <dirent.h>
#include <string.h>
#include "mpi.h"

//#define RTLD_NEXT ((void *) -1l)
#define MAX_FDS   1024
#define MAX_DIRS  32
#define PLUSXPN   65000

#undef __USE_FILE_OFFSET64
#undef __USE_LARGEFILE64


#define FD_FREE 0
#define FD_SYS  1
#define FD_XPN  2

struct generic_fd{
  int type;
  int real_fd;
};


struct generic_fd fdstable[MAX_FDS];
DIR * fdsdirtable[MAX_DIRS];


/*struct stat64
  {
  __dev_t st_dev;                     // Device.
  unsigned int __pad1;
        __ino_t __st_ino;                   // 32bit file serial number.
        __mode_t st_mode;                   // File mode.
        __nlink_t st_nlink;                 // Link count.
        __uid_t st_uid;                     // User ID of the file's owner.
        __gid_t st_gid;                     // Group ID of the file's group.
        __dev_t st_rdev;                    // Device number, if device.
        unsigned int __pad2;
        __off64_t st_size;                  // Size of file, in bytes.
        __blksize_t st_blksize;             // Optimal block size for I/O.
        __blkcnt64_t st_blocks;             // Number 512-byte blocks allocated.
        __time_t st_atime;                  // Time of last access.
        unsigned long int __unused1;
        __time_t st_mtime;                  // Time of last modification.
        unsigned long int __unused2;
        __time_t st_ctime;                  // Time of last status change.
        unsigned long int __unused3;
        __ino64_t st_ino;                   // File serial number.
};

struct dirent64
  {
  __ino64_t d_ino;
  __off64_t d_off;
  unsigned short int d_reclen;
  unsigned char d_type;
  char d_name[256];           // We must not include limits.h! 
  };*/



/*
int fdstable[MAX_FDS];
int fdsdirtable[MAX_DIRS];
*/
/*
int open(const char *path, int flags);
int open(const char *path, int flags, mode_t mode);
*/


// File API

//int open(const char *path, int flags, ...); //comment
int open(const char *path, int flags, mode_t mode);
//int open64(const char *path, int flags, ...); //comment
int open64(const char *path, int flags, mode_t mode);

int creat(const char *path, mode_t mode);

int ftruncate(int fildes, off_t length);

ssize_t read(int fildes, void *buf, size_t nbyte);
ssize_t write(int fildes, const void *buf, size_t nbyte);

off_t lseek(int fildes, off_t offset, int whence);

//int lstat64(const char *path, struct stat64 *buf); //old
int __lxstat64(int ver, const char *path, struct stat64 *buf);
//int stat64(const char *path, struct stat64 *buf); //old
int __xstat64(int ver, const char *path, struct stat64 *buf);
//int fstat64(int fildes, struct stat64 *buf); //old
int __fxstat64(int ver, int fildes, struct stat64 *buf);
//int lstat(const char *path, struct stat *buf); //old
int __lxstat(int ver, const char *path, struct stat *buf);
//int stat(const char *path, struct stat *buf); //old
int __xstat(int ver, const char *path, struct stat *buf);
//int fstat(int fildes, struct stat *buf); //old
int __fxstat(int ver, int fd, struct stat *buf);

int close(int fd);

int rename(const char *old_path, const char *new_path);

int unlink(const char *path);



// Directory API

DIR *opendir(const char *dirname);

int mkdir(const char *path, mode_t mode);

struct dirent *readdir(DIR *dirp);
struct dirent64 *readdir64(DIR *dirp);

int closedir(DIR *dirp);

int rmdir(const char *path);



// Proccess API

int fork();

int dup(int fildes);
int dup2(int fildes, int fildes2);

void exit(int status) ;



// Manager API

int chdir(const char *path);

int chmod(const char *path, mode_t mode);

int fchmod(int fildes, mode_t mode);

int chown(const char *path, uid_t owner, gid_t group);

int fcntl(int fd, int cmd, long arg);

int access(const char *path, int mode);


/**************************************************
 GETCWD TIENE MUCHA CHICHA...PA LUEGO
***************************************************

char *getcwd(char *path, size_t size);

**********************************************
**********************************************/

//int utime(const char *path, struct utimbuf *times);


#endif
