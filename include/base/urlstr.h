
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

#ifndef _URLSTR_H
#define _URLSTR_H


   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"
      #include "string_misc.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Functions / Funciones ......................................... */

      int    URLSTR_ParseURL 
      (
         /*IN */ char  *urlstr,
          /*OUT*/ char **protocol,
          /*OUT*/ char **user,
          /*OUT*/ char **machine,
          /*OUT*/ int   *port,
          /*OUT*/ char **file,
          /*OUT*/ char **relative,
          /*OUT*/ char **params
      ) ;
      /* - Fill 'url' with information from 'str' string. */
      /* - Rellena 'url' con la informacion de 'str'.  */


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

