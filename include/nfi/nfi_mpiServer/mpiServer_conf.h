#ifndef _MPISERVER_CONF_H_
#define _MPISERVER_CONF_H_


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


#ifndef KB
	#define KB (1024)
#endif

#ifndef MB
	#define MB (KB*KB)
#endif

#ifndef GB
	#define GB (MB*MB)
#endif

#ifndef TRUE
	#define TRUE 1
#endif
#ifndef FALSE
	#define FALSE 0
#endif


/* MAXPATHLEN */
#ifndef MAXPATHLEN
	#define MAXPATHLEN 255
#endif

#ifndef MAX_BUFFER_SIZE
	#define MAX_BUFFER_SIZE (2*MB)
#endif

/* GLOBAL VARIABLES */
extern char *MPISERVER_ALIAS_NAME_STRING;
extern char *MPISERVER_FILE_STRING;
extern char *MPISERVER_DIRBASE_STRING;

#endif
