
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
  #include "tcp_server_conf.h"
  //#include <semaphore.h>


  /*
   * Datatype
   */

  typedef struct
  {
    // Execution configuration
    int xpn_thread ;
    int xpn_session ;
    int xpn_locality ;


    //MQTT usage
    int xpn_mosquitto_mode;
    int xpn_mosquitto_qos;

    // client identification
    int  size ;
    int  rank ;

    // server port and comm
    // int   port_number;
    char  srv_name   [TCP_MAX_PORT_NAME] ;
    char  server_name[TCP_MAX_PORT_NAME];
    char  port_number[TCP_MAX_PORT_NAME];
    int   server ;

    int     locality ;
    char dirbase[PATH_MAX] ;
    //char    sem_name_server [PATH_MAX];
    //sem_t * sem_server ;

    // server arguments
    int    argc ;
    char **argv ;

  } tcpClient_param_st ;


#endif
