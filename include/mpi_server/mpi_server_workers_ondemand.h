#ifndef _MPI_SERVER_WORKERS_ONDEMAND_H_
#define _MPI_SERVER_WORKERS_ONDEMAND_H_

  #include "all_system.h"
  #include "mpi_server_params.h"
  #include "mpi_server_workers_common.h"


     /*
      *  API
      */

     int   mpi_server_worker_ondemand_init      ( void ) ;
     int   mpi_server_worker_ondemand_launch    ( mpi_server_param_st *params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_function)(struct st_th) ) ;
     void  mpi_server_workers_ondemand_destroy  ( void ) ;


     /* ................................................................... */

#endif

