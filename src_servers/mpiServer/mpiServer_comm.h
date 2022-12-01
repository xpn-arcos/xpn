#ifndef _MPISERVER_COMM_H_
#define _MPISERVER_COMM_H_

  #include "all_system.h"
  #include "base/utils.h"
  #include "base/time_misc.h"
  #include "mpiServer_params.h"
  #include "mpiServer_ns.h"


  /*
   *  API
   */

  int      mpiServer_comm_init      ( mpiServer_param_st *params ) ;
  int      mpiServer_comm_destroy   ( mpiServer_param_st *params ) ;

  MPI_Comm mpiServer_comm_accept    ( mpiServer_param_st *params ) ;
  int      mpiClient_comm_close     ( MPI_Comm fd ) ;

  ssize_t mpiServer_comm_write_data     ( mpiServer_param_st *params, MPI_Comm fd, char *data, ssize_t size, int  rank_client_id ) ;
  ssize_t mpiServer_comm_read_operation ( mpiServer_param_st *params, MPI_Comm fd, char *data, ssize_t size, int *rank_client_id ) ;
  ssize_t mpiServer_comm_read_data      ( mpiServer_param_st *params, MPI_Comm fd, char *data, ssize_t size, int  rank_client_id ) ;

#endif
