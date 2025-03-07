
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


#ifndef _NFI_LIB_H_
#define _NFI_LIB_H_

  /* ... Include / Inclusion ........................................... */

     #include "nfi.h"
     #include "nfi_worker.h"
     #include "base/path_misc.h"

     /* protocols */
     #include "nfi_local.h"
     #include "nfi_xpn_server.h"

     // BEGIN OF ENABLE_MODULE BLOCK. Do not remove this line. //
     // BEGIN OF ENABLE_NFS BLOCK. Do not remove this line. //
     #ifdef ENABLE_NFS
       #include "nfi_nfs.h"
     #endif
     // END OF ENABLE_NFS BLOCK. Do not remove this line. //
     // BEGIN OF ENABLE_NFS3 BLOCK. Do not remove this line. //
     #ifdef ENABLE_NFS3
       #include "nfi_nfs3.h"
     #endif
     // END OF ENABLE_NFS3 BLOCK. Do not remove this line. //
     // END OF ENABLE_MODULE BLOCK. Do not remove this line. //


  /* ................................................................... */

#endif

