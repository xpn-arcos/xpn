
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

#ifndef _MPI_SERVER_WORKERS_COMMON_H_
#define _MPI_SERVER_WORKERS_COMMON_H_

  #include "all_system.h"
  #include "mpi_server_params.h"
  #include "mpi_server_comm.h"


  /*
   * Constants
   */

  #define MAX_THREADS 2048
  #define MAX_OPERATIONS 1024
  #define STACK_SIZE (256*KB)


  /*
   * Datatype
   */

  struct st_th
  {
      mpi_server_param_st *params ;
      MPI_Comm sd ;
      void (*function)(struct st_th) ;

      int id ;
      int type_op ;
      int rank_client_id ;
  };

#endif
