
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Elias Del Pozo Puñal, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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


#ifndef _NFI_MQ_SERVER_COMM_H_
#define _NFI_MQ_SERVER_COMM_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "utils.h"
  #include "nfi_xpn_server.h"
  #include "xpn_server_params.h"


  /* ... Functions / Funciones ......................................... */

  void    nfi_mq_server_init      ( struct nfi_xpn_server *server_aux );
  void    nfi_mq_server_destroy   ( struct nfi_xpn_server *server_aux );
  ssize_t nfi_mq_server_publish   ( struct nfi_xpn_server *server_aux, struct nfi_xpn_server_fhandle *fh_aux, void * buffer, off_t offset, size_t size );

      
  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif

