
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

     long mpiServer_file_read_buffer ( mpiServer_param_st *params, int read_fd2, void *buffer, int buffer_size )
     {
	    ssize_t read_num_bytes       = -1 ;
	    ssize_t read_remaining_bytes = buffer_size ;
	    void   *read_buffer          = buffer ;

	    // check arguments...
	    if (NULL == params) {
	      debug_warning("WARNING[%s]:\t read with NULL mpiServer_param_st *.\n", params->srv_name) ;
	    }

	    while (read_remaining_bytes > 0)
	    {
	      /* Read from local file... */
	      read_num_bytes = read(read_fd2, read_buffer, read_remaining_bytes) ;

	      /* Check errors */
	      if (read_num_bytes == -1) {
		  perror("read: ") ;
		  debug_error("ERROR[%s]:\t read fails to read data.\n", params->srv_name) ;
		  return -1 ;
	      }

	      /* Check end of file */
	      if (read_num_bytes == 0)
	      {
		debug_error("INFO[%s]:\t end of file, readed %ld.\n", params->srv_name, 
								      (buffer_size - read_remaining_bytes)) ;

		return (buffer_size - read_remaining_bytes) ;
	      }

	      read_remaining_bytes -= read_num_bytes ;
	      read_buffer          += read_num_bytes ;
	    }

	    return buffer_size ;
     }

     long mpiServer_file_write_buffer ( mpiServer_param_st *params, int write_fd2, void *buffer, int buffer_size, int num_readed_bytes )
     {
	    ssize_t write_num_bytes       = -1 ;
	    ssize_t write_remaining_bytes = num_readed_bytes ;
	    void   *write_buffer          = buffer ;

	    // check arguments...
	    if (NULL == params) {
	      debug_warning("WARNING[%s]:\t read with NULL mpiServer_param_st *.\n", params->srv_name) ;
	    }
	    if (num_readed_bytes > buffer_size) {
	      debug_error("ERROR[%s]:\t write for %d bytes from a buffer with only %d bytes.\n", params->srv_name, num_readed_bytes, buffer_size) ;
	      return -1 ;
	    }

	    while (write_remaining_bytes > 0)
	    {
	      /* Write into local file (write_fd2)... */
	      write_num_bytes = write(write_fd2, write_buffer, write_remaining_bytes) ;

	      /* Check errors */
	      if (write_num_bytes == -1) {
		  perror("write: ") ;
		  debug_error("ERROR[%s]:\t write fails to write data.\n", params->srv_name) ;
		  return -1 ;
	      }

	      write_remaining_bytes -= write_num_bytes ;
	      write_buffer          += write_num_bytes ;
	    }

	    return num_readed_bytes ;
     }


  /* ................................................................... */

