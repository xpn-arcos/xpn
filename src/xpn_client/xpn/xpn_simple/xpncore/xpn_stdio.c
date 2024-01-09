
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


extern struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];


/************************************************************************/
ssize_t xpn_reader (void *cookie, char *buffer, size_t size)
{
  return xpn_simple_read(*((int *)cookie), buffer, size);
}

ssize_t xpn_writer (void *cookie, const char *buffer, size_t size)
{
  return xpn_simple_write(*((int *)cookie), (char *)buffer, size);
}

int xpn_seeker (void *cookie, __off64_t *position, int whence)
{
  return xpn_simple_lseek(*((int *)cookie), (off_t)*position, whence);
}

int xpn_cleaner (void *cookie)
{
  int fd;

  fd = *((int *)cookie);
  free(cookie);
  return xpn_simple_close(fd);
}

FILE *xpn_fopencookie(const char *path, const char *mode)
{
  FILE *stream;
  int flags, *fd;
  cookie_io_functions_t io_functions;

  flags = O_RDONLY;
  if (0 == strncmp(mode, "r+", 2)){
    flags = O_RDWR;
  }
  else if (0 == strncmp(mode, "w+", 2)){
    flags = O_RDWR | O_CREAT | O_TRUNC;
  }
  else if (0 == strncmp(mode, "a+", 2)){
    flags = O_RDWR;
  }
  else if (0 == strncmp(mode, "r", 1)){
    flags = O_RDONLY;
  }
  else if (0 == strncmp(mode, "w", 1)){
    flags = O_WRONLY | O_CREAT | O_TRUNC;
  }
  else if (0 == strncmp(mode, "a", 1)){
    flags = O_WRONLY;
  }

  fd = (int *)malloc(sizeof(int));
  *fd = xpn_simple_open((char *)path, flags, 07000);

  io_functions.read = xpn_reader;
  io_functions.write = xpn_writer;
  io_functions.seek = xpn_seeker;
  io_functions.close = xpn_cleaner;

  stream = fopencookie((void *)fd, mode, io_functions);

  return stream;
}

/************************************************************************/


FILE *xpn_simple_fopen(const char *filename, const char *mode)
{
  FILE *stream = NULL;
  int fd = 0;
  int flags = -1;

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

  if (flags >= 0)
  {
    fd = xpn_simple_open(filename, flags, 07000);
    if (fd >= 0)
    {
      stream = malloc(sizeof(FILE));
      memset(stream, 0, sizeof(FILE));
      stream->_fileno = fd;
    }
  }

  return stream;
}


int xpn_simple_fclose(FILE *stream)
{
  int ret = 0;

  if (stream != NULL)
  {
    //if ((stream->_IO_write_end - stream->_IO_write_base) > 0)
    //  xpn_simple_write(stream->_fileno, stream->_IO_write_base, stream->_IO_write_end - stream->_IO_write_base);
    xpn_simple_fflush(stream);

    ret = xpn_simple_close(stream->_fileno);
    if (stream->_IO_buf_base != NULL) {
      //free(stream->_IO_buf_base); // FIXME
      stream->_IO_buf_base = NULL;
    }
    free(stream);
  }
  else
  {
    ret = EOF;
    errno = EBADF;
  }

  return ret;
}



size_t xpn_simple_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  return xpn_simple_read(stream->_fileno, ptr, size*nmemb);
}


size_t xpn_simple_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  return xpn_simple_write(stream->_fileno, ptr, size*nmemb);
}


int xpn_simple_fgetc(FILE *stream)
{
  unsigned char car;

  xpn_simple_read(stream->_fileno, &car, 1*sizeof(unsigned char));

  return (int)car;
}


char *xpn_simple_fgets(char *s, int tam, FILE *stream)
{
  memset(s, 0, tam*sizeof(char));

  xpn_simple_fread(s, sizeof(char), tam-1, stream);

  return s;
}


int xpn_simple_getc(FILE *stream)
{
  return xpn_simple_fgetc(stream);
}


int xpn_simple_fseek(FILE *stream, off_t offset, int whence)
{
  off_t ret ;

  ret = xpn_simple_lseek(stream->_fileno, offset, whence);
  if (ret == (off_t) -1){
      return (off_t) -1 ;
  }

  return 0;
}


long xpn_simple_ftell(FILE *stream)
{
  // TODO: review that the offset is updated according to read/write...
  return xpn_file_table[stream->_fileno]->offset;
}


void xpn_simple_rewind(FILE *stream)
{
  xpn_simple_lseek(stream->_fileno, 0, SEEK_SET);
}


int xpn_simple_fflush ( __attribute__((__unused__)) FILE *stream )
{
  return 0;
}


int xpn_simple_fileno ( FILE *stream )
{
  return stream->_fileno;
}


int xpn_simple_ferror ( __attribute__((__unused__)) FILE *stream )
{
  // TODO: update errno or xpn_errno?

  return 0;
}

