#ifndef _MPI_SERVER_PARAMS_H_
#define _MPI_SERVER_PARAMS_H_

  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <semaphore.h>
  #include "mpi.h"
  #include "base/utils.h"
  #include "mpi_server_conf.h"


  //Constants
  #define TH_POOL 1
  #define TH_OP   2



  /*
   * Datatype
   */

  typedef struct
  {
    // server identification
    int  size ;
    int  rank ;
    char port_name[MPI_MAX_PORT_NAME] ;
    char srv_name[MPI_MAX_PORT_NAME] ;
    char dns_file[PATH_MAX] ;
    char host_file[PATH_MAX] ;

    // server configuration
    int thread_mode;

    //Semaphore for clients
    char sem_name_server [MAXPATHLEN];

    //Semaphore for server disk
    sem_t disk_sem;

    // associated client
    MPI_Comm client ;
    char dirbase[PATH_MAX] ;

    // server arguments
    int    argc ;
    char **argv ;
  } mpi_server_param_st ;


  /*
   * API
   */

  void mpi_server_params_show_usage ( void ) ;
  int  mpi_server_params_get        ( mpi_server_param_st *params, int argc, char *argv[] ) ;
  void mpi_server_params_show       ( mpi_server_param_st *params ) ;

#endif
