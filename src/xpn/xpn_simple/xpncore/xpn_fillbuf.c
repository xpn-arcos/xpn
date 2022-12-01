
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 *
 *  This file is part of mpiServer.
 *
 *  mpiServer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  mpiServer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with mpiServer.  If not, see <http://www.gnu.org/licenses/>.
 *
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
