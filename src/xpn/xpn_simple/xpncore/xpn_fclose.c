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
 * fclose.c - flush a stream and close the file
 */
/* $Header$ */

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
