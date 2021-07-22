#ifndef _NFI_FTP_H
#define _NFI_FTP_H

#include "all_system.h"
#include "base/string_misc.h"
#include "nfi/nfi.h"
#include "nfi/nfi_ftp/ftp.h"
#include "nfi/nfi_ftp/nfi_ftp_err.h"



 #ifdef  __cplusplus
    extern "C" {
	     #endif

struct nfi_ftp_server{
	int socket;
	char url[1024];
	char user[128];
	char password[25];
	char exe_dir[MAXPATHLEN];
	int fd;
	char ruta_local[MAXPATHLEN];
	int port;
};



struct nfi_ftp_fhandle{
	char path[MAXPATHLEN];
	char file[MAXPATHLEN];
	struct list_dir *lista_dir ;
};


//int nfi_ftp_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);
int nfi_ftp_init(char *url, struct nfi_server *serv, struct nfi_fhandle *fh);

int nfi_ftp_destroy(struct nfi_server *serv,struct nfi_fhandle *fh);

int nfi_ftp_reconnect(struct nfi_server *serv);
int nfi_ftp_disconnect(struct nfi_server *serv);

int nfi_ftp_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_ftp_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_ftp_open(struct nfi_server *server, char *url, struct nfi_fhandle *fh);

int nfi_ftp_close(struct nfi_server *server, struct nfi_fhandle *fh);

ssize_t nfi_ftp_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

ssize_t nfi_ftp_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

int nfi_ftp_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle *fh);

int nfi_ftp_remove(struct nfi_server *serv, char *url);

int nfi_ftp_rename(struct nfi_server *serv, char *old_url, char *new_url);

int nfi_ftp_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

int nfi_ftp_rmdir(struct nfi_server *serv, char *url);

int nfi_ftp_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fh);

int nfi_ftp_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry, unsigned char *type);

int nfi_ftp_closedir(struct nfi_server *server, struct nfi_fhandle *fh);

int nfi_ftp_statfs(struct nfi_server *serv, struct nfi_info *inf);

 #ifdef  __cplusplus
     }
 #endif

#endif
