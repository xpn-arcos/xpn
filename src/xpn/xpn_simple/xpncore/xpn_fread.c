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
 * fread.c - read a number of members into an array
 */
/* $Header$ */

#include	<stdio.h>
#include	"xpn/xpn_simple/loc_incl.h"
#include	"xpn_debug.h"

int xpn_fillbuf(register FILE *stream);
int xpn_fillbuf_noret(register FILE *stream);

size_t xpn_fread(void *ptr, size_t size, size_t nmemb, register FILE *stream)
{
	register char *cp = (char *)ptr;
	register int c;
	size_t ndone = 0;
	register size_t s;
	size_t res = (size_t) -1;

	XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)

	if (size)
		while ( ndone < nmemb ) {
			s = size;
			do {
				if ((c = xpn_getc(stream)) != EOF)
					*cp++ = (signed char) c;
				else {
					res = ndone;
					XPN_DEBUG_END_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)
					return ndone;
				}
			} while (--s);
			ndone++;
		}

	res = ndone;
	XPN_DEBUG_END_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)
	//fprintf(stderr,"xpn_fread: fd %d size %zu off %lld 1st byte='%c'.\n",fd,size,(long long int)offset,((char *)buffer)[0]);
	return ndone;
}

int
swap_file_ptrs(register FILE *stream)
{
	struct _prefetch_t **_prefetch_addr = (struct _prefetch_t **) &stream->_chain->_prefetch_rel_addr;

	if (io_testflag(stream, (_IOEOF | _IOERR ))) return EOF;

	//printf("swap_file_ptrs: lock\n");
	pthread_mutex_lock(&(_prefetch->mutex));
	while ((stream->_chain->_count <= 0) && !io_testflag(stream->_chain, (_IOEOF | _IOERR ))) {
		//printf("swap_file_ptrs: wait stream->_chain->_count=%d\n", stream->_chain->_count);
		pthread_cond_wait(&(_prefetch->cond_main), &(_prefetch->mutex));
	}
	//printf("swap_file_ptrs: wait stream->_chain->_count=%d\n", stream->_chain->_count);

	if ((stream->_chain->_count <= 0) && io_testflag(stream->_chain, (_IOEOF | _IOERR )))
		return EOF;

	char *buf = stream->_buf;
	char *ptr = stream->_ptr;

	stream->_count = stream->_chain->_count;
	stream->_flags = stream->_chain->_flags;
	stream->_buf   = stream->_chain->_buf;
	stream->_ptr   = stream->_chain->_ptr;

	stream->_chain->_count = 0;
	stream->_chain->_buf   = buf;
	stream->_chain->_ptr   = ptr;

	//printf("swap_file_ptrs: signal\n");
	pthread_cond_signal(&(_prefetch->cond_th));
	pthread_mutex_unlock(&(_prefetch->mutex));
	//printf("swap_file_ptrs: unlocked\n");

	stream->_count--;

	return (unsigned char)*stream->_ptr++;
}

void *prefetch_buf(void *arg) {
	FILE *stream = (FILE *) arg;
	struct _prefetch_t **_prefetch_addr = (struct _prefetch_t **) &stream->_prefetch_rel_addr;

	pthread_mutex_lock(&(_prefetch->mutex));
	//printf("prefetch_buf: fillbuf...\n");
	xpn_fillbuf_noret(stream);
	//printf("prefetch_buf: signal\n");
	pthread_cond_signal(&(_prefetch->cond_main));
	pthread_mutex_unlock(&(_prefetch->mutex));

	while (!io_testflag(stream, (_IOEOF | _IOERR ))) {
		//printf("prefetch_buf: lock\n");
		pthread_mutex_lock(&(_prefetch->mutex));
		while (stream->_count > 0) {
			//printf("prefetch_buf: wait stream->_count=%d\n", stream->_count);
			pthread_cond_wait(&(_prefetch->cond_th), &(_prefetch->mutex));
		}
		//printf("prefetch_buf: cont stream->_count=%d\n", stream->_count);

		xpn_fillbuf_noret(stream);
		//printf("prefetch_buf: fillbuf\n");

		pthread_cond_signal(&(_prefetch->cond_main));
		pthread_mutex_unlock(&(_prefetch->mutex));
		//printf("prefetch_buf: unlocked\n");
	}

	return NULL;
}

size_t
xpn_fread_prefetch(void *ptr, size_t size, size_t nmemb, register FILE *stream)
{
	register char *cp = (char *)ptr;
	register int c;
	size_t ndone = 0;
	register size_t s;
	struct _prefetch_t **_prefetch_addr = (struct _prefetch_t **) &stream->_chain->_prefetch_rel_addr;
	size_t res = (size_t) -1;

	XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)

	if ((stream->_chain != NULL) && (_prefetch != NULL) && (!_prefetch->do_prefetch)) { //TODO: Check that this condition really works well
		//printf("xpn_fread_prefetch: fillbuf...\n");
		xpn_fillbuf_noret(stream);
		_prefetch->do_prefetch = 1;
		pthread_create(&(_prefetch->thread), NULL, prefetch_buf, stream->_chain);
	}

	if (size)
		while ( ndone < nmemb ) {
			s = size;
			do {
				if ((c = xpn_getc_nofill(stream)) != EOF)
					*cp++ = (signed char) c;
				else {
					res = ndone;
					XPN_DEBUG_END_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)
					return ndone;
				}
			} while (--s);
			ndone++;
		}

	res = ndone;
	XPN_DEBUG_END_CUSTOM("%d, %zu, %zu", fileno(stream), size, nmemb)
	return ndone;
}
