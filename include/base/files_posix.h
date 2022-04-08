
#ifndef _FILES_POSIX_H_
#define _FILES__POSIXH_

     #include "all_system.h"
     #include "mpiServer_params.h"
     #include <sys/stat.h>
     #include <sys/types.h>

     int  files_posix_open         ( char *pathname, int flags ) ;
     int  files_posix_creat        ( mpiServer_param_st *params, char *pathname, mode_t mode ) ;
     int  files_posix_close        ( int fd ) ;

     long files_posix_read_buffer  ( mpiServer_param_st *params, int read_fd2,  void *buffer, int buffer_size ) ;
     long files_posix_write_buffer ( mpiServer_param_st *params, int write_fd2, void *buffer, int buffer_size, int num_readed_bytes ) ;

#endif

