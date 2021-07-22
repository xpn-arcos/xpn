#ifndef _MYSERVER_COMM_H_
#define _MYSERVER_COMM_H_

#include "all_system.h"

/* ENVIROMENT VARIABLE: DNS SERVICE */
#define MYSERVER_FILE "MYSERVER_FILE"
#define MYSERVER_FILE_DEFAULT "/etc/xpn/myServer.dns"


/* Nuevo */
#define CONST_TEMP              1024
#define MAX_MYSERVER_NODES      256


void myServer_readFile();
void myServer_translate(char *server, char *newserver, int *port);
int myServer_write_data(int fd, char *data, int size, char *id);
int myServer_read_data(int fd, char *data, int size, char *id);
int myServer_connect(char *server);	

#endif
