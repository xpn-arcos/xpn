
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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


#ifndef _XPN_BYPASS_H_
#define _XPN_BYPASS_H_

  /* ... Include / Inclusion ........................................... */

  #define _GNU_SOURCE

  #include "config.h"

  #include <dlfcn.h>
  #include <sys/stat.h>
  #include <stdarg.h>

  #include "xpn.h"
  #include "syscall_proxies.h"

  #include <dirent.h>
  #include <string.h>
  #include "mpi.h"

  //#include<pthread.h> //Mutex


  /* ... Const / Const ................................................. */

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


  // Types
  #define O_ACCMODE 00000003
  #define O_RDONLY  00000000
  #define O_WRONLY  00000001
  #define O_RDWR    00000002
  #ifndef O_CREAT
  #define O_CREAT   00000100  // not fcntl
  #endif
  #ifndef O_EXCL
  #define O_EXCL    00000200  // not fcntl
  #endif
  #ifndef O_NOCTTY
  #define O_NOCTTY  00000400  // not fcntl
  #endif
  #ifndef O_TRUNC
  #define O_TRUNC   00001000  // not fcntl
  #endif
  #ifndef O_APPEND
  #define O_APPEND  00002000
  #endif
  #ifndef O_NONBLOCK
  #define O_NONBLOCK  00004000
  #endif
  #ifndef O_DSYNC
  #define O_DSYNC   00010000  // used to be O_SYNC, see below
  #endif
  #ifndef FASYNC
  #define FASYNC    00020000  // fcntl, for BSD compatibility
  #endif
  #ifndef O_DIRECT
  #define O_DIRECT  00040000  // direct disk access hint
  #endif
  #ifndef O_LARGEFILE
  #define O_LARGEFILE 00100000
  #endif
  #ifndef O_DIRECTORY
  #define O_DIRECTORY 00200000  // must be a directory
  #endif
  #ifndef O_NOFOLLOW
  #define O_NOFOLLOW  00400000  // don't follow links
  #endif
  #ifndef O_NOATIME
  #define O_NOATIME 01000000
  #endif
  #ifndef O_CLOEXEC
  #define O_CLOEXEC 02000000  // set close_on_exec */
  #endif


  /* ... Data structures / Estructuras de datos ........................ */

  struct __dirstream
  {
    int fd;                       // File descriptor.
    //__libc_lock_define (, lock) // Mutex lock for this structure. //TODO
    size_t allocation;            // Space allocated for the block.
    size_t size;                  // Total valid data in the block.
    size_t offset;                // Current offset into the block.
    off_t  filepos;               // Position of next entry to read.
    // Directory block.
    char data[0] __attribute__ ((aligned (__alignof__ (void*))));

    char * path;
  };

  struct generic_fd
  {
    int type;
    int real_fd;
    int is_file;
  };


  /* ... Functions / Funciones ......................................... */

  // File API

  int open       ( const char *path, int flags, ... );
  int open64     ( const char *path, int flags, ... );
  int __open_2   ( const char *path, int flags, ... );
  int creat      ( const char *path, mode_t mode );
  int close      ( int fd );

  int ftruncate  ( int fildes, off_t length );

  ssize_t read   ( int fildes,       void *buf, size_t nbyte );
  ssize_t write  ( int fildes, const void *buf, size_t nbyte );

  ssize_t pread    ( int fd, void *buf, size_t count, off_t offset );
  ssize_t pwrite   ( int fd, const void *buf, size_t count, off_t offset );
  ssize_t pread64  ( int fd, void *buf, size_t count, off_t offset );
  ssize_t pwrite64 ( int fd, const void *buf, size_t count, off_t offset );

  off_t   lseek   ( int fildes, off_t offset, int whence );
  off64_t lseek64 ( int fd,   off64_t offset, int whence );

  int __lxstat     ( int ver, const char *path, struct stat   *buf );
  int __lxstat64   ( int ver, const char *path, struct stat64 *buf );
  int __xstat      ( int ver, const char *path, struct stat   *buf );
  int __xstat64    ( int ver, const char *path, struct stat64 *buf );
  int __fxstat     ( int ver,           int fd, struct stat   *buf );
  int __fxstat64   ( int ver,       int fildes, struct stat64 *buf );
  int __fxstatat   ( int ver, int dirfd, const char *path, struct stat   *buf, int flags );
  int __fxstatat64 ( int ver, int dirfd, const char *path, struct stat64 *buf, int flags );

  int rename     ( const char *old_path, const char *new_path );
  int unlink     ( const char *path );
  int remove     ( const char *path );


  // File API (stdio)

  FILE * fopen  ( const char *path, const char *mode );
  FILE * fdopen ( int fd, const char *mode );
  int    fclose ( FILE *stream );

  size_t fread  ( void *ptr, size_t size, size_t nmemb, FILE *stream );
  size_t fwrite ( const void *ptr, size_t size, size_t nmemb, FILE *stream );

  int  fseek      ( FILE *stream, long int offset, int whence );
  long ftell      ( FILE *stream );
  int  dlsym_feof ( FILE *stream );


  // Directory API

  int   mkdir   ( const char *path, mode_t mode );
  DIR * opendir ( const char *dirname );

  struct dirent   *readdir   ( DIR *dirp );
  struct dirent64 *readdir64 ( DIR *dirp );

  int closedir ( DIR *dirp );
  int rmdir    ( const char *path );


  // Proccess API

  int fork  ( void );
  int dup   ( int fildes );
  int dup2  ( int fildes, int fildes2 );
  void exit ( int status );


  // Manager API

  int    chdir    ( const char *path );
  int    chmod    ( const char *path, mode_t mode );
  int    fchmod   (       int fildes, mode_t mode );
  int    chown    ( const char *path, uid_t owner, gid_t group );
  int    fcntl    ( int fd, int cmd, long arg );
  int    access   ( const char *path, int mode );
  char * realpath ( const char *restrict path, char *restrict resolved_path );
  char * __realpath_chk ( const char * path, char * resolved_path, size_t resolved_len );
  int    fsync ( int fd );
  int    flock ( int fd, int operation );


  // MPI API
  int MPI_Init ( int *argc, char ***argv );
  int MPI_Init_thread ( int *argc, char ***argv, int required, int *provided );
  int MPI_Finalize (void);


  /* ................................................................... */

#endif
