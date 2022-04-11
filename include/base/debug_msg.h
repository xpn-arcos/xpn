

 #ifndef DEBUG_MSG_H
 #define DEBUG_MSG_H

   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"
      #include "string_misc.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


  /* ... Defines / Definiciones ........................................... */

     // debug messages
#ifdef DEBUG
     #define debug_error(...)    debug_msg_printf(1, __FILE__, __LINE__, stderr, __VA_ARGS__)
     #define debug_warning(...)  debug_msg_printf(2, __FILE__, __LINE__, stderr, __VA_ARGS__)
     #define debug_info(...)     debug_msg_printf(3, __FILE__, __LINE__, stdout, __VA_ARGS__)
#else
     #define debug_error(...)
     #define debug_warning(...)
     #define debug_info(...)
#endif

     // Current function
     #define DEBUG_BEGIN() \
             debug_info("Begin %s()\n", __func__)

     #define DEBUG_END() \
             debug_info("End   %s(), errno=%d\n", __func__, errno)


   /* ... Functions / Funciones ......................................... */
      
      //
      // Debug API
      //

      void debug_msg_init   ( void ) ;
      int  debug_msg_printf ( int src_type, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... ) ;


      //
      // Extra Debug API
      //

      /**
       *
       *  Set 'printer' dispacher. 
       *  @param printer the printer function to be used.
       *  @return nothing.
       *
       */
      void   DEBUG_MSG_setPrinter 
      ( 
        /*IN*/      int (*printer) (const   char *, va_list) 
      ) ;

      /**
       *
       *  Write a message using the format and the argument list given to it.
       *  @param line the line of code where message is generated.
       *  @param name the file name at the code where message is generated.
       *  @param pid  the process that send this message.
       *  @param type the type of message.
       *  @param fto  the message format.
       *  @param vl   the argument list.
       *  @return nothing.
       *
       */
      void   DEBUG_MSG_VPrintF    
      ( 
        /*IN*/      int    line,
        /*IN*/      char  *name,
        /*IN*/     long    pid,
        /*IN*/      int    type,
        /*IN*/      char  *fto,
        /*IN*/    va_list  vl 
      ) ;

      /**
       *
       *  Write a message using the format and arguments given to it.
       *  @param line the line of code where message is generated.
       *  @param name the file name at the code where message is generated.
       *  @param pid  the process that send this message.
       *  @param type the type of message.
       *  @param fto  the message format.
       *  @return nothing.
       *
       */
      void   DEBUG_MSG_PrintF     
      ( 
        /*IN*/      int    line,
        /*IN*/      char  *name,
        /*IN*/     long    pid,
        /*IN*/      int    type,
        /*IN*/      char  *fto, 
        ... 
      ) ;


   /* ................................................................... */

 #ifdef  __cplusplus
    }
 #endif

#endif

