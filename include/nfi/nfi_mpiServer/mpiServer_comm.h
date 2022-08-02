#ifndef _MPISERVER_COMM_H_
#define _MPISERVER_COMM_H_

      #include "all_system.h"
      #include "mpiServer_params.h"
      #include "base/utils.h"
      #include "mpiServer_ops.h"

      /*
       *  API
       */

      int     mpiClient_comm_init       ( mpiClient_param_st *params ) ;
      int     mpiClient_comm_destroy    ( mpiClient_param_st *params ) ;
      int     mpiClient_comm_connect    ( mpiClient_param_st *params ) ;
      int     mpiClient_comm_disconnect ( mpiClient_param_st *params ) ;
      int     mpiClient_comm_locality   ( mpiClient_param_st *params ) ;

      ssize_t mpiClient_write_operation ( MPI_Comm fd, char *data, ssize_t size, char *msg_id ) ;
      ssize_t mpiClient_write_data      ( MPI_Comm fd, char *data, ssize_t size, char *msg_id ) ;
      ssize_t mpiClient_read_data       ( MPI_Comm fd, char *data, ssize_t size, char *msg_id ) ;

#endif

