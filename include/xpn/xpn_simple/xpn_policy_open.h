#ifndef _XPN_POLICY_OPEN_H
#define _XPN_POLICY_OPEN_H

#include "xpn_file.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif

#define XPN_META_SIZE	(4*KB)

enum xpn_work{
	no_xpn_op	= -1,
	op_xpn_getattr	=  0,
	op_xpn_setattr	=  1,
	op_xpn_open	=  2,
	op_xpn_creat	=  3,
	op_xpn_close	=  4,
	op_xpn_remove	=  5,
	op_xpn_rename	=  6,
	op_xpn_read	=  7,
	op_xpn_write	=  8,
	op_xpn_mkdir	=  9,
	op_xpn_rmdir	= 10,
	op_xpn_opendir	= 11,
	op_xpn_readdir	= 12,
	op_xpn_closedir	= 13,
	op_xpn_statfs	= 14,
	op_xpn_preload	= 15,
	op_xpn_flush	= 16,
};

/*
#define no_xpn_op            -1
#define op_xpn_getattr        0
#define	op_xpn_setattr        1
#define	op_xpn_open           2
#define	op_xpn_creat          3
#define	op_xpn_close          4
#define	op_xpn_remove         5
#define	op_xpn_rename         6
#define	op_xpn_read           7
#define	op_xpn_write          8
#define	op_xpn_mkdir          9
#define	op_xpn_rmdir         10
#define	op_xpn_opendir       11
#define	op_xpn_readdir       12
#define	op_xpn_closedir      13
#define	op_xpn_statfs        14
*/

ssize_t XpnGetSizeThreads(struct xpn_partition *p);

void XpnGetURLServer( struct nfi_server *serv, char *abs_path, char *url_serv);

int XpnGetServers(int op, int pd, char *abs_path, int fd, struct nfi_server ***servers, int type);
int XpnGetThreads(int op, int pd, int size);

int XpnGetMetadataPos(struct xpn_metadata *mdata, int pos);
int XpnCreateMetadata(struct xpn_metadata *mdata, int pd, char *path);

int XpnReadMetadata(	struct xpn_metadata *mdata,
			int nserv,
			struct nfi_server **servers ,
			struct xpn_fh *fh,
			char *path,
			int pd);

int XpnUpdateMetadata(	struct xpn_metadata *mdata,
			int nserv,
			struct nfi_server **servers ,
			struct xpn_fh *fh,
			char *path);

int XpnGetFh(	struct xpn_metadata *mdata,
		struct nfi_fhandle **fh, 
		struct nfi_server *servers, 
		char *path);



int XpnGetAtrib(int fd, struct stat *st);


 #ifdef  __cplusplus
     }
 #endif


#endif

