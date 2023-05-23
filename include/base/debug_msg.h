
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


 #ifndef DEBUG_MSG_H
 #define DEBUG_MSG_H


 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"
      #include "string_misc.h"


   /* ... Functions / Funciones ......................................... */
      
      //
      // Debug API
      //

      void debug_msg_init   ( void ) ;

      int  debug_msg_printf ( int src_type, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... ) ;


   /* ... Macros / Macros ............................................... */

     // Message
#ifdef DEBUG
     #define debug_error(...)    debug_msg_printf(1, __FILE__, __LINE__, stderr, __VA_ARGS__)
     #define debug_warning(...)  debug_msg_printf(2, __FILE__, __LINE__, stderr, __VA_ARGS__)
     #define debug_info(...)     debug_msg_printf(3, __FILE__, __LINE__, stdout, __VA_ARGS__)
#else
     #define debug_error(...)
     #define debug_warning(...)
     #define debug_info(...)
#endif


     // TRACE
     #define DEBUG_BEGIN() \
             debug_info("Begin %s()\n", __func__)

     #define DEBUG_END() \
             debug_info("End   %s(), errno=%d\n", __func__, errno)

   /* ................................................................... */

 #ifdef  __cplusplus
    }
 #endif

#endif

