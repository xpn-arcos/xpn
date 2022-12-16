
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


#ifndef _MPI_SERVER_PARAMS_H_
#define _MPI_SERVER_PARAMS_H_

  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <semaphore.h>
  #include "mpi.h"
  #include "base/utils.h"
  #include "mpi_server_conf.h"


  //Constants
  #define TH_POOL 1
  #define TH_OP   2



  /*
   * Datatype
   */

  typedef struct
  {
    // server identification
    int  size ;
    int  rank ;
    char port_name[MPI_MAX_PORT_NAME] ;
    char srv_name[MPI_MAX_PORT_NAME] ;
    char dns_file[PATH_MAX] ;
    char host_file[PATH_MAX] ;

    // server configuration
    int thread_mode;

    //Semaphore for clients
    char sem_name_server [MAXPATHLEN];

    //Semaphore for server disk
    sem_t disk_sem;

    // associated client
    MPI_Comm client ;
    char dirbase[PATH_MAX] ;

    // server arguments
    int    argc ;
    char **argv ;
  } mpi_server_param_st ;


  /*
   * API
   */

  void mpi_server_params_show_usage ( void ) ;
  int  mpi_server_params_get        ( mpi_server_param_st *params, int argc, char *argv[] ) ;
  void mpi_server_params_show       ( mpi_server_param_st *params ) ;

#endif
