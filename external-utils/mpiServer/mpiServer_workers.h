#ifndef _MPISERVER_WORKERS_H_
#define _MPISERVER_WORKERS_H_

   #include "all_system.h"
   #include "mpiServer_params.h"
   #include "mpiServer_ops.h"
   #include "mpiServer_comm.h"


   /*
    * Constants
    */

   #define MAX_THREADS 1024 
   #define STACK_SIZE (256*KB)


   /*
    * Datatype
    */

   struct st_th
   {
	mpiServer_param_st *params;
	int sd;
	int id;
        void (*function)(struct st_th) ;
   };


   /*
    * API
    */

   int  mpiServer_init_worker   ( void ) ;
   int  mpiServer_launch_worker ( mpiServer_param_st *params, 
		                  int sd, 
                                  void (*worker_function)(struct st_th) ) ;
   void mpiServer_worker_run    ( void *arg ) ;
   void mpiServer_wait_workers  ( void ) ;

#endif

