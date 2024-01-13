
/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


#ifndef _XPN_ERR_H_
#define _XPN_ERR_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "xpn.h"
  #include "nfi/nfi_lib.h"


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */

  enum xpn_err_code{
    XPN_OK              = 0,
    XPNERR_PARAM        = 1,
    XPNERR_MEMORY       = 2,
    XPNERR_INVALURL     = 3,
    XPNERR_CONF         = 4,
    XPNERR_INITSERV     = 5, 
    XPNERR_GETATTR      = 6,
    XPNERR_LOOKUP       = 7,
    XPNERR_READ         = 8,
    XPNERR_WRITE        = 9,
    XPNERR_CREATE       = 10,
    XPNERR_REMOVE       = 11,
    XPNERR_MKDIR        = 12,
    XPNERR_READDIR      = 13,
    XPNERR_STATFS       = 14,
    XPNERR_PATH_NOEXIST = 15,
    XPNERR_PART_NOEXIST = 16,
    XPNERR_NOMEMORY     = 17,
  };

  extern int errno;
  extern int xpn_errno;


  /* ... Functions / Funciones ......................................... */
  
  int xpn_err(int err);


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
