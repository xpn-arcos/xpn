
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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


#ifndef _XPN_SERVER_COMM_H_
#define _XPN_SERVER_COMM_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

     #include "all_system.h"
     #include "base/utils.h"
     #include "base/time_misc.h"
     #include "base/ns.h"
     #include "socket.h"
     #include "xpn_server_params.h"
     #include "sck_server_comm.h"
     #include "mq_server_comm.h"
     #include "mq_server_ops.h"
#ifdef ENABLE_MPI_SERVER
     #include "mpi_server_comm.h"
#endif
#ifdef ENABLE_MPI_SERVER
     #include "sck_server_comm.h"
#endif   


  /* ... Const / Const ................................................. */

     // connection type
     #define XPN_SERVER_CONNECTION      0
     #define XPN_SERVER_CONNECTIONLESS  1


  /* ... Functions / Funciones ......................................... */

     int       xpn_server_comm_init              ( int server_type, xpn_server_param_st *params );
     int       xpn_server_comm_destroy           ( int server_type, xpn_server_param_st *params );

     int       xpn_server_comm_accept            ( int server_type, xpn_server_param_st * params, int connection_type, void ** new_sd );
     int       xpn_server_comm_disconnect        ( int server_type, void  *sd );

     ssize_t   xpn_server_comm_read_operation    ( int server_type, void *sd, int  *op,                 int *rank_client_id, int *tag_client_id );
     ssize_t   xpn_server_comm_write_data        ( int server_type, void *sd, char *data, ssize_t size, int  rank_client_id, int  tag_client_id );
     ssize_t   xpn_server_comm_read_data         ( int server_type, void *sd, char *data, ssize_t size, int  rank_client_id, int  tag_client_id );


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
