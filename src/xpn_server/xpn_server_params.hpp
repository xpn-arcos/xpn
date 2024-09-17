
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

#pragma once

  /* ... Include / Inclusion ........................................... */

  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <ctype.h>
  #include "base/utils.h"
  #include "base/workers.h"

  /* ... Const / Const ................................................. */

  #define XPN_SERVER_TYPE_MPI 0
  #define XPN_SERVER_TYPE_SCK 1

  /* MAX_BUFFER_SIZE */
  #ifndef MAX_BUFFER_SIZE
    #define MAX_BUFFER_SIZE (1*MB)
  #endif

  #ifdef MPI_MAX_PORT_NAME
    #define XPN_SERVER_MAX_PORT_NAME MPI_MAX_PORT_NAME
  #else
    #define XPN_SERVER_MAX_PORT_NAME 1024
  #endif

  /* ... Data structures / Estructuras de datos ........................ */

namespace XPN
{
  class xpn_server_params
  {
  public:
    // server identification
    int  size;
    int  rank;

    char port_name[XPN_SERVER_MAX_PORT_NAME] = {0};
    char srv_name [XPN_SERVER_MAX_PORT_NAME] = {0};

    // server configuration
    char shutdown_file[PATH_MAX] = {0};
    int  thread_mode_connections;
    int  thread_mode_operations;
    int  server_type;  // it can be XPN_SERVER_TYPE_MPI, XPN_SERVER_TYPE_SCK

    #ifdef ENABLE_SCK_SERVER
    int server_socket; // For sck_server
    #endif

    int await_stop;

    // server arguments
    int    argc;
    char **argv;

  public:
    xpn_server_params(int argc, char *argv[]);

    void show_usage();
    void show();
    bool have_threads() { return (thread_mode_connections + thread_mode_operations) > 0; }
    int get_argc() { return argc; }
    char** get_argv() { return argv; }
  };
  
  /* ................................................................... */

}