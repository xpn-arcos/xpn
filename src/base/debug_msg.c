

   /* ... Include / Inclusion ........................................... */

      #include "base/debug_msg.h"


   /* ... Globla var. / Variables glob. ................................. */

      int    (*DEBUG_MSG_PrintMsg)(const char *, va_list) = NULL ;


   /* ... Functions / Funciones ......................................... */

      void   DEBUG_MSG_setPrinter 
      ( 
        /*IN*/      int (*printer) (const char   *, va_list) 
      )
      {
        DEBUG_MSG_PrintMsg = (  int (*)(const char   *, va_list)) printer ;
      }

      void   DEBUG_MSG_doPrint    
      ( 
        /*IN*/    char    *fto,
        ... 
      )
      {
        if (DEBUG_MSG_PrintMsg != NULL)
           {
             va_list vl ;

             va_start(vl,fto) ;
             (*DEBUG_MSG_PrintMsg)(fto,vl) ; 
       	     va_end(vl) ;
           }
      }

      void   DEBUG_MSG_VPrintF    
      ( 
        /*IN*/      int    line,
        /*IN*/    char    *name,
        /*IN*/     long    pid,
        /*IN*/      int    type,
        /*IN*/    char    *fto,
        /*IN*/    va_list  vl 
      )
      {
        if (DEBUG_MSG_PrintMsg != NULL)
           {
             char     *msg ;

             msg = STRING_MISC_Dvsprintf(fto,vl) ;
             DEBUG_MSG_doPrint("trace(%i,\"%s\",%li,%i,\"%s\").",
                                  line,
                                  name,
                                  pid,
                                  type,
                                  msg) ;
             free(msg) ;
           }
      }

      void   DEBUG_MSG_PrintF     
      ( 
        /*IN*/      int    line,
        /*IN*/    char    *name,
        /*IN*/     long    pid,
        /*IN*/      int    type,
        /*IN*/    char    *fto, 
        ... 
      )
      {
        if (DEBUG_MSG_PrintMsg != NULL)
           {
             va_list vl ;

             va_start(vl,fto) ;
             DEBUG_MSG_VPrintF(line,name,pid,type,fto,vl) ;
       	     va_end(vl) ;
           }
      }


   /* ................................................................... */

