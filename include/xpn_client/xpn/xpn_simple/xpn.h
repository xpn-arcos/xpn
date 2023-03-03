#ifndef _XPN_H
#define _XPN_H

#include "nfi/nfi.h"
#include "nfi/nfi_worker.h"
#include "nfi/nfi_ops.h"
#include "nfi/nfi_lib.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif

#define XPN_CONF "XPN_CONF"

/* Paralel struct partition */
struct xpn_partition{
	int id;				/* id of partition */
	int type;			/* type of file :NORMAL, RAID5,... */
	char name[PATH_MAX];	/* name of partition */
	ssize_t block_size;		/* size of distribution used */
	ssize_t size_threads;
	
	int initialized; /* whether it is already initialized or not */

	int data_nserv;			/* number of server */
	struct nfi_server *data_serv;	/* list of data servers in the partition */
	
	int meta_nserv;			/* number of metadata_server */
	struct nfi_server *meta_serv;	/* list of metadata servers in the partition */
};


/* metadata of xpn file */
struct xpn_metadata{
	int sizem;
	int id;
	int version;		/* number of version */
	int type;		/* type of file: file or dir */
	ssize_t block_size;	/* size of distribution used */
	ssize_t size_threads;
	int data_nserv;		/* number of servers */
	int meta_nserv;		/* number of metadata servers */
	struct nfi_attr attr;
	int type_policy;	/* RAID0, RAID1 */
	void *policy;
};


enum xpn_policy{
	POLICY_RAID0,
	POLICY_RAID1
};


struct policy{
	int first_node;
};


 #ifdef  __cplusplus
     }
 #endif

#endif

