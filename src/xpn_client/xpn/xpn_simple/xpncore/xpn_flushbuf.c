
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
 * flushbuf.c - flush a buffer
 */

#include  <stdio.h>
#include  <stdlib.h>
#include  "xpn/xpn_simple/loc_incl.h"
#include  "base_debug.h"

#include  <sys/types.h>

#include <xpn.h>

static int xpn_flushbuf_do_write(int d, char *buf, int nbytes)
{
  int c;
  int res = -1;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %d", d, nbytes)

  /* POSIX actually allows write() to return a positive value less
     than nbytes, so loop ...
  */
  while ((c = _write(d, buf, nbytes)) > 0 && c < nbytes)
  {
    nbytes -= c;
    buf += c;
  }

  res = c > 0;
  XPN_DEBUG_END_CUSTOM("%d, %d", d, nbytes)
  return c > 0;
}

int xpn_flushbuf(int c, FILE * stream)
{
  int *_bufsiz_addr = (int *) &stream->_bufsiz_rel_addr;
  int res = EOF;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %d", c, fileno(stream))
  XPN_DEBUG("stream->_count = %d _buf = %p _ptr = %p count = %d", stream->_count, stream->_buf, stream->_ptr, (int)(stream->_ptr - stream->_buf))

  //_clean = __cleanup;
  if (fileno(stream) < 0)
  {
    res = (unsigned char) c;
    XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
    return (unsigned char) c;
  }
  if (!io_testflag(stream, _IOWRITE))
  {
    XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
    return EOF;
  }
  if (io_testflag(stream, _IOREADING) && !feof(stream))
  {
    XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
    return EOF;
  }

  stream->_flags &= ~_IOREADING;
  stream->_flags |= _IOWRITING;
  if (!io_testflag(stream, _IONBF))
  {
    if (!stream->_buf)
    {
      /*if (stream == stdout && _isatty(fileno(stdout))) {
        if (!(stream->_buf =
              (unsigned char *) malloc(BUFSIZ))) {
          stream->_flags |= _IONBF;
        } else {
          stream->_flags |= _IOLBF|_IOMYBUF;
          _bufsiz = BUFSIZ;
          stream->_count = -1;
        }
    } 
    else {*/
      if (!(stream->_buf = (char *) malloc(BUFSIZ+1))) 
      {
        stream->_flags |= _IONBF;
        XPN_DEBUG("Error in malloc: Changed to _IONBF")
      } 
      else {
        stream->_flags |= _IOMYBUF;
        _bufsiz = BUFSIZ;
        if (!io_testflag(stream, _IOLBF)){
          stream->_count = BUFSIZ - 1;
        }
        else{
          stream->_count = -1;
        }
        XPN_DEBUG("Reserved a buffer of %d bytes (default)", _bufsiz)
      }
      //}
      stream->_ptr = stream->_buf;
    }
  }

  if (io_testflag(stream, _IONBF))
  {
    char c1 = c;
    XPN_DEBUG("_IONBF count = 1")

    stream->_count = 0;
    if (io_testflag(stream, _IOAPPEND))
    {
      if (_lseek(fileno(stream), 0L, SEEK_END) == -1)
      {
        stream->_flags |= _IOERR;
        XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
        return EOF;
      }
    }

    if (_write(fileno(stream), &c1, 1) != 1)
    {
      stream->_flags |= _IOERR;
      XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
      return EOF;
    }

    res = (unsigned char) c;
    XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
    return (unsigned char) c;
  } 
  else if (io_testflag(stream, _IOLBF))
  {
    *stream->_ptr++ = (char) c;
    /* stream->_count has been updated in putc macro. */
    if (c == '\n' || stream->_count == -_bufsiz)
    {
      int count = -stream->_count;
      XPN_DEBUG("_IOLBF count = %d", count)

      stream->_ptr  = stream->_buf;
      stream->_count = 0;

      if (io_testflag(stream, _IOAPPEND))
      {
        if (_lseek(fileno(stream), 0L, SEEK_END) == -1)
        {
          stream->_flags |= _IOERR;
          XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
          return EOF;
        }
      }
      if (! xpn_flushbuf_do_write(fileno(stream), (char *)stream->_buf, count))
      {
        stream->_flags |= _IOERR;
        XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
        return EOF;
      }
    }
  } 
  else
  {
    int count = stream->_ptr - stream->_buf;
    XPN_DEBUG("!_IONBF && !_IOLBF count = %d, stream->_count = %d", count, stream->_count)

    stream->_count = _bufsiz - 1;
    stream->_ptr = stream->_buf + 1;

    if (count > 0)
    {
      if (io_testflag(stream, _IOAPPEND))
      {
        if (_lseek(fileno(stream), 0L, SEEK_END) == -1)
        {
          stream->_flags |= _IOERR;
          XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
          return EOF;
        }
      }
      if (! xpn_flushbuf_do_write(fileno(stream), (char *)stream->_buf, count))
      {
        *(stream->_buf) = (char) c;
        stream->_flags |= _IOERR;
        XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
        return EOF;
      }
    }
    *(stream->_buf) = (char) c;
  }
  
  res = (unsigned char) c;
  XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
  return (unsigned char) c;
}
