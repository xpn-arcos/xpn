
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
 * fseek.c - perform an fseek
 */
/* $Header$ */

#include	<stdio.h>

#if	(SEEK_CUR != 1) || (SEEK_END != 2) || (SEEK_SET != 0)
#error SEEK_* values are wrong
#endif

#include	"xpn/xpn_simple/loc_incl.h"

#include	<sys/types.h>

#include <xpn.h>

int
xpn_fseek(FILE *stream, long int offset, int whence)
{
	int adjust = 0;
	long pos;
//printf("xpn_fseek(%d, %ld, %d)|||||||||SEEK_CUR=%d\n", fileno(stream), offset, whence, SEEK_CUR);

	stream->_flags &= ~(_IOEOF | _IOERR);
	/* Clear both the end of file and error flags */

	if (io_testflag(stream, _IOREADING)) {
		if (whence == SEEK_CUR
		    && stream->_buf
		    && !io_testflag(stream,_IONBF))
			adjust = stream->_count;
		stream->_count = 0;
	} else if (io_testflag(stream,_IOWRITING)) {
		fflush(stream);
	} else {	/* neither reading nor writing. The buffer must be empty */
		/* EMPTY */ ;
	}

	pos = _lseek(fileno(stream), offset - adjust, whence);
	if (io_testflag(stream, _IOREAD) && io_testflag(stream, _IOWRITE))
		stream->_flags &= ~(_IOREADING | _IOWRITING);

	stream->_ptr = stream->_buf;
//printf("xpn_fseek(%d, %ld, %d)=%ld\n", fileno(stream), offset, whence, pos);
	return ((pos == -1) ? -1 : 0);
}
