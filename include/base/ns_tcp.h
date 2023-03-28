
  /*
   *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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


#ifndef _NS_TCP_H_
#define _NS_TCP_H_


      #include "all_system.h"
      #include "base/utils.h"


      /* 
       * ENVIROMENT VARIABLE: DNS SERVICE
       */

      #ifndef TCP_SERVER_FILE
      #define TCP_SERVER_FILE "TCP_SERVER_FILE"
      #endif

      #ifndef TCP_SERVER_FILE_DEFAULT
      #define TCP_SERVER_FILE_DEFAULT "/tmp/tcp_server.dns"
      #endif

      #ifndef MAX_TCP_SERVER_NODES
      #define MAX_TCP_SERVER_NODES 256
      #endif

      #ifndef CONST_TEMP
      #define CONST_TEMP 1024
      #endif


      /*
       *  API
       */

      char *      ns_tcp_get_hostname     ( void ) ;
      int         ns_tcp_publish          ( char * dns_file, char * param_srv_name, char * host_name, char * port_name ) ;
      int         ns_tcp_unpublish        ( char * dns_file, char * serv_name ) ;
      int         ns_tcp_lookup           ( char * param_srv_name, char * host_name, char * port_name ) ;


#endif

