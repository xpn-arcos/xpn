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

#ifndef _FILES_POSIX_H_
#define _FILES_POSIX_H_


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
#ifdef DLSYM_ON
     #define real_posix_creat(path,flags,mode)                dlsym_creat(path,flags,mode)
     #define real_posix_open(path,flags)                      dlsym_open(path,flags)
     #define real_posix_close(fd)                             dlsym_close(fd)
     #define real_posix_read_buffer(fd,buffer,buffer_size)    dlsym_read(fd,buffer,buffer_size)
     #define real_posix_write_buffer(fd,buffer,buffer_size)   dlsym_write(fd,buffer,buffer_size)
     #define real_posix_mkdir(pathname)                       dlsym_mkdir(pathname)
#else
     #define real_posix_creat(path,flags,mode)                creat(path,flags,mode)
     #define real_posix_open(path,flags)                      open(path,flags)
     #define real_posix_close(fd)                             close(fd)
     #define real_posix_read_buffer(fd,buffer,buffer_size)    read(fd,buffer,buffer_size)
     #define real_posix_write_buffer(fd,buffer,buffer_size)   write(fd,buffer,buffer_size)
     #define real_posix_mkdir(pathname)                       mkdir(pathname)
#endif


   /* ... Functions / Funciones ......................................... */

     int  files_posix_creat        ( char *pathname, int flags, mode_t mode ) ;
     int  files_posix_open         ( char *pathname, int flags ) ;
     int  files_posix_close        ( int fd ) ;

     long files_posix_read_buffer  ( int read_fd2,  void *buffer, int buffer_size ) ;
     long files_posix_write_buffer ( int write_fd2, void *buffer, int buffer_size, int num_readed_bytes ) ;

     int  files_posix_mkpath       ( char *pathname ) ;


#endif

