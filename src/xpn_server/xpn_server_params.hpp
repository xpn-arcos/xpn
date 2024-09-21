
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

  #include <string>
  #include <memory>
  #include <stdlib.h>
  #include <stdio.h>
  #include <ctype.h>
  #include "base_cpp/workers.hpp"

  /* ... Const / Const ................................................. */

  #define XPN_SERVER_TYPE_MPI 0
  #define XPN_SERVER_TYPE_SCK 1

namespace XPN
{
  constexpr const int KB = 1024;
  constexpr const int MB = (KB*KB);
  constexpr const int GB = (KB*MB);
  constexpr const int MAX_BUFFER_SIZE = (1*MB);

  #ifdef MPI_MAX_PORT_NAME
    constexpr const int MAX_PORT_NAME = MPI_MAX_PORT_NAME;
  #else
    constexpr const int MAX_PORT_NAME = 1024;
  #endif

  /* ... Data structures / Estructuras de datos ........................ */

  class xpn_server_params
  {
  public:
    // server identification
    int  size;
    int  rank;

    char port_name[MAX_PORT_NAME] = {0};
    char srv_name [MAX_PORT_NAME] = {0};

    // server configuration
    std::string shutdown_file;
    workers_mode  thread_mode_connections;
    workers_mode  thread_mode_operations;
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
    bool have_threads() { return (static_cast<int>(thread_mode_connections) + static_cast<int>(thread_mode_operations)) > 0; }
    int get_argc() { return argc; }
    char** get_argv() { return argv; }
  };
  
  /* ................................................................... */

}