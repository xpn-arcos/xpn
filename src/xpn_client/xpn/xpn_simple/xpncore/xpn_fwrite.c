
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
 * fwrite.c - write a number of array elements on a file
 */

#include  <stdio.h>
#include  "xpn/xpn_simple/loc_incl.h"
#include  "xpn_debug.h"

int xpn_flushbuf(int c, FILE * stream);

size_t xpn_fwrite(const void *ptr, size_t size, size_t nmemb, register FILE *stream)
{
  register const unsigned char *cp = (const unsigned char *)ptr;
  register size_t s;
  size_t ndone = 0;
  size_t res = (size_t) -1;

  XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)
  XPN_DEBUG("stream->_count = %d _buf = %p _ptr = %p count = %d", stream->_count, stream->_buf, stream->_ptr, (int)(stream->_ptr - stream->_buf))

  if (size){
    while ( ndone < nmemb )
    {
      s = size;
      do
      {
        if (xpn_putc(*cp, stream)
          == EOF) {
          res = ndone;
          XPN_DEBUG_END_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)
          return ndone;
        }
        cp++;
      } while (--s);
      ndone++;
    }
  }

  res = ndone;
  XPN_DEBUG_END_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)
  return ndone;
}
