#ifndef _MPISERVER_PARAMS_H_
#define _MPISERVER_PARAMS_H_

struct mpiServer_param_st{
        int port;
        int IOsize;
        char name[255];
        char file[255];
        char dirbase[255];
};

void params_show ( struct mpiServer_param_st *params ) ;
int  params_get  ( int argc, char *argv[], struct mpiServer_param_st *params ) ;

#endif// _MPISERVER_PARAMS_H_
