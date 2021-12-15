/** 
Only include the necessary data to realize the comunication between client
and server
*/
#ifndef _TCPSERVER_CLIENT_H_
#define _TCPSERVER_CLIENT_H_


#include "all_system.h"
#include "tcpServer_conf.h"
#include "tcpServer_comm.h"

#define _LARGEFILE64_ 1



/* Operation */
#define TCPSERVER_OPEN_FILE	0
#define TCPSERVER_CREAT_FILE	1
#define TCPSERVER_READ_FILE	2
#define TCPSERVER_WRITE_FILE	3
#define TCPSERVER_CLOSE_FILE	4
#define TCPSERVER_RM_FILE	5
#define TCPSERVER_GETATTR_FILE	6
#define TCPSERVER_SETATTR_FILE	7

#define TCPSERVER_MKDIR_DIR	8
#define TCPSERVER_RMDIR_DIR	9
#define TCPSERVER_OPENDIR_DIR	10
#define TCPSERVER_READDIR_DIR	11
#define TCPSERVER_CLOSEDIR_DIR	12

#define TCPSERVER_FLUSH_FILE	13
#define TCPSERVER_PRELOAD_FILE	14

#define TCPSERVER_STATFS_DIR	15

#define TCPSERVER_FINALIZE   	16
#define TCPSERVER_GETID          17
#define TCPSERVER_END 		-1



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


/* buffer size */
//#define BUFFER_SIZE (2*MB)

/* TCPSERVER_MAXPATHLEN */
#ifndef TCPSERVER_MAXPATHLEN
	//#define TCPSERVER_MAXPATHLEN 255
	#define TCPSERVER_MAXPATHLEN 128
#endif

/* TCPSERVER_MAXPATHLEN */
#ifndef TCPSERVER_ID
	#define TCPSERVER_ID 32
#endif


struct st_tcpServer_open{
	char path[TCPSERVER_MAXPATHLEN];
};

struct st_tcpServer_creat{
	char path[TCPSERVER_MAXPATHLEN];
};

struct st_tcpServer_read{
	int fd;
#ifdef _LARGEFILE64_
	long long int offset;
#else	
	off_t offset;
#endif
	size_t size;
};

struct st_tcpServer_write{
	int fd;
#ifdef _LARGEFILE64_
	long long int offset;
#else
	off_t offset;
#endif
	size_t size;
};

struct st_tcpServer_read_req{
	ssize_t size;
	char last;
};

struct st_tcpServer_write_req{
	ssize_t size;
};

struct st_tcpServer_open_req{
	int fd;
};

struct st_tcpServer_close{
	int fd;
};

struct st_tcpServer_rm{
	char path[TCPSERVER_MAXPATHLEN];
};

struct st_tcpServer_mkdir{	
	char path[TCPSERVER_MAXPATHLEN];
};

struct st_tcpServer_rmdir{
	char path[TCPSERVER_MAXPATHLEN];
};


struct st_tcpServer_getattr{	
	char path[TCPSERVER_MAXPATHLEN];
};

struct st_tcpServer_setattr{	
	char path[TCPSERVER_MAXPATHLEN];
	struct stat attr;
};

struct st_tcpServer_attr_req{
	char status;	
	struct stat attr;
};


struct st_tcpServer_flush{
	char storage_path[TCPSERVER_MAXPATHLEN];
	char virtual_path[TCPSERVER_MAXPATHLEN];
	char opt;
};

struct st_tcpServer_preload{
	char storage_path[TCPSERVER_MAXPATHLEN];
	char virtual_path[TCPSERVER_MAXPATHLEN];
	char opt;
};

struct st_tcpServer_end{
	char status;
};

struct st_tcpServer_msg{
	char type;
	char id[TCPSERVER_ID];
	union{
		struct st_tcpServer_flush 	op_flush;
		struct st_tcpServer_preload 	op_preload;

		struct st_tcpServer_open 	op_open;
		struct st_tcpServer_creat 	op_creat;
		struct st_tcpServer_read 	op_read;
		struct st_tcpServer_write 	op_write;
		struct st_tcpServer_close 	op_close;
		struct st_tcpServer_rm 		op_rm;		
		struct st_tcpServer_mkdir 	op_mkdir;
		struct st_tcpServer_rmdir 	op_rmdir;
		struct st_tcpServer_getattr 	op_getattr;
		struct st_tcpServer_setattr 	op_setattr;
		struct st_tcpServer_end 		op_end;
	}u_st_tcpServer_msg;
};



#endif
