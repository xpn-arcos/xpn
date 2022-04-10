/*
 *  Copyright 2020-2021 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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
      #include "mpiServer_params.h"
      #include <sys/stat.h>
      #include <sys/types.h>


  /* ... Defines / Definiciones ........................................... */

#ifdef _LARGEFILE64_
     typedef long long int offset_t;
#else
     typedef off_t offset_t;
#endif

  // DLSYM
#ifdef FILESYSTEM_DLSYM
     #define real_posix_creat(path,mode)                      dlsym_creat(path,mode)
     #define real_posix_open(path,flags)                      dlsym_open(path,flags)
     #define real_posix_close(fd)                             dlsym_close(fd)

     #define real_posix_lseek(fd,offset,whence)               dlsym_lseek(fd,offset,whence)
     #define real_posix_read(fd,buffer,buffer_size)           dlsym_read(fd,buffer,buffer_size)
     #define real_posix_write(fd,buffer,buffer_size)          dlsym_write(fd,buffer,buffer_size)

     #define real_posix_mkdir(pathname,mode)                  dlsym_mkdir(pathname,mode)
     #define real_posix_rmdir(pathname)                       dlsym_rmdir(pathname)
     #define real_posix_unlink(pathname)                      dlsym_unlink(pathname)
     #define real_posix_stat(pathname,info)                   dlsym_stat(pathname,info)

     #define real_posix_opendir(pathname)                     dlsym_opendir(pathname)
     #define real_posix_readdir(dirptr)                       dlsym_readdir(dirptr)
     #define real_posix_closedir(dirptr)                      dlsym_closedir(dirptr)
#else
     #define real_posix_creat(path,mode)                      creat(path,mode)
     #define real_posix_open(path,flags)                      open(path,flags)
     #define real_posix_close(fd)                             close(fd)

     #define real_posix_lseek(fd,offset,whence)               lseek(fd,offset,whence)
     #define real_posix_read(fd,buffer,buffer_size)           read(fd,buffer,buffer_size)
     #define real_posix_write(fd,buffer,buffer_size)          write(fd,buffer,buffer_size)

     #define real_posix_mkdir(pathname,mode)                  mkdir(pathname,mode)
     #define real_posix_rmdir(pathname)                       rmdir(pathname)
     #define real_posix_unlink(pathname)                      unlink(pathname)
     #define real_posix_stat(pathname,info)                   stat(pathname,info)

     #define real_posix_opendir(pathname)                     opendir(pathname)
     #define real_posix_readdir(dirptr)                       readdir(dirptr)
     #define real_posix_closedir(dirptr)                      closedir(dirptr)
#endif


   /* ... Functions / Funciones ......................................... */

     int  filesystem_creat        ( char *pathname, mode_t mode ) ;
     int  filesystem_open         ( char *pathname, int flags ) ;
     int  filesystem_close        ( int fd ) ;

     long filesystem_read_buffer  ( int read_fd2,  void *buffer, int buffer_size ) ;
     long filesystem_write_buffer ( int write_fd2, void *buffer, int buffer_size, int num_readed_bytes ) ;

     int  filesystem_mkpath       ( char *pathname ) ;


#endif

