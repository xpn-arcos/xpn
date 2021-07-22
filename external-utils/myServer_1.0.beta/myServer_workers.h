#ifndef _MYSERVER_WORKERS_H_
#define _MYSERVER_WORKERS_H_

#include "myServer.h"
#include "myServer_ops.h"

#define MAX_THREADS 1024 
#define STACK_SIZE (256*KB)

struct st_th{
//	pthread_t th;
	int value;
	int id;
};


//int myServer_init_worker(pthread_t *th);
int myServer_init_worker();
//int myServer_launch_worker(int sd, pthread_t *th);
int myServer_launch_worker(int sd);
void myServer_worker_run(void *arg);

#endif
