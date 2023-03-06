#ifndef _TCP_SERVER_COMM_H_
#define _TCP_SERVER_COMM_H_

#include "tcp_server.h"
#include "tcp_server_utils.h"
#include <mosquitto.h>

extern struct mosquitto *mosqtcpserver;

/* Nuevo */
#define CONST_TEMP              1024
#define MAX_TCP_SERVER_NODES      256

void on_message(struct mosquitto *mosqtcpserver, void *obj, const struct mosquitto_message *msg);

void    tcp_server_comm_reg (char *server, char *newserver, int *port);
ssize_t tcp_server_comm_write_data(int fd, char *data, ssize_t size, char *id);
ssize_t tcp_server_comm_read_data (int fd, char *data, ssize_t size, char *id);
int     tcp_server_comm_init (char *name, int port, char *file);
int     tcp_server_accept_comm ();
void    tcp_server_close_comm ();


#endif
