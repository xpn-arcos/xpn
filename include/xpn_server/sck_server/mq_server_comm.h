
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Elias Del Pozo Puñal, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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


#ifndef _MQ_SERVER_COMM_H_
#define _MQ_SERVER_COMM_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

     #include "all_system.h"
     #include <sys/time.h>

     #include "base/utils.h"
     #include "socket.h"
     #include "mq_server_utils.h"
     #include "xpn_server/xpn_server_params.h"


  /* ... Functions / Funciones ......................................... */
  
     #ifdef HAVE_MOSQUITTO_H
     void on_message( __attribute__((__unused__)) struct mosquitto * mqtt, void * obj, const struct mosquitto_message * msg) ;
     #endif

     int mq_server_mqtt_init    ( xpn_server_param_st * params ) ;
     int mq_server_mqtt_destroy ( xpn_server_param_st * params ) ;


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif

