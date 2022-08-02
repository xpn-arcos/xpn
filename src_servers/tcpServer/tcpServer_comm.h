#ifndef _TCPSERVER_COMM_H_
#define _TCPSERVER_COMM_H_

#include "tcpServer.h"
#include "tcpServer_utils.h"


/* Nuevo */
#define CONST_TEMP              1024
#define MAX_TCPSERVER_NODES      256



void    tcpServer_comm_reg (char *server, char *newserver, int *port);
ssize_t tcpServer_comm_write_data(int fd, char *data, ssize_t size, char *id);
ssize_t tcpServer_comm_read_data (int fd, char *data, ssize_t size, char *id);
int     tcpServer_comm_init (char *name, int port, char *file);	
int     tcpServer_accept_comm ();
void    tcpServer_close_comm ();


#endif
