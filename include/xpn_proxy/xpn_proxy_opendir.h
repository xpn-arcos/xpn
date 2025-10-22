
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Elias del Pozo Puñal
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


#ifndef _XPN_PROXY_OPENDIR_H_
#define _XPN_PROXY_OPENDIR_H_


  /* ... Include / Inclusion ........................................... */

     #include "config.h"


  /* ... Data structures / Estructuras de datos ........................ */

	struct __dirstream
	{
		int fd;                       // File descriptor.
		//__libc_lock_define (, lock) // Mutex lock for this structure. //TODO
		size_t allocation;            // Space allocated for the block.
		size_t size;                  // Total valid data in the block.
		size_t offset;                // Current offset into the block.
		off_t  filepos;               // Position of next entry to read.
		/* Directory block.  */
		char data[0] __attribute__ ((aligned (__alignof__ (void*))));

		char * path;
	};


   /* ... Const / Const ................................................. */

	#ifndef O_RDONLY
	#define O_RDONLY  00000000
	#endif

        // must be a directory
	#ifndef O_DIRECTORY
	#define O_DIRECTORY 00200000
	#endif


   /* ................................................................... */

#endif

