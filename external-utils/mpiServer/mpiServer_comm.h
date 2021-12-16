#ifndef _MPISERVER_COMM_H_
#define _MPISERVER_COMM_H_

#include "mpiServer.h"


/* Nuevo */
#define CONST_TEMP              1024
#define MAX_MPISERVER_NODES      256



void    mpiServer_comm_reg (char *server, char *newserver, int *port);
ssize_t mpiServer_comm_writedata(int fd, char *data, ssize_t size, char *id);
ssize_t mpiServer_comm_readdata (int fd, char *data, ssize_t size, char *id);
int     mpiServer_comm_init (char *name, int port, char *file);	
int     mpiServer_accept_comm ();
void    mpiServer_close_comm ();


#endif
