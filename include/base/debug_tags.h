
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


#ifndef DEBUG_TAGS_H
#define DEBUG_TAGS_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"


  /* ... Consts / Constantes ........................................... */

  #define      INFO __LINE__,__FILE__,getpid(),100
  #define   WARNING __LINE__,__FILE__,getpid(),101
  #define     ERROR __LINE__,__FILE__,getpid(),102

  #define  DEFCON_1 __LINE__,__FILE__,getpid(),110
  #define  DEFCON_2 __LINE__,__FILE__,getpid(),110
  #define  DEFCON_3 __LINE__,__FILE__,getpid(),111
  #define  DEFCON_4 __LINE__,__FILE__,getpid(),111
  #define  DEFCON_5 __LINE__,__FILE__,getpid(),112
  #define  DEFCON_6 __LINE__,__FILE__,getpid(),112


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
