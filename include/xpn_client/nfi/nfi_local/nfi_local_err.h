
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


#ifndef _NFI_LOCAL_ERR_H_
#define _NFI_LOCAL_ERR_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  
  /* ... Const / Const ................................................. */

  enum nfi_local_err
  {
    LOCAL_ERR_PARAM         = 0,
    LOCAL_ERR_MEMORY        = 1,
    LOCAL_ERR_URL           = 2,
    LOCAL_ERR_MNTCONNECTION = 3,
    LOCAL_ERR_MOUNT         = 4,
    LOCAL_ERR_NFSCONNECTION = 5,   
    LOCAL_ERR_GETATTR       = 6,
    LOCAL_ERR_LOOKUP        = 7,
    LOCAL_ERR_READ          = 8,
    LOCAL_ERR_WRITE         = 9,
    LOCAL_ERR_CREATE        = 10,
    LOCAL_ERR_REMOVE        = 11,
    LOCAL_ERR_MKDIR         = 12,
    LOCAL_ERR_READDIR       = 13,
    LOCAL_ERR_STATFS        = 14,
    LOCAL_ERR_NOTDIR        = 15,
  };
  

  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  void local_err(int err);


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
