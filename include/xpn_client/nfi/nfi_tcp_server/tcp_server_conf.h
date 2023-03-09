#ifndef _TCP_SERVER_CONF_H_
#define _TCP_SERVER_CONF_H_

#include "all_system.h"

/* Support for files of > 4GB */
#define _LARGEFILE64_ 1

/* MAX_BUFFER_SIZE */
#ifndef MAX_BUFFER_SIZE
	#define MAX_BUFFER_SIZE (1*MB)
#endif

/* ENVIROMENT VARIABLE: DNS SERVICE */
#ifndef TCP_SERVER_NAME_DEFAULT
	#define TCP_SERVER_NAME_DEFAULT	"node"
#endif

#ifndef TCP_SERVER_DIRBASE_DEFAULT
	#define TCP_SERVER_DIRBASE_DEFAULT	"/tmp"
#endif

#ifndef TCP_SERVER_PORT_DEFAULT
	#define TCP_SERVER_PORT_DEFAULT	9999
#endif

#ifndef TCP_SERVER_IOSIZE_DEFAULT
	#define TCP_SERVER_IOSIZE_DEFAULT	(MB)
#endif


#endif
