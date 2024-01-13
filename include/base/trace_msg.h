
/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


#ifndef TRACE_MSG_H
#define TRACE_MSG_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "string_misc.h"
  #include "trace_tags.h"


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */
      
  /**
   *
   *  Establece el gestor de impresión de mensajes.
   *  Set 'printer' dispacher. 
   *  @param printer the printer function to be used.
   *  @return nothing.
   *
   */
  void   TRACE_MSG_setPrinter 
  ( 
    /*IN*/      int (*printer) (const   char *, va_list) 
  );

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
    /*IN*/      int  line,
    /*IN*/      char  *name,
    /*IN*/     long    pid,
    /*IN*/      int    type,
    /*IN*/      char  *fto,
    /*IN*/    va_list  vl 
  );

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
    /*IN*/      int  line,
    /*IN*/      char  *name,
    /*IN*/     long    pid,
    /*IN*/      int    type,
    /*IN*/      char  *fto, 
    ... 
  );


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
