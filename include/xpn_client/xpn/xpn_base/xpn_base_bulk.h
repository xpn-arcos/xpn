
  /*
   *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

#ifndef _XPN_BASE_BULK_H
#define _XPN_BASE_BULK_H


 #ifdef  __cplusplus
    extern "C" {
 #endif


   #include "xpn_base.h"
   #include "xpn_policy_open.h"
   #include "xpn_policy_cwd.h"
   #include "xpn_base_init.h"
   #include "xpn_base_filetable.h"
   #include "xpn_base_unistd.h"


   extern struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];

   int xpn_base_preload(const char *virtual_path, const char *storage_path) ;
   int xpn_base_flush(const char *virtual_path, const char *storage_path) ;


 #ifdef  __cplusplus
     }
 #endif


#endif

