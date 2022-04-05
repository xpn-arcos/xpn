#ifndef _MPISERVER_UTILS_H_
#define _MPISERVER_UTILS_H_

   #include "all_system.h"
   #include "mpiServer_params.h"

 
  /* 
   * API
   */

#ifdef DEBUG
  #define debug_error(...)    mpiServer_utils_printf(1, __FILE__, __LINE__, stderr, __VA_ARGS__)
  #define debug_warning(...)  mpiServer_utils_printf(2, __FILE__, __LINE__, stderr, __VA_ARGS__)
  #define debug_info(...)     mpiServer_utils_printf(3, __FILE__, __LINE__, stdout, __VA_ARGS__)
#else
  #define debug_error(...)
  #define debug_warning(...)
  #define debug_info(...)
#endif

#ifdef _LARGEFILE64_
    typedef long long int offset_t;
#else 
    typedef off_t offset_t;
#endif

  // Memory
  #define FREE_AND_NULL(ptr) \
    if ((ptr) != NULL) { free((ptr)); (ptr) = NULL; }

  #define NULL_RET_ERR(ptr, err_val) \
    if (NULL == (ptr)) { mpiServer_err(err_val); return -1; }

  void mpiServer_utils_init     ( void ) ;
  int  mpiServer_utils_printf   ( int src_type, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... ) ;
  long mpiServer_utils_get_time ( void ) ;


#endif
