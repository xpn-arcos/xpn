
#include <stdio.h>
#include <sys/time.h>

 
  /* 
   * API
   */

#ifdef DEBUG
  #define debug_error(...)    utils_printf(1, __FILE__, __LINE__, stderr, __VA_ARGS__)
  #define debug_warning(...)  utils_printf(2, __FILE__, __LINE__, stderr, __VA_ARGS__)
  #define debug_info(...)     utils_printf(3, __FILE__, __LINE__, stdout, __VA_ARGS__)
#else
  #define debug_error(...)
  #define debug_warning(...)
  #define debug_info(...)
#endif

  void utils_init     ( void ) ;
  int  utils_printf   ( int src_type, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... ) ;
  long utils_get_time ( void ) ;
