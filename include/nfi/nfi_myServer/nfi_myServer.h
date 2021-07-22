#ifndef _NFI_MYSERVER_H
#define _NFI_MYSERVER_H

#include "myServer.h"
#include "all_system.h"
#include "base/string_misc.h"
#include "nfi_myServer_err.h"
#include "nfi.h"


#ifdef  __cplusplus
    extern "C" {
#endif

#ifndef KB
	#define KB (1024)
#endif

#ifndef MB
	#define MB (KB*KB)
#endif	

struct nfi_myServer_server{
	char id[NFIMAXPATHLEN];
	char path[NFIMAXPATHLEN];
	int sd;
};

struct nfi_myServer_fhandle{
	char path[NFIMAXPATHLEN];
	int fd;
	DIR *dir;
};


int nfi_myServer_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);
int nfi_myServer_destroy(struct nfi_server *server);


int nfi_myServer_preload(struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt);
int nfi_myServer_flush(struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt);

int nfi_myServer_reconnect(struct nfi_server *server);
int nfi_myServer_disconnect(struct nfi_server *server);

int nfi_myServer_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_myServer_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_myServer_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_myServer_close(struct nfi_server *server, struct nfi_fhandle *fh);

ssize_t nfi_myServer_read(struct nfi_server *server, struct nfi_fhandle *fh, 
		void *buffer, 
		off_t offset, 
		size_t size
		);

ssize_t nfi_myServer_write(struct nfi_server *server, struct nfi_fhandle *fh, 
		void *buffer, 
		off_t offset, 
		size_t size
		);

int nfi_myServer_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);

int nfi_myServer_remove(struct nfi_server *server, char *url);

int nfi_myServer_rename(struct nfi_server *server, char *old_url, char *new_url);

int nfi_myServer_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

int nfi_myServer_rmdir(struct nfi_server *server, char *url);

int nfi_myServer_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_myServer_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type);

int nfi_myServer_closedir(struct nfi_server *server, struct nfi_fhandle *fh);

int nfi_myServer_statfs(struct nfi_server *server, struct nfi_info *inf);


 #ifdef  __cplusplus
     }
 #endif


#endif
