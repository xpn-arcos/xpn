
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


#ifndef _SCK_SERVER_D2XPN_H_
#define _SCK_SERVER_D2XPN_H_

  /* ... Include / Inclusion ........................................... */

  #include <sys/param.h>
  #include <stdio.h>
  #include <sys/file.h>
  #include <sys/fcntl.h>
  #include <unistd.h>
  #include <sys/time.h>
  #include <sys/wait.h>
  #include <sys/errno.h>

  #include "all_system.h"
  #include "base/utils.h"
  #include "sck_server_params.h"
  #include "sck_server_ops.h"

  #include "xpn.h"


  /* ... Const / Const ................................................. */

  #define PRELOAD_SYNC  0
  #define PRELOAD_ASYNC 1


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  int sck_server_d2xpn ( sck_server_param_st *params, char *origen, char *destino );


  /* ................................................................... */

#endif
