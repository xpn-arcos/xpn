
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
 * @file debug_msg.h
 * @brief Header file for executing debug messages
 *
 * Header file to execute debug messages of
 * type information, warnings or error.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef DEBUG_MSG_H
#define DEBUG_MSG_H

#ifdef __cplusplus
extern "C"
{
#endif

   /************************************************
    *  ... Includes
    ***********************************************/
#include "all_system.h"
#include "string_misc.h"

   /************************************************
    *  ... Constants
    ***********************************************/
#define INFO 3, getpid(), __FILE__, __LINE__, stdout
#define WARNING 2, getpid(), __FILE__, __LINE__, stdout
#define ERROR 1, getpid(), __FILE__, __LINE__, stdout

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
   void debug_msg_init(void);

   /*
   int  debug_info    ( char *msg_fmt, ... ) ;
   int  debug_warning ( char *msg_fmt, ... ) ;
   int  debug_error   ( char *msg_fmt, ... ) ;
   */

   /**
    *  @brief 'TODO'.
    *
    *  'TODO'.
    *
    *  @param src_type 'TODO'.
    *  @param src_pid 'TODO'.
    *  @param src_fname  'TODO'.
    *  @param src_line 'TODO'.
    *  @param fd  'TODO'.
    *  @param msg_fmt  'TODO'.
    *  @par Returns
    *    Nothing.
    */
   int debug_msg_printf(int src_type, long src_pid, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ...);

   /************************************************
    *  ... Macros
    ***********************************************/
#ifdef DEBUG
#define debug_info(...) debug_msg_printf(3, getpid(), __FILE__, __LINE__, stdout, __VA_ARGS__)
#define debug_warning(...) debug_msg_printf(2, getpid(), __FILE__, __LINE__, stderr, __VA_ARGS__)
#define debug_error(...) debug_msg_printf(1, getpid(), __FILE__, __LINE__, stderr, __VA_ARGS__)
#else
#define debug_info(...)
#define debug_warning(...)
#define debug_error(...)
#endif

   /* ................................................................... */

#ifdef __cplusplus
}
#endif

#endif
