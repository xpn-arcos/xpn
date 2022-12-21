
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


#ifndef _NS_H_
#define _NS_H_


      #include "all_system.h"
      #include "base/utils.h"

      #include <netdb.h>
      #include <sys/socket.h>
      #include <netinet/in.h>
      #include <arpa/inet.h>


      /*
       *  API
       */

      int ns_publish   ( char * param_srv_name, char * dns_file, char * port_name ) ;
      int ns_unpublish ( char * dns_file ) ;
      int ns_lookup    ( char * param_srv_name, char * port_name ) ;


#endif

