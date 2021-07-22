#ifndef _NFI_IOPROXY_RPC_H
#define _NFI_IOPROXY_RPC_H


#include "all_system.h"
#include "base/string_misc.h"
#include "nfi_ioproxy-rpc_err.h"
#include "nfi.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


struct nfi_ioproxy-rpc_server{
	char path[NFIMAXPATHLEN];
};

struct nfi_ioproxy-rpc_fhandle{
	char path[NFIMAXPATHLEN];
	int fd;
	DIR *dir;
};



int nfi_ioproxy-rpc_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);
int nfi_ioproxy-rpc_destroy(struct nfi_server *server);



int nfi_ioproxy-rpc_reconnect(struct nfi_server *server);
int nfi_ioproxy-rpc_disconnect(struct nfi_server *server);

int nfi_ioproxy-rpc_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_ioproxy-rpc_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_ioproxy-rpc_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_ioproxy-rpc_close(struct nfi_server *server, struct nfi_fhandle *fh);

ssize_t nfi_ioproxy-rpc_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

ssize_t nfi_ioproxy-rpc_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

int nfi_ioproxy-rpc_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);

int nfi_ioproxy-rpc_remove(struct nfi_server *server, char *url);

int nfi_ioproxy-rpc_rename(struct nfi_server *server, char *old_url, char *new_url);

int nfi_ioproxy-rpc_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

int nfi_ioproxy-rpc_rmdir(struct nfi_server *server, char *url);

int nfi_ioproxy-rpc_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_ioproxy-rpc_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type);

int nfi_ioproxy-rpc_closedir(struct nfi_server *server, struct nfi_fhandle *fh);

int nfi_ioproxy-rpc_statfs(struct nfi_server *server, struct nfi_info *inf);


 #ifdef  __cplusplus
     }
 #endif


#endif
