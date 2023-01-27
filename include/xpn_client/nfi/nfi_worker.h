
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

#ifndef _NFI_WORKER_H_
#define _NFI_WORKER_H_


#include "nfi_worker_task.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


    int     nfi_worker_init    ( struct nfi_worker *wrk, struct nfi_server *serv, int thread );
    int     nfi_worker_thread  ( struct nfi_worker *wrk, int flag );
    ssize_t nfi_worker_wait    ( struct nfi_worker *wrk );
    int     nfi_worker_end     ( struct nfi_worker *wrk );
    int     nfi_worker_destroy ( );


    // NEW //////////////////////////////////////////
    int     nfiworker_launch ( void (*worker_function)(struct st_th), struct nfi_worker *wrk ) ;
    ssize_t nfiworker_wait   ( struct nfi_worker *wrk ) ;


 #ifdef  __cplusplus
    }
 #endif


#endif

