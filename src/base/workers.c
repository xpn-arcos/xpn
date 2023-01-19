
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

  #include "workers.h"


  /* ... Functions / Funciones ......................................... */


  int workers_init ( worker_t *w, int thread_mode )
  {
    if (NULL == w) {
      debug_error("[WORKER] worker_init with NULL worker_t\n");
      return -1 ;
    }

    w->thread_mode = thread_mode ;

    switch (w->thread_mode)
    {
      case TH_OP:
        debug_info("[WORKER] worker_ondemand_init\n") ;
        worker_ondemand_init(&(w->w1)) ;
        break ;

      case TH_POOL:
        debug_info("[WORKER] worker_pool_init\n") ;
        worker_pool_init(&(w->w2));
        break ;

      default:
        debug_info("[WORKER]: ERROR on thread_mode(%d).\n", w->thread_mode) ;
        return -1 ;
        break ;
    }

    return 1;
  }

  int workers_launch ( worker_t *w, struct st_th th_arg, void (*worker_function)(struct st_th) )
  {
    switch (w->thread_mode)
    {
     case TH_OP:
      debug_info("[WORKER] worker_ondemand_launch (ID=%d)\n", th_arg.rank_client_id) ;
      worker_ondemand_launch(&(w->w1), th_arg, worker_function ) ;
      break ;

     case TH_POOL:
      debug_info("[WORKER] worker_pool_enqueue (ID=%d)\n", th_arg.rank_client_id);
      worker_pool_enqueue(   &(w->w2), th_arg, worker_function ); // Enqueue the operation on the buffer
      break ;

     default:
      debug_info("[WORKER]: ERROR on thread_mode(%d).\n", w->thread_mode) ;
      return -1 ;
      break ;
    }

    return 1;
  }

  void workers_destroy ( worker_t *w )
  {
    switch (w->thread_mode)
    {
      case TH_OP:
        debug_info("[WORKER] workers_ondemand_destroy\n");
        workers_ondemand_destroy(&(w->w1)) ;
        break ;

      case TH_POOL:
        debug_info("[WORKER] worker_pool_destroy\n");
        worker_pool_destroy(&(w->w2)); // Destroy worker pool
        break ;

      default:
        debug_info("[WORKER]: ERROR on thread_mode(%d).\n", w->thread_mode) ;
        break ;
    }
  }


  /* ................................................................... */
