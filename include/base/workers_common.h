
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


#ifndef _WORKERS_COMMON_H_
#define _WORKERS_COMMON_H_

  #include "all_system.h"
<<<<<<< HEAD

=======
  #include "base/debug_msg.h"
>>>>>>> cc72b7699421582a5476d6eebadbb4b9e7a68fd8

  /*
   * Constants
   */

  #define MAX_THREADS 2048
  #define MAX_OPERATIONS 1024
  #define STACK_SIZE (256*KB)


  /*
   * Datatype
   */

<<<<<<< HEAD
=======
  typedef struct wod worker_ondemand_t;

>>>>>>> cc72b7699421582a5476d6eebadbb4b9e7a68fd8
  struct st_th
  {
      void *params ;
      long  sd ;
      void (*function)(struct st_th) ;

      int id ;
      int type_op ;
      int rank_client_id ;
<<<<<<< HEAD
=======

      worker_ondemand_t *w;
>>>>>>> cc72b7699421582a5476d6eebadbb4b9e7a68fd8
  };

#endif

