#ifndef _MPISERVER_PARAMS_H_
#define _MPISERVER_PARAMS_H_

   #include <stdlib.h>
   #include <stdio.h>
   #include <string.h>
   #include "mpi.h"
   #include "mpiServer_conf.h"
   #include "mpiServer_utils.h"


   /*
    * Datatype
    */

   typedef struct
   {
        // server identification
        int  size ;
        int  rank ;
        char port_name[MPI_MAX_PORT_NAME] ;
        char  srv_name[MPI_MAX_PORT_NAME] ;

        // associated client
        MPI_Comm client ;
        char dirbase[255] ;
        int  IOsize ;

        // server arguments
	int    argc ;
	char **argv ;
   } mpiServer_param_st ;


   /*
    * API
    */

   void mpiServer_params_show_usage ( void ) ;
   int  mpiServer_params_get        ( mpiServer_param_st *params, int argc, char *argv[] ) ;
   void mpiServer_params_show       ( mpiServer_param_st *params ) ;

#endif

