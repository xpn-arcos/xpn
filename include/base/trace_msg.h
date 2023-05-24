
  /*
   *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

  /**
   * @file trace_msg.h
   * @brief Header file to 'TODO'.
   *
   * Header file to 'TODO'.
   *
   * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
   * @date  Jul 22, 2021
   * @bug No known bugs.
   */


 #ifndef TRACE_MSG_H
 #define TRACE_MSG_H


 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"
      #include "string_misc.h"


   /* ... Consts / Constantes ........................................... */

      #define  TRACE_PRINTER_SIZE 8

      #define  SPOT __LINE__,__FILE__,getpid(),0


   /* ... Functions / Funciones ......................................... */
      
      void  trace_msg_init ( void ) ;

      /**
       *
       *  Establece el gestor de impresión de mensajes.
       *  Set 'printer' dispacher. 
       *  @param index   index of the printer, from zero up to TRACE_PRINTER_SIZE-1
       *  @param printer the printer function to be used.
       *  @return nothing.
       *
       */
      int   TRACE_MSG_setPrinter 
      ( 
        /*IN*/      int index,
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
      void   TRACE_MSG_VPrintF    
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
      void   TRACE_MSG_PrintF     
      ( 
        /*IN*/      int    line,
        /*IN*/      char  *name,
        /*IN*/     long    pid,
        /*IN*/      int    type,
        /*IN*/      char  *fto, 
        ... 
      ) ;


   /* ... Macros / Macros ............................................... */

     #define DEBUG_BEGIN() \
             debug_info("Begin %s()\n", __func__)

     #define DEBUG_END() \
             debug_info("End   %s(), errno=%d\n", __func__, errno)


   /* ................................................................... */

 #ifdef  __cplusplus
    }
 #endif


 #endif

