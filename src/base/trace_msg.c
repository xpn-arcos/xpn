

   /* ... Include / Inclusion ........................................... */

      #include "base/trace_msg.h"


   /* ... Globla var. / Variables glob. ................................. */

      int    (*TRACE_MSG_PrintMsg)(const char *, va_list) = NULL ;


   /* ... Functions / Funciones ......................................... */

      void   TRACE_MSG_setPrinter 
      ( 
        /*IN*/      int (*printer) (const char   *, va_list) 
      )
      {
        TRACE_MSG_PrintMsg = (  int (*)(const char   *, va_list)) printer ;
      }

      void   TRACE_MSG_doPrint    
      ( 
        /*IN*/    char    *fto,
        ... 
      )
      {
        if (TRACE_MSG_PrintMsg != NULL)
           {
             va_list vl ;

             va_start(vl,fto) ;
             (*TRACE_MSG_PrintMsg)(fto,vl) ; 
       	     va_end(vl) ;
           }
      }

      void   TRACE_MSG_VPrintF    
      ( 
        /*IN*/      int    line,
        /*IN*/    char    *name,
        /*IN*/     long    pid,
        /*IN*/      int    type,
        /*IN*/    char    *fto,
        /*IN*/    va_list  vl 
      )
      {
        if (TRACE_MSG_PrintMsg != NULL)
           {
             char     *msg ;

             msg = STRING_MISC_Dvsprintf(fto,vl) ;
             TRACE_MSG_doPrint("trace(%i,\"%s\",%li,%i,\"%s\").",
                                  line,
                                  name,
                                  pid,
                                  type,
                                  msg) ;
             free(msg) ;
           }
      }

      void   TRACE_MSG_PrintF     
      ( 
        /*IN*/      int    line,
        /*IN*/    char    *name,
        /*IN*/     long    pid,
        /*IN*/      int    type,
        /*IN*/    char    *fto, 
        ... 
      )
      {
        if (TRACE_MSG_PrintMsg != NULL)
           {
             va_list vl ;

             va_start(vl,fto) ;
             TRACE_MSG_VPrintF(line,name,pid,type,fto,vl) ;
       	     va_end(vl) ;
           }
      }


   /* ................................................................... */

