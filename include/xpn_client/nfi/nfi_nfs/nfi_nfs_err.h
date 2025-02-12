
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


#ifndef _NFI_NFS_ERR_H_
#define _NFI_NFS_ERR_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif
  

  /* ... Include / Inclusion ........................................... */

  #include "nfs.h"


  /* ... Const / Const ................................................. */

  enum nfi_nfs_err
  {
    NFSERR_PARAM         = 0,
    NFSERR_MEMORY        = 1,
    NFSERR_URL           = 2,
    NFSERR_MNTCONNECTION = 3,
    NFSERR_MOUNT         = 4,
    NFSERR_NFSCONNECTION = 5,   
    NFSERR_GETATTR       = 6,
    NFSERR_LOOKUP        = 7,
    NFSERR_READ          = 8,
    NFSERR_WRITE         = 9,
    NFSERR_CREATE        = 10,
    NFSERR_REMOVE        = 11,
    NFSERR_MKDIR         = 12,
    NFSERR_READDIR       = 13,
    NFSERR_STATFS        = 14,
  };


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  void nfs_err(int err);


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
