
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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


#ifndef _NFI_MPI_SERVER_COMM_H_
#define _NFI_MPI_SERVER_COMM_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "base/utils.h"
  #include "base/ns.h"
  #include "base/socket.h"
  #include "xpn_server/xpn_server_ops.h"
  #include "mpi.h"

  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  int     nfi_mpi_server_comm_init            ( int xpn_thread );
  int     nfi_mpi_server_comm_destroy         ( );

  int     nfi_mpi_server_comm_connect         ( char *srv_name, char *port_name, MPI_Comm *out_comm);
  int     nfi_mpi_server_comm_disconnect      ( MPI_Comm *fd );

  ssize_t nfi_mpi_server_comm_write_operation ( MPI_Comm fd, int op );
  ssize_t nfi_mpi_server_comm_write_data      ( MPI_Comm fd, char *data, ssize_t size );
  ssize_t nfi_mpi_server_comm_read_data       ( MPI_Comm fd, char *data, ssize_t size );

  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif

