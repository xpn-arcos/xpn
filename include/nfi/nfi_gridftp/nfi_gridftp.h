#ifndef _NFI_GRIDFTP_H
#define _NFI_GRIDFTP_H

#include "all_system.h"
#include "base/string_misc.h"
#include "nfi_gridftp_err.h"
#include "nfi.h"

#include <globus_ftp_client.h>


 #ifdef  __cplusplus
    extern "C" {
 #endif

struct nfi_gridftp_server{
	char path[NFIMAXPATHLEN];
};

struct nfi_gridftp_fhandle{
	char path[NFIMAXPATHLEN];
	globus_ftp_client_handle_t      handle;
	globus_ftp_client_handleattr_t  attr;
	globus_ftp_client_operationattr_t opattr;
	globus_mutex_t			gmutex;
	globus_cond_t			gcond;
	globus_bool_t			done;
	globus_off_t                    offset;
	globus_size_t			rbytes;
};


int nfi_gridftp_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);
int nfi_gridftp_destroy(struct nfi_server *server);

int nfi_gridftp_reconnect(struct nfi_server *server);
int nfi_gridftp_disconnect(struct nfi_server *server);

int nfi_gridftp_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_gridftp_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_gridftp_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_gridftp_close(struct nfi_server *server, struct nfi_fhandle *fh);

ssize_t nfi_gridftp_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

ssize_t nfi_gridftp_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

int nfi_gridftp_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);

int nfi_gridftp_remove(struct nfi_server *server, char *url);

int nfi_gridftp_rename(struct nfi_server *server, char *old_url, char *new_url);

int nfi_gridftp_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

int nfi_gridftp_rmdir(struct nfi_server *server, char *url);

int nfi_gridftp_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_gridftp_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type);

int nfi_gridftp_closedir(struct nfi_server *server, struct nfi_fhandle *fh);

int nfi_gridftp_statfs(struct nfi_server *server, struct nfi_info *inf);


 #ifdef  __cplusplus
     }
 #endif


#endif
