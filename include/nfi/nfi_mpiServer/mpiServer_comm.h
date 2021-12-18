#ifndef _MPISERVER_COMM_H_
#define _MPISERVER_COMM_H_

   #include "all_system.h"
   #include "mpiServer_params.h"
   #include "mpiServer_utils.h"
   #include "mpiServer_ops.h"


   /*
    *  API
    */

   int     mpiServer_comm_init      ( mpiServer_param_st *params ) ;
   int     mpiServer_comm_destroy   ( mpiServer_param_st *params ) ;

   int     mpiServer_comm_accept    ( mpiServer_param_st *params ) ;
   int     mpiServer_comm_close     ( mpiServer_param_st *params ) ;

   ssize_t mpiServer_comm_writedata ( mpiServer_param_st *params, int fd, char *data, ssize_t size ) ;
   ssize_t mpiServer_comm_readdata  ( mpiServer_param_st *params, int fd, char *data, ssize_t size ) ;

#endif
