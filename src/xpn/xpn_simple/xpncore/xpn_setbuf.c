
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
 * setbuf.c - control buffering of a stream
 */
/* $Header$ */

#include	<stdio.h>
#include	"xpn/xpn_simple/loc_incl.h"

int xpn_setvbuf(register FILE *stream, char *buf, int mode, size_t size);

void
xpn_setbuf(register FILE *stream, char *buf)
{
	(void) xpn_setvbuf(stream, buf, (buf ? _IOFBF : _IONBF), (size_t) BUFSIZ);
}

void
xpn_setbuffer(register FILE *stream, char *buf, size_t size)
{
	(void) xpn_setvbuf(stream, buf, (buf ? _IOFBF : _IONBF), size);
}

void
xpn_setlinebuf(register FILE *stream)
{
	(void ) xpn_setvbuf(stream, (char *) NULL, _IOLBF, 0);
}
