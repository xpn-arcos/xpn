
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

#ifndef _TCP_SERVER_COMM_H_
#define _TCP_SERVER_COMM_H_

      #include "all_system.h"
      #include "tcp_server_params.h"
      #include "base/utils.h"
      #include "base/ns.h"
      #include "tcp_server_ops.h"

      /*
       *  API
       */

      int     tcpClient_comm_init       ( tcpClient_param_st *params ) ;
      int     tcpClient_comm_destroy    ( tcpClient_param_st *params ) ;
      int     tcpClient_comm_connect    ( tcpClient_param_st *params ) ;
      int     tcpClient_comm_disconnect ( tcpClient_param_st *params ) ;
      int     tcpClient_comm_locality   ( tcpClient_param_st *params ) ;

      ssize_t tcpClient_write_operation ( int fd, char *data, ssize_t size, char *msg_id ) ;
      ssize_t tcpClient_write_data      ( int fd, char *data, ssize_t size, char *msg_id ) ;
      ssize_t tcpClient_read_data       ( int fd, char *data, ssize_t size, char *msg_id ) ;

#endif

