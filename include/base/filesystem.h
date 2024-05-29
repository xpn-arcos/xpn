
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

  // <IMPORTANT for XPN>
  //   -> In xpn_client, this line must be added at the beginning:
  //      filesystem_low_set(RTLD_NEXT) ;
  //   -> BUT in mpi_server, this line must be added at the beginning:
  //      filesystem_low_set(RTLD_DEFAULT) ;
  //   -> In order to close in a thread (background), then use:
  //      #define ASYNC_CLOSE 1
  // </IMPORTANT for XPN>


  /* ... Data structures / Estructuras de datos ........................ */

     typedef off_t offset_t;


  /* ... Functions / Funciones ......................................... */

    int  filesystem_low_set ( void * new_rtld ) ;
  //int  filesystem_init      ( void );
  //int  filesystem_destroy   ( void );

    int  filesystem_creat     ( char *pathname, mode_t mode );
    int  filesystem_open      ( char *pathname, int flags );
    int  filesystem_open2     ( char *pathname, int flags, mode_t mode );
    int  filesystem_close     ( int fd );

    ssize_t filesystem_read   ( int read_fd2,  void *buffer, size_t buffer_size );
    ssize_t filesystem_write  ( int write_fd2, void *buffer, size_t num_bytes_to_write );

    int  filesystem_mkdir     ( char *pathname, mode_t mode );
    int  filesystem_rmdir     ( char *pathname );
    int  filesystem_mkpath    ( char *pathname );

    DIR           *filesystem_opendir  ( char *name );
    long           filesystem_telldir  ( DIR  *dirp );
    void           filesystem_seekdir  ( DIR  *dirp, long loc );
    struct dirent *filesystem_readdir  ( DIR  *dirp );
    int            filesystem_closedir ( DIR  *dirp );

    int  filesystem_rename    ( char *old_pathname, char *new_pathname );
    off_t filesystem_lseek    ( int fd, off_t offset, int whence );
    int   filesystem_unlink   ( char *pathname );
    int   filesystem_stat     ( char *pathname, struct stat *sinfo );


  /* ...................................................................... */

#endif

