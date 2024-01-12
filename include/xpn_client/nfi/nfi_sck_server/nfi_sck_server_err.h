
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


#ifndef _NFI_SCK_SERVER_ERR_H_
#define _NFI_SCK_SERVER_ERR_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  
  /* ... Const / Const ................................................. */

  enum nfi_sck_server_err
  {
    SCK_SERVERERR_PARAM         = 0,
    SCK_SERVERERR_MEMORY        = 1,
    SCK_SERVERERR_URL           = 2,
    SCK_SERVERERR_MNTCONNECTION = 3,
    SCK_SERVERERR_MOUNT         = 4,
    SCK_SERVERERR_NFSCONNECTION = 5,    
    SCK_SERVERERR_GETATTR       = 6,
    SCK_SERVERERR_LOOKUP        = 7,
    SCK_SERVERERR_READ          = 8,
    SCK_SERVERERR_WRITE         = 9,
    SCK_SERVERERR_CREATE        = 10,
    SCK_SERVERERR_REMOVE        = 11,
    SCK_SERVERERR_MKDIR         = 12,
    SCK_SERVERERR_READDIR       = 13,
    SCK_SERVERERR_STATFS        = 14,
    SCK_SERVERERR_NOTDIR        = 15,
  };


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  void sck_server_err(int err);

  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
