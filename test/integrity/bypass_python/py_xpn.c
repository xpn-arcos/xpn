
  /*
   *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


int py_open ( char *name, char *flags )
{
	int ret ;
	int flags_c = 0 ;

	if (strchr(flags, 'a') != NULL)
	    flags_c = flags_c | O_APPEND ;
	if (strchr(flags, 'w') != NULL)
	    flags_c = flags_c | O_CREAT | O_WRONLY ;
	if (strchr(flags, 'r') != NULL)
	    flags_c = flags_c | O_RDONLY ;
	if (strchr(flags, '+') != NULL)
	    flags_c = flags_c | O_RDWR ;

	ret = open(name, flags_c, S_IRWXU) ;
	printf("%d = xpn_open(name:'%s', flags:%d, mode:%d)\n", ret, name, flags_c, S_IRWXU) ;

	return ret ;
}

int py_write ( int fd, char *buffer, int buffer_size )
{
	int ret ;

	ret = write(fd, buffer, buffer_size) ;
	printf("%d = xpn_write(fd:%d, buffer:'%.10s...', buffer_size:%d)\n", ret, fd, buffer, buffer_size) ;

	return ret ;
}


char * py_read ( int fd )
{
	int     ret = -1;
        struct  stat st;
        char   *buffer = NULL;

        ret = fstat(fd, &st);
	if (ret >= 0) {
	    buffer = malloc(st.st_size) ;
	}

	if (buffer != NULL) {
	    ret = read(fd, buffer, st.st_size) ;
	    printf("%d = xpn_read(fd:%d, buffer:'%.10s...', buffer_size:%d)\n", ret, fd, buffer, 1024) ;
	}

	return buffer ;
}

int py_close ( int fd )
{
	int ret ;

	ret = close(fd) ;
	printf("xpn_close(fd: %d)\n", fd) ;

	return ret ;
}

