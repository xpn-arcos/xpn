
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

  #include "mpiServer_workers_pool.h"


  /* ... Global variables / Variables globales ......................... */

  pthread_mutex_t m_pool;
  pthread_cond_t c_pool_no_full;
  pthread_cond_t c_poll_no_empty;
  pthread_mutex_t m_pool_end;

  pthread_t thid[MAX_THREADS];

  struct st_th operations_buffer[MAX_OPERATIONS]; // buffer
  int n_operation = 0;
  int deq_pos = 0;
  int enq_pos = 0;
  int pool_end = 0;


  /* ... Functions / Funciones ......................................... */

  /*
   *  Internal
   */

  void worker_pool_function ( void )
  {
    struct st_th th;

    while(1)
    {
        // Dequeue operation
        th = mpiServer_worker_pool_dequeue ( );
        th.function( th );
    }

    pthread_exit(0);
  }


  /*
   *  API
   */

  int mpiServer_worker_pool_init ( void )
  {
    DEBUG_BEGIN() ;

    // initialize variables...
    pthread_mutex_init(&m_pool,NULL);
    pthread_cond_init (&c_pool_no_full,NULL);
    pthread_cond_init (&c_poll_no_empty,NULL);
    pthread_mutex_init(&m_pool_end,NULL);

    // starting threads...
    for (int i = 0; i < MAX_THREADS; i++)
    {
      debug_info("[WORKERS] pthread_create: create_thread mpiServer_worker_pool_init\n") ;
      if (pthread_create(&thid[i], NULL, (void *)(worker_pool_function), NULL) !=0)
      {
        perror("Error creating thread pool\n");
        return -1;
      }
    }

    DEBUG_END() ;

    return 0;
  }


  void mpiServer_worker_pool_enqueue ( MPI_Comm sd, mpiServer_param_st *params, int op_type, int rank_client_id, void (*worker_function)(struct st_th))
  {
    DEBUG_BEGIN() ;

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_enqueue(...) lock\n", rank_client_id);
    pthread_mutex_lock(&m_pool);
    while (n_operation == MAX_OPERATIONS){
      debug_info("[WORKERS] client(%d): mpiServer_worker_pool_enqueue(...) wait c_pool_no_full\n", rank_client_id);
      pthread_cond_wait(&c_pool_no_full, &m_pool);
    }

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_enqueue(...) copy arguments\n", rank_client_id);
    struct st_th st_worker;

    st_worker.sd                = sd;
    st_worker.params            = params ;
    st_worker.type_op           = op_type ;
    st_worker.rank_client_id    = rank_client_id ;
    st_worker.function          = worker_function ;

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_enqueue(...) enqueue\n", rank_client_id);
    operations_buffer[enq_pos] = st_worker;
    enq_pos = (enq_pos+1) % MAX_OPERATIONS;
    n_operation++;

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_enqueue(...) signal c_poll_no_empty\n", rank_client_id);
    pthread_cond_signal(&c_poll_no_empty);
    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_enqueue(...) unlock\n", rank_client_id);
    pthread_mutex_unlock(&m_pool);

    DEBUG_END() ;
  }

  struct st_th mpiServer_worker_pool_dequeue ( void )
  {
    DEBUG_BEGIN() ;

    struct st_th th;

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) lock\n", th.id);
    pthread_mutex_lock(&m_pool);

    while (n_operation == 0) {
      debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) wait c_poll_no_empty\n", th.id);
      pthread_cond_wait(&c_poll_no_empty, &m_pool);
    }

    debug_info("[WORKERS] thread id = %ld\n", pthread_self());

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) dequeue\n", th.id);
    th = operations_buffer[deq_pos];
    deq_pos = (deq_pos + 1) % MAX_OPERATIONS;
    n_operation--;

    if ( pool_end == 1 || th.type_op == MPISERVER_FINALIZE ) {
      debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) unlock end\n", th.id);
      pthread_mutex_unlock(&m_pool);
      debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) exit\n", th.id);
      pthread_exit(0);
    }

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) signal c_pool_no_full\n", th.id);
    pthread_cond_signal(&c_pool_no_full);

    debug_info("[WORKERS] client(%d): mpiServer_worker_pool_dequeue(...) unlock\n", th.id);
    pthread_mutex_unlock(&m_pool);

    DEBUG_END() ;

    return th;
  }

  void mpiServer_worker_pool_destroy ( void )
  {
    DEBUG_BEGIN() ;

    debug_info("[WORKERS] client: mpiServer_worker_pool_destroy(...) lock\n");
    pthread_mutex_lock(&m_pool_end);
    pool_end=1;
    debug_info("[WORKERS] : mpiServer_worker_pool_destroy(...) unlock\n");
    pthread_mutex_unlock(&m_pool_end);

    debug_info("[WORKERS] : mpiServer_worker_pool_destroy(...) lock\n");
    pthread_mutex_lock(&m_pool);
    debug_info("[WORKERS] : mpiServer_worker_pool_destroy(...) broadcast\n");
    pthread_cond_broadcast(&c_poll_no_empty);
    debug_info("[WORKERS] : mpiServer_worker_pool_destroy(...) unlock\n");
    pthread_mutex_unlock(&m_pool);

    for (int i=0;i<MAX_THREADS;i++){
      debug_info("[WORKERS] : mpiServer_worker_pool_destroy(...) join\n");
      pthread_join(thid[i],NULL);
    }

    debug_info("[WORKERS] : mpiServer_worker_pool_destroy(...) destroy\n");
    pthread_mutex_destroy(&m_pool);
    pthread_cond_destroy(&c_pool_no_full);
    pthread_cond_destroy(&c_poll_no_empty);
    pthread_mutex_destroy(&m_pool_end);

    DEBUG_END() ;
  }
