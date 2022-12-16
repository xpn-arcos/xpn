
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
 * fopen.c - open a stream
 */

#if	defined(_POSIX_SOURCE)
#include	<sys/types.h>
#endif
#include	<stdio.h>
#include	<stdlib.h>
#include	"xpn/xpn_simple/loc_incl.h"
#include	"xpn_debug.h"
#include	<sys/stat.h>
#include	<fcntl.h>

#include <string.h>

#include <xpn.h>

/* eslint-disable no-use-before-define */
/* eslint-disable no-unused-vars */
/* eslint-disable block-scoped-var */

FILE * xpn_fopen(const char *name, const char *mode)
{
	//register int i;
	int rwmode = 0, rwflags = 0;
	FILE *stream;
	struct stat st;
	int fd, flags = 0;
	int res = -1;

	XPN_DEBUG_BEGIN_CUSTOM("%s, %s", name, mode)

	/*for (i = 0; __iotab[i] != 0 ; i++) 
		if ( i >= FOPEN_MAX-1 )
			return (FILE *)NULL;*/

	switch(*mode++) {
	case 'r':
		flags |= _IOREAD | _IOREADING;	
		rwmode = O_RDONLY;
		break;
	case 'w':
		flags |= _IOWRITE | _IOWRITING;
		rwmode = O_WRONLY;
		rwflags = O_CREAT | O_TRUNC;
		break;
	case 'a': 
		flags |= _IOWRITE | _IOWRITING | _IOAPPEND;
		rwmode = O_WRONLY;
		rwflags |= O_APPEND | O_CREAT;
		break;         
	default:
		XPN_DEBUG_END_CUSTOM("%s, %s", name, mode)
		return (FILE *)NULL;
	}

	while (*mode) {
		switch(*mode++) {
		case 'b':
			continue;
		case '+':
			rwmode = O_RDWR;
			flags |= _IOREAD | _IOWRITE;
			continue;
		/* The sequence may be followed by additional characters */
		default:
			break;
		}
		break;
	}

	/* Perform a creat() when the file should be truncated or when
	 * the file is opened for writing and the open() failed.
	 */
	if ((rwflags & O_TRUNC)
	    || (((fd = _open(name, rwmode)) < 0)
		    && (rwflags & O_CREAT))) {
		if (((fd = _creat(name, PMODE)) > 0) && flags  | _IOREAD) {
			(void) _close(fd);
			fd = _open(name, rwmode);
		}
			
	}

	if (fd < 0) {
		XPN_DEBUG_END_CUSTOM("%s, %s", name, mode)
		return (FILE *)NULL;
	}

	if ( fstat( fd, &st ) < 0 ) {
		_close(fd);
		XPN_DEBUG_END_CUSTOM("%s, %s", name, mode)
		return (FILE *)NULL;
	}
	
	//if ( S_ISFIFO(st.st_mode) ) flags |= _IOFIFO;
	
	if (( stream = (FILE *) malloc(sizeof(FILE))) == NULL ) {
		_close(fd);
		XPN_DEBUG_END_CUSTOM("%s, %s", name, mode)
		return (FILE *)NULL;
	}

	if ((flags & (_IOREAD | _IOWRITE))  == (_IOREAD | _IOWRITE))
		flags &= ~(_IOREADING | _IOWRITING);

	stream->_count = 0;
	stream->_fd = fd;
	stream->_flags = flags;
	stream->_buf = (char *)NULL;
	stream->_ptr = (char *)NULL;
	//__iotab[i] = stream;

	/* For prefetching */
	if ( (stream->_chain = (FILE *) malloc(sizeof(FILE))) != NULL ) {
		struct _prefetch_t **_prefetch_addr = (struct _prefetch_t **) &stream->_chain->_prefetch_rel_addr;

		stream->_chain->_count = 0;
		stream->_chain->_fd    = stream->_fd;
		stream->_chain->_flags = stream->_flags;
		stream->_chain->_buf   = (char *)NULL;
		stream->_chain->_ptr   = (char *)NULL;

		if ( (_prefetch = (struct _prefetch_t *) malloc(sizeof(struct _prefetch_t))) != NULL ) {
			_prefetch->do_prefetch = 0;
			memset(&(_prefetch->thread), 0, sizeof(pthread_t));
			pthread_mutex_init(&(_prefetch->mutex), NULL);
			pthread_cond_init(&(_prefetch->cond_main), NULL);
			pthread_cond_init(&(_prefetch->cond_th), NULL);
		}
	}

	res = fileno(stream);
	XPN_DEBUG_END_CUSTOM("%s, %s", name, mode)
	return stream;
}

