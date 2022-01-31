
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


   /* ... Global variables / Variables globales ......................... */

      int             busy_worker;
      pthread_mutex_t m_worker;
      pthread_cond_t  c_worker;

      pthread_cond_t  c_nworkers;
      long            n_workers;
      int             th_cont = 0;


   /* ... Functions / Funciones ......................................... */

      int mpiServer_init_worker ( void )
      {
	  busy_worker = TRUE;
          n_workers   = 0L;
	  pthread_cond_init (&c_worker,   NULL);
	  pthread_cond_init (&c_nworkers, NULL);
	  pthread_mutex_init(&m_worker,   NULL);

	  return 0;
      }

      int mpiServer_launch_worker ( mpiServer_param_st * params, MPI_Comm sd, int type_op, int rank_client_id, void (*worker_function)(struct st_th) )
      {
	  int ret;
	  pthread_attr_t th_attr;
          pthread_t      th_worker;
          struct st_th   st_worker;

          DEBUG_BEGIN() ;

	  pthread_attr_init(&th_attr);
	  pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_DETACHED);
 	  pthread_attr_setstacksize  (&th_attr, STACK_SIZE);
	  busy_worker = TRUE;

	  // prepare arguments...
	  st_worker.sd       = sd;
	  st_worker.id       = th_cont++;
	  st_worker.params   = params ;
    st_worker.type_op  = type_op ;
    st_worker.rank_client_id   = rank_client_id ;
	  st_worker.function = worker_function ;

	  // create thread...
          debug_info("[WORKERS] pthread_create: create_thread mpiServer_worker_run\n") ;
	  ret = pthread_create(&th_worker, &th_attr, (void *)(mpiServer_worker_run), (void *)&st_worker);
	  if (ret != 0){
              debug_error("[WORKERS] pthread_create %d\n", ret);
	      perror("pthread_create: Error en create_thread: ");
	  }

	  // wait to copy args...
          debug_info("[WORKERS] pthread_create: lock mpiServer_worker_run\n");
	  pthread_mutex_lock(&m_worker);
	  while (busy_worker == TRUE)
	  {
                debug_info("[WORKERS] pthread_create: wait mpiServer_worker_run\n");
		pthread_cond_wait(&c_worker, &m_worker);
	  }
          debug_info("[WORKERS] pthread_create: busy_worker= TRUE mpiServer_worker_run\n");
	  busy_worker = TRUE;
          debug_info("[WORKERS] pthread_create: unlock mpiServer_worker_run\n");
	  pthread_mutex_unlock(&m_worker);

          DEBUG_END() ;
	  return 0;
      }

      void mpiServer_worker_run ( void *arg )
      {
          struct st_th th;
      
          DEBUG_BEGIN() ;

          // prolog... 
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) lock\n", th.id);
          pthread_mutex_lock(&m_worker);
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) copy arguments\n", th.id);
	  memcpy(&th, arg, sizeof(struct st_th)) ;
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) busy_worker = FALSE\n", th.id);
          busy_worker = FALSE;
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) n_workers++\n", th.id);
          n_workers++ ;
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) signal c_worker\n", th.id);
          pthread_cond_broadcast(&c_worker); // pthread_cond_signal(&c_worker);
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) unlock\n", th.id);
          pthread_mutex_unlock(&m_worker);
      
	  // do function code...
	  th.function(th) ;

	  // epilog...
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) lock\n", th.id);
          pthread_mutex_lock(&m_worker);
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) n_workers--\n", th.id);
          n_workers-- ;
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) signal c_nworkers\n", th.id);
          pthread_cond_broadcast(&c_nworkers); // pthread_cond_signal(&c_nworkers);
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) unlock\n", th.id);
          pthread_mutex_unlock(&m_worker);

          DEBUG_END() ;

	  // end
          pthread_exit(0);
      }

      void mpiServer_wait_workers ( void )
      {
          DEBUG_BEGIN() ;

	  // wait to n_workers be zero...
          debug_info("[WORKERS] pthread_create: lock mpiServer_wait_workers\n");
	  pthread_mutex_lock(&m_worker);
	  while (n_workers != 0)
	  {
                debug_info("[WORKERS] pthread_create: wait mpiServer_wait_workers\n");
		pthread_cond_wait(&c_nworkers, &m_worker);
	  }
          debug_info("[WORKERS] pthread_create: unlock mpiServer_wait_workers\n");
	  pthread_mutex_unlock(&m_worker);

          DEBUG_END() ;
      }


   /* ................................................................... */

