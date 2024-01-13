
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


#ifndef _DTABLE_H
#define _DTABLE_H

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

  #include "darray.h"


  /* ... Constants / Constantes ........................................ */

  #define c_DTABLE_T    sizeof(dtable_t)
  #define c_DTELTO_T    sizeof(dtelto_t)

  #define c_DTABLE_T_NULL  {NULL,0,-1}


  /* ... Data structures / Estructuras de datos ........................ */

  typedef struct
  {
    void       *dt_info;
    int         next_free;
    int         myself;
  } dtelto_t;

  typedef struct
  {
    t_pointerDArray  dtable;
    long             neltos;
    long             free_list;
  } dtable_t;


  /* ... Functions / Funciones ......................................... */

    int    dtable_insert ( dtable_t *dt, void *dt_info );
    int    dtable_delete ( dtable_t *dt, int   fd );
    void  *dtable_get    ( dtable_t *dt, int   fd );

  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
