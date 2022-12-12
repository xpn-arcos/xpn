#ifndef _TCP_SERVER_CONF_H_
#define _TCP_SERVER_CONF_H_


/* ENVIROMENT VARIABLE: DNS SERVICE */
#ifndef TCP_SERVER_FILE
	#define TCP_SERVER_FILE "TCP_SERVER_FILE"
#endif

#ifndef TCP_SERVER_FILE_DEFAULT
	#define TCP_SERVER_FILE_DEFAULT "/etc/xpn/tcp_server.dns"
#endif

#ifndef TCP_SERVER_NAME_DEFAULT
	#define TCP_SERVER_NAME_DEFAULT	"node"
#endif

#ifndef TCP_SERVER_DIRBASE_DEFAULT
	#define TCP_SERVER_DIRBASE_DEFAULT	"/tmp"
#endif

#ifndef TCP_SERVER_PORT_DEFAULT
	#define TCP_SERVER_PORT_DEFAULT	9999
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
extern char *TCP_SERVER_ALIAS_NAME_STRING;
extern char *TCP_SERVER_FILE_STRING;
extern char *TCP_SERVER_DIRBASE_STRING;

#endif
