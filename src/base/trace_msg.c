
/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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
 * @file trace_msg.c
 * @brief File to 'TODO'.
 *
 * File to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

/************************************************
 *  ... Includes
 ***********************************************/
#include "base/trace_msg.h"

/************************************************
 *  ... Globla var.
 ***********************************************/
int (*TRACE_MSG_PrintMsg[TRACE_PRINTER_SIZE])(const char *, va_list);

/************************************************
 *  ... Functions
 ***********************************************/

void base_trace_msg_init(void)
{
   int i;

   for (i = 0; i < TRACE_PRINTER_SIZE; i++)
   {
      TRACE_MSG_PrintMsg[i] = NULL;
   }
}

int base_trace_msg_set_printer(int index, int (*printer)(const char *, va_list))
{
   if (index < 0)
   {
      return -1;
   }
   if (index > TRACE_PRINTER_SIZE)
   {
      return -1;
   }

   TRACE_MSG_PrintMsg[index] = (int (*)(const char *, va_list))printer;
   return 1;
}

void trace_msg_do_print(char *fto, ...)
{
   if (NULL == TRACE_MSG_PrintMsg)
   {
      return;
   }

   va_list vl;

   va_start(vl, fto);
   (*TRACE_MSG_PrintMsg)(fto, vl);
   va_end(vl);
}

void base_trace_msg_variable_printf(int line, char *name, long pid, int type, char *fto, va_list vl)
{
   if (NULL == TRACE_MSG_PrintMsg)
   {
      return;
   }

   char *msg;

   msg = base_string_misc_dynamic_vsprintf(fto, vl);
   trace_msg_do_print("trace(%i,\"%s\",%li,%i,\"%s\").", line, name, pid, type, msg);
   free(msg);
}

void base_trace_msg_printf(int line, char *name, long pid, int type, char *fto, ...)
{
   if (NULL == TRACE_MSG_PrintMsg)
   {
      return;
   }

   va_list vl;

   va_start(vl, fto);
   base_trace_msg_variable_printf(line, name, pid, type, fto, vl);
   va_end(vl);
}

/* ................................................................... */
