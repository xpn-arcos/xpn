
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


#ifndef _XPN_SERVER_CONF_H_
#define _XPN_SERVER_CONF_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #ifdef ENABLE_MPI_SERVER
  #include "mpi.h"
  #endif


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


  /* ... Functions / Funciones ......................................... */


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
