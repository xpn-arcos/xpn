
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


#ifndef _TCP_SERVER_PARAMS_H_
#define _TCP_SERVER_PARAMS_H_


  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <ctype.h>
  #include "base/utils.h"
  #include "base/workers.h"
  #include "tcp_server_conf.h"
  //#include <semaphore.h>


  /*
   * Constants
   */


  /*
   * Datatype
   */

  typedef struct
  {
    // server identification
    int  size ;
    int  rank ;
    char port_name[TCP_MAX_PORT_NAME] ;
    char srv_name[TCP_MAX_PORT_NAME] ;

    char name[TCP_MAX_PORT_NAME] ;
    char port[TCP_MAX_PORT_NAME] ;
    int  IOsize ;

    char dirbase[PATH_MAX] ;
    char dns_file[PATH_MAX] ;
    char shutdown_file[PATH_MAX] ;

    // server configuration
    int thread_mode;

    //mqtt configuration
    int mosquitto_mode;
    int mosquitto_qos;

#ifdef HAVE_MOSQUITTO_H
    struct mosquitto * mqtt;
#endif

    //Semaphore for clients
    //char sem_name_server [PATH_MAX];

    //Semaphore for server disk
    //sem_t disk_sem;

    // associated client
    int  client ;
    int  global_sock;

    // server arguments
    int    argc ;
    char **argv ;

  } tcp_server_param_st ;


  /*
   * API
   */

  void tcp_server_params_show_usage ( void ) ;
  int  tcp_server_params_get        ( tcp_server_param_st *params, int argc, char *argv[] ) ;
  void tcp_server_params_show       ( tcp_server_param_st *params ) ;

#endif
