
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

#ifndef _MPI_SERVER_WORKERS_POOL_H_
#define _MPI_SERVER_WORKERS_POOL_H_

  #include "all_system.h"
  #include "mpi_server_params.h"
  #include "mpi_server_ops.h"
  #include "mpi_server_comm.h"
  #include "mpi_server_workers_common.h"

  //Thread count multiplier
  #define POOL_OVERSUSCRIPTION 2


  /*
   * API
   */

  int          mpi_server_worker_pool_init    ( void ) ;
  void         mpi_server_worker_pool_destroy ( void ) ;

  void         mpi_server_worker_pool_enqueue ( mpi_server_param_st *params, MPI_Comm sd, int op_type, int rank_client_id, void (*worker_function)(struct st_th)) ;
  struct st_th mpi_server_worker_pool_dequeue ( void ) ;

#endif
