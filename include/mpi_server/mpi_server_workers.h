#ifndef _MPI_SERVER_WORKERS_H_
#define _MPI_SERVER_WORKERS_H_

  #include "all_system.h"
  #include "mpi_server_params.h"
  #include "mpi_server_workers_common.h"
  #include "mpi_server_workers_ondemand.h"
  #include "mpi_server_workers_pool.h"

  int mpi_server_workers_init     ( int thread_mode );
  int mpi_server_workers_launch   ( mpi_server_param_st * params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_function)(struct st_th) );
  void mpi_server_workers_destroy ( int thread_mode );


#endif
