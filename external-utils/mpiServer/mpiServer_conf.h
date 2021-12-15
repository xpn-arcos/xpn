#ifndef _MYSERVER_CONF_H_
#define _MYSERVER_CONF_H_

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
//	#define MAX_BUFFER_SIZE (64*KB)
//	#define MAX_BUFFER_SIZE (128*KB)
	#define MAX_BUFFER_SIZE (2*MB)
#endif

/* ENVIROMENT VARIABLE: DNS SERVICE */
#ifndef MYSERVER_FILE
	#define MYSERVER_FILE "MYSERVER_FILE"
#endif

#ifndef MYSERVER_FILE_DEFAULT
	#define MYSERVER_FILE_DEFAULT "/etc/xpn/mpiServer.dns"
#endif

#ifndef MYSERVER_NAME_DEFAULT
	#define MYSERVER_NAME_DEFAULT	"node"
#endif

#ifndef MYSERVER_DIRBASE_DEFAULT
	#define MYSERVER_DIRBASE_DEFAULT	"/tmp"
#endif

#ifndef MYSERVER_PORT_DEFAULT
	#define MYSERVER_PORT_DEFAULT	9999
#endif

#ifndef MYSERVER_IOSIZE_DEFAULT
	#define MYSERVER_IOSIZE_DEFAULT	(MB)
#endif

#ifndef MYSERVER_FLUSH_EXE
	#define MYSERVER_FLUSH_EXE "xpn2d_lock.exe" //params: virtual_path , storage_path, local_path
#endif

#ifndef MYSERVER_PRELOAD_EXE
	#define MYSERVER_PRELOAD_EXE "d2xpn_lock.exe" //params: virtual_path , storage_path, local_path
#endif



/* GLOBAL VARIABLES */
extern char *MYSERVER_ALIAS_NAME_STRING;
extern char *MYSERVER_FILE_STRING;
extern char *MYSERVER_DIRBASE_STRING;
extern int   MYSERVER_IOSIZE_INT;

#endif
