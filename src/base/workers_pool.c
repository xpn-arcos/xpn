
  /*
   *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
   *
   *  This file is part of Expand.
   *
   *  Expand is free software: you can redistribute it and/or modify
   *  it under the terms of the GNU Lesser General Public License as published by
   *  the Free Software Foundation, either version 3 of the License, or
   *  (at your option) any later version.
   *
   *  Expand is distributed in the hope that it will be useful,
   *  but WITHOUT ANY WARRANTY; without even the implied warranty of
   *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   *  GNU Lesser General Public License for more details.
   *
   *  You should have received a copy of the GNU Lesser General Public License
   *  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
   *
   */


  /* ... Include / Inclusion ........................................... */

     #include "workers_pool.h"


  /* ... Functions / Funciones ......................................... */

  /*
   *  Internal
   */

  void *worker_pool_function ( void *arg )
  {
    int is_true = 1;
    struct st_th th;
    worker_pool_t *w = (worker_pool_t *) arg ;

    while (is_true)
    {
        // Dequeue operation
        th = worker_pool_dequeue(w) ;
        th.function(th) ;
    }

    pthread_exit(0);
    return NULL ;
  }


  /*
   *  API
   */

  int worker_pool_init ( worker_pool_t *w )
  {
    DEBUG_BEGIN() ;

    // initialize variables...
    pthread_mutex_init(&(w->m_pool),          NULL) ;
    pthread_cond_init (&(w->c_pool_no_full),  NULL) ;
    pthread_cond_init (&(w->c_poll_no_empty), NULL) ;
    pthread_mutex_init(&(w->m_pool_end),      NULL) ;

    // malloc threads...
    w->POOL_MAX_THREADS = POOL_OVERSUSCRIPTION * sysconf(_SC_NPROCESSORS_ONLN) ;
    w->thid = (pthread_t *)malloc(w->POOL_MAX_THREADS * sizeof(pthread_t)) ;
    if (NULL == w->thid) {
        perror("malloc: ");
      return -1;
    }

    // starting threads...
    for (int i = 0; i < w->POOL_MAX_THREADS; i++)
    {
      debug_info("[WORKERS] pthread_create: create_thread worker_pool_init\n") ;
      if (pthread_create(&(w->thid[i]), NULL, (void *(*)(void *))(worker_pool_function), (void *)w) !=0)
      {
        perror("Error creating thread pool\n");
        return -1;
      }
    }

    DEBUG_END() ;

    return 0;
  }

  void worker_pool_enqueue ( worker_pool_t *w, struct st_th th_arg, void (*worker_function)(struct st_th) )
  {
    struct st_th st_worker;
    static int th_cont = 0;

    DEBUG_BEGIN() ;

    debug_info("[WORKERS] client(%d): worker_pool_enqueue(...) lock\n", rank_client_id);
    pthread_mutex_lock(&(w->m_pool));
    while (w->n_operation == MAX_OPERATIONS){
      debug_info("[WORKERS] client(%d): worker_pool_enqueue(...) wait c_pool_no_full\n", rank_client_id);
      pthread_cond_wait(&(w->c_pool_no_full), &(w->m_pool));
    }

    debug_info("[WORKERS] client(%d): worker_pool_enqueue(...) copy arguments\n", rank_client_id);

    st_worker          = th_arg ;
    st_worker.id       = th_cont++ ;
    st_worker.function = worker_function ;
    st_worker.w        = w ;

    debug_info("[WORKERS] client(%d): worker_pool_enqueue(...) enqueue\n", rank_client_id);
    w->operations_buffer[w->enq_pos] = st_worker;
    w->enq_pos = (w->enq_pos + 1) % MAX_OPERATIONS;
    w->n_operation++;

    debug_info("[WORKERS] client(%d): worker_pool_enqueue(...) signal c_poll_no_empty\n", rank_client_id);
    pthread_cond_signal(&(w->c_poll_no_empty));
    debug_info("[WORKERS] client(%d): worker_pool_enqueue(...) unlock\n", rank_client_id);
    pthread_mutex_unlock(&(w->m_pool));

    DEBUG_END() ;
  }

  struct st_th worker_pool_dequeue ( worker_pool_t *w )
  {
    struct st_th th;

    DEBUG_BEGIN() ;

    debug_info("[WORKERS] client(%d): worker_pool_dequeue(...) lock\n", th.id);
    pthread_mutex_lock(&(w->m_pool));

    while (w->n_operation == 0) {
      debug_info("[WORKERS] client(%d): worker_pool_dequeue(...) wait c_poll_no_empty\n", th.id);
      pthread_cond_wait(&(w->c_poll_no_empty), &(w->m_pool));
    }

    debug_info("[WORKERS] thread id = %ld\n", pthread_self());

    debug_info("[WORKERS] client(%d): worker_pool_dequeue(...) dequeue\n", th.id);
    th = w->operations_buffer[w->deq_pos];
    w->deq_pos = (w->deq_pos + 1) % MAX_OPERATIONS;
    w->n_operation--;

    if ( w->pool_end == 1 || th.type_op == TH_FINALIZE ) {
      debug_info("[WORKERS] client(%d): worker_pool_dequeue(...) unlock end\n", th.id);
      pthread_mutex_unlock(&(w->m_pool));
      debug_info("[WORKERS] client(%d): worker_pool_dequeue(...) exit\n", th.id);
      pthread_exit(0);
    }

    debug_info("[WORKERS] client(%d): worker_pool_dequeue(...) signal c_pool_no_full\n", th.id);
    pthread_cond_signal(&(w->c_pool_no_full));

    debug_info("[WORKERS] client(%d): worker_pool_dequeue(...) unlock\n", th.id);
    pthread_mutex_unlock(&(w->m_pool));

    DEBUG_END() ;

    return th;
  }

  void worker_pool_destroy ( worker_pool_t *w )
  {
    struct st_th th_arg ;

    DEBUG_BEGIN() ;

    // update pool_end...
    debug_info("[WORKERS] client: worker_pool_destroy(...) lock\n");
    pthread_mutex_lock(&(w->m_pool_end));
    w->pool_end = 1;
    debug_info("[WORKERS] : worker_pool_destroy(...) unlock\n");
    pthread_mutex_unlock(&(w->m_pool_end));

    // prepare arguments...
    memset(&th_arg, 0, sizeof(struct st_th)) ;
    th_arg.type_op = TH_FINALIZE ;

    for (int i = 0; i < w->POOL_MAX_THREADS; ++i) {
      worker_pool_enqueue(w, th_arg, NULL) ;
    }

    debug_info("[WORKERS] : worker_pool_destroy(...) lock\n");
    pthread_mutex_lock(&(w->m_pool));
    debug_info("[WORKERS] : worker_pool_destroy(...) broadcast\n");
    pthread_cond_broadcast(&(w->c_poll_no_empty));
    debug_info("[WORKERS] : worker_pool_destroy(...) unlock\n");
    pthread_mutex_unlock(&(w->m_pool));

    for (int i=0; i<w->POOL_MAX_THREADS; i++)
    {
      debug_info("[WORKERS] : worker_pool_destroy(...) join\n");
      pthread_join(w->thid[i],NULL);
    }

    // free threads...
    free(w->thid) ;
    w->thid = NULL ;

    debug_info("[WORKERS] : worker_pool_destroy(...) destroy\n");
    pthread_mutex_destroy(&(w->m_pool));
    pthread_cond_destroy (&(w->c_pool_no_full));
    pthread_cond_destroy (&(w->c_poll_no_empty));
    pthread_mutex_destroy(&(w->m_pool_end));

    DEBUG_END() ;
  }


  /* ................................................................... */
