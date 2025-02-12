
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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


#ifndef _WORKERS_POOL_H_
#define _WORKERS_POOL_H_

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "workers_common.h"

  
  /* ... Const / Const ................................................. */

  // Thread count multiplier
  #define POOL_OVERSUSCRIPTION 2
  // End pool
  #define TH_FINALIZE 200


  /* ... Data structures / Estructuras de datos ........................ */

  typedef struct
  {
    pthread_mutex_t m_pool;
    pthread_cond_t  c_pool_no_full;
    pthread_cond_t  c_poll_no_empty;
    pthread_mutex_t m_pool_end;

    int POOL_MAX_THREADS;
    pthread_t *thid;

    struct st_th operations_buffer[MAX_OPERATIONS];
    int n_operation;
    int deq_pos;
    int enq_pos;
    int pool_end; 
  } worker_pool_t;


  /* ... Functions / Funciones ......................................... */

  int          worker_pool_init    ( worker_pool_t *w );
  void         worker_pool_destroy ( worker_pool_t *w );

  void         worker_pool_enqueue ( worker_pool_t *w, struct st_th *th_arg, void (*worker_function)(struct st_th));
  struct st_th worker_pool_dequeue ( worker_pool_t *w );

  int          worker_pool_wait    ( struct st_th *th_arg );


  /* ................................................................... */

#endif
