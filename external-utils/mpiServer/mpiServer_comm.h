#ifndef _MPISERVER_COMM_H_
#define _MPISERVER_COMM_H_

// Include
#include "mpiServer.h"
#include "mpiServer_params.h"

// CONST
#define CONST_TEMP              1024
#define MAX_MPISERVER_NODES      256

// API
int     mpiServer_comm_init      ( mpiServer_param_st *params ) ;
int     mpiServer_accept_comm    ( mpiServer_param_st *params ) ;
void    mpiServer_close_comm     ( mpiServer_param_st *params ) ;
ssize_t mpiServer_comm_writedata ( mpiServer_param_st *params, int fd, char *data, ssize_t size, char *id ) ;
ssize_t mpiServer_comm_readdata  ( mpiServer_param_st *params, int fd, char *data, ssize_t size, char *id ) ;

#endif
