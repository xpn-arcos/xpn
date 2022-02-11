#ifndef _TCPSERVER_WORKERS_H_
#define _TCPSERVER_WORKERS_H_

#include "tcpServer.h"
#include "tcpServer_ops.h"

#define MAX_THREADS 32
#define MAX_OPERATIONS 256 
#define STACK_SIZE (256*KB)

struct st_th{
//	pthread_t th;
	int value;
	int id;
};


//int tcpServer_init_worker(pthread_t *th);
int tcpServer_init_worker();
//int tcpServer_launch_worker(int sd, pthread_t *th);
int tcpServer_launch_worker(int sd);
void tcpServer_worker_run(void *arg);

#endif
