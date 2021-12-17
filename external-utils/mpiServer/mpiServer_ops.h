#ifndef _MPISERVER_OPS_H_
#define _MPISERVER_OPS_H_

#include "mpiServer.h"
#include "mpiServer_params.h"


/*
 *  Constants
 */

#ifndef MPISERVER_MAXPATHLEN
	#define MPISERVER_MAXPATHLEN 128
#endif
#ifndef MPISERVER_ID
	#define MPISERVER_ID 32
#endif


/*
 *  Message struct
 */

struct st_mpiServer_open{
	char path[MPISERVER_MAXPATHLEN];
};

struct st_mpiServer_creat{
	char path[MPISERVER_MAXPATHLEN];
};

struct st_mpiServer_read{
	int fd;
#ifdef _LARGEFILE64_
	long long int offset;
#else	
	off_t offset;
#endif
	size_t size;
};

struct st_mpiServer_write{
	int fd;
#ifdef _LARGEFILE64_
	long long int offset;
#else
	off_t offset;
#endif
	size_t size;
};

struct st_mpiServer_read_req{
	ssize_t size;
	char last;
};

struct st_mpiServer_write_req{
	ssize_t size;
};

struct st_mpiServer_open_req{
	int fd;
};

struct st_mpiServer_close{
	int fd;
};

struct st_mpiServer_rm{
	char path[MPISERVER_MAXPATHLEN];
};

struct st_mpiServer_mkdir{	
	char path[MPISERVER_MAXPATHLEN];
};

struct st_mpiServer_rmdir{
	char path[MPISERVER_MAXPATHLEN];
};


struct st_mpiServer_getattr{	
	char path[MPISERVER_MAXPATHLEN];
};

struct st_mpiServer_setattr{	
	char path[MPISERVER_MAXPATHLEN];
	struct stat attr;
};

struct st_mpiServer_attr_req{
	char status;	
	struct stat attr;
};


struct st_mpiServer_flush{
	char storage_path[MPISERVER_MAXPATHLEN];
	char virtual_path[MPISERVER_MAXPATHLEN];
	char opt;
};

struct st_mpiServer_preload{
	char storage_path[MPISERVER_MAXPATHLEN];
	char virtual_path[MPISERVER_MAXPATHLEN];
	char opt;
};

struct st_mpiServer_end{
	char status;
};

struct st_mpiServer_msg{
	char type;
	char id[MPISERVER_ID];
	union{
		struct st_mpiServer_flush 	op_flush;
		struct st_mpiServer_preload 	op_preload;

		struct st_mpiServer_open 	op_open;
		struct st_mpiServer_creat 	op_creat;
		struct st_mpiServer_read 	op_read;
		struct st_mpiServer_write 	op_write;
		struct st_mpiServer_close 	op_close;
		struct st_mpiServer_rm 		op_rm;		
		struct st_mpiServer_mkdir 	op_mkdir;
		struct st_mpiServer_rmdir 	op_rmdir;
		struct st_mpiServer_getattr 	op_getattr;
		struct st_mpiServer_setattr 	op_setattr;
		struct st_mpiServer_end 		op_end;
	}u_st_mpiServer_msg;
};


/*
 *  API
 */

int  mpiServer_read_operation ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_open        ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_creat       ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_close       ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_rm          ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_read        ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_write       ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_rmdir       ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_mkdir       ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_fstat       ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_setattr     ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_getattr     ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_flush       ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_preload     ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;
void mpiServer_op_getid       ( mpiServer_param_st *params, int sd, struct st_mpiServer_msg *head ) ;

#endif

