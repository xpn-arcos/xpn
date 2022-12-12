
  /*
   *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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
 * fclose.c - flush a stream and close the file
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<signal.h>
#include	"xpn/xpn_simple/loc_incl.h"
#include	"xpn_debug.h"

#include <xpn.h>

int
xpn_fclose(FILE *fp)
{
	register int /*i, */retval = 0;
	struct _prefetch_t **_prefetch_addr = (struct _prefetch_t **) &fp->_chain->_prefetch_rel_addr;
	int res = -1;

	XPN_DEBUG_BEGIN_CUSTOM("%d", fileno(fp))

	/*for (i=0; i<FOPEN_MAX; i++)
		if (fp == __iotab[i]) {
			__iotab[i] = 0;
			break;
		}
	if (i >= FOPEN_MAX)
		return EOF;*/
	if (fflush(fp)) retval = EOF;
	XPN_DEBUG("Before locking _prefetch->mutex")
	pthread_mutex_lock(&(_prefetch->mutex));
	XPN_DEBUG("After locking _prefetch->mutex")
	if (_close(fileno(fp))) retval = EOF;
	pthread_mutex_unlock(&(_prefetch->mutex));
	if ( io_testflag(fp,_IOMYBUF) && fp->_buf )
		free((void *)fp->_buf);
	/* For prefetching */
	if ( fp->_chain != NULL ) {
		if ( _prefetch->do_prefetch )
			pthread_kill(_prefetch->thread, SIGTERM);
		if ( io_testflag(fp->_chain,_IOMYBUF) && fp->_chain->_buf )
			free((void *)fp->_chain->_buf);
		free((void *)fp->_chain);
	}
	//if (fp != stdin && fp != stdout && fp != stderr)
		free((void *)fp);
	res = retval;
	XPN_DEBUG_END_CUSTOM("%d", fileno(fp))
	return retval;
}
