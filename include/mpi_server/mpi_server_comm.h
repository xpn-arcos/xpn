#ifndef _MPI_SERVER_COMM_H_
#define _MPI_SERVER_COMM_H_

  #include "all_system.h"
  #include "base/utils.h"
  #include "base/time_misc.h"
  #include "mpi_server_params.h"
  #include "mpi_server_ns.h"


  /*
   *  API
   */

  int      mpi_server_comm_init      ( mpi_server_param_st *params ) ;
  int      mpi_server_comm_destroy   ( mpi_server_param_st *params ) ;

  MPI_Comm mpi_server_comm_accept    ( mpi_server_param_st *params ) ;
  int      mpiClient_comm_close     ( MPI_Comm fd ) ;

  ssize_t mpi_server_comm_write_data     ( mpi_server_param_st *params, MPI_Comm fd, char *data, ssize_t size, int  rank_client_id ) ;
  ssize_t mpi_server_comm_read_operation ( mpi_server_param_st *params, MPI_Comm fd, char *data, ssize_t size, int *rank_client_id ) ;
  ssize_t mpi_server_comm_read_data      ( mpi_server_param_st *params, MPI_Comm fd, char *data, ssize_t size, int  rank_client_id ) ;

#endif
