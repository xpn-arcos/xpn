#ifndef _MPISERVER_WORKERS_ONDEMAND_H_
#define _MPISERVER_WORKERS_ONDEMAND_H_

  #include "all_system.h"
  #include "mpiServer_params.h"
  #include "mpiServer_workers_common.h"


     /*
      *  API
      */

     int   mpiServer_worker_ondemand_init      ( void ) ;
     //int   mpiServer_worker_ondemand_launch    ( mpiServer_param_st *params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_function)(struct st_th) ) ;
     int   mpiServer_worker_ondemand_launch    ( mpiServer_param_st *params, MPI_Comm sd, void (*worker_function)(struct st_th) ) ;
     void  mpiServer_workers_ondemand_destroy  ( void ) ;


     /* ................................................................... */

#endif

