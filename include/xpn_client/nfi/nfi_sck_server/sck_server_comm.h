
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


#ifndef _SCK_SERVER_COMM_H_
#define _SCK_SERVER_COMM_H_

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "sck_server_params.h"
  #include "base/utils.h"
  #include "base/ns.h"
  #include "sck_server_ops.h"


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  int     sck_client_comm_init       ( sck_client_param_st *params );
  int     sck_client_comm_destroy    ( sck_client_param_st *params );

  int     sck_client_comm_connect    ( sck_client_param_st *params );
  int     sck_client_comm_disconnect ( sck_client_param_st *params );

  int     sck_client_comm_locality   ( sck_client_param_st *params );

  ssize_t sck_client_write_operation ( int fd, char *data, ssize_t size, char *msg_id );
  ssize_t sck_client_write_data      ( int fd, char *data, ssize_t size, char *msg_id );
  ssize_t sck_client_read_data       ( int fd, char *data, ssize_t size, char *msg_id );


  /* ................................................................... */

#endif
