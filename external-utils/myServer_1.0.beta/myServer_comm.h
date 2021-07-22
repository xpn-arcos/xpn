#ifndef _MYSERVER_COMM_H_
#define _MYSERVER_COMM_H_

#include "myServer.h"


/* Nuevo */
#define CONST_TEMP              1024
#define MAX_MYSERVER_NODES      256



void myServer_comm_reg(char *server, char *newserver, int *port);
int myServer_comm_writedata(int fd, char *data, int size, char *id);
int myServer_comm_readdata(int fd, char *data, int size, char *id);
int myServer_comm_init(char *name, int port, char *file);	
int myServer_accept_comm();
void myServer_close_comm();


#endif
