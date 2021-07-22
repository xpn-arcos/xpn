#ifndef _NFI_XIO_H
#define _NFI_XIO_H

#include "all_system.h"
#include "base/string_misc.h"
#include "nfi_xio_err.h"
#include "nfi.h"

#include <globus/globus_xio.h>
#include <globus/globus_xio_gridftp_driver.h>
#include <globus/globus_ftp_client.h>


 #ifdef  __cplusplus
    extern "C" {
 #endif

struct nfi_xio_server{
	char path[NFIMAXPATHLEN];
	globus_xio_driver_t             driver;
	globus_xio_stack_t              stack;
};

struct nfi_xio_fhandle{
	char path[NFIMAXPATHLEN];
	char contact_string[NFIMAXPATHLEN];
	globus_xio_handle_t             handle;
	globus_xio_attr_t               attr;
	globus_mutex_t			mutex;
	globus_cond_t			cond;
	globus_bool_t			done;
	globus_ftp_client_handle_t      handle_g;
	globus_ftp_client_handleattr_t  attr_g;
	globus_off_t                    offset;
	globus_result_t                 fd;
};


int nfi_xio_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);
int nfi_xio_destroy(struct nfi_server *server);

int nfi_xio_reconnect(struct nfi_server *server);
int nfi_xio_disconnect(struct nfi_server *server);

int nfi_xio_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_xio_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_xio_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_xio_close(struct nfi_server *server, struct nfi_fhandle *fh);

ssize_t nfi_xio_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

ssize_t nfi_xio_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

int nfi_xio_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);

int nfi_xio_remove(struct nfi_server *server, char *url);

int nfi_xio_rename(struct nfi_server *server, char *old_url, char *new_url);

int nfi_xio_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

int nfi_xio_rmdir(struct nfi_server *server, char *url);

int nfi_xio_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_xio_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type);

int nfi_xio_closedir(struct nfi_server *server, struct nfi_fhandle *fh);

int nfi_xio_statfs(struct nfi_server *server, struct nfi_info *inf);


 #ifdef  __cplusplus
     }
 #endif


#endif
