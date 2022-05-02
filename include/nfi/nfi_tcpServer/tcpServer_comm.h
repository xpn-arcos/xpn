#ifndef _TCPSERVER_COMM_H_
#define _TCPSERVER_COMM_H_

#include "all_system.h"

#include <dlfcn.h>

/* ENVIROMENT VARIABLE: DNS SERVICE */
#define TCPSERVER_FILE "TCPSERVER_FILE"
#define TCPSERVER_FILE_DEFAULT "/etc/xpn/tcpServer.dns"


/* Nuevo */
#define CONST_TEMP              1024
#define MAX_TCPSERVER_NODES      256


void    tcpServer_readFile();
void    tcpServer_translate(char *server, char *newserver, int *port);
ssize_t tcpServer_write_data(int fd, char *data, ssize_t size, char *id);
ssize_t tcpServer_read_data(int fd, char *data, ssize_t size, char *id);
int     tcpServer_connect(char *server);	

#endif
