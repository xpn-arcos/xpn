#ifndef _MPI_SERVER_CONF_H_
#define _MPI_SERVER_CONF_H_

#include "all_system.h"

/* MAX_BUFFER_SIZE */
#ifndef MAX_BUFFER_SIZE
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

#ifndef MPI_SERVER_IOSIZE_DEFAULT
	#define MPI_SERVER_IOSIZE_DEFAULT	(MB)
#endif


#endif
