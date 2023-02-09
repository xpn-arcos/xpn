
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


/*
 * fread.c - read a number of members into an array
 */

#include  <stdio.h>
#include  "xpn/xpn_simple/loc_incl.h"
#include  "base_debug.h"

int xpn_fillbuf(register FILE *stream);
int xpn_fillbuf_noret(register FILE *stream);

size_t xpn_simple_fread(void *ptr, size_t size, size_t nmemb, register FILE *stream)
{
  register char *cp = (char *)ptr;
  register int c;
  size_t ndone = 0;
  register size_t s;
  size_t res = (size_t) -1;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)

  if (size){
    while ( ndone < nmemb )
    {
      s = size;
      do
      {
        if ((c = xpn_getc(stream)) != EOF){
          *cp++ = (signed char) c;
        }
        else 
        {
          res = ndone;
          XPN_DEBUG_END_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)
          return ndone;
        }
      } while (--s);
      ndone++;
    }
  }

  res = ndone;
  XPN_DEBUG_END_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)
  //fprintf(stderr,"xpn_fread: fd %d size %zu off %lld 1st byte='%c'.\n",fd,size,(long long int)offset,((char *)buffer)[0]);
  return ndone;
}

int swap_file_ptrs(register FILE *stream)
{
  struct _prefetch_t **_prefetch_addr = (struct _prefetch_t **) &stream->_chain->_prefetch_rel_addr;

  if (io_testflag(stream, (_IOEOF | _IOERR ))){
    return EOF;
  }

  //printf("swap_file_ptrs: lock\n");
  pthread_mutex_lock(&(_prefetch->mutex));
  while ((stream->_chain->_count <= 0) && !io_testflag(stream->_chain, (_IOEOF | _IOERR ))) {
    //printf("swap_file_ptrs: wait stream->_chain->_count=%d\n", stream->_chain->_count);
    pthread_cond_wait(&(_prefetch->cond_main), &(_prefetch->mutex));
  }
  //printf("swap_file_ptrs: wait stream->_chain->_count=%d\n", stream->_chain->_count);

  if ((stream->_chain->_count <= 0) && io_testflag(stream->_chain, (_IOEOF | _IOERR ))){
    return EOF;
  }

  char *buf = stream->_buf;
  char *ptr = stream->_ptr;

  stream->_count = stream->_chain->_count;
  stream->_flags = stream->_chain->_flags;
  stream->_buf   = stream->_chain->_buf;
  stream->_ptr   = stream->_chain->_ptr;

  stream->_chain->_count = 0;
  stream->_chain->_buf   = buf;
  stream->_chain->_ptr   = ptr;

  //printf("swap_file_ptrs: signal\n");
  pthread_cond_signal(&(_prefetch->cond_th));
  pthread_mutex_unlock(&(_prefetch->mutex));
  //printf("swap_file_ptrs: unlocked\n");

  stream->_count--;

  return (unsigned char)*stream->_ptr++;
}

void *prefetch_buf(void *arg)
{
  FILE *stream = (FILE *) arg;
  struct _prefetch_t **_prefetch_addr = (struct _prefetch_t **) &stream->_prefetch_rel_addr;

  pthread_mutex_lock(&(_prefetch->mutex));
  //printf("prefetch_buf: fillbuf...\n");
  xpn_fillbuf_noret(stream);
  //printf("prefetch_buf: signal\n");
  pthread_cond_signal(&(_prefetch->cond_main));
  pthread_mutex_unlock(&(_prefetch->mutex));

  while (!io_testflag(stream, (_IOEOF | _IOERR )))
  {
    //printf("prefetch_buf: lock\n");
    pthread_mutex_lock(&(_prefetch->mutex));
    while (stream->_count > 0) {
      //printf("prefetch_buf: wait stream->_count=%d\n", stream->_count);
      pthread_cond_wait(&(_prefetch->cond_th), &(_prefetch->mutex));
    }
    //printf("prefetch_buf: cont stream->_count=%d\n", stream->_count);

    xpn_fillbuf_noret(stream);
    //printf("prefetch_buf: fillbuf\n");

    pthread_cond_signal(&(_prefetch->cond_main));
    pthread_mutex_unlock(&(_prefetch->mutex));
    //printf("prefetch_buf: unlocked\n");
  }

  return NULL;
}

size_t xpn_fread_prefetch(void *ptr, size_t size, size_t nmemb, register FILE *stream)
{
  register char *cp = (char *)ptr;
  register int c;
  size_t ndone = 0;
  register size_t s;
  struct _prefetch_t **_prefetch_addr = (struct _prefetch_t **) &stream->_chain->_prefetch_rel_addr;
  size_t res = (size_t) -1;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)

  if ((stream->_chain != NULL) && (_prefetch != NULL) && (!_prefetch->do_prefetch)) //TODO: Check that this condition really works well
  { 
    //printf("xpn_fread_prefetch: fillbuf...\n");
    xpn_fillbuf_noret(stream);
    _prefetch->do_prefetch = 1;
    pthread_create(&(_prefetch->thread), NULL, prefetch_buf, stream->_chain);
  }

  if (size){
    while ( ndone < nmemb )
    {
      s = size;
      do 
      {
        if ((c = xpn_getc_nofill(stream)) != EOF){
          *cp++ = (signed char) c;
        }
        else
        {
          res = ndone;
          XPN_DEBUG_END_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)
          return ndone;
        }
      } while (--s);
      ndone++;
    }
  }

  res = ndone;
  XPN_DEBUG_END_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)
  return ndone;
}

