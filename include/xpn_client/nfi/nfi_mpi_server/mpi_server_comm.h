
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

#ifndef _MPI_SERVER_COMM_H_
#define _MPI_SERVER_COMM_H_

      #include "all_system.h"
      #include "mpi_server_params.h"
      #include "base/utils.h"
      #include "base/ns.h"
      #include "mpi_server_ops.h"

      /*
       *  API
       */

      int     mpiClient_comm_init       ( mpiClient_param_st *params ) ;
      int     mpiClient_comm_destroy    ( mpiClient_param_st *params ) ;
      int     mpiClient_comm_connect    ( mpiClient_param_st *params ) ;
      int     mpiClient_comm_disconnect ( mpiClient_param_st *params ) ;
      int     mpiClient_comm_locality   ( mpiClient_param_st *params ) ;

      ssize_t mpiClient_write_operation ( MPI_Comm fd, char *data, ssize_t size, char *msg_id ) ;
      ssize_t mpiClient_write_data      ( MPI_Comm fd, char *data, ssize_t size, char *msg_id ) ;
      ssize_t mpiClient_read_data       ( MPI_Comm fd, char *data, ssize_t size, char *msg_id ) ;

#endif

