
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


#ifndef _TIME_MISC_H
#define _TIME_MISC_H

  #ifdef  __cplusplus
    extern "C" {
  #endif

  
  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"


  /* ... Const / Const ................................................. */

  #define USECPSEC    1000000


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  void TIME_MISC_Timer
  (
    struct timeval * t
  );

  void TIME_MISC_DiffTime
  (
    struct timeval * to,
    struct timeval * tn,
    struct timeval * dif
  );

  void TIME_MISC_AddTime
  (
    struct timeval * to,
    struct timeval * tn,
    struct timeval * sum
  );

  float TIME_MISC_TimevaltoFloat
  (
    struct timeval* timet
  );

  float TIME_MISC_TimevaltoMicro
  (
    struct timeval* timet
  );


  /* ................................................................... */


  #ifdef  __cplusplus
    }
  #endif

#endif /* _TIME_MISC_H */
