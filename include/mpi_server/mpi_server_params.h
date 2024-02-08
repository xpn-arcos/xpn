
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


#ifndef _MPI_SERVER_PARAMS_H_
#define _MPI_SERVER_PARAMS_H_

  /* ... Include / Inclusion ........................................... */

  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <ctype.h>
  #include "mpi.h"
  #include "base/utils.h"
  #include "base/workers.h"
  #include "mpi_server_conf.h"
  //#include <semaphore.h>


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */

  typedef struct
  {
    // server identification
    int  size;
    int  rank;

    char port_name[MPI_MAX_PORT_NAME];
    char srv_name [MPI_MAX_PORT_NAME];

    // server configuration
    char dirbase [PATH_MAX];
    char dns_file[PATH_MAX];
    char shutdown_file[PATH_MAX];
    int  thread_mode;
    int  IOsize;
    int  number_accepts;

    //Semaphore for clients
    //char sem_name_server [PATH_MAX];

    //Semaphore for server disk
    //sem_t disk_sem;

    // server arguments
    int    argc;
    char **argv;

  } mpi_server_param_st;


  /* ... Functions / Funciones ......................................... */

  void mpi_server_params_show_usage ( void );
  int  mpi_server_params_get        ( mpi_server_param_st *params, int argc, char *argv[] );
  void mpi_server_params_show       ( mpi_server_param_st *params );


  /* ................................................................... */

#endif
