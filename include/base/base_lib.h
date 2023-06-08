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
  
  /**
   * @file base_lib.h
   * @brief Library collector.
   *
   * Header file that includes the necessary libraries from the base directory. 
   * It works as a collector of the essential/base libraries to be used in xpand. 
   * This allows that when a new library is required, only this file is modified 
   * and not all the .c files that implement each library.
   *
   * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
   * @date  Jul 22, 2021
   * @bug No known bugs.
   */


#ifndef _BASE_H_
#define _BASE_H_


/************************************************
 *  ... Includes
 ***********************************************/
   #include "debug_msg.h"  /// Library for debug messages.
   #include "trace_msg.h"  /// Library to trace messages.

   #include "filesystem.h" /// Library for 'TODO'
   #include "time_misc.h"  /// Library for 'TODO'
   #include "math_misc.h"  /// Library for 'TODO'

   #include "darray.h " /// Library for dynamic array operations
   #include "dtable.h"  /// Library for dynamic table operations

   #include "string_misc.h" /// Library for 'TODO'
   #include "path_misc.h"   /// Library for 'TODO'
   #include "urlstr.h"      /// Library for 'TODO'

#endif

