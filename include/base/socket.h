
/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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


#ifndef _SOCKET_H_
#define _SOCKET_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "debug_msg.h"


  /* ... Const / Const ................................................. */

  #define DEFAULT_XPN_SCK_PORT 3456
  #define SOCKET_ACCEPT_CODE 123
  #define SOCKET_FINISH_CODE 666

  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  int socket_send ( int socket, void * buffer, int size );
  int socket_recv ( int socket, void * buffer, int size );
  int socket_server_create ( int *out_socket );
  int socket_server_accept ( int socket, int *out_conection_socket );
  int socket_client_connect ( char * srv_name, int *out_socket );
  int socket_close ( int socket );

  /* ... Macros / Macros .................................................. */

  #ifdef  __cplusplus
    }
  #endif

#endif
