#ifndef _TCPSERVER_UTILS_H_
#define _TCPSERVER_UTILS_H_

   #include "all_system.h"
   #include "tcpServer_params.h"

 
  /* 
   * API
   */

   // debug print
#ifdef DEBUG
  #define debug_error(...)    tcpServer_utils_printf(1, __FILE__, __LINE__, stderr, __VA_ARGS__)
  #define debug_warning(...)  tcpServer_utils_printf(2, __FILE__, __LINE__, stderr, __VA_ARGS__)
  #define debug_info(...)     tcpServer_utils_printf(3, __FILE__, __LINE__, stdout, __VA_ARGS__)
#else
  #define debug_error(...)
  #define debug_warning(...)
  #define debug_info(...)
#endif

  #define DEBUG_BEGIN() \
	  debug_info("Begin %s()\n", __func__)
  #define DEBUG_END() \
	  debug_info("End   %s(), errno=%d\n", __func__, errno)

   // Memory
  #define FREE_AND_NULL(ptr) \
	  if ((ptr) != NULL) { free((ptr)); (ptr) = NULL; }

  #define NULL_RET_ERR(ptr, err_val) \
	  if (NULL == (ptr)) { tcpServer_err(err_val); return -1; }

  // print message
  void tcpServer_utils_init     ( void ) ;
  int  tcpServer_utils_printf   ( int src_type, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... ) ;

  // get time
  long tcpServer_utils_get_time ( void ) ;


#endif
