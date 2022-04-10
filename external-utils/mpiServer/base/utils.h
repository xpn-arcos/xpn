#ifndef _UTILS_H
#define _UTILS_H


   /* ... Include / Inclusion ........................................... */

#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

 
 #ifdef  __cplusplus
    extern "C" {
 #endif


  /* ... Defines / Definiciones ........................................... */

     //
     // Debug
     //
#ifdef DEBUG
  #define debug_error(...)    utils_printf(1, __FILE__, __LINE__, stderr, __VA_ARGS__)
  #define debug_warning(...)  utils_printf(2, __FILE__, __LINE__, stderr, __VA_ARGS__)
  #define debug_info(...)     utils_printf(3, __FILE__, __LINE__, stdout, __VA_ARGS__)
#else
  #define debug_error(...)
  #define debug_warning(...)
  #define debug_info(...)
#endif

  #define DEBUG_BEGIN() \
    debug_info("Begin %s()\n", __func__)

  #define DEBUG_END() \
    debug_info("End   %s(), errno=%d\n", __func__, errno)


     //
     // Memory
     //
     #define FREE_AND_NULL(ptr) \
             if ((ptr) != NULL) { free((ptr)); (ptr) = NULL; }

     #define NULL_RET_ERR(ptr, err_val) \
             if (NULL == (ptr)) { return -1; }


   /* ... Functions / Funciones ......................................... */

      //
      // debug
      //
      void utils_init     ( void ) ;
      int  utils_printf   ( int src_type, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... ) ;

      //
      // time
      //
      long utils_get_time ( void ) ;


  /* .................................................................... */

 #ifdef  __cplusplus
    }
 #endif

#endif
