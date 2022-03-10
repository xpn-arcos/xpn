#include "xpn/xpn_simple/xpn_open.h"
#include "xpn/xpn_simple/xpn_opendir.h"

#define MASK 0000777

extern struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];

int ino_counter = 0;


/*****************************************************************/
void XpnShowFileTable() {
	int i=0;

	while ((i<XPN_MAX_FILE) && (xpn_file_table[i] != NULL) && (xpn_file_table[i]->path != NULL)) {
		XPN_DEBUG("%d %s", i, xpn_file_table[i]->path)
		i++;
	}
}

int XpnSearchSlotFile(int pd, char *path, struct xpn_fh *vfh, struct xpn_metadata *mdata, int mode)
{
	int i, res;

	XPN_DEBUG_BEGIN_ARGS1(path)
	
	i=0;
	while((i<XPN_MAX_FILE-1)&&(xpn_file_table[i] != NULL)) // FIXME? Por que i<XPN_MAX_FILE-1, no deberia ser i<XPN_MAX_FILE
		i++;
	
	if(i == XPN_MAX_FILE){
		/* xpn_err() ? */
		return -1;
	}
	
	xpn_file_table[i]= (struct xpn_filedesc *)malloc(sizeof(struct xpn_filedesc));
	
	if(xpn_file_table[i] == NULL){
		/* xpn_err() ? */
		return -1;
	}
	
	xpn_file_table[i]->id 		= i;
	xpn_file_table[i]->type 	= mdata->type;
	strcpy(xpn_file_table[i]->path, path);
	xpn_file_table[i]->mode 	= mode;
	xpn_file_table[i]->links 	= 1;
	xpn_file_table[i]->part 	= XpnSearchPart(pd);
	xpn_file_table[i]->offset 	= 0;
	xpn_file_table[i]->block_size 	= xpn_file_table[i]->part->block_size;
	xpn_file_table[i]->mdata 	= mdata;
	xpn_file_table[i]->data_vfh 	= vfh;
	xpn_file_table[i]->size_threads	= XpnGetSizeThreads(xpn_file_table[i]->part);
	
	res = i;
	XPN_DEBUG_END_ARGS1(path)

	return res;
}

int XpnSearchFile(const char *path)
{
	int res, i=0;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	while (i<XPN_MAX_FILE) {
		if ( (xpn_file_table[i] != NULL) &&
		     (xpn_file_table[i]->path != NULL) &&
		     (strcmp(xpn_file_table[i]->path, path) == 0) )
			break;
		i++;
	}
	
	if(i == XPN_MAX_FILE){
		res = -1;
	} else {
		res = i;
	}
	
	XPN_DEBUG_END
	
	return res;
}
/*****************************************************************/

int xpn_internal_creat(const char *path, mode_t perm, struct xpn_fh **vfh, struct xpn_metadata **mdata)
{
	char abs_path[MAX_PATH_LEN], url_serv[MAX_PATH_LEN];
	struct nfi_server **servers;
	struct xpn_metadata *mdata_aux;
	struct nfi_attr attr;
	int res, err, i, j, n, pd;
	struct xpn_fh *vfh_aux;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	if(path == NULL){
		xpn_err(XPNERR_PARAM);
		res = -1;
		XPN_DEBUG_END
		return res;
	}
	
	res = XpnGetAbsolutePath(path, abs_path); /* this function generates the absolute path */
	if(res<0){
		xpn_err(XPNERR_PATH_NOEXIST);
		res = -1;
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	pd = XpnGetPartition(abs_path); /* return partition's id and remove partition name from abs_path */
	if(pd<0){
		xpn_err(XPNERR_PART_NOEXIST);
		res = -1;
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	/* params:
	 * flag operation, partition id, absolute path, file descriptor, pointer to server
	 */
	servers = NULL;
	n = XpnGetServers(op_xpn_creat, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
	if(n<=0){
		/*free(servers);*/
		res = -1;
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}

	bzero(&attr, sizeof(struct nfi_attr));
	attr.at_mode = perm;
	
	vfh_aux = (struct xpn_fh *)malloc(sizeof(struct xpn_fh));
	
	/* vfh construction */
	if(vfh_aux == NULL){
		xpn_err(XPNERR_NOMEMORY);
		free(servers);
		res = -1;
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	vfh_aux->n_nfih = n;
	vfh_aux->nfih = (struct nfi_fhandle **)malloc(sizeof(struct nfi_fhandle*) * n);
	if(vfh_aux->nfih == NULL){
		xpn_err(XPNERR_NOMEMORY);
		free(servers);
		res = -1;
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	for(i=0;i<n;i++){
		vfh_aux->nfih[i] = NULL;
	}
	
	mdata_aux = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
	if(mdata_aux == NULL){
		free(servers);
		/* also indirect pointers */
		for(i=0;i<n;i++){
			if(vfh_aux->nfih[i] != NULL){
				if(vfh_aux->nfih[i]->priv_fh != NULL){
					vfh_aux->nfih[i]->server->ops->nfi_close(
						vfh_aux->nfih[i]->server,
						vfh_aux->nfih[i]);
				}
				free(vfh_aux->nfih[i]);
			}
		}
		free(vfh_aux->nfih);
		free(vfh_aux);
		free(mdata_aux);
		res = -1;
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	XpnCreateMetadata(mdata_aux, pd, abs_path);
	
	for(j=0;j<n;j++){
		i = XpnGetMetadataPos(mdata_aux, j);
		
		XpnGetURLServer(servers[i], abs_path, url_serv);
		vfh_aux->nfih[i] = (struct nfi_fhandle*)malloc(sizeof(struct nfi_fhandle));
		bzero(vfh_aux->nfih[i], sizeof(struct nfi_fhandle));
		if(vfh_aux->nfih[i] == NULL){
			free(servers);
			res = -1;
			XPN_DEBUG_END_ARGS1(path)
			return res;
		}
		
		// Default Value
		nfi_worker_thread(servers[i]->wrk, XpnGetThreads(op_xpn_creat,pd, 0));
		/* worker */
		nfi_worker_do_create(servers[i]->wrk,
					url_serv,
					&attr,
					vfh_aux->nfih[i]);
		
	}
	
	/* wait */
	err = 0;
	for(j=0;j<n;j++){	
		i = XpnGetMetadataPos(mdata_aux, j); 
		res = nfi_worker_wait(servers[i]->wrk);
		/* error checking */
		if((res<0)&&(!err)){
			err = 1;
			/* erase the file create before de server number i */
		/*
			for(j=0; j<i; j++){
				XpnGetURLServer(servers[j], abs_path, url_serv);
				nfi_worker_do_remove(servers[j]->wrk, url_serv);
				nfi_worker_wait(servers[j]->wrk);
			}
		}else{
			if((res>=0)&&(err)){
				XpnGetURLServer(servers[i], abs_path, url_serv);
				nfi_worker_do_remove(servers[i]->wrk, url_serv); 
				nfi_worker_wait(servers[i]->wrk);
			}
		*/
		}
	}
	
	/* error checking */
	if(err){
		xpn_err(XPNERR_CREATE);
		free(servers);
		for(i=0;i<n;i++){
			if(vfh_aux->nfih[i] != NULL){
				if(vfh_aux->nfih[i]->priv_fh != NULL){
					vfh_aux->nfih[i]->server->ops->nfi_close(
						vfh_aux->nfih[i]->server,
						vfh_aux->nfih[i]);
				}
				free(vfh_aux->nfih[i]);
			}
		}
		free(vfh_aux->nfih);
		res = -1;
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	res = XpnUpdateMetadata(mdata_aux, n, servers, vfh_aux, abs_path);
	if(res<0){
		free(servers);
		/* also indirect pointers */
		for(i=0;i<n;i++){
			if(vfh_aux->nfih[i] != NULL){
				if(vfh_aux->nfih[i]->priv_fh != NULL){
					vfh_aux->nfih[i]->server->ops->nfi_close(
						vfh_aux->nfih[i]->server,
						vfh_aux->nfih[i]);
				}
				free(vfh_aux->nfih[i]);
			}
		}
		free(vfh_aux->nfih);
		free(vfh_aux);
		free(mdata_aux);
		res = -1;
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	(*vfh )= vfh_aux;
	(*mdata )= mdata_aux;
	free(servers);
	
	xpn_err(XPNERR_PARAM);
	res = 0;
	XPN_DEBUG_END_ARGS1(path)
	return res;
}

int xpn_internal_open(const char *path, struct xpn_fh *vfh, struct xpn_metadata *mdata, int mode)
{
	char abs_path[MAX_PATH_LEN];
	
	struct nfi_server **servers;
	int n, pd, i;
	int res = -1;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	res = XpnGetAbsolutePath(path, abs_path); /* this function generates the absolute path */
	if(res<0){
		xpn_err(XPNERR_PATH_NOEXIST);
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	pd = XpnGetPartition(abs_path); /* returns partition id and remove partition name from abs_path */
	if(pd<0){
		xpn_err(XPNERR_PART_NOEXIST);
		XPN_DEBUG_END_ARGS1(path)
		return pd;
	}

	res = XpnSearchFile(abs_path);
	if (res < 0) { // FIXME: CAUTION, this caused the call to fail some changes before, although now it seems to work.
		/* params:
		 * flag operation, partition id, absolute path, file descriptor, pointer to server
		 */
		servers = NULL;
		n = XpnGetServers(op_xpn_open, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
		if(n<=0){
			/*free(servers);*/
			XPN_DEBUG_END_ARGS1(path)
			return res;
		}
		
		if(mdata == NULL){
			mdata = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
			if(mdata == NULL){
				free(servers);
				XPN_DEBUG_END_ARGS1(path)
				return res;
			}
			
			if(vfh == NULL){
				vfh = (struct xpn_fh *)malloc(sizeof(struct xpn_fh));
				if(vfh == NULL){
					free(servers);
					XPN_DEBUG_END_ARGS1(path)
					return res;
				}

				vfh->n_nfih = n;
				vfh->nfih = (struct nfi_fhandle **)malloc(sizeof(struct nfi_fhandle *)*n);
				for(i=0;i<n;i++){
					vfh->nfih[i] = NULL;
				}
			}

			/* read the metadata */
			memset(mdata, 0, sizeof(struct xpn_metadata));
			res = XpnReadMetadata(mdata, n, servers, vfh, abs_path, pd);
			if(res<0){
				free(servers);
				free(vfh->nfih);
				free(vfh);
				free(mdata);
				XPN_DEBUG_END_ARGS1(path)
				return res;
			}
		}
		
		free(servers);

		res = XpnSearchSlotFile(pd, abs_path, vfh, mdata, mode);
	}
	
	XPN_DEBUG_END_ARGS1(path)
	return res;
}

int xpn_internal_resize(__attribute__((__unused__)) char *path, __attribute__((__unused__)) struct xpn_fh **vfh, __attribute__((__unused__)) int size)
{
	return 0;
}

int xpn_internal_fresize(__attribute__((__unused__)) int fd, __attribute__((__unused__)) int size)
{
	return 0;
}

int xpn_internal_remove(const char *path)
{
	char abs_path[MAX_PATH_LEN], url_serv[MAX_PATH_LEN]; 
	int res, err, i, n, pd;
	struct nfi_server **servers;
	
	if(path == NULL){
		xpn_err(XPNERR_PARAM);
		return -1;
	}
	
	res = XpnGetAbsolutePath(path, abs_path); /* esta funcion genera el path absoluto */
	if(res<0){
		xpn_err(XPNERR_PATH_NOEXIST);
		return -1;
	}
	
	pd = XpnGetPartition(abs_path); /* return partition's id */
	if(pd<0){
		xpn_err(XPNERR_PART_NOEXIST);
		return -1;
	}
	
	/* params:
	 * flag operation, partition id, absolute path, file descriptor, pointer to server
	 */
	servers = NULL;
	n = XpnGetServers(op_xpn_remove, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
	if(n<=0){
		/* free(servers); */
		return -1;
	}
	
	for(i=0;i<n;i++){
		XpnGetURLServer(servers[i], abs_path, url_serv);
		// Default Value
		nfi_worker_thread(servers[i]->wrk, XpnGetThreads(op_xpn_remove,pd, 0));
		
		/* worker */
		nfi_worker_do_remove(servers[i]->wrk, url_serv);
	}
	
	/* wait */
	err = 0;
	for(i=0;i<n;i++){
		res = nfi_worker_wait(servers[i]->wrk);
		/* tratamiento de errores */
		if((res<0)&&(!err)){
			err = 1;
		}
	}
	
	/* tratamiento de errores */
	if(err){
		xpn_err(XPNERR_REMOVE);
		free(servers);
		return -1;
	}
	
	free(servers);
	xpn_err(XPN_OK);
	return 0;
}






















/*****************************************************************/
int xpn_preload(const char *virtual_path, const char *storage_path)
{
	char abs_path[MAX_PATH_LEN], url_serv[MAX_PATH_LEN];
	struct nfi_server **servers;
	struct xpn_metadata *mdata;
	int res, i, n, pd;
	
	XPN_DEBUG_BEGIN_ARGS2(virtual_path, storage_path)
	
	if(virtual_path == NULL){
		xpn_err(XPNERR_PARAM);
		res = -1;
		XPN_DEBUG_END
		return res;
	}
	
	if(storage_path == NULL){
		xpn_err(XPNERR_PARAM);
		res = -1;
		XPN_DEBUG_END
		return res;
	}
	
	res = XpnGetAbsolutePath(virtual_path, abs_path); /* esta funcion genera el path absoluto */
	if(res<0){
		xpn_err(XPNERR_PATH_NOEXIST);
		res = -1;
		XPN_DEBUG_END
		return res;
	}
	
	pd = XpnGetPartition(abs_path); /* return partition's id */
	if(pd<0){
		xpn_err(XPNERR_PART_NOEXIST);
		res = -1;
		XPN_DEBUG_END
		return res;
	}
	/* params:
	 * flag operation, partition id, absolute path, file descriptor, pointer to server
	 */
	servers = NULL;
	n = XpnGetServers(op_xpn_flush, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
	if(n<=0){
		/*free(servers);*/
		res = -1;
		XPN_DEBUG_END
		return res;
	}
	
	mdata = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
	if(mdata == NULL){
		free(servers);
		free(mdata);
		/* */
		res = -1;
		XPN_DEBUG_END
		return res;
	}
	
	XpnCreateMetadata(mdata, pd, abs_path);
	
	i = XpnGetMetadataPos(mdata, -1);
	
	XpnGetURLServer(servers[i], abs_path, url_serv);
		
	// Default Value
	nfi_worker_thread(servers[i]->wrk, XpnGetThreads(op_xpn_preload, pd, 0));
	//nfi_worker_thread(servers[i]->wrk, 0);
	/* worker */
	nfi_worker_do_preload(	servers[i]->wrk,
				url_serv,
				(char *)url_serv,
				(char *)storage_path,
				1);
	
	/* wait */
	res = nfi_worker_wait(servers[i]->wrk);
	free(servers);
	free(mdata);
	
	/* tratamiento de errores */
	if(res){
		xpn_err(XPNERR_PARAM);
		res = -1;
		XPN_DEBUG_END
		return res;
	}
	
	res = 0;
	XPN_DEBUG_END
	return res;
}




























int xpn_flush(const char *virtual_path, const char *storage_path)
{
	char abs_path[MAX_PATH_LEN], url_serv[MAX_PATH_LEN];
	struct nfi_server **servers;
	struct xpn_metadata *mdata;
	int res,/* err,*/ i,  n, pd;
	
	
	if(virtual_path == NULL){
		xpn_err(XPNERR_PARAM);
		return -1;
	}
	
	if(storage_path == NULL){
		xpn_err(XPNERR_PARAM);
		return -1;
	}
	
	res = XpnGetAbsolutePath(virtual_path, abs_path); /* esta funcion genera el path absoluto */
	if(res<0){
		xpn_err(XPNERR_PATH_NOEXIST);
		return -1;
	}
	
	pd = XpnGetPartition(abs_path); /* return partition's id */
	if(pd<0){
		xpn_err(XPNERR_PART_NOEXIST);
		return -1;
	}
	
	/* params:
	 * flag operation, partition id, absolute path, file descriptor, pointer to server
	 */
	servers = NULL;
	n = XpnGetServers(op_xpn_flush, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
	if(n<=0){
		/*free(servers);*/
		return -1;
	}
	
	mdata = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
	if(mdata == NULL){
		free(servers);
		free(mdata);
		/* */
		return -1;
	}
	
	XpnCreateMetadata(mdata, pd, abs_path);
	
	i = XpnGetMetadataPos(mdata, -1);
		
	XpnGetURLServer(servers[i], abs_path, url_serv);
		
	// Default Value
	nfi_worker_thread(servers[i]->wrk, XpnGetThreads(op_xpn_flush, pd, 0));
	/* worker */
	nfi_worker_do_flush(	servers[i]->wrk,
				url_serv,
				(char *)virtual_path, //TODO: xpn///quijote_preload.txt --> /server0/quijote_preload.txt
				(char *)storage_path,
				1);
		
	/* wait */
	res = nfi_worker_wait(servers[i]->wrk);
	
	free(servers);
	free(mdata);
	
	/* tratamiento de errores */
	/*if(err){
		xpn_err(XPNERR_PARAM);
		return -1;
	}*/
	
	return 0;
}

int xpn_creat(const char *path, mode_t perm)
{
	struct xpn_fh *vfh;
	struct xpn_metadata *mdata;
	int res;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	if((path == NULL)||(strlen(path)>MAX_PATH_LEN)){
		res = -1;
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	vfh = NULL;
	mdata = NULL;
	/*
	res = xpn_internal_resize(path, &vfh, 0);
	if(res<0){
		res = xpn_internal_creat(path, perm, &vfh, &mdata);
		if (res <0){
			XPN_DEBUG_END_ARGS1(path)
			return res;
		}
	}
	*/
	xpn_internal_remove(path);
	
	res = xpn_internal_creat(path, perm, &vfh, &mdata);
	if (res < 0){
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	res = xpn_internal_open(path, vfh, mdata, perm);
	if (res < 0){
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	xpn_err(XPN_OK);
	XPN_DEBUG_END_ARGS1(path)
	return res;
}

int xpn_open(const char *path, int flags , ...)
{
	mode_t mode;
	va_list ap;
	struct xpn_fh *vfh;
	struct xpn_metadata *mdata;
	int md;
	int res = -1;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	if((path == NULL)||(strlen(path)>MAX_PATH_LEN)){
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	mode = 0;
	if ((flags & O_CREAT) > 0) {
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
		if (mode > 0177777) {
			XPN_DEBUG_END_ARGS1(path)
			return res;
		}
	}
	
	vfh = NULL;
	mdata = NULL;
	
	/*
	flags = flags & (O_TRUNC | O_CREAT | O_RDWR | O_WRONLY |  O_RDONLY);
	if( (flags != (O_TRUNC | O_CREAT | O_RDWR )) &&
	    (flags != (O_TRUNC | O_CREAT | O_WRONLY )) &&
	    (flags != (O_TRUNC | O_CREAT | O_RDONLY )) &&
	    (flags != (O_CREAT | O_RDWR )) &&
	    (flags != (O_CREAT | O_WRONLY)) &&
	    (flags != (O_CREAT | O_RDONLY )) &&
	    (flags != (O_RDWR )) &&
	    (flags != (O_RDONLY)) &&
	    (flags != (O_WRONLY)) ){
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	*/
	if( (O_WRONLY == (flags & O_WRONLY)) ||
	    (O_RDWR == (flags & O_RDWR)) ){
		if(O_TRUNC == (flags & O_TRUNC)){
			res = xpn_internal_remove(path);
			/* res = xpn_internal_resize(path, 0)*/
		}
		if(  (O_TRUNC == (flags & O_TRUNC)) ||
		    ((O_CREAT == (flags & O_CREAT)) && (mode != 0))  ){
			//mode = mode & MASK;
			res = xpn_internal_creat(path, mode, &vfh, &mdata);
			if (res < 0){
				XPN_DEBUG_END_ARGS1(path)
				return res;
		        }
		}
	}
	
	md = (flags & O_RDWR)|(flags & O_RDONLY)|(flags & O_WRONLY);
	
	res = xpn_internal_open(path, vfh, mdata, md);
	
	xpn_err(XPN_OK);
	
	XPN_DEBUG_END_ARGS1(path)
	return res;
}

int xpn_close(int fd)
{
	int res, i;
	
	XPN_DEBUG_BEGIN_CUSTOM("%d", fd)
	
	if((fd<0)||(fd>XPN_MAX_FILE-1)){
		/* xpn_err */
		res = -1;
		XPN_DEBUG_END_CUSTOM("%d", fd)
		return res;
	}
	
	if(xpn_file_table[fd] == NULL){
		/* xpn_err */
		res = -1;
		XPN_DEBUG_END_CUSTOM("%d", fd)
		return res;
	}
	
	xpn_file_table[fd]->links--;
	if(xpn_file_table[fd]->links == 0){
        	for(i=0;i<xpn_file_table[fd]->data_vfh->n_nfih;i++){
                	if(xpn_file_table[fd]->data_vfh->nfih[i] != NULL){
				if(xpn_file_table[fd]->data_vfh->nfih[i]->priv_fh != NULL){
					xpn_file_table[fd]->data_vfh->nfih[i]->server->ops->nfi_close(
							xpn_file_table[fd]->data_vfh->nfih[i]->server,
							xpn_file_table[fd]->data_vfh->nfih[i]);
				}
				free(xpn_file_table[fd]->data_vfh->nfih[i]);
			}
		}
		
		free(xpn_file_table[fd]->data_vfh->nfih);
		free(xpn_file_table[fd]->data_vfh);
		free(xpn_file_table[fd]->mdata->policy);
		free(xpn_file_table[fd]->mdata);
		free(xpn_file_table[fd]);	
		xpn_file_table[fd] = NULL;
	}
	xpn_err(XPN_OK);
	
	res = 0;
	XPN_DEBUG_END_CUSTOM("%d", fd)
	return res;
}

int xpn_unlink(const char *path)
{
	int res;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	res = xpn_internal_remove(path);
	
	XPN_DEBUG_END_ARGS1(path)
	return res;
}

int xpn_rename(const char *path, const char *newpath)
{
	char abs_path[MAX_PATH_LEN], url_serv[MAX_PATH_LEN];
	char newabs_path[MAX_PATH_LEN], newurl_serv[MAX_PATH_LEN];
	struct nfi_server **servers;
	struct xpn_metadata *mdata_aux;
	int res, err, i, n, pd, newpd;
	struct xpn_fh *vfh_aux;
	
	if(path == NULL){
		xpn_err(XPNERR_PARAM);
		return -1;
	}
	
	if(newpath == NULL){
		xpn_err(XPNERR_PARAM);
		return -1;
	}
	
	res = XpnGetAbsolutePath(path, abs_path); /* esta funcion genera el path absoluto */
	if(res<0){
		xpn_err(XPNERR_PATH_NOEXIST);
		return -1;
	}
	
	res = XpnGetAbsolutePath(newpath, newabs_path); /* esta funcion genera el path absoluto */
	if(res<0){
		xpn_err(XPNERR_PATH_NOEXIST);
		return -1;
	}
	
	pd = XpnGetPartition(abs_path); /* return partition's id */
	if(pd<0){
		xpn_err(XPNERR_PART_NOEXIST);
		return -1;
	}
	
	newpd = XpnGetPartition(newabs_path); /* return partition's id */
	if(newpd<0){
		xpn_err(XPNERR_PART_NOEXIST);
		return -1;
	}
	
	if(pd != newpd){
		return -1;
	}
	
	/* params:
	 * flag operation, partition id, absolute path, file descriptor, pointer to server
	 */
	servers = NULL;
	n = XpnGetServers(op_xpn_rename, pd, abs_path, -1, &servers, XPN_DATA_SERVER);
	if(n<=0){
		/*free(servers);*/
		return -1;
	}
	
	vfh_aux = (struct xpn_fh *)malloc(sizeof(struct xpn_fh));
	
	/* construccion del vfh */
	if(vfh_aux == NULL){
		xpn_err(XPNERR_NOMEMORY);
		free(servers);
		return -1;
	}
	
	vfh_aux->n_nfih = n;
	vfh_aux->nfih = (struct nfi_fhandle **)malloc(sizeof(struct nfi_fhandle*) * n);
	if(vfh_aux->nfih == NULL){
		xpn_err(XPNERR_NOMEMORY);
		free(servers);
		return -1;
	}
	
	for(i=0;i<n;i++){
		vfh_aux->nfih[i] = NULL;
	}
	
	mdata_aux = (struct xpn_metadata *)malloc(sizeof(struct xpn_metadata));
	if(mdata_aux == NULL){
		free(servers);
		free(vfh_aux->nfih);
		free(vfh_aux);
		return -1;
	}
	
	memset(mdata_aux, 0, sizeof(struct xpn_metadata));

	res = XpnReadMetadata(mdata_aux, n, servers, vfh_aux, abs_path, pd);
	if(res<0){
		free(servers);
		/* tambien los punteros indirectos */
		for(i=0;i<n;i++){
			if(vfh_aux->nfih[i] != NULL){
				if(vfh_aux->nfih[i]->priv_fh != NULL){
					vfh_aux->nfih[i]->server->ops->nfi_close(
						vfh_aux->nfih[i]->server,
						vfh_aux->nfih[i]);
				}
				
				free(vfh_aux->nfih[i]);
			}
		}
		free(vfh_aux->nfih);
		free(vfh_aux);
		free(mdata_aux);
		/* */
		return -1;
	}
	
	res = XpnUpdateMetadata(mdata_aux, n, servers, vfh_aux, newabs_path);
	if(res<0){
		free(servers);
		/* tambien los punteros indirectos */
		for(i=0;i<n;i++){
			if(vfh_aux->nfih[i] != NULL){
				if(vfh_aux->nfih[i]->priv_fh != NULL){
					vfh_aux->nfih[i]->server->ops->nfi_close(
						vfh_aux->nfih[i]->server,
						vfh_aux->nfih[i]);
				}
				
				free(vfh_aux->nfih[i]);
			}
		}
		free(vfh_aux->nfih);
		free(vfh_aux);
		free(mdata_aux);
		/* */
		return -1;
	}
	
	for(i=0;i<n;i++){
		XpnGetURLServer(servers[i], abs_path, url_serv);
		XpnGetURLServer(servers[i], newabs_path, newurl_serv);
		// Default Value
		nfi_worker_thread(servers[i]->wrk, XpnGetThreads(op_xpn_rename,pd, 0));
		/* worker */
		nfi_worker_do_rename(	servers[i]->wrk,
					url_serv,
					newurl_serv);
	}
	
	err = 0;
	for(i=0;i<n;i++){
		res = nfi_worker_wait(servers[i]->wrk);
		if((!err)&&(res<0)){
			err = 1;
		}
	}
	
	free(servers);
	/* tambien los punteros indirectos */
	for(i=0;i<n;i++){
		if(vfh_aux->nfih[i] != NULL){
			if(vfh_aux->nfih[i]->priv_fh != NULL){
				vfh_aux->nfih[i]->server->ops->nfi_close(
					vfh_aux->nfih[i]->server,
					vfh_aux->nfih[i]);
				}
				free(vfh_aux->nfih[i]);
			}
		}
	free(vfh_aux->nfih);
	free(vfh_aux);
	free(mdata_aux);
	
	/* tratamiento de errores */
	if(err){
		
		res = XpnUpdateMetadata(mdata_aux, n, servers, vfh_aux, newabs_path);
		if(res<0){
			free(servers);
			/* tambien los punteros indirectos */
			for(i=0;i<n;i++){
				if(vfh_aux->nfih[i] != NULL){
					if(vfh_aux->nfih[i]->priv_fh != NULL){
						vfh_aux->nfih[i]->server->ops->nfi_close(
							vfh_aux->nfih[i]->server,
							vfh_aux->nfih[i]);
					}
					
					free(vfh_aux->nfih[i]);
				}
			}
			free(vfh_aux->nfih);
			free(vfh_aux);
			free(mdata_aux);
			/* */
			return -1;
		}
	}
	return 0;
}

int xpn_fstat(int fd, struct stat *sb)
{
	int res;
	
	XPN_DEBUG_BEGIN_CUSTOM("%d", fd)
	
	if(fd<0){
		errno = EBADF;
		res = -1;
		XPN_DEBUG_END_CUSTOM("%d", fd)
		return res;
	}
	
	res = XpnGetAtrib(fd , sb);
	
	XPN_DEBUG_END_CUSTOM("%d", fd)
	return res;
}

// FIXME: If the user has already opened the max number of files he is allowed to, then this call will fail with EMFILE.
int xpn_stat(const char *path, struct stat *sb)
{
	char abs_path[MAX_PATH_LEN];
	char abs_path2[MAX_PATH_LEN];
	int res, pd, fd;
	DIR *dir;
	char *new_path;
	
	XPN_DEBUG_BEGIN_ARGS1(path)
	
	if ((path == NULL) || (strlen(path) == 0)) {
		//xpn_err(XPNERR_PARAM);
		errno = ENOENT;
		res = -1;
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	res = XpnGetAbsolutePath(path, abs_path); /* this function generates the absolute path */
	if(res<0){
		xpn_err(XPNERR_PATH_NOEXIST);
		res = -1;
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	strcpy(abs_path2, abs_path);
	
	pd = XpnGetPartition(abs_path2); /* return partition's id and remove partition name from abs_path */
	if(pd<0){
		xpn_err(XPNERR_PART_NOEXIST);
		res = -1;
		XPN_DEBUG_END_ARGS1(path)
		return res;
	}
	
	//debug=1;
	fd = XpnSearchFile(abs_path2);
	//debug=0;
	//printf("xpn_stat: XpnSearchFile(%s->%s) = %d\n", path, abs_path2, fd);
	
	// If the file/directory is opened then just return its attributes, else open it and close it.
	if (fd>=0) {
		res = xpn_fstat(fd, sb);
	} else {
		fd = xpn_open(abs_path, O_RDONLY);
		if(fd>=0){
			res = XpnGetAtrib(fd, sb);
			xpn_close(fd);
		} else {
			new_path=malloc(strlen(path)+2);
			strcpy(new_path, "/");
			strcat(new_path,abs_path);
			dir = xpn_opendir(new_path);
			free(new_path);
			if (dir != NULL) {
				res = XpnGetAtrib(fd, sb);
				xpn_closedir(dir);
			} else {
				errno = ENOENT;
				res = -1;
				XPN_DEBUG_END_ARGS1(path)
				return res;
			}
		}
	}
	
	XPN_DEBUG_END_ARGS1(path)
	return res;
}

int xpn_chown(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) uid_t owner, __attribute__((__unused__)) gid_t group)
{
	return 0;
}

int xpn_fchown(int __attribute__((__unused__)) fd, __attribute__((__unused__)) uid_t owner, __attribute__((__unused__)) gid_t group)
{
	return 0;
}

int xpn_chmod(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) mode_t mode)
{
	return 0;
}

int xpn_fchmod(__attribute__((__unused__)) int fd, __attribute__((__unused__)) mode_t mode)
{
	return 0;
}

int xpn_truncate(__attribute__((__unused__)) const char *path, __attribute__((__unused__)) off_t length)
{
	return 0;
}

int xpn_ftruncate(__attribute__((__unused__)) int fd, __attribute__((__unused__)) off_t length)
{
	return 0;
}

int xpn_dup(int fd)
{
	int i;
	
	if((fd > XPN_MAX_FILE-1)||(fd <0))
		return -1;
	
	if(xpn_file_table[fd] == NULL)
		return -1;
	
	i =0;
	while((i<XPN_MAX_FILE-1)&&(xpn_file_table[i] != NULL))
		i++;
	if(i == XPN_MAX_FILE){
		/* xpn_err() ? */
		return -1;
	}
	xpn_file_table[i] = xpn_file_table[fd];
	xpn_file_table[fd]->links++;
	
	return i;
}

int xpn_dup2(int fd, int fd2)
{
	if((fd > XPN_MAX_FILE-1)||(fd <0))
	        return -1;
	if(xpn_file_table[fd] == NULL)
		return -1;
	if((fd2 > XPN_MAX_FILE-1)||(fd2 <0))
	        return -1;
	if(xpn_file_table[fd2] != NULL)
		return -1;
		
	xpn_file_table[fd2] = xpn_file_table[fd];
	xpn_file_table[fd]->links++;
	
	return 0;
}
