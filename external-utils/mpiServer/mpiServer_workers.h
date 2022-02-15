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


  /*
   * API
   */

  int mpiServer_init_worker      ( void );
  int mpiServer_init_worker_pool ( void );

  int mpiServer_launch_worker      ( mpiServer_param_st * params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_function)(struct st_th) );
  int mpiServer_launch_worker_pool ( void (*worker_pool_function)(void) );

  void mpiServer_worker_run ( void *arg ) ;

  void mpiServer_worker_pool_enqueue         ( MPI_Comm sd, mpiServer_param_st *params, int op_type, int rank_client_id);
  struct st_th mpiServer_worker_pool_dequeue ( int the_end );

  void worker_function      ( struct st_th th );
  void worker_pool_function ( void );

  void mpiServer_wait_workers ( void ) ;

  void mpiServer_destroy_worker_pool ( void );














  //TODO
  int mpiServer_launch_worker_proccess ( mpiServer_param_st * params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_client_function)(struct st_th) );
  void mpiServer_worker_client_run ( void *arg ) ;
  void worker_client_function ( struct st_th th ) ;
#endif