
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


#ifndef _FILESYSTEM_LOW_H_
#define _FILESYSTEM_LOW_H_

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "syscall_proxies.h"
  #include "base/utils.h"
  #include <sys/stat.h>
  #include <sys/types.h>


  /* ... Const / Const ................................................. */

  // <IMPORTANT for XPN>
  //   fs_low uses DLSYM in xpn_client BUT NOT in mpi_server
  // </IMPORTANT for XPN>


  /* ... Data structures / Estructuras de datos ........................ */

     typedef off_t offset_t;


  /* ... Functions / Funciones ......................................... */

    int  fs_low_set ( void * new_rtld ) ;

    // Files
    int     fs_low_creat ( char * pathname, mode_t mode ) ;
    int     fs_low_open  ( char * pathname, int flags ) ;
    int     fs_low_open2 ( char * pathname, int flags, mode_t mode ) ;
    int     fs_low_close ( int fd ) ;
    ssize_t fs_low_read  ( int read_fd2, void * buffer, size_t buffer_size ) ;
    ssize_t fs_low_write ( int write_fd2, void * buffer, size_t num_bytes_to_write ) ;
    off_t   fs_low_lseek ( int fd, off_t offset, int whence ) ;
    off64_t fs_low_lseek64 ( int fd, off64_t offset, int whence ) ;
    int     fs_low_stat  ( char * pathname, struct stat * sinfo ) ;
    
    // Entries in directory
    int     fs_low_mkdir   ( char * pathname, mode_t mode ) ;
    int     fs_low_rmdir   ( char * pathname ) ;
    int     fs_low_unlink  ( char * pathname ) ;
    int     fs_low_rename  ( char * old_pathname, char * new_pathname ) ;
    
    // Directory listing
    DIR *   fs_low_opendir ( char * pathname ) ;
    long    fs_low_telldir ( DIR * dirp ) ;
    void    fs_low_seekdir ( DIR * dirp, long loc ) ;
    struct dirent * fs_low_readdir ( DIR * dirp ) ;
    int     fs_low_closedir ( DIR * dirp ) ;


  /* ................................................................... */


#endif

