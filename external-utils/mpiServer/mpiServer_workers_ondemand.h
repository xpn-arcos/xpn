#ifndef _MPISERVER_WORKERS_ONDEMAND_H_
#define _MPISERVER_WORKERS_ONDEMAND_H_

  #include "all_system.h"
  #include "mpiServer_params.h"
  #include "mpiServer_workers_common.h"


     /*
      *  API
      */

     int   mpiServer_init_worker   ( void ) ;
     int   mpiServer_launch_worker ( mpiServer_param_st *params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_function)(struct st_th) ) ;
     void  mpiServer_wait_workers  ( void ) ;


     /* ................................................................... */

#endif

