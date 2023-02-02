
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
 * setbuf.c - control buffering of a stream
 */

#include  <stdio.h>
#include  <stdlib.h>
#include  "xpn/xpn_simple/loc_incl.h"
#include  "base_debug.h"

//extern void (*_clean)(void);

#define _IOMYBUF        0x008

int xpn_setvbuf(register FILE *stream, char *buf, int mode, size_t size)
{
  int retval = 0;
  int *_bufsiz_addr = (int *) &stream->_bufsiz_rel_addr;
  int res = EOF;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %d, %zu", fileno(stream), mode, size)

  //_clean = __cleanup;
  if (mode != _IOFBF && mode != _IOLBF && mode != _IONBF)
  {
    XPN_DEBUG_END_CUSTOM("%d, %d, %zu", fileno(stream), mode, size)
    return EOF;
  }

  if (stream->_buf && io_testflag(stream,_IOMYBUF) )
  {
    free((void *)stream->_buf);
    /* For prefetching */
    if ((stream->_chain != NULL) && (stream->_chain->_buf != NULL)){
      free((void *)stream->_chain->_buf);
    }
  }

  stream->_flags &= ~(_IOMYBUF | _IONBF | _IOLBF);

  if (buf && size <= 0){
    retval = EOF;
  }
  if (!buf && (mode != _IONBF))
  {
    if (size <= 0 || (buf = (char *) malloc(size+1)) == NULL) {
      retval = EOF;
    } 
    else {
      stream->_flags |= _IOMYBUF;
    }
  }

  //stream->_buf = (unsigned char *) buf; //Original
  stream->_buf = (char *) buf;

  stream->_count = 0;
  stream->_flags |= mode;
  stream->_ptr = stream->_buf; // Original
  //stream->_ptr = NULL; // Mine: fails in writes

  if (!buf) {
    _bufsiz = 1;
  } 
  else {
    _bufsiz = size;
  }

  /* For prefetching */
  if (stream->_chain != NULL)
  {
    int *_bufsiz_chain_addr = (int *) &stream->_chain->_bufsiz_rel_addr;

    stream->_chain->_buf = (char *) malloc(size+1);
    stream->_chain->_count = 0;
    stream->_chain->_flags = stream->_flags;
    stream->_chain->_ptr = NULL;
    *_bufsiz_chain_addr = _bufsiz;
  }

  res = retval;
  XPN_DEBUG_END_CUSTOM("%d, %d, %zu", fileno(stream), mode, size)
  return retval;
}

