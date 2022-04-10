#ifndef _MPISERVER_UTILS_H_
#define _MPISERVER_UTILS_H_

  #include "all_system.h"
  #include "mpiServer_params.h"

 
  /* 
   * API
   */

  // debug print
#ifdef DEBUG
  #define debug_error(...)    mpiServer_utils_printf(1, __FILE__, __LINE__, stderr, __VA_ARGS__)
  #define debug_warning(...)  mpiServer_utils_printf(2, __FILE__, __LINE__, stderr, __VA_ARGS__)
  #define debug_info(...)     mpiServer_utils_printf(3, __FILE__, __LINE__, stdout, __VA_ARGS__)
#else
  #define debug_error(...)
  #define debug_warning(...)
  #define debug_info(...)
#endif


  // print message
  void mpiServer_utils_init     ( void ) ;
  int  mpiServer_utils_printf   ( int src_type, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... ) ;

  // get time
  long mpiServer_utils_get_time ( void ) ;

#endif

