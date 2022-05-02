#ifndef _TCPSERVER_CONF_H_
#define _TCPSERVER_CONF_H_


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
	#define MAX_BUFFER_SIZE (1*MB)
#endif

/* GLOBAL VARIABLES */
extern char *TCPSERVER_ALIAS_NAME_STRING;
extern char *TCPSERVER_FILE_STRING;
extern char *TCPSERVER_DIRBASE_STRING;

#endif
