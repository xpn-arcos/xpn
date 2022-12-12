#ifndef _MPI_SERVER_WORKERS_COMMON_H_
#define _MPI_SERVER_WORKERS_COMMON_H_

  #include "all_system.h"
  #include "mpi_server_params.h"
  #include "mpi_server_comm.h"


  /*
   * Constants
   */

  #define MAX_THREADS 2048
  #define MAX_OPERATIONS 1024
  #define STACK_SIZE (256*KB)


  /*
   * Datatype
   */

  struct st_th
  {
      mpi_server_param_st *params ;
      MPI_Comm sd ;
      void (*function)(struct st_th) ;

      int id ;
      int type_op ;
      int rank_client_id ;
  };

#endif
