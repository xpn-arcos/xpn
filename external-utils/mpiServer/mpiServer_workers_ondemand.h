#ifndef _MPISERVER_WORKERS_ONDEMAND_H_
#define _MPISERVER_WORKERS_ONDEMAND_H_

  #include "all_system.h"
  #include "mpiServer_params.h"

/*
     / *
      * Constants
      * /

     #define MAX_THREADS 32
     #define MAX_OPERATIONS 256
     #define STACK_SIZE (256*KB)

     / *
      * Datatype
      * /

     struct st_th
     {
  	   mpiServer_param_st *params;
  	   MPI_Comm sd;
   	   int id;
       int type_op;
       int rank_client_id;
       void (*function)(struct st_th) ;
     };
*/

     /*
      *  API
      */

     int   mpiServer_init_worker   ( void ) ;
     int   mpiServer_launch_worker ( mpiServer_param_st *params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_function)(struct st_th) ) ;
     void  mpiServer_wait_workers  ( void ) ;


     /* ................................................................... */

#endif

