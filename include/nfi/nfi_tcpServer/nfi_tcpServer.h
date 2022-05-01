#ifndef _NFI_TCPSERVER_H
#define _NFI_TCPSERVER_H

#include "tcpServer.h"
#include "all_system.h"
#include "base/string_misc.h"
#include "nfi_tcpServer_err.h"
#include "nfi.h"


#ifdef  __cplusplus
    extern "C" {
#endif

struct nfi_tcpServer_server{
	char id[NFIMAXPATHLEN];
	char path[NFIMAXPATHLEN];
	int sd;
};

struct nfi_tcpServer_fhandle{
	char path[NFIMAXPATHLEN];
	int fd;
	DIR *dir;
};


int nfi_tcpServer_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);
int nfi_tcpServer_destroy(struct nfi_server *server);


int nfi_tcpServer_preload(struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt);
int nfi_tcpServer_flush(struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt);

int nfi_tcpServer_reconnect(struct nfi_server *server);
int nfi_tcpServer_disconnect(struct nfi_server *server);

int nfi_tcpServer_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);
int nfi_tcpServer_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_tcpServer_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);
int nfi_tcpServer_close(struct nfi_server *server, struct nfi_fhandle *fh);
ssize_t nfi_tcpServer_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);
ssize_t nfi_tcpServer_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

int nfi_tcpServer_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);
int nfi_tcpServer_remove(struct nfi_server *server, char *url);
int nfi_tcpServer_rename(struct nfi_server *server, char *old_url, char *new_url);
int nfi_tcpServer_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);
int nfi_tcpServer_rmdir(struct nfi_server *server, char *url);
int nfi_tcpServer_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);
int nfi_tcpServer_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type);
int nfi_tcpServer_closedir(struct nfi_server *server, struct nfi_fhandle *fh);
int nfi_tcpServer_statfs(struct nfi_server *server, struct nfi_info *inf);


 #ifdef  __cplusplus
     }
 #endif


#endif
