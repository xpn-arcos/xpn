
/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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


#ifndef _ENV_H_
#define _ENV_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "debug_msg.h"


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  int xpn_session_dir( void );
  int xpn_session_file( void );

  /* ... Macros / Macros .................................................. */

  #ifdef  __cplusplus
    }
  #endif

#endif
