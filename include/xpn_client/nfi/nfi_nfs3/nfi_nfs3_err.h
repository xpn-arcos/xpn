
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


#ifndef _NFI_NFS3_ERR_H_
#define _NFI_NFS3_ERR_H_

  /* ... Include / Inclusion ........................................... */

  #include "nfs3.h"

  
  /* ... Const / Const ................................................. */

  enum nfi_nfs3_err
  {
    NFS3ERR_PARAM         = 0,
    NFS3ERR_MEMORY        = 1,
    NFS3ERR_URL           = 2,
    NFS3ERR_MNTCONNECTION = 3,
    NFS3ERR_MOUNT         = 4,
    NFS3ERR_NFSCONNECTION = 5,    
    NFS3ERR_GETATTR       = 6,
    NFS3ERR_SETATTR       = 7,
    NFS3ERR_LOOKUP        = 8,
    NFS3ERR_READ          = 9,
    NFS3ERR_WRITE         = 10,
    NFS3ERR_CREATE        = 11,
    NFS3ERR_REMOVE        = 12,
    NFS3ERR_MKDIR         = 13,
    NFS3ERR_READDIR       = 14,
    NFS3ERR_STATFS        = 15,
  };


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  
  /* ................................................................... */

#endif
