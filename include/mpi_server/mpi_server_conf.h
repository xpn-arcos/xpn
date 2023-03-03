
  /*
   *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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


#ifndef _MPI_SERVER_CONF_H_
#define _MPI_SERVER_CONF_H_

    #include "all_system.h"

/* Support for files of > 4GB */
#define _LARGEFILE64_ 1

/* MAX_BUFFER_SIZE */
#ifndef MAX_BUFFER_SIZE
//	#define MAX_BUFFER_SIZE (64*KB)
	#define MAX_BUFFER_SIZE (1*MB)
#endif

/* ENVIROMENT VARIABLE: DNS SERVICE */
#ifndef MPI_SERVER_FILE
	#define MPI_SERVER_FILE "MPI_SERVER_FILE"
#endif

#ifndef MPI_SERVER_FILE_DEFAULT
	#define MPI_SERVER_FILE_DEFAULT "/etc/xpn/mpi_server.dns"
#endif

#ifndef MPI_SERVER_NAME_DEFAULT
	#define MPI_SERVER_NAME_DEFAULT	"node"
#endif

#ifndef MPI_SERVER_DIRBASE_DEFAULT
	#define MPI_SERVER_DIRBASE_DEFAULT	"/tmp"
#endif

#ifndef MPI_SERVER_PORT_DEFAULT
	#define MPI_SERVER_PORT_DEFAULT	9999
#endif


#endif
