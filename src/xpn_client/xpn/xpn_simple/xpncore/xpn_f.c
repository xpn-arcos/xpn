
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

int buffer_size=64*1024;

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
  *fd = xpn_open((char *)path, flags);

  io_functions.read = xpn_reader;
  io_functions.write = xpn_writer;
  io_functions.seek = xpn_seeker;
  io_functions.close = xpn_cleaner;

  stream = fopencookie((void *)fd, mode, io_functions);

  return stream;
}
/************************************************************************/



/*
int xpn_fgetc(FILE *flujo) {
  unsigned char car;

  xpn_simple_fread(&car, sizeof(unsigned char), 1, flujo);

  return (int)car;
}

char *xpn_fgets(char *s, int tam, FILE *flujo) {
  bzero(s, tam*sizeof(char));
  xpn_simple_fread(s, sizeof(char), tam-1, flujo);
  return s;
}

int xpn_getc(FILE *flujo) {
  return xpn_fgetc(flujo);
}

size_t xpn_simple_fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  size_t count;
  size_t already_read;

  if (size*nmemb <= 0) {
    return 0;
  }

  if (stream->_IO_buf_base == NULL) {
    stream->_IO_buf_base = malloc(buffer_size);
    stream->_IO_buf_end = stream->_IO_buf_base + buffer_size;
    stream->_IO_read_ptr = stream->_IO_buf_base;
    stream->_IO_read_end = stream->_IO_buf_base;
    stream->_IO_read_base = stream->_IO_buf_base;
    stream->_IO_write_base = stream->_IO_buf_base;
    stream->_IO_write_ptr = stream->_IO_buf_base;
    stream->_IO_write_end = stream->_IO_buf_base;

    count = xpn_simple_read(stream->_fileno, stream->_IO_buf_base, stream->_IO_buf_end - stream->_IO_buf_base);
    stream->_IO_read_end = stream->_IO_read_base + count;
  }

  if (stream->_IO_read_end - stream->_IO_read_ptr >= size*nmemb) {
    //fprintf(stderr, "R: _IO_read_ptr=%p, size*nmemb=%d\n", stream->_IO_read_ptr, size*nmemb);
    memcpy(ptr, stream->_IO_read_ptr, size*nmemb);
    stream->_IO_read_ptr += size*nmemb;
    //fprintf(stderr, "R: _IO_read_ptr=%p\n", stream->_IO_read_ptr);
    count = nmemb;
  } else {
    //fprintf(stderr, "[1/2] R: _IO_read_ptr=%p, size=%d\n", stream->_IO_read_ptr, stream->_IO_read_end - stream->_IO_read_ptr);
    memcpy(ptr, stream->_IO_read_ptr, stream->_IO_read_end - stream->_IO_read_ptr);
    already_read = stream->_IO_read_end - stream->_IO_read_ptr;
    stream->_IO_read_ptr = stream->_IO_buf_base;
    count = xpn_simple_read(stream->_fileno, stream->_IO_buf_base, stream->_IO_buf_end - stream->_IO_buf_base);
    stream->_IO_read_end = stream->_IO_read_base + count;
    //fprintf(stderr, "[2/2] R: _IO_read_ptr=%p, size=%d\n", stream->_IO_read_ptr, stream->_IO_read_end - stream->_IO_read_ptr);
    memcpy((char *)((int)ptr + already_read), stream->_IO_read_base, size*nmemb - already_read);
    stream->_IO_read_ptr = stream->_IO_read_base + (size*nmemb - already_read);
    //fprintf(stderr, "R: _IO_read_ptr=%p\n", stream->_IO_read_ptr);
    count = nmemb;
  }

  return count;
}

size_t xpn_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
  size_t count;
  size_t already_read;

  if (size*nmemb <= 0) {
    return 0;
  }

  if (stream->_IO_buf_base == NULL) {
    stream->_IO_buf_base = malloc(buffer_size);
    stream->_IO_buf_end = stream->_IO_buf_base + buffer_size;
    stream->_IO_read_ptr = stream->_IO_buf_base;
    stream->_IO_read_end = stream->_IO_buf_base;
    stream->_IO_read_base = stream->_IO_buf_base;
    stream->_IO_write_base = stream->_IO_buf_base;
    stream->_IO_write_ptr = stream->_IO_buf_base;
    stream->_IO_write_end = stream->_IO_buf_base;
  }

  if (stream->_IO_write_end == stream->_IO_buf_base) {
    stream->_IO_write_base = stream->_IO_read_ptr;
  }

  if (stream->_IO_buf_end - stream->_IO_read_ptr >= size*nmemb) {
    //fprintf(stderr, "W: _IO_buf=%d bytes, %d bytes restantes\n", (int)(stream->_IO_buf_end - stream->_IO_buf_base), (int)(stream->_IO_buf_end - stream->_IO_read_ptr));
    //fprintf(stderr, "W: _IO_read_ptr=%d, size*nmemb=%d\n", (int)(stream->_IO_read_ptr - stream->_IO_read_base), size*nmemb);
    memcpy(stream->_IO_read_ptr, ptr, size*nmemb);
    stream->_IO_read_ptr += size*nmemb;
    //fprintf(stderr, "W: _IO_read_ptr=%d\n", (int)(stream->_IO_read_ptr - stream->_IO_read_base));
    count = nmemb;

  } else {
    //fprintf(stderr, "[1/2] W: _IO_read_ptr=%p, size=%d\n", stream->_IO_read_ptr, stream->_IO_read_end - stream->_IO_read_ptr);
    memcpy(stream->_IO_read_ptr, ptr, stream->_IO_buf_end - stream->_IO_read_ptr);
    already_read = stream->_IO_buf_end - stream->_IO_read_ptr;
    stream->_IO_read_ptr = stream->_IO_buf_base;
    xpn_simple_write(stream->_fileno, stream->_IO_write_base, stream->_IO_buf_end - stream->_IO_write_base);
    stream->_IO_write_base = stream->_IO_buf_base;
    //fprintf(stderr, "[2/2] W: _IO_read_ptr=%p, size=%d\n", stream->_IO_read_ptr, stream->_IO_buf_end - stream->_IO_read_ptr);
    memcpy((char *)((int)ptr + already_read), stream->_IO_read_base, size*nmemb - already_read);
    stream->_IO_read_ptr = stream->_IO_read_base + (size*nmemb - already_read);
    //fprintf(stderr, "W: _IO_read_ptr=%p\n", stream->_IO_read_ptr);
    count = nmemb;
  }

  stream->_IO_write_end = stream->_IO_read_ptr;

  return count;
}
*/

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
    fd = xpn_open(filename, flags, 0666);
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

//#define FAST_FREAD_FWRITE
#ifdef FAST_FREAD_FWRITE

/*****************************/
/* FAST BUT NOT SAFE VERSION */
/*****************************/

//FIXME: xpn_simple_fread/xpn_simple_fwrite/xpn_simple_fseek fail when Reading AND Writing

int xpn_flushbuf(FILE *stream)
{
  if (stream == NULL){
    return EOF;
  }
  else if (stream->_IO_buf_base == NULL)
  {
    stream->_IO_buf_base   = malloc(buffer_size);
    stream->_IO_buf_end    = stream->_IO_buf_base + buffer_size;
    bzero(stream->_IO_buf_base, buffer_size);
    stream->_IO_read_base  = stream->_IO_buf_base;
    stream->_IO_read_ptr   = stream->_IO_buf_base;
    stream->_IO_read_end   = stream->_IO_buf_base;
    stream->_IO_write_base = stream->_IO_buf_base;
    stream->_IO_write_ptr  = stream->_IO_buf_base;
    stream->_IO_write_end  = stream->_IO_buf_base;
  } 
  else if (stream->_IO_write_ptr != stream->_IO_write_base)
  {
    //if (stream->_IO_read_end-stream->_IO_read_base > 0)
    //xpn_simple_lseek(stream->_fileno, -(stream->_IO_read_end-stream->_IO_read_base), SEEK_CUR);
    fprintf(stderr, "xpn_flushbuf: Antes offset=%d\n", xpn_file_table[stream->_fileno]->offset);
    xpn_simple_write(stream->_fileno, stream->_IO_write_base, stream->_IO_write_end-stream->_IO_write_base);
    fprintf(stderr, "xpn_flushbuf: Despues offset=%d\n", xpn_file_table[stream->_fileno]->offset);
    stream->_IO_write_base = stream->_IO_buf_base;
    stream->_IO_write_ptr  = stream->_IO_buf_base;
    stream->_IO_write_end  = stream->_IO_buf_base;
  }

  return 0;
}

void xpn_fillbuf(FILE *stream)
{
  int size;

  if (stream->_IO_buf_base == NULL)
  {
    stream->_IO_buf_base   = malloc(buffer_size);
    stream->_IO_buf_end    = stream->_IO_buf_base + buffer_size;
    bzero(stream->_IO_buf_base, buffer_size);
    stream->_IO_read_base  = stream->_IO_buf_base;
    stream->_IO_read_ptr   = stream->_IO_buf_base;
    stream->_IO_read_end   = stream->_IO_buf_base;
    stream->_IO_write_base = stream->_IO_buf_base;
    stream->_IO_write_ptr  = stream->_IO_buf_base;
    stream->_IO_write_end  = stream->_IO_buf_base;
  }
  else if (stream->_IO_write_end > stream->_IO_write_base){
    xpn_flushbuf(stream);
  }

  fprintf(stderr, "xpn_fillbuf: Antes offset=%d\n", xpn_file_table[stream->_fileno]->offset);
  fprintf(stderr, "xpn_simple_read (%d, buf, %d)", stream->_fileno, buffer_size);

  size = xpn_simple_read(stream->_fileno, stream->_IO_buf_base, buffer_size);

  fprintf(stderr, "=%d\n", size);
  fprintf(stderr, "xpn_fillbuf: Despues offset=%d\n", xpn_file_table[stream->_fileno]->offset);

  if (size > 0)
  {
    stream->_IO_read_base = stream->_IO_buf_base;
    stream->_IO_read_ptr = stream->_IO_buf_base;
    stream->_IO_read_end = stream->_IO_read_base + size;
  }
}

int xpn_getc(FILE *stream)
{
  int ret;

  if ((stream->_IO_read_end <= stream->_IO_read_base) || (stream->_IO_read_ptr >= stream->_IO_read_end)){
    xpn_fillbuf(stream);
  }

  if (stream->_IO_read_ptr < stream->_IO_read_end){
    ret = *(stream->_IO_read_ptr++);
  }
  else{
    ret = EOF;
  }

  return ret;
}

//#define xpn_getc(p) (p->_IO_read_ptr < p->_IO_read_end ? \
//      (int) (*(p)->_IO_read_ptr++) : \
//      xpn_fillbuf(p))

int xpn_fgetc(FILE *stream)
{
  return xpn_getc(stream);
}

size_t xpn_simple_fread(void *ptr, size_t size, size_t nmemb, register FILE *stream)
{
  register char *cp = ptr;
  //register int c;
  size_t ndone = 0;
  register size_t s;

  if (size)
  {
    while ( ndone < nmemb )
    {
      s = size;
      do
      {
        //if ((c = xpn_getc(stream)) != EOF)
        //  *cp++ = c;
        //else
        //  return ndone;
        *cp++ = xpn_getc(stream);
      } while (--s);
      ndone++;
    }
  }

  return ndone;
}

int xpn_putc(int c, FILE *stream)
{
  int ret;

  if (stream->_IO_write_ptr >= stream->_IO_buf_end){
    xpn_flushbuf(stream);
  }

  ret = *(stream)->_IO_write_ptr++ = c;
  if (stream->_IO_write_ptr > stream->_IO_write_end){
    stream->_IO_write_end = stream->_IO_write_ptr;
  }

  return ret;
}

//#define xpn_putc(c, p) (p->_IO_write_ptr < p->_IO_write_end ? \
//      (int) (*(p)->_IO_write_ptr++ = (c)) : \
//      xpn_flushbuf(p))

int xpn_fputc(int c, FILE *stream)
{
  return xpn_putc(c, stream);
}

size_t xpn_simple_fwrite(const void *ptr, size_t size, size_t nmemb, register FILE *stream)
{
  register const unsigned char *cp = ptr;
  register size_t s;
  size_t ndone = 0;

  //if (stream->_IO_buf_base == NULL)
  //  xpn_fillbuf(stream);

  if (size)
  {
    while ( ndone < nmemb ) {
      s = size;
      do
      {
        xpn_putc((int)*cp, stream);
        //if (xpn_putc((int)*cp, stream)
        //  == EOF)
        //  return ndone;
        cp++;
      }
      while (--s);
      ndone++;
    }
  }
  return ndone;
}

int xpn_simple_fseek(FILE *stream, long offset, int whence)
{ 
  //FIXME: Needs lots of debugging!
  int off;

   switch(whence)
   {
      case SEEK_SET:
        fprintf(stderr, "Calling xpn_simple_fseek(%d, %lu, SEEK_SET)\n", stream->_fileno, (unsigned long)offset);
        break;
      case SEEK_CUR:
        fprintf(stderr, "Calling xpn_simple_fseek(%d, %lu, SEEK_CUR)\n", stream->_fileno, (unsigned long)offset);
        break;
      case SEEK_END:
        fprintf(stderr, "Calling xpn_simple_fseek(%d, %lu, SEEK_END)\n", stream->_fileno, (unsigned long)offset);
        break;
      default:
        fprintf(stderr, "Calling xpn_simple_fseek(%d, %lu, %d)\n", stream->_fileno, (unsigned long)offset, whence);
        break;
   }

   off = stream->_IO_write_ptr;
   xpn_flushbuf(stream);
   fprintf(stderr, "xpn_simple_fseek: Before offset=%d\n", xpn_file_table[stream->_fileno]->offset);
   xpn_simple_lseek(stream->_fileno, offset, whence);
   fprintf(stderr, "xpn_simple_fseek: After offset=%d\n", xpn_file_table[stream->_fileno]->offset);

   return 0;
}

long xpn_simple_ftell(FILE *stream)
{
  return xpn_file_table[stream->_fileno]->offset-(stream->_IO_read_end-stream->_IO_read_ptr);
}

int xpn_simple_fflush(FILE *stream)
{
  if (stream != NULL){
    return xpn_flushbuf(stream);
  }
  else{
    return EOF;
  }
}

#else

/*****************************/
/* SAFE BUT NOT FAST VERSION */
/*****************************/

size_t xpn_simple_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  //fprintf(stderr, "SAFE xpn_simple_read\n");
  return xpn_simple_read(stream->_fileno, ptr, size*nmemb);
}

size_t xpn_simple_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  //fprintf(stderr, "SAFE xpn_simple_fwrite\n");
  return xpn_simple_write(stream->_fileno, ptr, size*nmemb);
}

int xpn_fgetc(FILE *flujo)
{
  unsigned char car;

  xpn_simple_fread(&car, sizeof(unsigned char), 1, flujo);
  return (int)car;
}

char *xpn_fgets(char *s, int tam, FILE *flujo)
{
  bzero(s, tam*sizeof(char));
  xpn_simple_fread(s, sizeof(char), tam-1, flujo);
  return s;
}

int xpn_getc(FILE *flujo)
{
  return xpn_fgetc(flujo);
}

int xpn_simple_fseek(FILE *stream, long offset, int whence)
{
  //fprintf(stderr, "SAFE xpn_simple_lseek\n");
  xpn_simple_lseek(stream->_fileno, offset, whence);
  return 0;
}

long xpn_simple_ftell(FILE *stream)
{
  return xpn_file_table[stream->_fileno]->offset;
}

void xpn_rewind(FILE *stream)
{
  xpn_simple_fseek(stream, 0, 0);
}

int xpn_simple_fflush(FILE *stream)
{
  return 0;
}

int xpn_ferror(FILE *stream)
{
  return 0;
}

#endif

