
#ifndef _MPISERVER_FILE_H_
#define _MPISERVER_FILE_H_

     #include "all_system.h"
     #include "mpiServer_params.h"
     #include <sys/stat.h>
     #include <sys/types.h>


     int  mpiServer_file_open         ( char *pathname, int flags ) ;
     int  mpiServer_file_creat        ( mpiServer_param_st *params, char *pathname, mode_t mode ) ;
     int  mpiServer_file_close        ( int fd ) ;

     long mpiServer_file_read_buffer  ( mpiServer_param_st *params, int read_fd2,  void *buffer, int buffer_size ) ;
     long mpiServer_file_write_buffer ( mpiServer_param_st *params, int write_fd2, void *buffer, int buffer_size, int num_readed_bytes ) ;

#endif

