
/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Dario Muñoz Muñoz
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


#ifndef _SOCKET_H_
#define _SOCKET_H_

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "debug_msg.h"


  /* ... Const / Const ................................................. */

  #define MPI_SOCKET_PORT 3456
  #define MPI_SOCKET_ACCEPT 123
  #define MPI_SOCKET_FINISH 666

  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  int socket_accept_read ( int socket );   
  int socket_send ( char * srv_name, int code );  
  int socket_create ( int *out_socket );

  /* ... Macros / Macros .................................................. */


#endif
