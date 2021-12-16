#ifndef _MPISERVER_PARAMS_H_
#define _MPISERVER_PARAMS_H_

struct mpiServer_param_st
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
};

void params_show ( struct mpiServer_param_st *params ) ;
int  params_get  ( int argc, char *argv[], struct mpiServer_param_st *params ) ;

#endif// _MPISERVER_PARAMS_H_
