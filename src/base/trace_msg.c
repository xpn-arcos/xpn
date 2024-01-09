
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 *
 *  This file is part of Expand.
 *
 *  Expand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Expand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


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

