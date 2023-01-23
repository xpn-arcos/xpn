#ifndef _NFI_LOCAL_H
#define _NFI_LOCAL_H


     #include "all_system.h"
     #include "base/string_misc.h"
     #include "base/filesystem.h"
     #include "nfi/nfi.h"
     #include "nfi/nfi_local/nfi_local_err.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


struct nfi_local_server{
	char path[PATH_MAX];
};

struct nfi_local_fhandle{
	char path[PATH_MAX];
	int fd;
	DIR *dir;
};



int nfi_local_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);
int nfi_local_destroy(struct nfi_server *server);

int nfi_local_connect( struct nfi_server *serv, char *url, char* prt, char* server, char* dir ) ;
int nfi_local_disconnect(struct nfi_server *server);
int nfi_local_reconnect(struct nfi_server *server);

int nfi_local_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);
int nfi_local_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_local_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);
int nfi_local_close(struct nfi_server *server, struct nfi_fhandle *fh);

ssize_t nfi_local_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);
ssize_t nfi_local_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

int nfi_local_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);
int nfi_local_remove(struct nfi_server *server, char *url);
int nfi_local_rename(struct nfi_server *server, char *old_url, char *new_url);

int nfi_local_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);
int nfi_local_rmdir(struct nfi_server *server, char *url);

int nfi_local_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);
int nfi_local_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type);
int nfi_local_closedir(struct nfi_server *server, struct nfi_fhandle *fh);

int nfi_local_statfs(struct nfi_server *server, struct nfi_info *inf);


 #ifdef  __cplusplus
     }
 #endif


#endif

