/** 
Only include the necessary data to realize the comunication between client
and server
*/
#ifndef _TCP_SERVER_CLIENT_H_
#define _TCP_SERVER_CLIENT_H_

  /* ... Include / Inclusion ........................................... */

    #include "all_system.h"
    #include "base/utils.h"
    #include "base/string_misc.h"
    #include "base/debug_msg.h"
    #include "base/workers.h"
    #include "tcp_server_conf.h"
    #include "tcp_server_comm.h"
    #include "nfi.h"
    #include "nfi_worker.h"


#define _LARGEFILE64_ 1



/* Operation */
#define TCP_SERVER_OPEN_FILE	0
#define TCP_SERVER_CREAT_FILE	1
#define TCP_SERVER_READ_FILE	2
#define TCP_SERVER_WRITE_FILE	3
#define TCP_SERVER_CLOSE_FILE	4
#define TCP_SERVER_RM_FILE	5
#define TCP_SERVER_GETATTR_FILE	6
#define TCP_SERVER_SETATTR_FILE	7

#define TCP_SERVER_MKDIR_DIR	8
#define TCP_SERVER_RMDIR_DIR	9
#define TCP_SERVER_OPENDIR_DIR	10
#define TCP_SERVER_READDIR_DIR	11
#define TCP_SERVER_CLOSEDIR_DIR	12

#define TCP_SERVER_FLUSH_FILE	13
#define TCP_SERVER_PRELOAD_FILE	14

#define TCP_SERVER_STATFS_DIR	15

#define TCP_SERVER_FINALIZE   	16
#define TCP_SERVER_GETID          17
#define TCP_SERVER_END 		-1


/* buffer size */
//#define BUFFER_SIZE (2*MB)

/* TCP_SERVER_ID */
#ifndef TCP_SERVER_ID
	#define TCP_SERVER_ID 32
#endif


struct st_tcp_server_open{
	char path[PATH_MAX];
};

struct st_tcp_server_creat{
	char path[PATH_MAX];
};

struct st_tcp_server_read{
	int fd;
#ifdef _LARGEFILE64_
	long long int offset;
#else	
	off_t offset;
#endif
	size_t size;
};

struct st_tcp_server_write{
	int fd;
#ifdef _LARGEFILE64_
	long long int offset;
#else
	off_t offset;
#endif
	size_t size;
};

struct st_tcp_server_read_req{
	ssize_t size;
	char last;
};

struct st_tcp_server_write_req{
	ssize_t size;
};

struct st_tcp_server_open_req{
	int fd;
};

struct st_tcp_server_close{
	int fd;
};

struct st_tcp_server_rm{
	char path[PATH_MAX];
};

struct st_tcp_server_mkdir{	
	char path[PATH_MAX];
};

struct st_tcp_server_rmdir{
	char path[PATH_MAX];
};


struct st_tcp_server_getattr{	
	char path[PATH_MAX];
};

struct st_tcp_server_setattr{	
	char path[PATH_MAX];
	struct stat attr;
};

struct st_tcp_server_attr_req{
	char status;	
	struct stat attr;
};


struct st_tcp_server_flush{
	char storage_path[PATH_MAX];
	char virtual_path[PATH_MAX];
	char opt;
};

struct st_tcp_server_preload{
	char storage_path[PATH_MAX];
	char virtual_path[PATH_MAX];
	char opt;
};

struct st_tcp_server_end{
	char status;
};

struct st_tcp_server_msg{
	char type;
	char id[TCP_SERVER_ID];
	union{
		struct st_tcp_server_flush 	op_flush;
		struct st_tcp_server_preload 	op_preload;

		struct st_tcp_server_open 	op_open;
		struct st_tcp_server_creat 	op_creat;
		struct st_tcp_server_read 	op_read;
		struct st_tcp_server_write 	op_write;
		struct st_tcp_server_close 	op_close;
		struct st_tcp_server_rm 		op_rm;		
		struct st_tcp_server_mkdir 	op_mkdir;
		struct st_tcp_server_rmdir 	op_rmdir;
		struct st_tcp_server_getattr 	op_getattr;
		struct st_tcp_server_setattr 	op_setattr;
		struct st_tcp_server_end 		op_end;
	}u_st_tcp_server_msg;
};



#endif
