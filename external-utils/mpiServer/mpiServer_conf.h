#ifndef _MPISERVER_CONF_H_
#define _MPISERVER_CONF_H_

#include "all_system.h"

/* Support for files of > 4GB */
#define _LARGEFILE64_ 1

/* MAXPATHLEN */
#ifndef MAXPATHLEN
	#define MAXPATHLEN 255
#endif

/* MAX_BUFFER_SIZE */
#ifndef MAX_BUFFER_SIZE
//	#define MAX_BUFFER_SIZE (64*KB)
//	#define MAX_BUFFER_SIZE (128*KB)
	#define MAX_BUFFER_SIZE (2*MB)
#endif

/* ENVIROMENT VARIABLE: DNS SERVICE */
#ifndef MPISERVER_FILE
	#define MPISERVER_FILE "MPISERVER_FILE"
#endif

#ifndef MPISERVER_FILE_DEFAULT
	#define MPISERVER_FILE_DEFAULT "/etc/xpn/mpiServer.dns"
#endif

#ifndef MPISERVER_NAME_DEFAULT
	#define MPISERVER_NAME_DEFAULT	"node"
#endif

#ifndef MPISERVER_DIRBASE_DEFAULT
	#define MPISERVER_DIRBASE_DEFAULT	"/tmp"
#endif

#ifndef MPISERVER_PORT_DEFAULT
	#define MPISERVER_PORT_DEFAULT	9999
#endif

#ifndef MPISERVER_IOSIZE_DEFAULT
	#define MPISERVER_IOSIZE_DEFAULT	(MB)
#endif

#ifndef MPISERVER_FLUSH_EXE
	#define MPISERVER_FLUSH_EXE "xpn2d_lock.exe" //params: virtual_path , storage_path, local_path
#endif

#ifndef MPISERVER_PRELOAD_EXE
	#define MPISERVER_PRELOAD_EXE "d2xpn_lock.exe" //params: virtual_path , storage_path, local_path
#endif

/* GLOBAL VARIABLES */
extern char *MPISERVER_ALIAS_NAME_STRING;
extern char *MPISERVER_FILE_STRING;
extern char *MPISERVER_DIRBASE_STRING;
extern int   MPISERVER_IOSIZE_INT;


#endif
