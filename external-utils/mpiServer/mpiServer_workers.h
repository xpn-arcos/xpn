#ifndef _MPISERVER_WORKERS_H_
#define _MPISERVER_WORKERS_H_

  #include "all_system.h"
  #include "mpiServer_params.h"
  #include "mpiServer_workers_common.h"
  #include "mpiServer_workers_ondemand.h"
  #include "mpiServer_workers_pool.h"

  int mpiServer_workers_init     ( int thread_mode );
  int mpiServer_workers_launch   ( mpiServer_param_st * params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_function)(struct st_th) );
  void mpiServer_workers_destroy ( int thread_mode );


#endif
