
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


#ifndef _XPN_RW_H
#define _XPN_RW_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "xpn.h"
  #include "xpn_file.h"
  #include "xpn_open.h"
  #include "xpn_policy_rw.h"
  #include "xpn_err.h"
  #include "base/workers.h"


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  ssize_t xpn_simple_read  ( int fd, void *buffer, size_t size );
  ssize_t xpn_simple_write ( int fd, const void *buffer, size_t size );
  off_t   xpn_simple_lseek ( int fd, off_t offset, int flag );

  FILE   *xpn_simple_fopen(const char *filename, const char *mode);
  int     xpn_simple_fclose(FILE *stream);
  size_t  xpn_simple_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
  size_t  xpn_simple_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
  int     xpn_simple_fseek(FILE *stream, long offset, int whence);
  long    xpn_simple_ftell(FILE *stream);
  int     xpn_simple_fflush(FILE *stream);

  ssize_t xpn_sread(int fd, const void *buffer, size_t size, off_t offset);
  ssize_t xpn_pread(int fd, void *buffer, size_t size, off_t offset);
  ssize_t xpn_swrite(int fd, const void *buffer, size_t size, off_t offset);
  ssize_t xpn_pwrite(int fd, const void *buffer, size_t size, off_t offset);
  ssize_t xpn_reader (void *cookie, char *buffer, size_t size);
  ssize_t xpn_writer (void *cookie, const char *buffer, size_t size);
  //int xpn_seeker (void *cookie, fpos_t *position, int whence);
  int xpn_seeker (void *cookie, __off64_t *position, int whence);
  int xpn_cleaner (void *cookie);
  int xpn_fgetc(FILE *flujo);
  char *xpn_fgets(char *s, int tam, FILE *flujo);
  int xpn_fgetc(FILE *flujo);
  void xpn_rewind(FILE *stream);
  int xpn_ferror(FILE *stream);


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
