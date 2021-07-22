/*
 * Copyright (c) 1987, 1997, 2006, Vrije Universiteit, Amsterdam,
 * The Netherlands All rights reserved. Redistribution and use of the MINIX 3
 * operating system in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 
 *     * Neither the name of the Vrije Universiteit nor the names of the
 *     software authors or contributors may be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 * 
 *     * Any deviations from these conditions require written permission
 *     from the copyright holder in advance
 * 
 * 
 * Disclaimer
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS, AUTHORS, AND
 *  CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 *  NO EVENT SHALL PRENTICE HALL OR ANY AUTHORS OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * flushbuf.c - flush a buffer
 */
/* $Id: flushbuf.c 5 2005-04-21 14:53:53Z beng $ */

#include	<stdio.h>
#include	<stdlib.h>
#include	"xpn/xpn_simple/loc_incl.h"
#include	"xpn_debug.h"

#include	<sys/types.h>

#include <xpn.h>

static int
xpn_flushbuf_do_write(int d, char *buf, int nbytes)
{
	int c;
	int res = -1;

	XPN_DEBUG_BEGIN_CUSTOM("%d, %d", d, nbytes)

	/* POSIX actually allows write() to return a positive value less
	   than nbytes, so loop ...
	*/
	while ((c = _write(d, buf, nbytes)) > 0 && c < nbytes) {
		nbytes -= c;
		buf += c;
	}
	res = c > 0;
	XPN_DEBUG_END_CUSTOM("%d, %d", d, nbytes)
	return c > 0;
}

int
xpn_flushbuf(int c, FILE * stream)
{
	int *_bufsiz_addr = (int *) &stream->_bufsiz_rel_addr;
	int res = EOF;

	XPN_DEBUG_BEGIN_CUSTOM("%d, %d", c, fileno(stream))
	XPN_DEBUG("stream->_count = %d _buf = %p _ptr = %p count = %d", stream->_count, stream->_buf, stream->_ptr, (int)(stream->_ptr - stream->_buf))

	//_clean = __cleanup;
	if (fileno(stream) < 0) {
		res = (unsigned char) c;
		XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
		return (unsigned char) c;
	}
	if (!io_testflag(stream, _IOWRITE)) {
		XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
		return EOF;
	}
	if (io_testflag(stream, _IOREADING) && !feof(stream)) {
		XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
		return EOF;
	}

	stream->_flags &= ~_IOREADING;
	stream->_flags |= _IOWRITING;
	if (!io_testflag(stream, _IONBF)) {
		if (!stream->_buf) {
			/*if (stream == stdout && _isatty(fileno(stdout))) {
				if (!(stream->_buf =
					    (unsigned char *) malloc(BUFSIZ))) {
					stream->_flags |= _IONBF;
				} else {
					stream->_flags |= _IOLBF|_IOMYBUF;
					_bufsiz = BUFSIZ;
					stream->_count = -1;
				}
			} else {*/
				if (!(stream->_buf =
					    (char *) malloc(BUFSIZ+1))) {
					stream->_flags |= _IONBF;
					XPN_DEBUG("Error in malloc: Changed to _IONBF")
				} else {
					stream->_flags |= _IOMYBUF;
					_bufsiz = BUFSIZ;
					if (!io_testflag(stream, _IOLBF))
						stream->_count = BUFSIZ - 1;
					else	stream->_count = -1;
					XPN_DEBUG("Reserved a buffer of %d bytes (default)", _bufsiz)
				}
			//}
			stream->_ptr = stream->_buf;
		}
	}

	if (io_testflag(stream, _IONBF)) {
		char c1 = c;
		XPN_DEBUG("_IONBF count = 1")

		stream->_count = 0;
		if (io_testflag(stream, _IOAPPEND)) {
			if (_lseek(fileno(stream), 0L, SEEK_END) == -1) {
				stream->_flags |= _IOERR;
				XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
				return EOF;
			}
		}
		if (_write(fileno(stream), &c1, 1) != 1) {
			stream->_flags |= _IOERR;
			XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
			return EOF;
		}
		res = (unsigned char) c;
		XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
		return (unsigned char) c;
	} else if (io_testflag(stream, _IOLBF)) {
		*stream->_ptr++ = (char) c;
		/* stream->_count has been updated in putc macro. */
		if (c == '\n' || stream->_count == -_bufsiz) {
			int count = -stream->_count;
			XPN_DEBUG("_IOLBF count = %d", count)

			stream->_ptr  = stream->_buf;
			stream->_count = 0;

			if (io_testflag(stream, _IOAPPEND)) {
				if (_lseek(fileno(stream), 0L, SEEK_END) == -1) {
					stream->_flags |= _IOERR;
					XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
					return EOF;
				}
			}
			if (! xpn_flushbuf_do_write(fileno(stream), (char *)stream->_buf,
					count)) {
				stream->_flags |= _IOERR;
				XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
				return EOF;
			}
		}
	} else {
		int count = stream->_ptr - stream->_buf;
		XPN_DEBUG("!_IONBF && !_IOLBF count = %d, stream->_count = %d", count, stream->_count)

		stream->_count = _bufsiz - 1;
		stream->_ptr = stream->_buf + 1;

		if (count > 0) {
			if (io_testflag(stream, _IOAPPEND)) {
				if (_lseek(fileno(stream), 0L, SEEK_END) == -1) {
					stream->_flags |= _IOERR;
					XPN_DEBUG_END_CUSTOM("%d, %d", c, fileno(stream))
					return EOF;
				}
			}
			if (! xpn_flushbuf_do_write(fileno(stream), (char *)stream->_buf, count)) {
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
