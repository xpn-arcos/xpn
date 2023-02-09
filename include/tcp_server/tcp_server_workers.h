#ifndef _TCP_SERVER_WORKERS_H_
#define _TCP_SERVER_WORKERS_H_

#include "tcp_server.h"
#include "tcp_server_ops.h"

#define MAX_THREADS 32
#define MAX_OPERATIONS 256 
#define STACK_SIZE (256*KB)

struct st_th{
//	pthread_t th;
	int sd;
	int id;
};


//int tcp_server_init_worker(pthread_t *th);
int tcp_server_init_worker      ( void );
int tcp_server_init_worker_pool ( void );

//int tcp_server_launch_worker(int sd, pthread_t *th);
int tcp_server_launch_worker      ( int sd );
int tcp_server_launch_worker_pool ( void (*worker_pool_function)(void) );


void *       tcp_server_worker_run(void *arg);

void         tcp_server_worker_pool_enqueue ( int sd, int id );
struct st_th tcp_server_worker_pool_dequeue ( int the_end );

void worker_function      ( struct st_th th );
void worker_pool_function ( void );

void tcp_server_destroy_worker_pool ( void );

#endif
