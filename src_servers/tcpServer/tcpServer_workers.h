#ifndef _TCPSERVER_WORKERS_H_
#define _TCPSERVER_WORKERS_H_

#include "tcpServer.h"
#include "tcpServer_ops.h"

#define MAX_THREADS 32
#define MAX_OPERATIONS 256 
#define STACK_SIZE (256*KB)

struct st_th{
//	pthread_t th;
	int sd;
	int id;
};


//int tcpServer_init_worker(pthread_t *th);
int tcpServer_init_worker      ( void );
int tcpServer_init_worker_pool ( void );

//int tcpServer_launch_worker(int sd, pthread_t *th);
int tcpServer_launch_worker      ( int sd );
int tcpServer_launch_worker_pool ( void (*worker_pool_function)(void) );


void *       tcpServer_worker_run(void *arg);

void         tcpServer_worker_pool_enqueue ( int sd, int id );
struct st_th tcpServer_worker_pool_dequeue ( int the_end );

void worker_function      ( struct st_th th );
void worker_pool_function ( void );

void tcpServer_destroy_worker_pool ( void );

#endif
