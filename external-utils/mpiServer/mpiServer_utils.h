#ifndef _MPISERVER_UTILS_H_
#define _MPISERVER_UTILS_H_

  #include "mpiServer.h"
  #include "mpiServer_ops.h"

 
  /* 
   * API
   */

#ifdef DEBUG
  #define debug_error(...)    mpisrv_print(1, __FILE__, __LINE__, stderr, __VA_ARGS__)
  #define debug_warning(...)  mpisrv_print(2, __FILE__, __LINE__, stderr, __VA_ARGS__)
  #define debug_info(...)     mpisrv_print(3, __FILE__, __LINE__, stdout, __VA_ARGS__)
#else
  #define debug_error(...)
  #define debug_warning(...)
  #define debug_info(...)
#endif

  // mpisrv_print(...)
  int mpisrv_print ( int src_type, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... ) ;

  // mpisrv_get_time()
  long mpisrv_get_time ( void ) ;


#endif
