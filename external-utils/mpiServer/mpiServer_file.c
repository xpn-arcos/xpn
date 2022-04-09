
  /*
   *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
   *
   *  This file is part of mpiServer.
   *
   *  mpiServer is free software: you can redistribute it and/or modify
   *  it under the terms of the GNU Lesser General Public License as published by
   *  the Free Software Foundation, either version 3 of the License, or
   *  (at your option) any later version.
   *
   *  mpiServer is distributed in the hope that it will be useful,
   *  but WITHOUT ANY WARRANTY; without even the implied warranty of
   *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   *  GNU Lesser General Public License for more details.
   *
   *  You should have received a copy of the GNU Lesser General Public License
   *  along with mpiServer.  If not, see <http://www.gnu.org/licenses/>.
   *
   */


  /* ... Include / Inclusion ........................................... */

     #include "mpiServer_file.h"


  /* ... Functions / Funciones ......................................... */

    int  mpiServer_file_open ( char *pathname, int flags )
    {
         return files_posix_open(pathname, flags) ;
    }

    int  mpiServer_file_creat ( mpiServer_param_st *params, char *pathname, mode_t mode )
    {
         int fd, ret ;

         fd = files_posix_creat(pathname, O_CREAT | O_RDWR, mode) ; // TODO: mpiServer_file_creat don't use 'int flags' and set 'O_CREAT | O_RDWR' ?
         if (fd == -1)
         {
             ret = files_posix_mkpath(pathname) ;
             fd = files_posix_creat(pathname, O_CREAT | O_RDWR, 0770) ;
         }

         return fd ;
    }

    int  mpiServer_file_close ( int fd )
    {
         return files_posix_close(fd) ;
    }

    long mpiServer_file_read_buffer ( mpiServer_param_st *params, int read_fd2, void *buffer, int buffer_size )
    {
         // check arguments...
         if (NULL == params) {
             debug_warning("WARNING[%s]:\t read with NULL mpiServer_param_st *.\n", params->srv_name) ;
         }
         if (0 == buffer_size) {
             debug_warning("WARNING[%s]:\t request for reading zero bytes.\n", params->srv_name) ;
         }

         return files_posix_read_buffer(read_fd2, buffer, buffer_size) ;
    }

    long mpiServer_file_write_buffer ( mpiServer_param_st *params, int write_fd2, void *buffer, int buffer_size, int num_readed_bytes )
    {
         // check arguments...
         if (NULL == params) {
             debug_warning("WARNING[%s]:\t read with NULL mpiServer_param_st *.\n", params->srv_name) ;
         }
         if (num_readed_bytes > buffer_size) {
             debug_error("ERROR[%s]:\t write for %d bytes from a buffer with only %d bytes.\n", params->srv_name, num_readed_bytes, buffer_size) ;
             return -1 ;
         }

         return files_posix_write_buffer(write_fd2, buffer, buffer_size, num_readed_bytes) ;
     }


  /* ................................................................... */

