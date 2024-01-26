
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


#ifndef _WORKERS_H_
#define _WORKERS_H_

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "base/debug_msg.h"
  #include "base/trace_msg.h"
  #include "workers_common.h"
  #include "workers_ondemand.h"
  #include "workers_pool.h"
 

  /* ... Const / Const ................................................. */

  // No threads are executed
  #define TH_NOT 0  
  // A pool of N threads is executed where there is an intermediate queue and there are producers and consumers of operations.
  #define TH_POOL 1  
  // A thread is created according to an operation. That is, on demand.
  #define TH_OP 2  


  /* ... Data structures / Estructuras de datos ........................ */

  typedef struct
  {
    worker_ondemand_t w1;
    worker_pool_t     w2;
    int               thread_mode;
  } worker_t;


  /* ... Functions / Funciones ......................................... */

  int  base_workers_init       ( worker_t *w, int thread_mode );
  void base_workers_destroy    ( worker_t *w );

  int  base_workers_launch     ( worker_t *w, struct st_th *th_arg, void (*worker_function)(struct st_th) );
  int  base_workers_wait       ( worker_t *w, struct st_th *th_arg );


  /* ................................................................... */

#endif
