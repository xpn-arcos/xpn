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
 * fflush.c - flush stream(s)
 */
/* $Header$ */

#include	<sys/types.h>
#include	<stdio.h>
#include	<errno.h>
#include	"xpn/xpn_simple/loc_incl.h"

#include <xpn.h>

int
xpn_fflush(FILE *stream)
{
	int count, c1/*, i, retval = 0*/;

	/*if (!stream) {
	    for(i= 0; i < FOPEN_MAX; i++)
		if (__iotab[i] && fflush(__iotab[i]))
			retval = EOF;
	    return retval;
	}*/

	if (!stream->_buf
	    || (!io_testflag(stream, _IOREADING)
		&& !io_testflag(stream, _IOWRITING)))
		return 0;
	if (io_testflag(stream, _IOREADING)) {
		/* (void) fseek(stream, 0L, SEEK_CUR); */
		int adjust = 0;
		if (io_testflag(stream, _IOFIFO)) {
			/* Can't seek in a pipe. */
			return 0;
		}
		if (stream->_buf && !io_testflag(stream,_IONBF))
			adjust = -stream->_count;
		stream->_count = 0;
		if (_lseek(fileno(stream), (off_t) adjust, SEEK_CUR) == -1 &&
		  errno != ESPIPE) {
			stream->_flags |= _IOERR;
			return EOF;
		}
		errno = 0;
		if (io_testflag(stream, _IOWRITE))
			stream->_flags &= ~(_IOREADING | _IOWRITING);
		stream->_ptr = stream->_buf;
		return 0;
	} else if (io_testflag(stream, _IONBF)) return 0;

	if (io_testflag(stream, _IOREAD))		/* "a" or "+" mode */
		stream->_flags &= ~_IOWRITING;

	count = stream->_ptr - stream->_buf;
	stream->_ptr = stream->_buf;

	if ( count <= 0 )
		return 0;

	if (io_testflag(stream, _IOAPPEND)) {
		if (_lseek(fileno(stream), 0L, SEEK_END) == -1) {
			stream->_flags |= _IOERR;
			return EOF;
		}
	}
	c1 = _write(stream->_fd, (char *)stream->_buf, count);

	stream->_count = 0;

	if ( count == c1 )
		return 0;

	stream->_flags |= _IOERR;
	return EOF; 
}

/*void
__cleanup(void)
{
	register int i;

	for(i= 0; i < FOPEN_MAX; i++)
		if (__iotab[i] && io_testflag(__iotab[i], _IOWRITING))
			(void) fflush(__iotab[i]);
}*/
