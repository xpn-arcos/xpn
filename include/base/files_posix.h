
/*
 *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

#ifndef __FILES_POSIX_H__
#define __FILES_POSIX_H__

    // Includes
    #include "base_lib.h"

    #include <sys/stat.h>
    #include <sys/mman.h>


    // API
    int  file_posix_init     ( void ) ;
    int  file_posix_finalize ( void ) ;

    int  file_posix_open  ( long *fd, const char *path_name, int flags ) ;
    int  file_posix_close ( int fd ) ;
    int  file_posix_read  ( int fd, void *buffer, int buffer_size ) ;
    int  file_posix_write ( int fd, void *buffer, int buffer_size ) ;

#endif

