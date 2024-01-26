
/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "syscall_proxies.h"
  #include "base/utils.h"
  #include <pthread.h> 
  #include <sys/stat.h>
  #include <sys/types.h>


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */

  typedef off_t offset_t;

  // <IMPORTANT>: next two lines MUST be before each file in xpn_client that uses filesystem.h BUT NOT in mpi_server that uses filesystem.h
  //   #define ASYNC_CLOSE 1
  //   #define FILESYSTEM_DLSYM 1
  // </IMPORTANT>


  /* ... Functions / Funciones ......................................... */

  //int  filesystem_init      ( void );
  //int  filesystem_destroy   ( void );

  int  filesystem_creat     ( char *pathname, mode_t mode );
  int  filesystem_open      ( char *pathname, int flags );
  int  filesystem_open2     ( char *pathname, int flags, mode_t mode );
  int  filesystem_close     ( int fd );

  ssize_t filesystem_read   ( int read_fd2,  void *buffer, size_t buffer_size );
  ssize_t filesystem_write  ( int write_fd2, void *buffer, size_t num_bytes_to_write );

  int  filesystem_rename    ( char *old_pathname, char *new_pathname );

  int  filesystem_mkpath    ( char *pathname );
  int  filesystem_mkdir     ( char *pathname, mode_t mode );
  int  filesystem_rmdir     ( char *pathname );

  DIR           *filesystem_opendir  ( char *name );
  struct dirent *filesystem_readdir  ( DIR  *dirp );
  int            filesystem_closedir ( DIR  *dirp );

  off_t filesystem_lseek    ( int fd, off_t offset, int whence );
  int   filesystem_unlink   ( char *pathname );
  int   filesystem_stat     ( char *pathname, struct stat *sinfo );


  /* ... Macros / Macros .................................................. */

  #ifdef FILESYSTEM_DLSYM
    #define real_posix_creat(path,mode)                      dlsym_creat(path,mode)
    #define real_posix_open(path,flags)                      dlsym_open(path,flags)
    #define real_posix_open2(path,flags,mode)                dlsym_open2(path,flags,mode)
    #define real_posix_close(fd)                             dlsym_close(fd)

    #define real_posix_lseek(fd,offset,whence)               dlsym_lseek(fd,offset,whence)
    #define real_posix_lseek64(fd,offset,whence)             dlsym_lseek64(fd,offset,whence)
    #define real_posix_read(fd,buffer,buffer_size)           dlsym_read(fd,buffer,buffer_size)
    #define real_posix_write(fd,buffer,buffer_size)          dlsym_write(fd,buffer,buffer_size)

    #define real_posix_rename(old_path, new_path)            dlsym_rename(old_path, new_path)

    #define real_posix_mkdir(pathname,mode)                  dlsym_mkdir(pathname,mode)
    #define real_posix_rmdir(pathname)                       dlsym_rmdir(pathname)
    #define real_posix_unlink(pathname)                      dlsym_unlink(pathname)
    //#define real_posix_stat(pathname,info)                   dlsym_xstat64(pathname,info)
    #define real_posix_stat(pathname,info)                   stat(pathname,info)


    #define real_posix_opendir(pathname)                     dlsym_opendir(pathname)
    #define real_posix_readdir(dirptr)                       dlsym_readdir(dirptr)
    #define real_posix_closedir(dirptr)                      dlsym_closedir(dirptr)
  #else
    #define real_posix_creat(path,mode)                      creat(path,mode)
    #define real_posix_open(path,flags)                      open(path,flags)
    #define real_posix_open2(path,flags,mode)                open(path,flags,mode)
    #define real_posix_close(fd)                             close(fd)

    #define real_posix_lseek(fd,offset,whence)               lseek(fd,offset,whence)
    #define real_posix_lseek64(fd,offset,whence)             lseek64(fd,offset,whence)
    #define real_posix_read(fd,buffer,buffer_size)           read(fd,buffer,buffer_size)
    #define real_posix_write(fd,buffer,buffer_size)          write(fd,buffer,buffer_size)

    #define real_posix_rename(old_path, new_path)            rename(old_path, new_path)

    #define real_posix_mkdir(pathname,mode)                  mkdir(pathname,mode)
    #define real_posix_rmdir(pathname)                       rmdir(pathname)
    #define real_posix_unlink(pathname)                      unlink(pathname)
    #define real_posix_stat(pathname,info)                   stat(pathname,info)

    #define real_posix_opendir(pathname)                     opendir(pathname)
    #define real_posix_readdir(dirptr)                       readdir(dirptr)
    #define real_posix_closedir(dirptr)                      closedir(dirptr)
  #endif

#endif
