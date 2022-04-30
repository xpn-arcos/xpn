#ifndef _UTILS_H
#define _UTILS_H


   /* ... Include / Inclusion ........................................... */

      #include <stdio.h>
      #include <stdarg.h>
      #include <sys/time.h>
      #include "base/debug_msg.h"

 
 #ifdef  __cplusplus
    extern "C" {
 #endif


  /* ... Defines / Definiciones ........................................... */

     // Check arguments
     #define FREE_AND_NULL(ptr) \
             if ((ptr) != NULL) { free((ptr)); (ptr) = NULL; }

     #define NULL_RET_ERR(ptr, err_val) \
             if (NULL == (ptr)) { return -1; }


   /* ... Functions / Funciones ......................................... */

      //
      // time
      //
      long utils_get_time ( void ) ;


  /* .................................................................... */

 #ifdef  __cplusplus
    }
 #endif

#endif
