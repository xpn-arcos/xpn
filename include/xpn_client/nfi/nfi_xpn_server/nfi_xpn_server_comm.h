
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


#ifndef _NFI_XPN_SERVER_COMM_H_
#define _NFI_XPN_SERVER_COMM_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "nfi_xpn_server.h"
  #include "base/utils.h"
  #include "base/ns.h"
  #include "base/socket.h"

  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  int     nfi_xpn_server_comm_init              ( struct nfi_xpn_server *params );
  int     nfi_xpn_server_comm_destroy           ( struct nfi_xpn_server *params );

  int     nfi_xpn_server_comm_connect           ( struct nfi_xpn_server *params );
  int     nfi_xpn_server_comm_disconnect        ( struct nfi_xpn_server *params );

  int     nfi_xpn_server_comm_write_operation   ( struct nfi_xpn_server *params, int op);
  ssize_t nfi_xpn_server_comm_write_data        ( struct nfi_xpn_server *params, char *data, ssize_t size );
  ssize_t nfi_xpn_server_comm_read_data         ( struct nfi_xpn_server *params, char *data, ssize_t size );


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif

