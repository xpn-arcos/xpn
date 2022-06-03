#include "xpn/xpn_simple/xpn_policy_open.h"



ssize_t XpnGetSizeThreads(struct xpn_partition *p){
	return p->size_threads; 
}

void XpnGetURLServer(struct nfi_server *serv, char *abs_path, char *url_serv)	
{
	char dir[MAX_PATH_LEN];
	char dir_aux[MAX_PATH_LEN];
	int abs_path_len;
	int put_slash;
	int pos_abs_path;
	int pos_dir;
	
	//strcpy(dir_aux,abs_path);
	strcpy(dir,abs_path);
	/*********************************************************
	if(strcmp(dir,"/") == 0){
		sprintf(url_serv,"%s",serv->url);
		return;
	}
	if(dir[0] == '/'){
	        sprintf(url_serv,"%s%s",serv->url,dir+1);
	        return;
	}
	********************************************************/
	abs_path_len = strlen(abs_path);
	put_slash = 1;
	pos_abs_path = 0;
	pos_dir = 0;
	while (pos_abs_path < abs_path_len){
		if (abs_path[pos_abs_path] == '/'){
			if (put_slash){
				dir_aux[pos_dir] = abs_path[pos_abs_path];
				pos_dir++;
				put_slash = 0;
			}
		} else {
			dir_aux[pos_dir] = abs_path[pos_abs_path];
			pos_dir++;
			put_slash = 1;
		}
		pos_abs_path++;
	}
	dir_aux[pos_dir] = '\0';
	strcpy(dir,dir_aux);
	//getNamePart(part, dir);
	sprintf(url_serv,"%s%s",serv->url, dir);
}


int XpnGetThreads(int op, int pd, int size){
	int i, thread = 0;
	
	/* the param op and abs_path are not used in this version */
	switch(op){
		case op_xpn_creat:
		case op_xpn_open:
		case op_xpn_close:
			thread = 1;// default value
			break;
		case op_xpn_read:
		case op_xpn_write:
	
			if (pd >= 0){
				i = 0;
				while((i<XPN_MAX_PART) && (xpn_parttable[i].id != pd))
					i++;
			}else{
				thread = 0;
				break;					
			}

			//printf("xpn_parttable[%d].size_threads: %d\n", i, xpn_parttable[i].size_threads);
			switch(xpn_parttable[i].size_threads){
				case -1:
					thread = 0;
					break;					
				case 0:
					thread = 1;
					break;
				default:
					if(xpn_parttable[i].size_threads <= size){
						thread = 1;
					}else{
						thread = 0;
					}
			}
			break;

		default:
			thread = 1;
			break;	
	}

	if(thread){
		XPN_DEBUG("Activate threads")
	}else{
		XPN_DEBUG("Deactivate threads")
	}

	return thread;
}

/**
 * Returns the data or metadata servers.
 * 
 * @param op Flag operation.
 * @param pd Partition id.
 * @param abs_path Absolute path.
 * @param fd File descriptor.
 * @param servers[out] The data or metadata servers to be obtained.
 * @param type Metadata or data server.
 * 
 * @return The number of data or metadata servers on success or -1 on error,
 */
int XpnGetServers(int op, int pd, __attribute__((__unused__)) char *abs_path, int fd, struct nfi_server ***servers, int type)
{
	struct nfi_server **serv;
	int i, j, n;
	
	/* params op and abs_path are not used in this version */
	switch(op){
		default:
			if (pd >= 0){
				i = 0;
				while((i<XPN_MAX_PART) && (xpn_parttable[i].id != pd))
					i++;
				
				if(i == XPN_MAX_PART){
					return -1;
				}
				
				switch(type){
					case  XPN_DATA_SERVER:
						n = xpn_parttable[i].data_nserv;
						break;
					case  XPN_META_SERVER:
						n = xpn_parttable[i].meta_nserv;
						break;
				}
				
				serv = (struct nfi_server **)malloc(sizeof(struct nfi_server *) * n);
				if(serv == NULL){
					/* xpn_err()*/
					return -1;
				}
				
				for(j=0;j<n;j++){
					switch(type){
						case  XPN_DATA_SERVER:
							serv[j] = &(xpn_parttable[i].data_serv[j]);
							break;
						case  XPN_META_SERVER:
							serv[j] = &(xpn_parttable[i].meta_serv[j]);
							break;
					}
				}
				(*servers) = serv;
			} else if (fd >=0){
				if((fd>XPN_MAX_FILE)||(xpn_file_table[fd] == NULL))
					return -1;
				
				switch(type){
					case  XPN_DATA_SERVER:
						n = xpn_file_table[fd]->data_vfh->n_nfih;
						break;
					case  XPN_META_SERVER:
						n = xpn_file_table[fd]->meta_vfh->n_nfih;
						break;
				}
				
				serv = (struct nfi_server **)malloc(sizeof(struct nfi_server*) * n);
				if(serv == NULL){
					/* xpn_err()*/
					return -1;
				}
				
				for(j=0;j<n;j++){
					switch(type){
						case  XPN_DATA_SERVER:
							serv[j] = &(xpn_file_table[fd]->part->data_serv[j]);
							break;
						case  XPN_META_SERVER:
							serv[j] = &(xpn_file_table[fd]->part->meta_serv[j]);
							break;
					}
				}
				(*servers) = serv;
			}else{
				return -1;
			}
			break;
	}
	return n;
}

int XpnGetMetadataPos(struct xpn_metadata *mdata, int pos){
	struct policy *p;
	
	if(mdata == NULL){
		return pos;
	}
	
	if (mdata->type_policy == -1)
		return -1;
	
	switch(mdata->type_policy){
		default:
			p = (struct policy *) mdata->policy;
			if (p == NULL){
				return pos;
			}
			if(pos == -1){
				return (p->first_node)%(mdata->data_nserv);
			}else{
				return (p->first_node+pos)%(mdata->data_nserv);
			}
			break;
	}
	
	return pos;
}

int XpnCreateMetadata(struct xpn_metadata *mdata, int pd, char *path)
{
	int i;
	struct policy *p;
	
	i = 0;
	while((i<XPN_MAX_PART) && (xpn_parttable[i].id != pd))
		i++;
	
	if(i == XPN_MAX_PART){
		return -1;
	}
	
	/* initial values */
	bzero(mdata, sizeof(struct xpn_metadata));
	mdata->meta_nserv	= xpn_parttable[i].meta_nserv;
	mdata->data_nserv	= xpn_parttable[i].data_nserv;
	mdata->id		= 0;
	mdata->version		= 1;
	mdata->type		= 0;
	mdata->block_size 	= xpn_parttable[i].block_size;
	mdata->type_policy	= xpn_parttable[i].type;
	
	if (mdata->type_policy == -1)
		return -1;
	
	switch(mdata->type_policy){
		default:
			p = (struct policy *)malloc(sizeof(struct policy));
			if (p == NULL){
				return -1;
			}
			
			p->first_node = hash(path, xpn_parttable[i].data_nserv);
			mdata->policy = (void *)p;
			
			mdata->sizem = sizeof(struct policy)
				+ sizeof(struct xpn_metadata)
				- sizeof(void *);
			break;
	}
	return 0;
}

int XpnUpdateMetadata( __attribute__((__unused__)) struct xpn_metadata *mdata,
		       __attribute__((__unused__)) int nserv,
		       __attribute__((__unused__)) struct nfi_server **servers,
		       __attribute__((__unused__)) struct xpn_fh *fh,
		       __attribute__((__unused__)) char *path)
{
	return 0;
}

int XpnGetFh(	struct xpn_metadata *mdata,
		struct nfi_fhandle **fh,
		struct nfi_server *servers,
		char *path)
{
	int res;
	char url_serv[MAX_PATH_LEN];
	struct nfi_fhandle *fh_aux;
	
	XPN_DEBUG_BEGIN
	
	if (mdata->type_policy == -1) {
		res = -1;
		XPN_DEBUG_END
		return res;
	}
	
	switch(mdata->type_policy){
		default:
			if((*fh) != NULL){
				res = 0;
				XPN_DEBUG_END
				return res;
			}
			
			fh_aux = (struct nfi_fhandle *) malloc(sizeof(struct nfi_fhandle));
			if(fh_aux == NULL){
				res = -1;
				XPN_DEBUG_END
				return res;
			}
			memset(fh_aux, 0, sizeof(struct nfi_fhandle));
			
			XpnGetURLServer(servers, path, url_serv);
			
			if(path[strlen(path)-1] == '/') {
				// Default Value
				nfi_worker_thread(servers->wrk, XpnGetThreads(op_xpn_opendir, 0, 0));
				
				res = servers->ops->nfi_opendir(servers,
							url_serv,
							fh_aux); // FIXME: When do we do nfi_closedir()?
			} else {
				// Default Value
				nfi_worker_thread(servers->wrk, XpnGetThreads(op_xpn_open, 0, 0));
				
				res = servers->ops->nfi_open(servers,
							url_serv,
							fh_aux);
			}
			
			if(res<0){
				free(fh_aux);
				res = -1;
				XPN_DEBUG_END
				return res;
			}
			(*fh) = fh_aux;
			break;
	}

	res = 0;
	XPN_DEBUG_END
	return res;
}


int XpnReadMetadata(	struct xpn_metadata *mdata,
			int nserv,
			struct nfi_server **servers,
			struct xpn_fh *fh,
			char *path,
			int pd)
{
	int res, n, i;
	
	XPN_DEBUG_BEGIN
	
	if (mdata->type_policy == -1)
		return -1;
	
	switch(mdata->type_policy){
		default:
			n = hash(path, nserv);
			res = XpnGetFh(mdata, &(fh->nfih[n]), servers[n], path);
			if(res < 0){
				res = -1;
				XPN_DEBUG_END
				return res;
			}
			
			XpnCreateMetadata(mdata, pd, path);
			
			if(fh->nfih[n]->type == NFIDIR){
				i = 0;
				while((i<XPN_MAX_PART) && (xpn_parttable[i].id != pd))
					i++;
				if(i == XPN_MAX_PART){
					res = -1;
					XPN_DEBUG_END
					return res;
				}
				mdata->type = XPN_DIR;
				res = XPN_DIR;
				XPN_DEBUG_END
				return res;
			}
			/********************************************
			//XpnGetFh(mdata, &(fh->nfih[n]), servers[n], path);
			ret = servers[n]->ops->nfi_read(servers[n],
					fh->nfih[n],
					buffer,
					0,
					XPN_SIZE_HEAD);
			if(ret <0){
				res = -1;
				XPN_DEBUG_END
				return res;
			}
			***********************************************/
			break;
	}
	
	res = XPN_FILE;
	XPN_DEBUG_END
	return res;
}

int XpnGetAtrib(int fd, struct stat *st) {
	int res,i,n;
	struct nfi_server **servers;
	struct nfi_attr attr;
	
	XPN_DEBUG_BEGIN_CUSTOM("%d", fd)
	
	servers = NULL;
	n = XpnGetServers(op_xpn_getattr,
			  xpn_file_table[fd]->part->id,
			  NULL,
			  fd,
			  &servers,
			  XPN_DATA_SERVER);
	if (n<=0) {
		/*free(servers);*/
		res = -1;
		XPN_DEBUG_END_CUSTOM("%d", fd)
		return res;
	}
	
	memset(&attr, 0, sizeof(struct nfi_attr));
	memset(st, 0, sizeof(struct stat));

	
	
	switch (xpn_file_table[fd]->mdata->type_policy) {
		case POLICY_RAID1:
			/* For RAID1, we need to take the file size of only one server */
			n=1;
			
			/* Then, proceed as in RAID0 case */
		case POLICY_RAID0:
			/* For RAID0, we need to add the sizes of the file in every server */
			
			for(i=0;i<n;i++){
				res = XpnGetFh(xpn_file_table[fd]->mdata,
						&(xpn_file_table[fd]->data_vfh->nfih[i]),
						servers[i],
						xpn_file_table[fd]->path);
				if (res<0){
					XPN_DEBUG_END_CUSTOM("%d", fd)
					return res;
				}

				xpn_file_table[fd]->data_vfh->nfih[i]->server->ops->nfi_getattr(
						xpn_file_table[fd]->data_vfh->nfih[i]->server,
						xpn_file_table[fd]->data_vfh->nfih[i],
						&attr);
				if (attr.at_size > 0)
					st->st_size += attr.at_size; /* total size, in bytes */
			}
			break;
		default:
			
			return -1;
	}

	free(servers);
	
	//st->st_dev     = 9;                 /* device */
	//st->st_ino     = 1;                 /* inode */
	//if (0 == attr.at_type){	/* It is a file */
	//	st->st_mode = S_IFREG | 0777;
	//}
	//if (1 == attr.at_type){	/* It is a directory */
	//	st->st_mode = S_IFDIR | 0777;
	//}
	st->st_mode    = attr.at_mode ;     /* protection */
	st->st_nlink   = attr.at_nlink;     /* number of hard links */
	//st->st_uid     = attr.at_uid;     /* user ID of owner */
	st->st_uid     = getuid() ;         /* user ID of owner */
	//st->st_gid     = attr.at_gid ;    /* group ID of owner */
	st->st_gid     = getgid() ;         /* group ID of owner */
	//st->st_rdev    = 0;                 /* device type (if inode device) */
	//st->st_blksize = attr.at_blksize ;  /* blocksize for filesystem I/O */
	st->st_blksize = xpn_file_table[fd]->block_size ;  /* blocksize for filesystem I/O */
	st->st_blocks  = attr.at_blocks ;   /* number of blocks allocated */
	st->st_atime   = attr.at_atime ;    /* time of last access */
	st->st_mtime   = attr.at_mtime ;    /* time of last modification */
	st->st_ctime   = attr.at_ctime ;    /* time of last change */
	
	res = 0;
	XPN_DEBUG_END_CUSTOM("%d", fd)
	return res;
}

/*int XpnPutAtrib(struct xpn_fh *fh, struct stat *st) {
	return 0;
}*/
