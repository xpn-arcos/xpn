#ifndef _TCP_SERVER_D2XPN_H_
#define _TCP_SERVER_D2XPN_H_


#include <sys/param.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include "tcp_server.h"
#include "xpn.h"

#define PRELOAD_SYNC  0
#define PRELOAD_ASYNC 1

int tcp_server_d2xpn(char *origen, char *destino, int opt);

#endif
