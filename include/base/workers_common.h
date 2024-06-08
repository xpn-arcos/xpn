
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


#ifndef _WORKERS_COMMON_H_
#define _WORKERS_COMMON_H_

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "base/debug_msg.h"


  /* ... Const / Const ................................................. */

  #define MAX_THREADS     2048
  #define MAX_OPERATIONS  1024
  #define STACK_SIZE     (256*KB)


  /* ... Data structures / Estructuras de datos ........................ */

  struct st_th
  {
    void  *params;
    void (*function)(struct st_th);

    // server stuff
    int   id;
    int   type_op;
    int   rank_client_id;
    int   tag_client_id;
    long  sd;
    void *comm;

    // w: worker_ondemand/worker_pool as void *
    void *w;
    // v: original st_th as void *
    void *v;

    // client stuff
    pthread_t       th_worker;
    pthread_mutex_t m_wait;
    pthread_cond_t  c_wait;
    int             r_wait;
    int             wait4me; // (wait4me==1) ? launch + wait : launch
  };


  /* ... Functions / Funciones ......................................... */


  /* ................................................................... */

#endif
