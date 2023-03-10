
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


#ifndef _TCP_SERVER_PARAMS_H_
#define _TCP_SERVER_PARAMS_H_


  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <semaphore.h>
  #include "base/utils.h"
  #include "tcp_server_conf.h"


  //Constants
  #define TH_POOL 1
  #define TH_OP   2

  #define TCP_MAX_PORT_NAME   1024


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
    int  port ;
    int  IOsize ;
    char dns_file[PATH_MAX] ;
    char host_file[PATH_MAX] ;

    // server configuration
    int thread_mode;

    //Semaphore for clients
    char sem_name_server [PATH_MAX];

    //Semaphore for server disk
    sem_t disk_sem;

    // associated client
    int  client ;
    int  global_sock;
    char dirbase[PATH_MAX] ;

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
