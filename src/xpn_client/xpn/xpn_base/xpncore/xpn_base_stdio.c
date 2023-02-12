
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


#include "xpn/xpn_base/xpn_base_stdio.h"


extern struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];


FILE *xpn_base_fopen(const char *filename, const char *mode)
{
  FILE *stream = NULL;
  int fd = 0;
  int flags ;

  // update flags
  flags = -1;
  if (strstr(mode, "r+") != NULL){
    flags = O_RDWR;
  }
  else if (strstr(mode, "r") != NULL){
    flags = O_RDONLY;
  }
  else if (strstr(mode, "w+") != NULL){
    flags = O_RDWR | O_CREAT | O_TRUNC;
  }
  else if (strstr(mode, "w") != NULL){
    flags = O_WRONLY | O_CREAT | O_TRUNC;
  }
  else if (strstr(mode, "a+") != NULL){
    flags = O_RDWR | O_CREAT | O_APPEND;
  }
  else if (strstr(mode, "a") != NULL){
    flags = O_WRONLY | O_CREAT | O_APPEND;
  }

  if (flags < 0) {
      return NULL ;
  }

  // update file descriptor
  fd = xpn_base_open(filename, flags, 07000);
  if (fd < 0) {
      return NULL ;
  }

  // update stream
  stream = malloc(sizeof(FILE));
  if (stream != NULL) {
      memset(stream, 0, sizeof(FILE));
      stream->_fileno = fd;
  }

  return stream;
}

int xpn_base_fclose(FILE *stream)
{
  int ret = 0;

  // check arguments
  if (NULL == stream) {
      errno = EBADF;
      return EOF ;
  }

  // flush
  xpn_base_fflush(stream);

  // close
  ret = xpn_base_close(stream->_fileno);

  // free internal buffer
  if (stream->_IO_buf_base != NULL) {
      //free(stream->_IO_buf_base); // FIXME
      stream->_IO_buf_base = NULL;
  }

  // free stream
  free(stream);

  return ret;
}

size_t xpn_base_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
   return xpn_base_read(stream->_fileno, ptr, size*nmemb);
}

size_t xpn_base_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
   return xpn_base_write(stream->_fileno, ptr, size*nmemb);
}

int xpn_base_fgetc(FILE *flujo)
{
  unsigned char car;

  xpn_base_fread(&car, sizeof(unsigned char), 1, flujo);

  return (int)car;
}

char *xpn_base_fgets ( char *s, int tam, FILE *flujo )
{
  bzero(s, tam*sizeof(char));
  xpn_base_fread(s, sizeof(char), tam-1, flujo);

  return s;
}

int xpn_base_getc ( FILE *flujo )
{
   return xpn_fgetc(flujo);
}

int xpn_base_fseek ( FILE *stream, long offset, int whence )
{
   return xpn_base_lseek(stream->_fileno, offset, whence);
}

long xpn_base_ftell ( FILE *stream )
{
   return xpn_file_table[stream->_fileno]->offset;
}

void xpn_base_rewind ( FILE *stream )
{
   xpn_base_fseek(stream, 0, 0);
}

int xpn_base_fflush ( FILE *stream )
{
   return 0;
}

int xpn_base_ferror ( FILE *stream )
{
   return 0;
}

int xpn_base_fileno ( FILE *stream )
{
  return stream->_fileno;
}

