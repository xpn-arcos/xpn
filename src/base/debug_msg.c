
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

#include "base/debug_msg.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */

int    (*DEBUG_MSG_PrintMsg)(const char *, va_list) = NULL;


/* ... Functions / Funciones ......................................... */

// Debug API
void debug_msg_init ( void )
{
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
}

int debug_msg_printf ( int src_type, char *src_fname, long src_line, FILE *fd, char *msg_fmt, ... )
{
  va_list valist;
  int ret;

  va_start(valist, msg_fmt);
  switch (src_type)
  {
    case  3:
      fprintf(fd, "[%s:%4ld] [INFO] ", src_fname, src_line);
      ret = vfprintf(fd, msg_fmt, valist);
      break;

    case  2:
      fprintf(fd, "[%s:%4ld] [WARN] ", src_fname, src_line);
      ret = vfprintf(fd, msg_fmt, valist);
      break;

    case  1:
      fprintf(fd, "[%s:%4ld] [ERROR] ", src_fname, src_line);
      ret = vfprintf(fd, msg_fmt, valist);
      break;

    default:
      ret = vfprintf(fd, msg_fmt, valist);
      break;
  }
  va_end(valist);

  //fflush(fd);

  return ret;
}

// Extra Debug API
void DEBUG_MSG_setPrinter ( int (*printer) (const char *, va_list) )
{
  DEBUG_MSG_PrintMsg = ( int (*)(const char *, va_list)) printer;
}

void DEBUG_MSG_doPrint ( char *fto, ... )
{
  if (DEBUG_MSG_PrintMsg != NULL)
  {
    va_list vl;

    va_start(vl,fto);
    (*DEBUG_MSG_PrintMsg)(fto,vl); 
    va_end(vl);
  }
}

void DEBUG_MSG_VPrintF ( int line, char *name, long pid, int type, char *fto, va_list  vl )
{
  if (DEBUG_MSG_PrintMsg != NULL)
  {
    char *msg;

    msg = STRING_MISC_Dvsprintf(fto,vl);
    DEBUG_MSG_doPrint("trace(%i,\"%s\",%li,%i,\"%s\").", line, name, pid, type, msg);
    free(msg);
  }
}

void DEBUG_MSG_PrintF ( int line, char *name, long pid, int type, char *fto,  ... )
{
  if (DEBUG_MSG_PrintMsg != NULL)
  {
    va_list vl;

    va_start(vl,fto);
    DEBUG_MSG_VPrintF(line,name,pid,type,fto,vl);
    va_end(vl);
  }
}


/* ................................................................... */
