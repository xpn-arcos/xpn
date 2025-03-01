
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


#ifndef _XPN_SERVER_PARAMS_H_
#define _XPN_SERVER_PARAMS_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

     #include "all_system.h"
     #include "base/utils.h"
     #include "base/service_socket.h"
     #include "base/workers.h"
     #include "xpn_server_conf.h"


  /* ... Data structures / Estructuras de datos ........................ */

typedef struct
{
    // server identification
    int  size;
    int  rank;

    char port_name[MAX_PORT_NAME_LENGTH];
    char srv_name [MAX_PORT_NAME_LENGTH];

    // server configuration
    char shutdown_file[PATH_MAX];
    int  thread_mode_connections;
    int  thread_mode_operations;
    int  server_type;  // it can be XPN_SERVER_TYPE_MPI, XPN_SERVER_TYPE_SCK

    #ifdef ENABLE_SCK_SERVER
    char port_name_conn[MAX_PORT_NAME_LENGTH];
    int  server_socket;
    char port_name_no_conn[MAX_PORT_NAME_LENGTH];
    int  server_socket_no_conn;
    #endif

    int await_stop;

    // server arguments
    int    argc;
    char **argv;

    // MQTT configuration
    int mosquitto_mode;
    int mosquitto_qos;

    #ifdef HAVE_MOSQUITTO_H
      struct mosquitto * mqtt;
    #endif

} xpn_server_param_st;


  /* ... Functions / Funciones ......................................... */

     void xpn_server_params_show_usage ( void );
     int  xpn_server_params_get        ( xpn_server_param_st *params, int argc, char *argv[] );
     void xpn_server_params_show       ( xpn_server_param_st *params );


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
