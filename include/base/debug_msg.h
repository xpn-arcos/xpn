

 #ifndef DEBUG_MSG_H
 #define DEBUG_MSG_H

   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"
      #include "string_misc.h"
      #include "debug_tags.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Functions / Funciones ......................................... */
      
      /**
       *
       *  Establece el gestor de impresión de mensajes.
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
       *  Escribe un mensaje, usando formato y lista
       *  de argumentos variables. 
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
        /*IN*/      int  line,
        /*IN*/      char  *name,
        /*IN*/     long    pid,
        /*IN*/      int    type,
        /*IN*/      char  *fto,
        /*IN*/    va_list  vl 
      ) ;

      /**
       *
       *  Escribe una tira CON FORMATO usando PrintMsg. 
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
        /*IN*/      int  line,
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

