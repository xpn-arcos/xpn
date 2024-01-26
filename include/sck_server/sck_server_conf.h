
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


#ifndef _SCK_SERVER_CONF_H_
#define _SCK_SERVER_CONF_H_

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"


  /* ... Const / Const ................................................. */

  /* MAX_BUFFER_SIZE */
  #ifndef MAX_BUFFER_SIZE
    #define MAX_BUFFER_SIZE (1*MB)
  #endif

  /* ENVIROMENT VARIABLE: DNS SERVICE */
  #ifndef SCK_SERVER_FILE
    #define SCK_SERVER_FILE "SCK_SERVER_FILE"
  #endif

  #ifndef SCK_SERVER_FILE_DEFAULT
    #define SCK_SERVER_FILE_DEFAULT "/etc/xpn/sck_server.dns"
  #endif

  #ifndef SCK_SERVER_NAME_DEFAULT
    #define SCK_SERVER_NAME_DEFAULT "node"
  #endif

  #ifndef SCK_SERVER_DIRBASE_DEFAULT
    #define SCK_SERVER_DIRBASE_DEFAULT  "/"
  #endif

  #ifndef SCK_SERVER_PORT_DEFAULT
    #define SCK_SERVER_PORT_DEFAULT 9999
  #endif

  #ifndef SCK_MAX_PORT_NAME
    #define SCK_MAX_PORT_NAME 1024
  #endif

  #ifndef SCK_SERVER_IOSIZE_DEFAULT
    #define SCK_SERVER_IOSIZE_DEFAULT (MB)
  #endif

  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */


  /* ................................................................... */

#endif
