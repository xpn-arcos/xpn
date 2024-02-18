
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

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "mpi_server_params.h"
  #include "base/utils.h"
  #include "base/ns.h"
  #include "mpi_server_ops.h"


  /* ... Const / Const ................................................. */

  #define MPI_SOCKET_PORT 3456
  #define MPI_SOCKET_ACCEPT 123
  #define MPI_SOCKET_FINISH 666

  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  int     mpi_client_comm_init       ( mpi_client_param_st *params );
  int     mpi_client_comm_destroy    ( mpi_client_param_st *params );

  int     mpi_client_comm_connect    ( mpi_client_param_st *params );
  int     mpi_client_comm_disconnect ( mpi_client_param_st *params );
  
  int     mpi_client_comm_locality   ( mpi_client_param_st *params );

  ssize_t mpi_client_write_operation ( MPI_Comm fd, int op );
  ssize_t mpi_client_write_data      ( MPI_Comm fd, char *data, ssize_t size );
  ssize_t mpi_client_read_data       ( MPI_Comm fd, char *data, ssize_t size );


  /* ................................................................... */

#endif

