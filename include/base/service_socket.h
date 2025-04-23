
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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


#ifndef _SERVICE_SOCKET_H_
#define _SERVICE_SOCKET_H_

  /* ... Include / Inclusion ........................................... */

     #include "all_system.h"
     #include "base/utils.h"
     #include "base/socket.h"

  
  /* ... Const / Const ................................................. */

     // NS base on sockets
     #define DEFAULT_XPN_SCK_PORT    3456

     #define SOCKET_ACCEPT_CODE_MPI            100
     #define SOCKET_ACCEPT_CODE_SCK_CONN       151
     #define SOCKET_ACCEPT_CODE_SCK_NO_CONN    152
     #define SOCKET_FINISH_CODE                750
     #define SOCKET_FINISH_CODE_AWAIT          751

     #ifdef MPI_MAX_PORT_NAME
        #define MAX_PORT_NAME_LENGTH MPI_MAX_PORT_NAME
     #else
        #define MAX_PORT_NAME_LENGTH 256
     #endif


  /* ... Functions / Funciones ......................................... */

     int sersoc_do_send_recv ( char * srv_name, int port, int req_id, char *res_val ) ;
     int sersoc_do_send ( char * srv_name, int port, int req_id ) ;

     int sersoc_lookup_port_name ( char * srv_name, char * port_name, int socket_accept_code ) ;


  /* ................................................................... */

#endif

