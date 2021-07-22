#ifndef _NFI_HTTP_H
#define _NFI_HTTP_H


#include "all_system.h"
#include "base/string_misc.h"
#include "nfi/nfi.h"
#include "nfi/nfi_http/nfi_http_err.h"

#include "fusedav.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


struct nfi_http_server{
	char path[NFIMAXPATHLEN];
	struct session_info *sess;
};

struct nfi_http_fhandle{
	char path[NFIMAXPATHLEN];
	int fd;
	DIR *dir;
	void *buf;
	size_t size;
	off_t offset;
};



int nfi_http_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);
int nfi_http_destroy(struct nfi_server *server);



int nfi_http_reconnect(struct nfi_server *server);
int nfi_http_disconnect(struct nfi_server *server);

int nfi_http_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_http_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_http_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_http_close(struct nfi_server *server, struct nfi_fhandle *fh);

ssize_t nfi_http_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

ssize_t nfi_http_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

int nfi_http_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);

int nfi_http_remove(struct nfi_server *server, char *url);

int nfi_http_rename(struct nfi_server *server, char *old_url, char *new_url);

int nfi_http_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

int nfi_http_rmdir(struct nfi_server *server, char *url);

int nfi_http_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_http_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type);

int nfi_http_closedir(struct nfi_server *server, struct nfi_fhandle *fh);

int nfi_http_statfs(struct nfi_server *server, struct nfi_info *inf);


 #ifdef  __cplusplus
     }
 #endif


#endif
