/** 
Only include the necessary data to realize the comunication between client
and server
*/
#ifndef _MYSERVER_CLIENT_H_
#define _MYSERVER_CLIENT_H_


#include "all_system.h"
#include "myServer_conf.h"
#include "myServer_comm.h"

#define _LARGEFILE64_ 1



/* Operation */
#define MYSERVER_OPEN_FILE	0
#define MYSERVER_CREAT_FILE	1
#define MYSERVER_READ_FILE	2
#define MYSERVER_WRITE_FILE	3
#define MYSERVER_CLOSE_FILE	4
#define MYSERVER_RM_FILE	5
#define MYSERVER_GETATTR_FILE	6
#define MYSERVER_SETATTR_FILE	7

#define MYSERVER_MKDIR_DIR	8
#define MYSERVER_RMDIR_DIR	9
#define MYSERVER_OPENDIR_DIR	10
#define MYSERVER_READDIR_DIR	11
#define MYSERVER_CLOSEDIR_DIR	12

#define MYSERVER_FLUSH_FILE	13
#define MYSERVER_PRELOAD_FILE	14

#define MYSERVER_STATFS_DIR	15

#define MYSERVER_FINALIZE   	16
#define MYSERVER_GETID          17
#define MYSERVER_END 		-1



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

/* MYSERVER_MAXPATHLEN */
#ifndef MYSERVER_MAXPATHLEN
	//#define MYSERVER_MAXPATHLEN 255
	#define MYSERVER_MAXPATHLEN 128
#endif

/* MYSERVER_MAXPATHLEN */
#ifndef MYSERVER_ID
	#define MYSERVER_ID 32
#endif


struct st_myServer_open{
	char path[MYSERVER_MAXPATHLEN];
};

struct st_myServer_creat{
	char path[MYSERVER_MAXPATHLEN];
};

struct st_myServer_read{
	int fd;
#ifdef _LARGEFILE64_
	long long int offset;
#else	
	off_t offset;
#endif
	size_t size;
};

struct st_myServer_write{
	int fd;
#ifdef _LARGEFILE64_
	long long int offset;
#else
	off_t offset;
#endif
	size_t size;
};

struct st_myServer_read_req{
	ssize_t size;
	char last;
};

struct st_myServer_write_req{
	ssize_t size;
};

struct st_myServer_open_req{
	int fd;
};

struct st_myServer_close{
	int fd;
};

struct st_myServer_rm{
	char path[MYSERVER_MAXPATHLEN];
};

struct st_myServer_mkdir{	
	char path[MYSERVER_MAXPATHLEN];
};

struct st_myServer_rmdir{
	char path[MYSERVER_MAXPATHLEN];
};


struct st_myServer_getattr{	
	char path[MYSERVER_MAXPATHLEN];
};

struct st_myServer_setattr{	
	char path[MYSERVER_MAXPATHLEN];
	struct stat attr;
};

struct st_myServer_attr_req{
	char status;	
	struct stat attr;
};


struct st_myServer_flush{
	char storage_path[MYSERVER_MAXPATHLEN];
	char virtual_path[MYSERVER_MAXPATHLEN];
	char opt;
};

struct st_myServer_preload{
	char storage_path[MYSERVER_MAXPATHLEN];
	char virtual_path[MYSERVER_MAXPATHLEN];
	char opt;
};

struct st_myServer_end{
	char status;
};

struct st_myServer_msg{
	char type;
	char id[MYSERVER_ID];
	union{
		struct st_myServer_flush 	op_flush;
		struct st_myServer_preload 	op_preload;

		struct st_myServer_open 	op_open;
		struct st_myServer_creat 	op_creat;
		struct st_myServer_read 	op_read;
		struct st_myServer_write 	op_write;
		struct st_myServer_close 	op_close;
		struct st_myServer_rm 		op_rm;		
		struct st_myServer_mkdir 	op_mkdir;
		struct st_myServer_rmdir 	op_rmdir;
		struct st_myServer_getattr 	op_getattr;
		struct st_myServer_setattr 	op_setattr;
		struct st_myServer_end 		op_end;
	}u_st_myServer_msg;
};



#endif
