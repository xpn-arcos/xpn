
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


#ifndef _WORKERS_H_
#define _WORKERS_H_

  #include "all_system.h"
  #include "workers_common.h"
  #include "workers_ondemand.h"
  #include "workers_pool.h"
  #include "base/debug_msg.h"
 

  /*
   * Constants
   */

  #define TH_NOT  0
  #define TH_POOL 1
  #define TH_OP   2

  /*
   * Datatype
   */

  typedef struct {

    worker_ondemand_t w1 ;
    worker_pool_t     w2 ;
    int               thread_mode ;

  } worker_t ;


  /*
   *  API
   */

  int  workers_init       ( worker_t *w, int thread_mode ) ;
  void workers_destroy    ( worker_t *w ) ;

  int  workers_launch     ( worker_t *w, struct st_th th_arg, void (*worker_function)(struct st_th) ) ;
  int  workers_launch_nfi ( worker_t *w, void (*worker_function)(struct st_th), void *args ) ;
  int  workers_wait_nfi   ( worker_t *w, struct st_th th_arg ) ;


#endif

