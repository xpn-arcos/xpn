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
 * fillbuf.c - fill a buffer
 */
/* $Header$ */

#if	defined(_POSIX_SOURCE)
#include	<sys/types.h>
#endif
#include	<stdio.h>
#include	<stdlib.h>
#include	"xpn/xpn_simple/loc_incl.h"
#include	"xpn_debug.h"

#include <xpn.h>

int
xpn_fillbuf(register FILE *stream)
{
	static unsigned char ch[FOPEN_MAX];
	//register int i;
	int *_bufsiz_addr = (int *) &stream->_bufsiz_rel_addr;
	int res = EOF;

	XPN_DEBUG_BEGIN_CUSTOM("%d", fileno(stream))

	stream->_count = 0;
	if (fileno(stream) < 0) {
		XPN_DEBUG_END_CUSTOM("%d", fileno(stream))
		return EOF;
	}
	if (io_testflag(stream, (_IOEOF | _IOERR ))) { 
		XPN_DEBUG_END_CUSTOM("%d", fileno(stream))
		return EOF;
	}
	if (!io_testflag(stream, _IOREAD))
	     { stream->_flags |= _IOERR;
		XPN_DEBUG_END_CUSTOM("%d", fileno(stream))
		return EOF; }
	if (io_testflag(stream, _IOWRITING))
	     { stream->_flags |= _IOERR;
		XPN_DEBUG_END_CUSTOM("%d", fileno(stream))
		return EOF; }

	if (!io_testflag(stream, _IOREADING))
		stream->_flags |= _IOREADING;
	
	if (!io_testflag(stream, _IONBF) && !stream->_buf) {
		stream->_buf = (char *) malloc(BUFSIZ+1);
		if (!stream->_buf) {
			stream->_flags |= _IONBF;
		}
		else {
			stream->_flags |= _IOMYBUF;
			_bufsiz = BUFSIZ;
		}
	}

	/* flush line-buffered output when filling an input buffer */
	/*for (i = 0; i < FOPEN_MAX; i++) {
		if (__iotab[i] && io_testflag(__iotab[i], _IOLBF))
			if (io_testflag(__iotab[i], _IOWRITING))
				(void) fflush(__iotab[i]);
	}*/
		if (stream && io_testflag(stream, _IOLBF))
			if (io_testflag(stream, _IOWRITING))
				(void) fflush(stream);

	if (!stream->_buf) {
		stream->_buf = (char *) &ch[fileno(stream)];
		_bufsiz = 1;
	}
	stream->_ptr = stream->_buf;
	stream->_count = _read(stream->_fd, (char *)stream->_buf, _bufsiz);

	if (stream->_count <= 0){
		if (stream->_count == 0) {
			stream->_flags |= _IOEOF;
		}
		else 
			stream->_flags |= _IOERR;

		XPN_DEBUG_END_CUSTOM("%d", fileno(stream))
		return EOF;
	}
	stream->_count--;

	res = (unsigned char)*stream->_ptr++;
	XPN_DEBUG_END_CUSTOM("%d", fileno(stream))
	return (unsigned char)*stream->_ptr++;
}

void
xpn_fillbuf_noret(register FILE *stream)
{
	xpn_fillbuf(stream);
	stream->_ptr--;
	stream->_count++;
	//printf("xpn_fillbuf_noret\n");
}
