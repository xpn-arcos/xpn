#ifndef _MYSERVER_WORKERS_H_
#define _MYSERVER_WORKERS_H_

#include "mpiServer.h"
#include "mpiServer_ops.h"

#define MAX_THREADS 1024 
#define STACK_SIZE (256*KB)

struct st_th{
//	pthread_t th;
	int value;
	int id;
};


//int mpiServer_init_worker(pthread_t *th);
int mpiServer_init_worker();
//int mpiServer_launch_worker(int sd, pthread_t *th);
int mpiServer_launch_worker(int sd);
void mpiServer_worker_run(void *arg);

#endif
