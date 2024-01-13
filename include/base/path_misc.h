
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


#ifndef _PATH_MISC_H_
#define _PATH_MISC_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "base/string_misc.h"


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  int hash (char *file,int nServ);

  int getFirstDir   ( char *dir, char *path);
  int getSizeFactor ( char *name );

  // get the last name of a path and erase the file name
  int getNameFile(char *file, char *dir);

  // get the first name of the path and erase the part name
  int getNamePart(char *part, char *dir);

  // erase the initial dir used in the url
  int getDirWithURL(char *url, char *dir);


  /* .................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif /* _PATH_MISC_H */
