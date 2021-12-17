#ifndef _MPISERVER_UTILS_H_
#define _MPISERVER_UTILS_H_

  #include "mpiServer.h"
  #include "mpiServer_ops.h"

 
  /* 
   * Const
   */

  #define DBG_ERROR 1, __FILE__, __LINE__
  #define DBG_WARN  2, __FILE__, __LINE__
  #define DBG_INFO  3, __FILE__, __LINE__


  /* 
   * API
   */

  // debug_printf(...)
#ifdef DEBUG
  #define debug_printf(level, ...) mpisrv_print(level, stderr, __VA_ARGS__)
#else
  #define debug_printf(level, ...)
#endif

  // mpisrv_print(...)
  int mpisrv_print ( int src_type, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... ) ;

  // mpisrv_get_time()
  long mpisrv_get_time ( void ) ;


#endif
