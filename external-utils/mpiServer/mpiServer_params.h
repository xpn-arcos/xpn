#ifndef _MPISERVER_PARAMS_H_
#define _MPISERVER_PARAMS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include "mpiServer_conf.h"

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

void params_show ( mpiServer_param_st *params ) ;
int  params_get  ( mpiServer_param_st *params, int argc, char *argv[] ) ;

#endif// _MPISERVER_PARAMS_H_
