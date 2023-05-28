
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

#ifdef __cplusplus
extern "C"
{
#endif

  /************************************************
   *  ... Includes
   ***********************************************/
#include "all_system.h"
#include "string_misc.h"

#ifdef HAVE_PROM_H
#include "prom.h"
#include "promhttp.h"
#include "microhttpd.h"
#endif

  /************************************************
   *  ... Constants
   ***********************************************/

#define TRACE_PRINTER_SIZE 8

#define SPOT __LINE__, __FILE__, getpid(), 0

  /************************************************
   *  ... Functions
   ***********************************************/

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @par Parameters
   *    None.
   * @par Returns
   *    Nothing.
   */
  int trace_msg_init(void);

  /**
   *  @brief Set 'printer' dispacher.
   *
   *  @param index   index of the printer, from zero up to TRACE_PRINTER_SIZE-1
   *  @param printer the printer function to be used.
   *  @par Returns
   *    Nothing.
   */
  int TRACE_MSG_setPrinter(
      /*IN*/ int index,
      /*IN*/ int (*printer)(const char *, va_list));

  /**
   *  @brief Write a message.
   *
   *  Writes a message, using a variable format and argument list.
   *
   *  @param line the line of code where message is generated.
   *  @param name the file name at the code where message is generated.
   *  @param pid  the process that send this message.
   *  @param type the type of message.
   *  @param fto  the message format.
   *  @param vl   the argument list.
   *  @par Returns
   *    Nothing.
   */
  void TRACE_MSG_VPrintF(
      /*IN*/ int line,
      /*IN*/ char *name,
      /*IN*/ long pid,
      /*IN*/ int type,
      /*IN*/ char *fto,
      /*IN*/ va_list vl);

  /**
   *  @brief Write a message.
   *
   *  Write a message using the format and arguments given to it (using PrintMsg).
   *
   *  @param line the line of code where message is generated.
   *  @param name the file name at the code where message is generated.
   *  @param pid  the process that send this message.
   *  @param type the type of message.
   *  @param fto  the message format.
   *  @par Returns
   *    Nothing.
   */
  void TRACE_MSG_PrintF(
      /*IN*/ int line,
      /*IN*/ char *name,
      /*IN*/ long pid,
      /*IN*/ int type,
      /*IN*/ char *fto,
      ...);


  int trace_prom_init     ( void ) ;
  int trace_prom_finalize ( void ) ;


  /************************************************
   *  ... Macros
   ***********************************************/
#define DEBUG_BEGIN() \
  debug_info("Begin %s()\n", __func__)

#define DEBUG_END() \
  debug_info("End   %s(), errno=%d\n", __func__, errno)

  /* ................................................................... */

#ifdef __cplusplus
}
#endif

#endif
