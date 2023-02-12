
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

#ifndef _XPN_BASE_STDIO_H
#define _XPN_BASE_STDIO_H


 #ifdef  __cplusplus
    extern "C" {
 #endif


#include "xpn_base.h"
#include "xpn_policy_open.h"
#include "xpn_policy_cwd.h"
#include "xpn_base_init.h"
#include "xpn_base_filetable.h"
#include "xpn_base_unistd.h"


extern struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];

FILE * xpn_base_fopen(const char *filename, const char *mode) ;
int    xpn_base_fclose(FILE *stream) ;
size_t xpn_base_fread(void *ptr, size_t size, size_t nmemb, FILE *stream) ;
size_t xpn_base_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) ;
int    xpn_base_fgetc(FILE *flujo) ;
char * xpn_base_fgets ( char *s, int tam, FILE *flujo ) ;
int    xpn_base_getc ( FILE *flujo ) ;
int    xpn_base_fseek ( FILE *stream, long offset, int whence ) ;
long   xpn_base_ftell ( FILE *stream ) ;
void   xpn_base_rewind ( FILE *stream ) ;
int    xpn_base_fflush ( FILE *stream ) ;
int    xpn_base_ferror ( FILE *stream ) ;
int    xpn_base_fileno ( FILE *stream ) ;


 #ifdef  __cplusplus
     }
 #endif


#endif

