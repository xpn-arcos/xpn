
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


    /*
     * API
     */

    int  files_posix_mkpath ( char *pathname )
    {
	 int ret ;
         char dir[MAXPATHLEN] ;

         DEBUG_BEGIN() ;

         for (int i=0; aux_get_dirs(path, i, dir) != 0; i++)
         {
              ret = mkdir(dir, 0770) ;
	      if (ret < 0) {
                  debug_warning("[FILE_POSIX]: cannot mkdir(%s)\n", dir) ;
                  perror("mkdir: ") ;
	      }
         }

         DEBUG_END() ;

	 // Return OK
         return 1 ;
    }

    int  files_posix_creat ( char *pathname, int flags, mode_t mode )
    {
	 int ret ;

         DEBUG_BEGIN() ;

         // Check params
         if (NULL == pathname) {
             debug_warning("[FILE_POSIX]: pathname is NULL\n") ;
         }
         if (0 == mode) {
             debug_warning("[FILE_POSIX]: mode is zero\n") ;
         }

	 // Try to open file
         ret = open(pathname, flags, mode) ;
         if (ret < 0) {
             debug_warning("[FILE_POSIX]: open(pathname:%s, flags:%d, mode:%d) -> %d\n", pathname, flags, mode, ret) ;
             perror("open: ") ;
         }

         DEBUG_END() ;

	 // Return OK/KO
	 return ret ;
    }

    int  files_posix_open ( char *pathname, int flags )
    {
	 int ret ;

         DEBUG_BEGIN() ;

         // Check params
         if (NULL == pathname) {
             debug_warning("[FILE_POSIX]: pathname is NULL\n") ;
         }

	 // Try to open file
         ret = open(pathname, flags) ;
         if (ret < 0) {
             debug_warning("[FILE_POSIX]: open(pathname:%s, flags:%d) -> %d\n", pathname, flags, ret) ;
             perror("open: ") ;
         }

         DEBUG_END() ;

	 // Return OK/KO
	 return ret ;
    }

    int  files_posix_close ( int fd )
    {
	 int ret ;

         DEBUG_BEGIN() ;

         // Check params
         if (fd < 0){
             debug_warning("[FILE_POSIX]: close file with fd < 0\n") ;
         }

	 // Try to close file
         ret = close(fd) ;
         if (ret < 0) {
             debug_warning("[FILE_POSIX]: close(fd:%d) -> %d\n", fd, ret) ;
             perror("close: ") ;
         }

         DEBUG_END() ;

	 // Return OK/KO
	 return ret ;
    }

    long files_posix_read_buffer ( int read_fd2, void *buffer, int buffer_size )
    {
         ssize_t read_num_bytes       = -1 ;
         ssize_t read_remaining_bytes = buffer_size ;
         void   *read_buffer          = buffer ;

         // check arguments...
         if (NULL == buffer) {
             debug_warning("[FILE_POSIX]: read_buffer with NULL buffer\n") ;
         }

         while (read_remaining_bytes > 0)
         {
             /* Read from local file... */
             read_num_bytes = read(read_fd2, read_buffer, read_remaining_bytes) ;

             /* Check errors */
             if (read_num_bytes == -1) {
                 perror("read: ") ;
                 debug_error("[FILE_POSIX]: read fails to read data.\n") ;
                 return -1 ;
             }

             /* Check end of file */
             if (read_num_bytes == 0)
             {
                 debug_error("[FILE_POSIX]: end of file, readed %ld.\n", (buffer_size - read_remaining_bytes)) ;
                 return (buffer_size - read_remaining_bytes) ;
             }

             read_remaining_bytes -= read_num_bytes ;
             read_buffer          += read_num_bytes ;
         }

         return buffer_size ;
    }

    long files_posix_write_buffer ( int write_fd2, void *buffer, int buffer_size, int num_readed_bytes )
    {
         ssize_t write_num_bytes       = -1 ;
         ssize_t write_remaining_bytes = num_readed_bytes ;
         void   *write_buffer          = buffer ;

         // check arguments...
         if (NULL == buffer) {
             debug_warning("[FILE_POSIX]: read_buffer with NULL buffer\n") ;
         }
         if (num_readed_bytes > buffer_size) {
             debug_error("[FILE_POSIX]: write for %d bytes from a buffer with only %d bytes.\n", num_readed_bytes, buffer_size) ;
             return -1 ;
         }

         while (write_remaining_bytes > 0)
         {
             /* Write into local file (write_fd2)... */
             write_num_bytes = write(write_fd2, write_buffer, write_remaining_bytes) ;

             /* Check errors */
             if (write_num_bytes == -1) {
                 perror("write: ") ;
                 debug_error("[FILE_POSIX]: write fails to write data.\n") ;
                 return -1 ;
             }

             write_remaining_bytes -= write_num_bytes ;
             write_buffer          += write_num_bytes ;
         }

         return num_readed_bytes ;
     }


  /* ................................................................... */

