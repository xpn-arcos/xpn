#ifndef _MPI_SERVER_WORKERS_POOL_H_
#define _MPI_SERVER_WORKERS_POOL_H_

  #include "all_system.h"
  #include "mpi_server_params.h"
  #include "mpi_server_ops.h"
  #include "mpi_server_comm.h"
  #include "mpi_server_workers_common.h"

  //Thread count multiplier
  #define POOL_OVERSUSCRIPTION 2


  /*
   * API
   */

  int          mpi_server_worker_pool_init    ( void ) ;
  void         mpi_server_worker_pool_destroy ( void ) ;

  void         mpi_server_worker_pool_enqueue ( mpi_server_param_st *params, MPI_Comm sd, int op_type, int rank_client_id, void (*worker_function)(struct st_th)) ;
  struct st_th mpi_server_worker_pool_dequeue ( void ) ;

#endif
