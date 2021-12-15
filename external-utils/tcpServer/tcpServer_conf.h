#ifndef _TCPSERVER_CONF_H_
#define _TCPSERVER_CONF_H_

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
#ifndef TCPSERVER_FILE
	#define TCPSERVER_FILE "TCPSERVER_FILE"
#endif

#ifndef TCPSERVER_FILE_DEFAULT
	#define TCPSERVER_FILE_DEFAULT "/etc/xpn/tcpServer.dns"
#endif

#ifndef TCPSERVER_NAME_DEFAULT
	#define TCPSERVER_NAME_DEFAULT	"node"
#endif

#ifndef TCPSERVER_DIRBASE_DEFAULT
	#define TCPSERVER_DIRBASE_DEFAULT	"/tmp"
#endif

#ifndef TCPSERVER_PORT_DEFAULT
	#define TCPSERVER_PORT_DEFAULT	9999
#endif

#ifndef TCPSERVER_IOSIZE_DEFAULT
	#define TCPSERVER_IOSIZE_DEFAULT	(MB)
#endif

#ifndef TCPSERVER_FLUSH_EXE
	#define TCPSERVER_FLUSH_EXE "xpn2d_lock.exe" //params: virtual_path , storage_path, local_path
#endif

#ifndef TCPSERVER_PRELOAD_EXE
	#define TCPSERVER_PRELOAD_EXE "d2xpn_lock.exe" //params: virtual_path , storage_path, local_path
#endif



/* GLOBAL VARIABLES */
extern char *TCPSERVER_ALIAS_NAME_STRING;
extern char *TCPSERVER_FILE_STRING;
extern char *TCPSERVER_DIRBASE_STRING;
extern int   TCPSERVER_IOSIZE_INT;

#endif
