#ifndef _NFI_NFS_H
#define _NFI_NFS_H

#include "all_system.h"
#include "base/string_misc.h"
#include "nfi/nfi.h"
#include "nfi/nfi_nfs/nfs.h"
#include "nfi/nfi_nfs/nfi_nfs_err.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


struct nfi_nfs_server{
	CLIENT *cl;
	fhandle fh;
};

struct nfi_nfs_fhandle{
	int eofdir;
	nfscookie cookie;
	fhandle fh;
};


void NFItoNFSattr(fattr *nfs_att,struct nfi_attr *nfi_att);
void NFStoNFIattr(struct nfi_attr *nfi_att, fattr *nfs_att);
void NFStoNFIInfo(struct nfi_info *nfi_inf, struct nfs_info *nfs_inf);

int nfi_nfs_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);
int nfi_nfs_destroy(struct nfi_server *server);

int nfi_nfs_reconnect(struct nfi_server *server);
int nfi_nfs_disconnect(struct nfi_server *server);

int nfi_nfs_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);
int nfi_nfs_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_nfs_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);
int nfi_nfs_close(struct nfi_server *server, struct nfi_fhandle *fh);
ssize_t nfi_nfs_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);
ssize_t nfi_nfs_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

int nfi_nfs_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);
int nfi_nfs_remove(struct nfi_server *server, char *url);
int nfi_nfs_rename(struct nfi_server *server, char *old_url, char *new_url);
int nfi_nfs_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);
int nfi_nfs_rmdir(struct nfi_server *server, char *url);
int nfi_nfs_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);
int nfi_nfs_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type);
int nfi_nfs_closedir(struct nfi_server *server, struct nfi_fhandle *fh);
int nfi_nfs_statfs(struct nfi_server *server, struct nfi_info *inf);

 #ifdef  __cplusplus
     }
 #endif

#endif

