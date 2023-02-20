#ifndef _NFI_TCP_SERVER_H
#define _NFI_TCP_SERVER_H

#include "tcp_server.h"
#include "all_system.h"
#include "base/string_misc.h"
#include "nfi_tcp_server_err.h"
#include "nfi.h"

#include "mosquitto.h"


#ifdef  __cplusplus
    extern "C" {
#endif

struct nfi_tcp_server_server{
	char id[PATH_MAX];
	char path[PATH_MAX];
	int sd;
};

struct nfi_tcp_server_fhandle{
	char path[PATH_MAX];
	int fd;
	DIR *dir;
};


int nfi_tcp_server_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);
int nfi_tcp_server_destroy(struct nfi_server *server);


int nfi_tcp_server_preload(struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt);
int nfi_tcp_server_flush(struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt);

int nfi_tcp_server_reconnect(struct nfi_server *server);
int nfi_tcp_server_disconnect(struct nfi_server *server);

int nfi_tcp_server_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);
int nfi_tcp_server_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_tcp_server_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);
int nfi_tcp_server_close(struct nfi_server *server, struct nfi_fhandle *fh);
ssize_t nfi_tcp_server_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);
ssize_t nfi_tcp_server_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

int nfi_tcp_server_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);
int nfi_tcp_server_remove(struct nfi_server *server, char *url);
int nfi_tcp_server_rename(struct nfi_server *server, char *old_url, char *new_url);
int nfi_tcp_server_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);
int nfi_tcp_server_rmdir(struct nfi_server *server, char *url);
int nfi_tcp_server_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);
int nfi_tcp_server_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type);
int nfi_tcp_server_closedir(struct nfi_server *server, struct nfi_fhandle *fh);
int nfi_tcp_server_statfs(struct nfi_server *server, struct nfi_info *inf);


 #ifdef  __cplusplus
     }
 #endif


#endif
