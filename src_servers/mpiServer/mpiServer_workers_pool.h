#ifndef _MPISERVER_WORKERS_POOL_H_
#define _MPISERVER_WORKERS_POOL_H_

  #include "all_system.h"
  #include "mpiServer_params.h"
  #include "mpiServer_ops.h"
  #include "mpiServer_comm.h"
  #include "mpiServer_workers_common.h"


  /*
   * API
   */

  int          mpiServer_worker_pool_init    ( void ) ;
  void         mpiServer_worker_pool_destroy ( void ) ;

  //void         mpiServer_worker_pool_enqueue ( MPI_Comm sd, mpiServer_param_st *params, int op_type, int rank_client_id, void (*worker_function)(struct st_th)) ;
  void         mpiServer_worker_pool_enqueue ( MPI_Comm sd, mpiServer_param_st *params, void (*worker_function)(struct st_th)) ;
  struct st_th mpiServer_worker_pool_dequeue ( void ) ;

#endif
