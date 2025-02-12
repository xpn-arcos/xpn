
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


#ifndef _MATH_MISC_H
#define _MATH_MISC_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  int MATH_MISC_hash
  (
    char *file,
    int  nServ
  );

  int MATH_MISC_locateInRAID5withInternalParity 
  (
    int i,    // block index
    int m,    // how many servers
    int *SP,  // Server for Parity
    int *IP,  // Index for Parity
    int *SD,  // Server for Data
    int *ID   // Index for Data
  );

  int MATH_MISC_locateInRAID5withExternalParity 
  (
    int i,    // block index
    int m,    // how many servers
    int *SP,  // Server for Parity
    int *IP,  // Index for Parity
    int *SD,  // Server for Data
    int *ID   // Index for Data
  );

  int MATH_MISC_Xor
  (
    char *block_result,
    char *block_1,
    char *block_2,
    int   block_size
  );

  int MATH_MISC_Xor3
  (
    char *block_result,
    char *block_1,
    char *block_2,
    char *block_3,
    int   block_size
  );

  int MATH_MISC_XorN
  (
    char  *block_result,
    char **blocks,
    int    nblocks,
    int    block_size
  );


  /* .................................................................... */


  #ifdef  __cplusplus
    }
  #endif

#endif /* _MATH_MISC_H */
