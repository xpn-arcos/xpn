/*
 *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez
 * Garcia, Borja Bergua Guerra
 *
 *  This file is part of Expand.
 *
 *  Expand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Expand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * @file xpn_bypass.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia,
 * Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _XPN_BYPASS_H_
#define _XPN_BYPASS_H_

#define _GNU_SOURCE

/************************************************
 *  ... Includes
 ***********************************************/
#include <dirent.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>

#include "config.h"
#include "mpi.h"
#include "syscall_proxies.h"
#include "xpn.h"

// #include<pthread.h> //Mutex

/************************************************
 *  ... Constants
 ***********************************************/

#ifndef _STAT_VER
#define _STAT_VER 0
#endif

// #define RTLD_NEXT ((void *) -1l)
#define MAX_FDS  10000
#define MAX_DIRS 10000
#define PLUSXPN  1000

#undef __USE_FILE_OFFSET64
#undef __USE_LARGEFILE64

#define FD_FREE   0
#define FD_SYS    1
#define FD_XPN    2

#define O_ACCMODE 00000003
#define O_RDONLY  00000000
#define O_WRONLY  00000001
#define O_RDWR    00000002
#ifndef O_CREAT
#define O_CREAT 00000100  /* not fcntl */
#endif
#ifndef O_EXCL
#define O_EXCL 00000200   /* not fcntl */
#endif
#ifndef O_NOCTTY
#define O_NOCTTY 00000400 /* not fcntl */
#endif
#ifndef O_TRUNC
#define O_TRUNC 00001000  /* not fcntl */
#endif
#ifndef O_APPEND
#define O_APPEND 00002000
#endif
#ifndef O_NONBLOCK
#define O_NONBLOCK 00004000
#endif
#ifndef O_DSYNC
#define O_DSYNC 00010000  /* used to be O_SYNC, see below */
#endif
#ifndef FASYNC
#define FASYNC 00020000   /* fcntl, for BSD compatibility */
#endif
#ifndef O_DIRECT
#define O_DIRECT 00040000 /* direct disk access hint */
#endif
#ifndef O_LARGEFILE
#define O_LARGEFILE 00100000
#endif
#ifndef O_DIRECTORY
#define O_DIRECTORY 00200000 /* must be a directory */
#endif
#ifndef O_NOFOLLOW
#define O_NOFOLLOW 00400000  /* don't follow links */
#endif
#ifndef O_NOATIME
#define O_NOATIME 01000000
#endif
#ifndef O_CLOEXEC
#define O_CLOEXEC 02000000 /* set close_on_exec */
#endif

/************************************************
 *  ... Typedef
 ***********************************************/

/** @struct __dirstream
 *  This is a struct
 *
 *  @var __dirstream::fd
 *    File descriptor.
 *  @var __dirstream::allocation
 *    Space allocated for the block.
 *  @var __dirstream::size
 *    Total valid data in the block.
 *  @var __dirstream::offset
 *    Current offset into the block.
 *  @var __dirstream::filepos
 *    Position of next entry to read.
 *  @var __dirstream::path
 *    A 'TODO'.
 */
struct __dirstream {
    int fd;
    //__libc_lock_define (, lock) /* Mutex lock for this structure.  */ //TODO
    size_t allocation;
    size_t size;
    size_t offset;
    off_t filepos;
    /* Directory block.  */
    char data[0] __attribute__((aligned(__alignof__(void *))));
    char *path;
};

/** @struct generic_fd
 *  This is a struct
 *
 *  @var generic_fd::type
 *    A 'TODO'.
 *  @var generic_fd::real_fd
 *    A 'TODO'.
 *  @var generic_fd::is_file
 *    A 'TODO'.
 */
struct generic_fd {
    int type;
    int real_fd;
    int is_file;
};

/************************************************
 *  ... Functions
 ***********************************************/

/************************************************
 *  ... File API
 ***********************************************/

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * @param flags  'TODO'.
 * 
 * @return 'TODO'.
 */
int open(const char *path, int flags, ...);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * @param flags  'TODO'.
 * 
 * @return 'TODO'.
 */
int open64(const char *path, int flags, ...);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * @param flags  'TODO'.
 * 
 * @return 'TODO'.
 */
int __open_2(const char *path, int flags, ...);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * @param mode  'TODO'.
 * 
 * @return 'TODO'.
 */
int creat(const char *path, mode_t mode);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fd  'TODO'.
 * 
 * @return 'TODO'.
 */
int close(int fd);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fildes  'TODO'.
 * @param length  'TODO'.
 * 
 * @return 'TODO'.
 */
int ftruncate(int fildes, off_t length);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fildes  'TODO'.
 * @param buf  'TODO'.
 * @param nbyte  'TODO'.
 * 
 * @return 'TODO'.
 */
ssize_t read(int fildes, void *buf, size_t nbyte);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fd  'TODO'.
 * @param buf  'TODO'.
 * @param count  'TODO'.
 * @param offset  'TODO'.
 * 
 * @return 'TODO'.
 */
ssize_t pread(int fd, void *buf, size_t count, off_t offset);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fd  'TODO'.
 * @param buf  'TODO'.
 * @param count  'TODO'.
 * @param offset  'TODO'.
 * 
 * @return 'TODO'.
 */
ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fildes  'TODO'.
 * @param buf  'TODO'.
 * @param nbyte  'TODO'.
 * 
 * @return 'TODO'.
 */
ssize_t write(int fildes, const void *buf, size_t nbyte);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fildes  'TODO'.
 * @param offset  'TODO'.
 * @param whence  'TODO'.
 * 
 * @return 'TODO'.
 */
off_t lseek(int fildes, off_t offset, int whence);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fd  'TODO'.
 * @param offset  'TODO'.
 * @param whence  'TODO'.
 * 
 * @return 'TODO'.
 */
off64_t lseek64(int fd, off64_t offset, int whence);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param ver  'TODO'.
 * @param path  'TODO'.
 * @param buf  'TODO'.
 * 
 * @return 'TODO'.
 */
int __lxstat(int ver, const char *path, struct stat *buf);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param ver  'TODO'.
 * @param path  'TODO'.
 * @param buf  'TODO'.
 * 
 * @return 'TODO'.
 */
int __lxstat64(int ver, const char *path, struct stat64 *buf);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param ver  'TODO'.
 * @param path  'TODO'.
 * @param buf  'TODO'.
 * 
 * @return 'TODO'.
 */
int __xstat(int ver, const char *path, struct stat *buf);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param ver  'TODO'.
 * @param path  'TODO'.
 * @param buf  'TODO'.
 * 
 * @return 'TODO'.
 */
int __xstat64(int ver, const char *path, struct stat64 *buf);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param ver  'TODO'.
 * @param fd  'TODO'.
 * @param buf  'TODO'.
 * 
 * @return 'TODO'.
 */
int __fxstat(int ver, int fd, struct stat *buf);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param ver  'TODO'.
 * @param fildes  'TODO'.
 * @param buf  'TODO'.
 * 
 * @return 'TODO'.
 */
int __fxstat64(int ver, int fildes, struct stat64 *buf);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param ver  'TODO'.
 * @param dirfd  'TODO'.
 * @param path  'TODO'.
 * @param buf  'TODO'.
 * @param flags  'TODO'.
 * 
 * @return 'TODO'.
 */
int __fxstatat(int ver, int dirfd, const char *path, struct stat *buf, int flags);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param ver  'TODO'.
 * @param dirfd  'TODO'.
 * @param path  'TODO'.
 * @param buf  'TODO'.
 * @param flags  'TODO'.
 * 
 * @return 'TODO'.
 */
int __fxstatat64(int ver, int dirfd, const char *path, struct stat64 *buf, int flags);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param old_path  'TODO'.
 * @param new_path  'TODO'.
 * 
 * @return 'TODO'.
 */
int rename(const char *old_path, const char *new_path);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * 
 * @return 'TODO'.
 */
int unlink(const char *path);

/************************************************
 *  ... File API (stdio)
 ***********************************************/

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * @param mode  'TODO'.
 * 
 * @return 'TODO'.
 */
FILE *fopen(const char *path, const char *mode);

/**
 * @brief 'TODO'.
 *
 * Create a new stream that refers to an existing system file descriptor.
 *
 * @param fd  'TODO'.
 * @param mode  'TODO'.
 * 
 * @return 'TODO'.
 */
FILE *fdopen(int fd, const char *mode);

/**
 * @brief Close STREAM.
 *
 * This function is a possible cancellation point and therefore not 
 * marked with __THROW.
 *
 * @param stream  'TODO'.
 * 
 * @return 'TODO'.
 */
int fclose(FILE *stream);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param ptr 'TODO'.
 * @param size 'TODO'.
 * @param nmemb 'TODO'.
 * @param stream 'TODO'.
 *  
 * @return 'TODO'.
 */
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param ptr 'TODO'.
 * @param size 'TODO'.
 * @param nmemb 'TODO'.
 * @param stream 'TODO'.
 *  
 * @return 'TODO'.
 */
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

/**
 * @brief Seek to a certain position on STREAM.
 *
 * This function is a possible cancellation point and therefore not 
 * marked with __THROW.
 *
 * @param stream  'TODO'.
 * @param offset  'TODO'.
 * @param whence  'TODO'.
 * 
 * @return 'TODO'.
 */
int fseek(FILE *stream, long int offset, int whence);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param stream 'TODO'.
 * 
 * @return 'TODO'.
 */
int feof(FILE *stream);

/************************************************
 *  ... Directory API
 ***********************************************/

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * @param mode  'TODO'.
 * 
 * @return 'TODO'.
 */
int mkdir(const char *path, mode_t mode);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param dirname  'TODO'.
 * 
 * @return 'TODO'.
 */
DIR *opendir(const char *dirname);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param dirp  'TODO'.
 * 
 * @return 'TODO'.
 */
struct dirent *readdir(DIR *dirp);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param dirp  'TODO'.
 * 
 * @return 'TODO'.
 */
struct dirent64 *readdir64(DIR *dirp);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param dirp  'TODO'.
 * 
 * @return 'TODO'.
 */
int closedir(DIR *dirp);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * 
 * @return 'TODO'.
 */
int rmdir(const char *path);

/************************************************
 *  ... Proccess API
 ***********************************************/
/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @par Parameters
 *    None.
 * @return 'TODO'.
 */
int fork(void);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param pipefd  'TODO'.
 * 
 * @return 'TODO'.
 */
int pipe(int pipefd[2]);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fildes  'TODO'.
 * 
 * @return 'TODO'.
 */
int dup(int fildes);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fildes  'TODO'.
 * @param fildes2  'TODO'.
 * 
 * @return 'TODO'.
 */
int dup2(int fildes, int fildes2);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param status  'TODO'.
 * @par Returns
 *    Nothing.
 */
void exit(int status);

/************************************************
 *  ... Manager API
 ***********************************************/
/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * 
 * @return 'TODO'.
 */
int chdir(const char *path);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * @param mode  'TODO'.
 * 
 * @return 'TODO'.
 */
int chmod(const char *path, mode_t mode);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fildes  'TODO'.
 * @param mode  'TODO'.
 * 
 * @return 'TODO'.
 */
int fchmod(int fildes, mode_t mode);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * @param owner  'TODO'.
 * @param group  'TODO'.
 * 
 * @return 'TODO'.
 */
int chown(const char *path, uid_t owner, gid_t group);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fd  'TODO'.
 * @param cmd  'TODO'.
 * @param arg  'TODO'.
 * 
 * @return 'TODO'.
 */
int fcntl(int fd, int cmd, long arg);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * @param mode  'TODO'.
 * 
 * @return 'TODO'.
 */
int access(const char *path, int mode);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * @param resolved_path  'TODO'.
 * 
 * @return 'TODO'.
 */
char *realpath(const char *restrict path, char *restrict resolved_path);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param path  'TODO'.
 * @param resolved_path  'TODO'.
 * @param resolved_len  'TODO'.
 * 
 * @return 'TODO'.
 */
char *__realpath_chk(const char *path, char *resolved_path, size_t resolved_len);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param fd  'TODO'.
 * 
 * @return 'TODO'.
 */
int fsync(int fd);

/************************************************
 *  ... MPI API
 ***********************************************/

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param argc  'TODO'.
 * @param argv  'TODO'.
 *
 * @return 'TODO'.
 */
int MPI_Init(int *argc, char ***argv);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @param argc  'TODO'.
 * @param argv  'TODO'.
 * @param required  'TODO'.
 * @param provided  'TODO'.
 *
 * @return 'TODO'.
 */
int MPI_Init_thread(int *argc, char ***argv, int required, int *provided);

/**
 * @brief 'TODO'.
 *
 * 'TODO'.
 *
 * @par Parameters
 *    None.
 * @return 'TODO'.
 */
int MPI_Finalize(void);

#endif
