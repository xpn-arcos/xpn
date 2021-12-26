
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

      int mpiServer_launch_worker ( mpiServer_param_st *params, 
		                    int sd, 
                                    void (*worker_function)(struct st_th) )
      {
	  int ret;
	  pthread_attr_t tattr;

          DEBUG_BEGIN() ;

	  pthread_attr_init(&tattr);
	  pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
 	  pthread_attr_setstacksize  (&tattr, STACK_SIZE);

	  debug_info("[WORKERS] mpiServer_launch_worker(%d)\n",sd) ;
	
	  st_worker.sd = sd;	
	  st_worker.id = th_cont++;	
	  st_worker.params = params ;
	  st_worker.function = worker_function ;

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
	
          DEBUG_END() ;
	  return 0;
      }

      void mpiServer_worker_run ( void *arg )
      {
          struct st_th th;
      
          DEBUG_BEGIN() ;

	  memcpy(&th, arg, sizeof(struct st_th)) ;
      
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) antes lock\n", th.id);
          pthread_mutex_lock(&m_worker);
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) desp. lock\n", th.id);
      
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) busy_worker = FALSE\n", th.id);
          busy_worker = FALSE;
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) signal\n", th.id);
       // pthread_cond_signal(&c_worker);
          pthread_cond_broadcast(&c_worker);

          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) antes unlock\n", th.id);
          pthread_mutex_unlock(&m_worker);
          debug_info("[WORKERS] client(%d): mpiServer_worker_run(...) desp. unlock\n", th.id);
      
	  // do function code...
	  th.function(th) ;

	  // end
          DEBUG_END() ;
          pthread_exit(0);
      }


   /* ................................................................... */

