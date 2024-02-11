
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


/* ... Include / Inclusion ........................................... */

#include "xpn.h"
#include "xpn_client/xpn/xpn_simple/xpn_simple_lib.h"
#include "xpn_api_mutex.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Functions / Funciones ......................................... */

FILE * xpn_fopen ( const char *name, const char *mode )
{
  FILE *ret = NULL;

  debug_info("[XPN_STDIO] [xpn_fopen] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_fopen(name, mode);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_fopen] >> End\n");

  return ret;
}

int xpn_fclose ( FILE *fp )
{
  int ret;

  debug_info("[XPN_STDIO] [xpn_fclose] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_fclose(fp);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_fclose] >> End\n");

  return ret;
}

size_t xpn_fread ( void *ptr, size_t size, size_t nmemb, register FILE *stream )
{
  size_t ret;

  debug_info("[XPN_STDIO] [xpn_fread] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_fread(ptr, size, nmemb, stream);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_fread] >> End\n");

  return ret;
}

size_t xpn_fwrite ( const void *ptr, size_t size, size_t nmemb, register FILE *stream )
{
  size_t ret;

  debug_info("[XPN_STDIO] [xpn_fwrite] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_fwrite(ptr, size, nmemb, stream);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_fwrite] >> End\n");

  return ret;
}

int xpn_fflush ( FILE *stream )
{
  int ret;

  debug_info("[XPN_STDIO] [xpn_fflush] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_fflush(stream);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_fflush] >> End\n");

  return ret;
}

int xpn_fseek ( FILE *stream, long int offset, int whence )
{
  int ret;

  debug_info("[XPN_STDIO] [xpn_fseek] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_fseek(stream, offset, whence);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_fseek] >> End\n");

  return ret;
}

long xpn_ftell ( FILE *stream )
{
  long ret;

  debug_info("[XPN_STDIO] [xpn_ftell] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_ftell(stream);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_ftell] >> End\n");

  return ret;
}

void xpn_rewind ( FILE *stream )
{
  debug_info("[XPN_STDIO] [xpn_rewind] >> Begin\n");

  XPN_API_LOCK();
  xpn_simple_rewind(stream);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_rewind] >> End\n");
}

int xpn_preload ( const char *virtual_path, const char *storage_path )
{
  int ret;

  debug_info("[XPN_STDIO] [xpn_preload] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_preload(virtual_path, storage_path);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_preload] >> End\n");

  return ret;
}

int xpn_flush ( const char *virtual_path, const char *storage_path )
{
  int ret;

  debug_info("[XPN_STDIO] [xpn_flush] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_flush(virtual_path, storage_path);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_flush] >> End\n");

  return ret;
}

int xpn_fgetc ( FILE *stream )
{
  int ret;

  debug_info("[XPN_STDIO] [xpn_fgetc] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_fgetc(stream);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_fgetc] >> End\n");

  return ret;
}

char * xpn_fgets ( char *s, int tam, FILE *stream )
{
  char * ret;

  debug_info("[XPN_STDIO] [xpn_fgets] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_fgets(s, tam, stream);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_fgets] >> End\n");

  return ret;
}

int xpn_getc ( FILE *stream )
{
  int ret;

  debug_info("[XPN_STDIO] [xpn_getc] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_getc(stream);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_getc] >> End\n");

  return ret;
}

int xpn_fileno ( FILE *stream )
{
  int ret;

  debug_info("[XPN_STDIO] [xpn_fileno] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_fileno(stream);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_fileno] >> End\n");

  return ret;
}

int xpn_ferror ( FILE *stream )
{
  int ret;

  debug_info("[XPN_STDIO] [xpn_ferror] >> Begin\n");

  XPN_API_LOCK();
  ret = xpn_simple_ferror(stream);
  XPN_API_UNLOCK();

  debug_info("[XPN_STDIO] [xpn_ferror] >> End\n");

  return ret;
}


/* ................................................................... */
