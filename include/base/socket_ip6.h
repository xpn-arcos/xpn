
/*
 *  Copyright 2000-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Elias Del Pozo Puñal
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


#ifndef _SOCKET_IP6_H_
#define _SOCKET_IP6_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

     #include "all_system.h"
     #include "debug_msg.h"
     #include "utils.h"

     #include <ifaddrs.h>


  /* ... Functions / Funciones ......................................... */

     int socket_ip6_server_create  ( int *out_socket, int port ) ;
     int socket_ip6_server_accept  ( int socket, int *out_conection_socket ) ;
     int socket_ip6_client_connect              ( char *srv_name, int   port,      int *out_socket ) ;
     int socket_ip6_client_connect_with_retries ( char *srv_name, char *port_name, int *out_socket, int n_retries ) ;

     int socket_ip6_gethostname    ( char * srv_name ) ;
     int socket_ip6_gethostbyname  ( char * ip, size_t ip_size, char * srv_name ) ;
     int socket_ip6_getsockname    ( char * port_name, int new_socket ) ;


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif

