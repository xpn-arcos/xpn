#ifndef _NFI_GSOAP_H
#define _NFI_GSOAP_H

#include "all_system.h"
#include "base/string_misc.h"
#include "nfi/nfi.h"
#include "nfi/nfi_gsoap/nfi_gsoap_err.h"

 #ifdef  __cplusplus
    extern "C" {
 #endif

#include "src/nfi/nfi_gsoap/PosixIOService/libIOServiceClient.h"

struct nfi_gsoap_server{
	char path[NFIMAXPATHLEN];
	int port;
	struct ioservice *ios;
};

struct nfi_gsoap_fhandle{
	char path[NFIMAXPATHLEN];
	int fd;
	DIR *dir;
};


int nfi_gsoap_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);
int nfi_gsoap_destroy(struct nfi_server *server);

int nfi_gsoap_reconnect(struct nfi_server *server);
int nfi_gsoap_disconnect(struct nfi_server *server);

int nfi_gsoap_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_gsoap_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_gsoap_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_gsoap_close(struct nfi_server *server, struct nfi_fhandle *fh);

ssize_t nfi_gsoap_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

ssize_t nfi_gsoap_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

int nfi_gsoap_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);

int nfi_gsoap_remove(struct nfi_server *server, char *url);

int nfi_gsoap_rename(struct nfi_server *server, char *old_url, char *new_url);

int nfi_gsoap_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

int nfi_gsoap_rmdir(struct nfi_server *server, char *url);

int nfi_gsoap_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_gsoap_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type);

int nfi_gsoap_closedir(struct nfi_server *server, struct nfi_fhandle *fh);

int nfi_gsoap_statfs(struct nfi_server *server, struct nfi_info *inf);


 #ifdef  __cplusplus
     }
 #endif


#endif
