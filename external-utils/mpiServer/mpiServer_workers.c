
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 *
 *  This file is part of mpiServer.
 *
 *  mpiServer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  mpiServer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with mpiServer.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


   /* ... Include / Inclusion ........................................... */

#include "mpiServer_workers.h"

int busy_worker;
pthread_mutex_t m_worker;
pthread_cond_t c_worker;
pthread_t th_worker;

struct st_th st_worker;
int th_cont = 0;


   /* ... Functions / Funciones ......................................... */

int mpiServer_init_worker ( void )
{
	busy_worker = TRUE;
	pthread_cond_init(&c_worker, NULL);
	pthread_mutex_init(&m_worker, NULL);
	return 0;
}

int mpiServer_launch_worker ( mpiServer_param_st *params, int sd )
{
	int ret;
	pthread_attr_t tattr;

	pthread_attr_init(&tattr);
	pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
 	pthread_attr_setstacksize  (&tattr, STACK_SIZE);

	debug_info("[WORKERS] mpiServer_launch_worker(%d)\n",sd) ;
	
	st_worker.sd = sd;	
	st_worker.id = th_cont++;	
	st_worker.params = params ;

	busy_worker = TRUE;
	
        debug_info("[WORKERS] pthread_create: antes create_thread mpiServer_worker_run\n") ;
	ret = pthread_create(&th_worker, &tattr, (void *)(mpiServer_worker_run), (void *)&st_worker);
	if (ret != 0){
            debug_error("[WORKERS] pthread_create %d\n", ret);
	    perror("pthread_create: Error en create_thread: ");
	}
	
        debug_info("[WORKERS] pthread_create: antes lock mpiServer_worker_run\n");
	pthread_mutex_lock(&m_worker);
        debug_info("[WORKERS] pthread_create: desp. lock mpiServer_worker_run\n");
	while (busy_worker == TRUE)
	{
                debug_info("[WORKERS] pthread_create: antes wait mpiServer_worker_run\n");
		pthread_cond_wait(&c_worker, &m_worker);
                debug_info("[WORKERS] pthread_create: desp. wait mpiServer_worker_run\n");
	}

        debug_info("[WORKERS] pthread_create: busy_worker= TRUE mpiServer_worker_run\n");
	busy_worker = TRUE;

        debug_info("[WORKERS] pthread_create: antes unlock mpiServer_worker_run\n");
	pthread_mutex_unlock(&m_worker);
        debug_info("[WORKERS] pthread_create: desp. unlock mpiServer_worker_run\n");
	
	return 0;
}

/* thread process */
void mpiServer_worker_run ( void *arg )
{
        struct st_th th;
        int op;
        struct st_mpiServer_msg head;
      
      
        debug_info("[WORKERS] begin mpiServer_worker_run(...)\n");
	memcpy(&th, arg, sizeof(struct st_th)) ;
      
        debug_info("[WORKERS] client: mpiServer_worker_run(...) antes lock\n");
        pthread_mutex_lock(&m_worker);
        debug_info("[WORKERS] client: mpiServer_worker_run(...) desp. lock\n");
      
        debug_info("[WORKERS] client: mpiServer_worker_run(...) busy_worker = FALSE\n");
        busy_worker = FALSE;
        debug_info("[WORKERS] client: mpiServer_worker_run(...) signal\n");
        //pthread_cond_signal(&c_worker);
        pthread_cond_broadcast(&c_worker);

        debug_info("[WORKERS] client: mpiServer_worker_run(...) antes unlock\n");
        pthread_mutex_unlock(&m_worker);
        debug_info("[WORKERS] client: mpiServer_worker_run(...) desp. unlock\n");
      
        do {
		head.type = MPISERVER_END;

		debug_info("[WORKERS] (ID=%d) mpiServer_read_operation begin...\n", th.id);
		op = mpiServer_read_operation(th.params, th.sd, &head);
			
		debug_info("[WORKERS] (ID=%d) mpiServer_op_<%d> begins\n", th.id, op);
		switch(op)
		{
			case MPISERVER_OPEN_FILE:
				mpiServer_op_open(th.params, th.sd, &head);
				break;
			case MPISERVER_CREAT_FILE:
				mpiServer_op_creat(th.params, th.sd, &head);
				break;
			case MPISERVER_READ_FILE:
				mpiServer_op_read(th.params, th.sd, &head);
				break;
			case MPISERVER_WRITE_FILE:
				mpiServer_op_write(th.params, th.sd, &head);
				break;
			case MPISERVER_CLOSE_FILE:
				mpiServer_op_close(th.params, th.sd, &head);
				break;
			case MPISERVER_RM_FILE:
				mpiServer_op_rm(th.params, th.sd, &head);
				break;
			case MPISERVER_GETATTR_FILE:
				mpiServer_op_getattr(th.params, th.sd, &head);
				break;
			case MPISERVER_SETATTR_FILE:
				mpiServer_op_setattr(th.params, th.sd, &head);
				break;
			case MPISERVER_MKDIR_DIR:
				mpiServer_op_mkdir(th.params, th.sd, &head);
				break;
			case MPISERVER_RMDIR_DIR:
				mpiServer_op_rmdir(th.params, th.sd, &head);
				break;
			case MPISERVER_PRELOAD_FILE:
				mpiServer_op_preload(th.params, th.sd, &head);
				break;
			case MPISERVER_FLUSH_FILE:
				mpiServer_op_flush(th.params, th.sd, &head);
				break;
			case MPISERVER_GETID:
				mpiServer_op_getid(th.params, th.sd, &head);
				break;
			case MPISERVER_FINALIZE:
				op = MPISERVER_FINALIZE;
				break;
			default:
				op = MPISERVER_END;
				break;
		}
		debug_info("[WORKERS] (ID=%d) mpiServer_op_<%d> ends\n", th.id, op);
				
        } while(op != MPISERVER_END);
      
        debug_info("[WORKERS] mpiServer_worker_run (ID=%d) close\n", th.id);
        mpiServer_comm_close(th.params) ;

        debug_info("[WORKERS] mpiServer_worker_run (ID=%d) ends\n", th.id);
        pthread_exit(0);
}

   /* ................................................................... */

