
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


#ifndef _XPN_STDIO_H
#define _XPN_STDIO_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "xpn.h"
  #include "xpn_err.h"
  #include "xpn_policy_cwd.h"

  #include <stdio.h>
  #include <fcntl.h>
  #include <string.h>
  #include <stdlib.h>
  #include <errno.h>

  #include <xpn.h>

  #include <unistd.h>
  #include <stdlib.h>
  #include <strings.h>
  #include <string.h>

  #include "xpn_open.h"
  #include <xpn.h>


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */

  extern struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];


  /* ... Functions / Funciones ......................................... */

  /************************************************************************/
  FILE *xpn_fopencookie(const char *path, const char *mode);
  /************************************************************************/

  FILE  *xpn_simple_fopen  (const char *filename, const char *mode);
  int    xpn_simple_fclose (FILE *stream);
  size_t xpn_simple_fread  (void *ptr, size_t size, size_t nmemb, FILE *stream);
  size_t xpn_simple_fwrite (const void *ptr, size_t size, size_t nmemb, FILE *stream);

  int   xpn_simple_fgetc  (FILE *stream);
  char *xpn_simple_fgets  (char *s, int tam, FILE *stream);
  int   xpn_simple_getc   (FILE *stream);
  int   xpn_simple_fseek  (FILE *stream, long offset, int whence);
  long  xpn_simple_ftell  (FILE *stream);
  void  xpn_simple_rewind (FILE *stream);
  int   xpn_simple_fflush (FILE *stream);
  int   xpn_simple_fileno ( FILE *stream );
  int   xpn_simple_ferror (FILE *stream);


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
