#ifndef _MPISERVER_WORKERS_H_
#define _MPISERVER_WORKERS_H_

#include "mpiServer.h"
#include "mpiServer_ops.h"
#include "mpiServer_params.h"

#define MAX_THREADS 1024 
#define STACK_SIZE (256*KB)

/*
 * Datatype
 */

struct st_th
{
	mpiServer_param_st *params;
	int sd;
	int id;
};


/*
 * API
 */

int  mpiServer_init_worker   ( void ) ;
int  mpiServer_launch_worker ( mpiServer_param_st *params, int sd ) ;
void mpiServer_worker_run    ( void *arg ) ;

#endif
