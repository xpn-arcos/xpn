
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


   /* ... Include / Inclusion ........................................... */

      #include "xpn.h"


   /* ... Functions / Funciones ......................................... */


      char* xpn_getcwd(char *path, size_t size)
      {
            return xpn_simple_getcwd(path, size) ;
      }

      int xpn_chdir(char *path)
      {
          return xpn_simple_chdir(path) ;
      }


      int xpn_mkdir(const char *path, mode_t perm)
      {
          return xpn_simple_mkdir(path, perm) ;
      }

      int xpn_rmdir(const char *path)
      {
          return xpn_simple_rmdir(path) ;
      }


      DIR *xpn_opendir ( const char *path )
      {
          return xpn_simple_opendir(path) ;
      }

      struct dirent* xpn_readdir ( DIR *dirp )
      {
          return xpn_simple_readdir(dirp) ;
      }

      int  xpn_closedir ( DIR *dirp )
      {
          return xpn_simple_closedir(dirp) ;
      }

      void  xpn_rewinddir ( DIR *dirp )
      {
          return xpn_simple_rewinddir(dirp) ;
      }


   /* ................................................................... */

