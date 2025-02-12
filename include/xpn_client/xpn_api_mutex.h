
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


#ifndef _XPN_API_MUTEX_H
#define _XPN_API_MUTEX_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Macros / Macros ............................................... */

  #ifdef _REENTRANT

    extern pthread_mutex_t xpn_api_mutex ;

    #define XPN_API_LOCK()    pthread_mutex_lock(&xpn_api_mutex)
    #define XPN_API_UNLOCK()  pthread_mutex_unlock(&xpn_api_mutex)

  #else

    #define XPN_API_LOCK()    (0)
    #define XPN_API_UNLOCK()  (0)

  #endif

  /* ... Functions / Funciones ......................................... */


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
