
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


#ifndef _WORKERS_ONDEMAND_H_
#define _WORKERS_ONDEMAND_H_

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "workers_common.h"

  
  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */

  typedef struct
  {
    // number of active threads (launch + destroy)
    int             busy_worker;
    pthread_mutex_t m_worker;
    pthread_cond_t  c_worker;
    pthread_cond_t  c_nworkers;
    long            n_workers;
  } worker_ondemand_t;


  /* ... Functions / Funciones ......................................... */

  int   worker_ondemand_init     ( worker_ondemand_t *w );
  void  workers_ondemand_destroy ( worker_ondemand_t *w );

  int   worker_ondemand_launch   ( worker_ondemand_t *w, struct st_th *th_arg, void (*worker_function)(struct st_th) );
  int   worker_ondemand_wait     ( struct st_th *th_arg );

  /* ................................................................... */

#endif
