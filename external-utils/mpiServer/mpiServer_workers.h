#ifndef _MPISERVER_WORKERS_H_
#define _MPISERVER_WORKERS_H_

   #include "all_system.h"
   #include "mpiServer_params.h"
   #include "mpiServer_ops.h"
   #include "mpiServer_comm.h"


   /*
    * Constants
    */

   #define MAX_THREADS 32
   #define MAX_OPERATIONS 256
   #define STACK_SIZE (256*KB)


   /*
    * Datatype
    */

   struct st_th
   {
   	mpiServer_param_st *params;
   	MPI_Comm sd;
   	int id;
      int type_op;
      int rank_client_id;
      void (*function)(struct st_th) ;
   };







     extern pthread_mutex_t m_pool;
     extern pthread_cond_t c_pool_no_full;
     extern pthread_cond_t C_poll_no_empty;
     extern pthread_mutex_t m_pool_end;

     extern pthread_t thid[MAX_THREADS];

     
     extern struct st_th operations_buffer[MAX_OPERATIONS]; // buffer
     extern int n_operation;
     extern int position;
     extern int pos;





   /*
    * API
    */

   int mpiServer_init_worker      ( void ) ;
   int mpiServer_init_worker_pool ( void );

   int mpiServer_launch_worker      ( mpiServer_param_st * params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_function)(struct st_th) );
   int mpiServer_launch_worker_pool ( void (*worker_function)(struct st_th) );

   void mpiServer_worker_run      ( void *arg ) ;
   void mpiServer_worker_pool_run ( MPI_Comm sd, mpiServer_param_st *params, int op_type, int rank_client_id);

   void mpiServer_wait_workers  ( void ) ;

#endif

