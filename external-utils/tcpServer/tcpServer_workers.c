
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

#include "tcpServer_workers.h"


   /* ... Global variables / Variables globales ......................... */

int busy_worker;
pthread_mutex_t m_worker;
pthread_cond_t c_worker;
pthread_t th_worker;

struct st_th st_worker;
int th_cont = 0;


   /* ... Functions / Funciones ......................................... */

int tcpServer_init_worker()
{
	//int i;

	//c_worker = PTHREAD_COND_INITIALIZER;
	//m_worker = PTHREAD_MUTEX_INITIALIZER;

	busy_worker = TRUE;
	pthread_cond_init(&c_worker, NULL);
	pthread_mutex_init(&m_worker, NULL);
	return 0;
}

int tcpServer_launch_worker(int sd)
{
	pthread_attr_t   tattr;
	//pthread_t *th;
	//int i;
	int ret;

	pthread_attr_init(&tattr);
	pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);
 	pthread_attr_setstacksize (&tattr , STACK_SIZE);

	debug_info("[WORKERS] pthread_create(%d)\n",sd);

	st_worker.value = sd;
	st_worker.id = th_cont++;

	busy_worker = TRUE;

        debug_info("[WORKERS] pthread_create: antes create_thread tcpServer_worker_run\n");
	//usleep(100);
	ret = pthread_create(&th_worker, &tattr, (void *)(tcpServer_worker_run), (void *)&st_worker);

        debug_info("[WORKERS] pthread_create: desp. create_thread tcpServer_worker_run = %d\n", ret);
	if (ret != 0) {
		perror("pthread_create: Error en create_thread: ");
	}

        debug_info("[WORKERS] pthread_create: antes lock tcpServer_worker_run\n");
	pthread_mutex_lock(&m_worker);
        debug_info("[WORKERS] pthread_create: desp. lock tcpServer_worker_run\n");
	while (busy_worker == TRUE)
	{
                debug_info("[WORKERS] pthread_create: antes wait tcpServer_worker_run\n");
		pthread_cond_wait(&c_worker, &m_worker);
                debug_info("[WORKERS] pthread_create: desp. wait tcpServer_worker_run\n");
	}

        debug_info("[WORKERS] pthread_create: busy_worker= TRUE tcpServer_worker_run\n");
	busy_worker = TRUE;

        debug_info("[WORKERS] pthread_create: antes unlock tcpServer_worker_run\n");
	pthread_mutex_unlock(&m_worker);
        debug_info("[WORKERS] pthread_create: desp. unlock tcpServer_worker_run\n");

	// siguiente hijo
/*
	tcpServer_worker_run((void*)sd);
*/
	return 0;
}

/* thread process */
void tcpServer_worker_run(void *arg)
{
  int op, sd;
  struct st_tcpServer_msg head;
  struct st_th *th;

  int cont_aux;
  char host[255];
  char id[1024];


  th = (struct st_th *)arg;

  sd = th->value;
  cont_aux = th->id;
  gethostname(host, 255);
  sprintf(id,"[%s:%d:%d]", host, cont_aux, sd);

  debug_info("[WORKERS] begin tcpServer_worker_run(%s)\n", id);

  debug_info("[WORKERS] client: tcpServer_worker_run(%s) antes lock\n", id);
  pthread_mutex_lock(&m_worker);
  debug_info("[WORKERS] client: tcpServer_worker_run(%s) desp. lock\n", id);

  //cont_aux = cont;
  //gethostname(host, 255);
  //cont++;

  debug_info("[WORKERS] client: tcpServer_worker_run(%s) busy_worker = FALSE\n", id);

  busy_worker = FALSE;
  debug_info("[WORKERS] client: tcpServer_worker_run(%s) signal\n", id);
  //pthread_cond_signal(&c_worker);
  pthread_cond_broadcast(&c_worker);
  debug_info("[WORKERS] client: tcpServer_worker_run(%s) antes unlock\n", id);
  pthread_mutex_unlock(&m_worker);
  debug_info("[WORKERS] client: tcpServer_worker_run(%s) desp. unlock\n", id);

  do
  {
	  /* */
  	head.type = TCPSERVER_END;
	strcpy(head.id, id);
	debug_info("[WORKERS] tcpServer_read_operation begin (%s)\n", id);
	op = tcpServer_read_operation(sd, &head);
	strcpy(head.id, id);
	debug_info("[WORKERS] OP = %d; ID =%s\n", op, id);
 
	switch(op){
		case TCPSERVER_OPEN_FILE:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_open begin (sd, &head);\n", id);
			tcpServer_op_open(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_open end (sd, &head);\n", id);
			break;
		case TCPSERVER_CREAT_FILE:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_creat begin (sd, &head);\n", id);
			tcpServer_op_creat(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_creat end(sd, &head);\n", id);
			break;
		case TCPSERVER_READ_FILE:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_read begin (sd, &head);\n", id);
			tcpServer_op_read(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_read end (sd, &head);\n", id);
			break;
		case TCPSERVER_WRITE_FILE:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_write begin (sd, &head);\n", id);
			tcpServer_op_write(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_write end (sd, &head);\n", id);
			break;
		case TCPSERVER_CLOSE_FILE:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_close begin (sd, &head);\n", id);
			tcpServer_op_close(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_close end (sd, &head);\n", id);
			break;
		case TCPSERVER_RM_FILE:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_rm begin (sd, &head);\n", id);
			tcpServer_op_rm(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_rm end (sd, &head);\n", id);
			break;
		case TCPSERVER_GETATTR_FILE:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_getattr begin (sd, &head);\n", id);
			tcpServer_op_getattr(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_getattr end (sd, &head);\n", id);
			break;
		case TCPSERVER_SETATTR_FILE:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_setattr begin (sd, &head);\n", id);
			tcpServer_op_setattr(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_setattr end (sd, &head);\n", id);
			break;
		case TCPSERVER_MKDIR_DIR:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_mkdir begin (sd, &head);\n", id);
			tcpServer_op_mkdir(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_mkdir end (sd, &head);\n", id);
			break;
		case TCPSERVER_RMDIR_DIR:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_rmdir begin (sd, &head);\n", id);
			tcpServer_op_rmdir(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_rmdir end (sd, &head);\n", id);
			break;
		case TCPSERVER_PRELOAD_FILE:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_preload begin (sd, &head);\n", id);
			tcpServer_op_preload(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_preload end (sd, &head);\n", id);
			break;
		case TCPSERVER_FLUSH_FILE:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_flush begin (sd, &head);\n", id);
			tcpServer_op_flush(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_flush end (sd, &head);\n", id);
			break;
		case TCPSERVER_GETID:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_getid begin (sd, &head);\n", id);
			tcpServer_op_getid(sd, &head);
			debug_info("[WORKERS] (ID=%s) tcpServer_op_getid end (sd, &head);\n", id);
			break;
                case TCPSERVER_FINALIZE:
                        debug_info("[WORKERS] (ID=%s) tcpServer_op_finalize begin (sd, &head);\n", id);
                        op = TCPSERVER_FINALIZE;
                        printf("[WORKERS] EXIT\n");
                        debug_info("[WORKERS] (ID=%s) tcpServer_op_finalize end (sd, &head);\n", id);
                        exit(0);
                        break;

		default:
			debug_info("[WORKERS] (ID=%s) tcpServer_op_end begin (sd, &head);\n", id);
			op = TCPSERVER_END;
			//printf("[WORKERS] EXIT\n");
			debug_info("[WORKERS] (ID=%s) tcpServer_op_end end(sd, &head);\n", id);
			break;
	}

  } while (op != TCPSERVER_END);

  debug_info("[WORKERS] tcpServer_worker_run (ID=%s): close\n", id);
  close(sd);

  debug_info("[WORKERS] end tcpServer_worker_run (ID=%s): end\n", id);
  pthread_exit(0);
}

