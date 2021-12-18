#ifndef _MPISERVER_COMM_H_
#define _MPISERVER_COMM_H_

#include "all_system.h"

#include <dlfcn.h>

/* ENVIROMENT VARIABLE: DNS SERVICE */
#define MPISERVER_FILE "MPISERVER_FILE"
#define MPISERVER_FILE_DEFAULT "/etc/xpn/mpiServer.dns"


/* Nuevo */
#define CONST_TEMP              1024
#define MAX_MPISERVER_NODES      256


void mpiServer_readFile();
void mpiServer_translate(char *server, char *newserver, int *port);
ssize_t mpiServer_write_data(int fd, char *data, ssize_t size, char *id);
ssize_t mpiServer_read_data(int fd, char *data, ssize_t size, char *id);
int mpiServer_connect(char *server);	

#endif
