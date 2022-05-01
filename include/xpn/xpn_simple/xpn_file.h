#ifndef _XPN_FILE_H
#define _XPN_FILE_H

#include "xpn.h"
#include "xpn_err.h"
#include "xpn_policy_init.h"


 #ifdef  __cplusplus
    extern "C" {
 #endif


/* max number of file descriptors */
#define XPN_MAX_FILE 	128
#define XPN_MAX_PATH	255

/* FILE or DIR */
#define XPN_FILE 	0
#define XPN_DIR 	1

struct xpn_fh{
	int n_nfih;
        struct nfi_fhandle **nfih; 	/* NFI handler              	*/
};



struct xpn_attr{
	int 	at_type; 	/* FILE or DIR			*/	
	mode_t 	at_mode;	/* protection			*/
	nlink_t at_nlink;	/* number of hard links		*/
	uid_t	at_uid;		/* user ID of owner		*/
	gid_t	at_gid;		/* group ID of owner		*/
	off_t	at_size;	/* total size, in bytes		*/
	u_long  at_blksize;	/* blocksize for filesystem I/O */
	u_long  at_blocks;  	/* number of blocks allocated 	*/
	time_t  at_atime;   	/* time of last access 		*/
	time_t  at_mtime;   	/* time of last modification 	*/
	time_t  at_ctime;   	/* time of last status change	*/
	void	*private_info;
					     
};


/* File table */
struct xpn_filedesc{
        int id;         /* id of file                                   */
	char path[MAX_PATH_LEN];/* absolute path 			*/
        int type;       /* indicate FILE or DIR                         */
        int links;      /* number of links that this file has           */
	mode_t mode;	/* O_RDONLY, O_WRONLY,....			*/
        struct xpn_partition *part;     /* partition                    */
 	struct xpn_metadata *mdata; /* metadata				*/
        struct xpn_attr attr;   /* attributes of the open file         	*/
        off_t offset;       /* offset of the open file              	*/
        ssize_t block_size;         /* size of distribution used            */
        ssize_t size_threads;
        struct xpn_fh *data_vfh; /* virtual FH                         	*/
        struct xpn_fh *meta_vfh; /* virtual METADATA FH                         	*/
};



/* global  */
extern struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];

int xpn_init_file_table();
int xpn_destroy_file_table();


#ifdef  __cplusplus
     }
 #endif


#endif

