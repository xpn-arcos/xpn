#ifndef _MPISERVER_PARAMS_H_
#define _MPISERVER_PARAMS_H_

   #include <stdlib.h>
   #include <stdio.h>
   #include <string.h>
   #include "mpi.h"
   #include "mpiServer_conf.h"


   /*
    * Datatype
    */

   typedef struct
   {
        // client identification
        int  size ;
        int  rank ;

        // server port and comm
        char port_name[MPI_MAX_PORT_NAME] ;
        char  srv_name[MPI_MAX_PORT_NAME] ;
        MPI_Comm server ;

        int *locality ; //NEW

        // server arguments
        int    argc ;
        char **argv ;
   } mpiClient_param_st ;


#endif

