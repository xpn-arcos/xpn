#ifndef _MPISERVER_COMM_H_
#define _MPISERVER_COMM_H_

  #include "all_system.h"
  #include "base/utils.h"
  #include "mpiServer_params.h"


  /*
   *  API
   */

  int     mpiServer_comm_init      ( mpiServer_param_st *params ) ;
  int     mpiServer_comm_destroy   ( mpiServer_param_st *params ) ;

  int     mpiServer_comm_accept    ( mpiServer_param_st *params ) ;
  int     mpiServer_comm_close     ( mpiServer_param_st *params ) ;

  ssize_t mpiServer_comm_write_data     ( mpiServer_param_st *params, MPI_Comm fd, char *data, ssize_t size, int  rank_client_id ) ;
  ssize_t mpiServer_comm_read_operation ( mpiServer_param_st *params, MPI_Comm fd, char *data, ssize_t size, int *rank_client_id ) ;
  ssize_t mpiServer_comm_read_data      ( mpiServer_param_st *params, MPI_Comm fd, char *data, ssize_t size, int  rank_client_id ) ;


  //TODO
  ssize_t mpiServer_comm_read_operation_client ( mpiServer_param_st *params, MPI_Comm fd, char *data, ssize_t size, int rank_client_id );

#endif
