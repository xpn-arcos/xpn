
/*
 *  Copyright 2000-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Dario Muñoz Muñoz
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

#ifndef _XPN_H
#define _XPN_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "nfi/nfi.h"
  #include "nfi/nfi_worker.h"
  #include "nfi/nfi_ops.h"
  #include "nfi/nfi_lib.h"


  /* ... Const / Const ................................................. */

  #define XPN_CONF "XPN_CONF"


  /* ... Data structures / Estructuras de datos ........................ */

  // Paralel struct partition
  struct xpn_partition
  {
    int id;       // id of partition 
    int replication_level;     // replication_level of files :0, 1, 2,... 
    char name[PATH_MAX];  // name of partition 
    ssize_t block_size;   // size of distribution used 

    int data_nserv;     // number of server 
    struct nfi_server *data_serv; // list of data servers in the partition 

    int local_serv; // server with locality
  };

  /* ... Functions / Funciones ......................................... */


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
