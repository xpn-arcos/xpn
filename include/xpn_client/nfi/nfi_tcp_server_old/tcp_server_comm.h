#ifndef _TCP_SERVER_COMM_H_
#define _TCP_SERVER_COMM_H_

    #include "all_system.h"
    #include "base/utils.h"
    #include "base/string_misc.h"
    #include "base/debug_msg.h"

    #include <dlfcn.h>


/* ENVIROMENT VARIABLE: DNS SERVICE */
#define TCP_SERVER_FILE "TCP_SERVER_FILE"
#define TCP_SERVER_FILE_DEFAULT "/etc/xpn/tcp_server.dns"


/* Nuevo */
#define CONST_TEMP              1024
#define MAX_TCP_SERVER_NODES      256


void    tcp_server_readFile();
void    tcp_server_translate(char *server, char *newserver, int *port);
ssize_t tcp_server_write_data(int fd, char *data, ssize_t size, char *id);
ssize_t tcp_server_read_data(int fd, char *data, ssize_t size, char *id);
int     tcp_server_connect(char *server);	

#endif
