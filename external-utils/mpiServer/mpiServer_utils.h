#ifndef _MPISERVER_UTILS_H_
#define _MPISERVER_UTILS_H_

  #include "mpiServer.h"
  #include "mpiServer_ops.h"


  // debug_printf(...)
#ifdef DEBUG
  #define debug_printf(...) mpisrv_print(__FILE__, __LINE__, stderr, __VA_ARGS__)
#else
  #define debug_printf(...)
#endif

  // mpisrv_print(...)
  int mpisrv_print ( int src_type, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... ) ;

  // mpisrv_get_time()
  long mpisrv_get_time ( void ) ;


#endif
