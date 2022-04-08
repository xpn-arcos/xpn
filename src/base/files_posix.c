
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

    
     #include "files_posix.h"


  /* ... Functions / Funciones ......................................... */

    /*
     * Internal
     */

    int aux_clear_dirs ( char *path )
    {
      int j;
      char ant = '\0', s[255];

      j=0;
      for (unsigned i=0; i < strlen(path); i++)
      {
        switch(path[i])
        {
          case '/':
            if(ant != '/'){
              ant = s[j] = '/';
              j++;
            }
            break;
          default:
            ant = s[j] = path[i];
            j++;
        }

        s[j] = '\0';
      }

      strcpy(path, s);
      return 0;
    }

    int aux_get_dirs ( char *path, int n, char *s )
    {
      long unsigned i=0;
      long j=0, ant=-1, pos=-1;
      int cont=-1;
      char new_path[MAXPATHLEN];

      strcpy(new_path, path);

      path = new_path;
      aux_clear_dirs(path);
      s[0] = '\0';
      if (path == NULL) {
        return 0;
      }

      for (i=0; i<strlen(path) && cont<(n+1); i++)
      {
        if (path[i] == '/'){
          if(ant == -1){
            ant = pos = i;
          }
          pos = i;
          cont++;
        }
      }

      if(cont<(n+1)){
        return 0;
      }

      for(j=ant; j<pos; j++){
        s[j] = path[j];
      }

      s[j] = '\0';

      return strlen(s);
     }

    int mpiServer_create_spacename ( mpiServer_param_st *params, char *path )
    {
      int i;
      char dir[MAXPATHLEN];

      // check params...
      if (NULL == params) {
        return -1;
      }

      debug_info("[OPS] (ID=%s) mpiServer_create_spacename: %s\n", params->srv_name, path);
      for (i=0; 0 != aux_get_dirs(path, i, dir); i++)
      {
        mkdir(dir, 0777);
      }

      return 0;
     }


    /*
     * API
     */

    int  files_posix_open ( char *pathname, int flags )
    {
      return open(pathname, flags) ;
    }

    int  files_posix_creat ( mpiServer_param_st *params, char *pathname, mode_t mode )
    {
      int fd ;

      fd = open(pathname, O_CREAT | O_RDWR, mode);
      if (fd == -1)
      {
        mpiServer_create_spacename(params, pathname);
        fd = open(pathname, O_CREAT | O_RDWR, 0660);
      }

      return fd ;
    }

    int  files_posix_close ( int fd )
    {
      return close(fd) ;
    }

    long files_posix_read_buffer ( mpiServer_param_st *params, int read_fd2, void *buffer, int buffer_size )
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

    long files_posix_write_buffer ( mpiServer_param_st *params, int write_fd2, void *buffer, int buffer_size, int num_readed_bytes )
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

