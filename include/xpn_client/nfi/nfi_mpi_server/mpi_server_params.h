#ifndef _MPI_SERVER_PARAMS_H_
#define _MPI_SERVER_PARAMS_H_

  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include "mpi.h"
  #include "mpi_server_conf.h"
  //#include <semaphore.h>


  /*
   * Datatype
   */

  typedef struct
  {
    // Execution configuration
    int xpn_thread ;
    int xpn_session ;
    int xpn_locality ;

    // client identification
    int  size ;
    int  rank ;

    // server port and comm
    char port_name[MPI_MAX_PORT_NAME] ;
    char  srv_name[MPI_MAX_PORT_NAME] ;
    MPI_Comm server ;

    int locality ;
    //char sem_name_server [PATH_MAX];
    //sem_t * sem_server ;

    // server arguments
    int    argc ;
    char **argv ;
  } mpiClient_param_st ;


#endif

