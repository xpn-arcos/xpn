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

/************************************************
 *  ... Includes
 ***********************************************/
#include "all_system.h"
#include "string_misc.h"
#include "trace_tags.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /************************************************
     *  ... Functions
     ***********************************************/
    /**
     *  @brief Set 'printer' dispacher.
     *
     *  Set 'printer' dispacher.
     *
     *  @param printer The printer function to be used.
     *  @par Returns
     *      Nothing.
     *
     */
    void TRACE_MSG_setPrinter(
        /*IN*/ int (*printer)(const char *, va_list));

    /**
     *
     *  Write a message using the format and the argument
     *  list given to it.
     *
     *  @param line The line of code where message is generated.
     *  @param name The file name at the code where message is generated.
     *  @param pid  The process that send this message.
     *  @param type The type of message.
     *  @param fto  The message format.
     *  @param vl   The argument list.
     *  @par Returns
     *      Nothing.
     *
     */
    void TRACE_MSG_VPrintF(
        /*IN*/ int line,
        /*IN*/ char *name,
        /*IN*/ long pid,
        /*IN*/ int type,
        /*IN*/ char *fto,
        /*IN*/ va_list vl);

    /**
     *
     *  Write a message using the format and arguments given to it.
     * 
     *  @param line The line of code where message is generated.
     *  @param name The file name at the code where message is generated.
     *  @param pid  The process that send this message.
     *  @param type The type of message.
     *  @param fto  The message format.
     *  @par Returns
     *      Nothing.
     *
     */
    void TRACE_MSG_PrintF(
        /*IN*/ int line,
        /*IN*/ char *name,
        /*IN*/ long pid,
        /*IN*/ int type,
        /*IN*/ char *fto,
        ...);

    /* ................................................................... */

#ifdef __cplusplus
}
#endif

#endif
