
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

#ifndef _XPN_BASE_DIR_H
#define _XPN_BASE_DIR_H


 #ifdef  __cplusplus
    extern "C" {
 #endif


   #include "xpn_base.h"
   #include "xpn_base_err.h"
   #include "xpn/xpn_base/xpn_policy_init.h"
   #include "xpn/xpn_base/xpn_policy_opendir.h"
   #include "xpn/xpn_base/xpn_base_dir.h"
   #include "xpn/xpn_base/xpn_base_unistd.h"


   struct __dirstream{
      int fd;
   };

   int            xpn_base_mkdir(const char *path, mode_t perm) ;
   int            xpn_base_rmdir(const char *path) ;

   DIR *          xpn_base_opendir(const char *path) ;
   struct dirent* xpn_base_readdir(DIR *dirp) ;
   int            xpn_base_closedir(DIR *dirp) ;
   void           xpn_base_rewinddir(DIR *dirp) ;


 #ifdef  __cplusplus
     }
 #endif

#endif

