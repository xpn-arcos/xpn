#ifndef _NFI_WORKER_H_
#define _NFI_WORKER_H_


#include "all_system.h"
#include "nfi.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif

#define NFIWRKMAXPATHLEN 255
#define XPN_THREADS 0

enum nfi_work{
	noop		= -1,
	op_getattr 	=  0,
	op_setattr 	=  1,
	op_open 	=  2,
	op_create 	=  3,
	op_close 	=  4,
	op_remove 	=  5,
	op_rename 	=  6,
	op_read		=  7,
	op_write	=  8,
	op_mkdir	=  9,
	op_rmdir	= 10,
	op_opendir	= 11,
	op_readdir	= 12,
	op_closedir	= 13,
	op_preload	= 14,
	op_flush	= 15,
	op_statfs	= 16,
};

struct nfi_worker_io{
	off_t offset;
	size_t size;
	void *buffer;
};

struct nfi_worker_args{
	int operation;
	ssize_t result;

	char url[NFIWRKMAXPATHLEN];
	char virtual_path[NFIWRKMAXPATHLEN];
	char storage_path[NFIWRKMAXPATHLEN];
	char newurl[NFIWRKMAXPATHLEN];
	struct nfi_fhandle *fh;
	struct nfi_attr *attr;
	int opt;

	struct nfi_info *inf;

	int n_io;
	struct nfi_worker_io *io;
	char *entry;
	unsigned char *type;
};

struct nfi_worker{
	int thread;
#ifdef POSIX_THREADS
	pthread_t pth;
	pthread_mutex_t mt;
	pthread_cond_t cnd;
#endif
	int ready;
	struct nfi_server *server;
	struct nfi_worker_args arg; // TODO: Convert this into a list of 'struct nfi_worker_args' to make Expand reentrant
};

int nfi_worker_init(struct nfi_worker *wrk, struct nfi_server *serv, int thread);

int nfi_worker_thread(struct nfi_worker *wrk, int flag);

int nfi_worker_do_getattr(struct nfi_worker *wrk, 
			  struct nfi_fhandle *fh, 
			  struct nfi_attr *attr);

int nfi_worker_do_setattr(struct nfi_worker *wrk, 
			  struct nfi_fhandle *fh, 
			  struct nfi_attr *attr);

int nfi_worker_do_preload(struct nfi_worker *wrk, 
			 char *url, 
			 char *virtual_path,
			 char *storage_path,
			 int opt);

int nfi_worker_do_flush(struct nfi_worker *wrk, 
			char *url, 
			char *virtual_path,
			char *storage_path,
			int opt);

int nfi_worker_do_open(struct nfi_worker *wrk, 
			char *url, 
			struct nfi_fhandle *fho);

int nfi_worker_do_create(struct nfi_worker *wrk, 
			char *url,  
			struct nfi_attr *attr, 
			struct nfi_fhandle  *fh);

int nfi_worker_do_close(struct nfi_worker *wrk, 
			struct nfi_fhandle *fh);

int nfi_worker_do_remove(struct nfi_worker *wrk, 
			 char *url);

int nfi_worker_do_rename(struct nfi_worker *wrk, 
			 char *old_url, 
			 char *new_url);

int nfi_worker_do_read( struct nfi_worker *wrk, 
			struct nfi_fhandle *fh, 
			struct nfi_worker_io *io,
			int n);

int nfi_worker_do_write(struct nfi_worker *wrk, 
			struct nfi_fhandle *fh, 
			struct nfi_worker_io *io,
			int n);

int nfi_worker_do_mkdir(struct nfi_worker *wrk, 
			char *url, 
			struct nfi_attr *attr, 
			struct nfi_fhandle *fh);

int nfi_worker_do_rmdir(struct nfi_worker *wrk, 
			char *url);

int nfi_worker_do_opendir(struct nfi_worker *wrk, 
			  char *url, 
			  struct nfi_fhandle *fho);

int nfi_worker_do_readdir(struct nfi_worker *wrk, 
			  struct nfi_fhandle *fhd, 
			  char *entry,
			  unsigned char *type);

int nfi_worker_do_closedir(struct nfi_worker *wrk, 
			   struct nfi_fhandle *fh);

int nfi_worker_do_statfs(struct nfi_worker *wrk, 
			 struct nfi_info *inf);


ssize_t nfi_worker_wait(struct nfi_worker *wrk);

int nfi_worker_end(struct nfi_worker *wrk);

int nfi_worker_destroy();


 #ifdef  __cplusplus
    }
 #endif


#endif

