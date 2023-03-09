#ifndef _TCP_SERVER_CONF_H_
#define _TCP_SERVER_CONF_H_

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

#ifndef MAX_BUFFER_SIZE
	#define MAX_BUFFER_SIZE (1*MB)
#endif

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

#ifndef TCP_SERVER_IOSIZE_DEFAULT
	#define TCP_SERVER_IOSIZE_DEFAULT	(MB)
#endif

#ifndef TCP_SERVER_FLUSH_EXE
	#define TCP_SERVER_FLUSH_EXE "xpn2d_lock.exe" //params: virtual_path , storage_path, local_path
#endif

#ifndef TCP_SERVER_PRELOAD_EXE
	#define TCP_SERVER_PRELOAD_EXE "d2xpn_lock.exe" //params: virtual_path , storage_path, local_path
#endif



/* GLOBAL VARIABLES */
extern char *TCP_SERVER_ALIAS_NAME_STRING;
extern char *TCP_SERVER_FILE_STRING;
extern char *TCP_SERVER_DIRBASE_STRING;
extern int   TCP_SERVER_IOSIZE_INT;

#endif
