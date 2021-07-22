#include "nfi_ioproxy-rpc/ioproxy-rpc_lib.h"

/*
 * TODO:
 *  -> local_tcp, local_udp ??????????????
 *  -> local_xxx a nfi_ioproxy-rpc_xxx
 *  -> local_misc.[ch]:
 *     -> ParseURL
 *  -> ioproxy-rpc_error.[ch]:
 *     -> ioproxy-rpc_err
 *
 * -> pasar CLIENT * a servidor (es comn a todos...)
 * -> Poner todo a punteros
 * -> struct nfi_fhandle ** Paso de puntero par�etro por referencia
 *
 */


/************************************************************
 * PRIVATE FUNCTIONS TO USE local SERVERS	            *
 ************************************************************/
void NFItoIOPROXY_RPCattr(struct stat *att, struct nfi_attr *nfi_att)
{
	if(nfi_att->at_type == NFIFILE){
		att->st_mode	= nfi_att->at_mode | S_IFREG;	/* protection */
	}
	
	if(nfi_att->at_type == NFIDIR){
		att->st_mode	= nfi_att->at_mode | S_IFDIR;	/* protection */
	}
	
	att->st_nlink	= nfi_att->at_nlink;	/* number of hard links		*/
	att->st_uid	= nfi_att->at_uid;	/* user ID of owner		*/
	att->st_gid	= nfi_att->at_gid;	/* group ID of owner		*/
	att->st_size	= nfi_att->at_size;	/* total size, in bytes		*/
	att->st_blksize	= nfi_att->at_blksize;	/* blocksize for filesystem I/O	*/
	att->st_blocks	= nfi_att->at_blocks;	/* number of blocks allocated	*/
	att->st_atime	= nfi_att->at_atime;	/* time of last access		*/
	att->st_mtime	= nfi_att->at_mtime;	/* time of last modification	*/
	att->st_ctime	= nfi_att->at_ctime;	/* time of last status change	*/
}

void IOPROXY_RPCtoNFIattr(struct nfi_attr *nfi_att, struct stat *att)
{
	if(S_ISREG(att->st_mode)){
		nfi_att->at_type = NFIFILE;
	}
	if(S_ISDIR(att->st_mode)){
		nfi_att->at_type = NFIDIR;
	}
	
	nfi_att->at_mode	= att->st_mode/*&(S_IRWXU|S_IRWXG|S_IRWXO)*/;		/* protection			*/
	nfi_att->at_nlink	= att->st_nlink;	/* number of hard links		*/
	nfi_att->at_uid		= att->st_uid;		/* user ID of owner		*/
	nfi_att->at_gid		= att->st_gid;		/* group ID of owner		*/
	nfi_att->at_size	= att->st_size;		/* total size, in bytes		*/
	nfi_att->at_blksize	= att->st_blksize;	/* blocksize for filesystem I/O	*/
	nfi_att->at_blocks	= att->st_blocks;	/* number of blocks allocated	*/
	nfi_att->at_atime	= att->st_atime;	/* time of last access		*/
	nfi_att->at_mtime	= att->st_mtime;	/* time of last modification	*/
	nfi_att->at_ctime	= att->st_ctime;	/* time of last status change	*/
}

void IOPROXY_RPCtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *local_inf)
{
}

/************************************************************
 * PUBLIC FUNCTIONS TO USE IOPROXY_RPC			    *
 ************************************************************/

/************************************************************
 * Init Local 					    	    *
 ************************************************************/
int nfi_ioproxy-rpc_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr)
{
	char server[MAXPATHLEN], dir[MAXPATHLEN] ,prt [MAXPATHLEN];
	int ret;
	struct nfi_ioproxy-rpc_server *server_aux;


	if(serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

	/* functions */
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
	if (serv->ops == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		return -1;
	}

	bzero(serv->ops, sizeof(struct nfi_ops));
	
	serv->ops->nfi_reconnect  = nfi_ioproxy-rpc_reconnect;
	serv->ops->nfi_disconnect = nfi_ioproxy-rpc_disconnect;

	serv->ops->nfi_getattr	= nfi_ioproxy-rpc_getattr;
	serv->ops->nfi_setattr	= nfi_ioproxy-rpc_setattr;

	serv->ops->nfi_open	= nfi_ioproxy-rpc_open;
	serv->ops->nfi_close	= nfi_ioproxy-rpc_close;

	serv->ops->nfi_read	= nfi_ioproxy-rpc_read;
	serv->ops->nfi_write	= nfi_ioproxy-rpc_write;

	serv->ops->nfi_create	= nfi_ioproxy-rpc_create;
	serv->ops->nfi_remove	= nfi_ioproxy-rpc_remove;
	serv->ops->nfi_rename	= nfi_ioproxy-rpc_rename;

	serv->ops->nfi_mkdir	= nfi_ioproxy-rpc_mkdir;
	serv->ops->nfi_rmdir	= nfi_ioproxy-rpc_rmdir;
	serv->ops->nfi_opendir	= nfi_ioproxy-rpc_opendir;
	serv->ops->nfi_readdir	= nfi_ioproxy-rpc_readdir;
	serv->ops->nfi_closedir	= nfi_ioproxy-rpc_closedir;
	serv->ops->nfi_statfs	= nfi_ioproxy-rpc_statfs;

	ret = ParseURL(url,  prt, NULL, NULL, server,  NULL,  dir);
	//ret = ParseURL(url,  prt, NULL, NULL, NULL,  NULL,  dir);
	if(ret <0 ){
		ioproxy-rpc_err(IOPROXY_RPCERR_URL);
		fprintf(stderr,"nfi_ioproxy-rpc_init: url %s incorrect.\n",url);
		free(serv->ops);
		return -1;
	}
	server_aux = (struct nfi_ioproxy-rpc_server *)malloc(sizeof(struct nfi_ioproxy-rpc_server));
	if(server_aux == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		return -1;
	}

	strcpy(server_aux->path, dir);

	serv->private_info = (void *)server_aux;

	//serv->protocol = IOPROXY_RPC;

	serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1));	/* server address */
	if(serv->server == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		return -1;
	}
	strcpy(serv->server, server);

	serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1));	/* server address */
	if(serv->url == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		return -1;
	}

	strcpy(serv->url, url);
        serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
	memset(serv->wrk, 0, sizeof(struct nfi_worker));
	if(strcmp(prt, "filehilos") == 0){
#if defined(DEBUG_NFI)
		printf("filehilos\n");
#endif
		nfi_worker_init(serv->wrk, serv, 1);
	}
	else{
		nfi_worker_init(serv->wrk, serv, 0);
	}
	
	return 0;
}

/************************************************************
 * Disconnect from the server				    *
 * **********************************************************/
int nfi_ioproxy-rpc_disconnect(struct nfi_server *serv)
{

        struct nfi_ioproxy-rpc_server *server_aux;
        

	if (serv == NULL)
	       return 0;


        server_aux = (struct nfi_ioproxy-rpc_server *)serv->private_info;
        if(server_aux != NULL){
                 free(serv->private_info);
        }
	
        if(serv->url != NULL){
        	free(serv->url);
	}

	if(serv->server != NULL){
		free(serv->server);
	}

	//serv->protocol = -1;

	nfi_worker_end(serv->wrk);
	
	return 0;
}



/************************************************************
 * Reconnect to the IOPROXY_RPC server				    *
 ************************************************************/
int nfi_ioproxy-rpc_reconnect(struct nfi_server *serv)
{
	/* Don't see the serv result */
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_ioproxy-rpc_server *server_aux;

	ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
	//ret = ParseURL(serv->url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(ret <0 ){
		ioproxy-rpc_err(IOPROXY_RPCERR_URL);
		fprintf(stderr,"nfi_ioproxy-rpc_reconnect: url %s incorrect.\n",serv->url);
		return -1;
	}

	server_aux = (struct nfi_ioproxy-rpc_server *)malloc(sizeof(struct nfi_ioproxy-rpc_server));
	if(server_aux == NULL){		
		ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		return -1;
	}

	strcpy(server_aux->path, dir);

	serv->private_info = (void *)server_aux;
	return 0;
}

/************************************************************
 * Destroy IOPROXY_RPC operations				    *
 ************************************************************/

int nfi_ioproxy-rpc_destroy(struct nfi_server *serv)
{
	struct nfi_ioproxy-rpc_server *server_aux;

	if (serv == NULL)
		return 0;

	if(serv->ops != NULL)
		free(serv->ops);

	server_aux = (struct nfi_ioproxy-rpc_server *)serv->private_info;

	if(server_aux != NULL){
		free(serv->private_info);
	}

	if(serv->url != NULL){
		free(serv->url);
	}

	if(serv->server != NULL){
		free(serv->server);
	}

	//serv->protocol = -1;

	nfi_worker_end(serv->wrk);
	
	return 0;
}




/************************************************************
 * GENERIC FUNCTIONS 					    *
 ************************************************************/

int nfi_ioproxy-rpc_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){

	int ret;
	struct stat st;

	struct nfi_ioproxy-rpc_server *server_aux;
	struct nfi_ioproxy-rpc_fhandle *fh_aux;


	if (attr == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}


	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
			/* ioproxy-rpc_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif
	fh_aux = (struct nfi_ioproxy-rpc_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_ioproxy-rpc_server *) serv->private_info;

	ret = stat(fh_aux->path, &st);
	if(ret < 0){
		//fprintf(stderr,"nfi_ioproxy-rpc_getattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		fprintf(stderr,"nfi_ioproxy-rpc_getattr: Fail stat %s.\n",fh_aux->path);
		ioproxy-rpc_err(IOPROXY_RPCERR_GETATTR);
		return -1;
	}

 	IOPROXY_RPCtoNFIattr(attr, &st);
	return 0;
}


int nfi_ioproxy-rpc_setattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){

	int ret;
	struct stat st;
	struct nfi_ioproxy-rpc_server *server_aux;
	struct nfi_ioproxy-rpc_fhandle *fh_aux;


	if (attr == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
			/* ioproxy-rpc_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_ioproxy-rpc_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_ioproxy-rpc_server *) serv->private_info;

 	NFItoIOPROXY_RPCattr(&st, attr);
	/* no se comp hacer el setattr */
	/*
	ret = stat(fh_aux->path, &st);
	*/
	ret = truncate(fh_aux->path, st.st_size);
	if(ret < 0){
		//fprintf(stderr,"nfi_ioproxy-rpc_setattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		fprintf(stderr,"nfi_ioproxy-rpc_setattr: Fail stat %s.\n",fh_aux->path);
		ioproxy-rpc_err(IOPROXY_RPCERR_GETATTR);
		return -1;
	}
	
	return 0;
}

int nfi_ioproxy-rpc_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int ret;
	struct stat st;
	struct nfi_ioproxy-rpc_server *server_aux;
	struct nfi_ioproxy-rpc_fhandle *fh_aux;

	if(url[strlen(url)-1] == '/'){
		return nfi_ioproxy-rpc_opendir(serv, url, fho);
	}
	
	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}
	
	if (fho == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
			/* ioproxy-rpc_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	//ret = ParseURL(url, NULL, NULL, NULL, NULL,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_ioproxy-rpc_open: url %s incorrect.\n",url);
		ioproxy-rpc_err(IOPROXY_RPCERR_URL);
		return -1;
	}
	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_ioproxy-rpc_fhandle *)malloc(sizeof(struct nfi_ioproxy-rpc_fhandle));
	if (fh_aux == NULL){		
		ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_ioproxy-rpc_server *) serv->private_info;
	ret = open(dir, O_RDWR);

    if (ret < 0 ){
		ret = open(dir, O_RDONLY);
		if(ret <0){
			//fprintf(stderr,"nfi_ioproxy-rpc_open: Fail open %s in server %s.\n",dir,serv->server);
			//fprintf(stderr,"nfi_ioproxy-rpc_open: Fail open %s.\n",dir);
			ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);		
			free(fh_aux);
			free(fho->url);
			return -1;
		}
	}
	
	fh_aux->fd = ret;
	strcpy(fh_aux->path, dir);
	
    ret = stat(fh_aux->path, &st);
    if(ret < 0){
        ioproxy-rpc_err(IOPROXY_RPCERR_GETATTR);
		//fprintf(stderr,"nfi_ioproxy-rpc_open: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		fprintf(stderr,"nfi_ioproxy-rpc_open: Fail stat %s.\n",fh_aux->path);
		free(fh_aux);
		free(fho->url);
		return -1;
	}
	
	if(S_ISDIR(st.st_mode)){
		fho->type = NFIDIR;
	}
	else{
		if(S_ISREG(st.st_mode)){
			fho->type = NFIFILE;			
		}
		else{
			fprintf(stderr,"nfi_ioproxy-rpc_open: %s isn't a FILE or FILE.\n",fh_aux->path);
        	ioproxy-rpc_err(IOPROXY_RPCERR_GETATTR);
			free(fh_aux);
			free(fho->url);
			return -1;
		}
	}
	fho->server = NULL;
	fho->priv_fh = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;

	return 0;

}


int nfi_ioproxy-rpc_close(struct nfi_server *server,  struct nfi_fhandle *fh){
	
	struct nfi_ioproxy-rpc_fhandle *fh_aux;

	if (server == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_ioproxy-rpc_fhandle *) fh->priv_fh;
		close(fh_aux->fd);
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;

	}
	
	if (fh->url != NULL){
		//free(fh->url); // FIXME
		fh->url = NULL;
	}
	
	fh->type = NFINULL;
	fh->server = NULL;

	return 0;
}

ssize_t nfi_ioproxy-rpc_read(	struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{

	ssize_t new_size;

	struct nfi_ioproxy-rpc_server *server_aux;
	struct nfi_ioproxy-rpc_fhandle *fh_aux;

	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
			/* ioproxy-rpc_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_ioproxy-rpc_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_ioproxy-rpc_server *) serv->private_info;
	lseek(fh_aux->fd, offset, SEEK_SET);
	new_size = read(fh_aux->fd,
			    buffer,
			    size);
#ifdef  DBG_NFI 
fprintf(stderr,"nfi_ioproxy-rpc_read: read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
#endif
	if(new_size < 0){
		fprintf(stderr,"nfi_ioproxy-rpc_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
		ioproxy-rpc_err(IOPROXY_RPCERR_READ);
		return -1;
	}


	return new_size;
}

ssize_t nfi_ioproxy-rpc_write(struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{

	ssize_t new_size;
        struct nfi_ioproxy-rpc_server *server_aux;
        struct nfi_ioproxy-rpc_fhandle *fh_aux;

	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
			/* ioproxy-rpc_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif


        fh_aux = (struct nfi_ioproxy-rpc_fhandle *) fh->priv_fh;
        server_aux = (struct nfi_ioproxy-rpc_server *) serv->private_info;

	lseek(fh_aux->fd, offset, SEEK_SET);
	new_size = write(fh_aux->fd,
			buffer,		
			size);
#ifdef  DBG_NFI 
	//fprintf(stderr,"nfi_ioproxy-rpc_write: write %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
#endif
	if(new_size < 0){
		fprintf(stderr,"nfi_ioproxy-rpc_write: Fail write %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
		
		ioproxy-rpc_err(IOPROXY_RPCERR_WRITE);
		return -1;
	}

	return new_size;
}

int nfi_ioproxy-rpc_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh){

	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret,fd;
	struct nfi_ioproxy-rpc_server *server_aux;
        struct nfi_ioproxy-rpc_fhandle *fh_aux;
	struct stat st;

	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
			/* ioproxy-rpc_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif
	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	//ret = ParseURL(url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_ioproxy-rpc_create: url %s incorrect.\n",url);
		ioproxy-rpc_err(IOPROXY_RPCERR_URL);
		return -1;
	}
	/* private_info file handle */
	fh_aux = (struct nfi_ioproxy-rpc_fhandle *)malloc(sizeof(struct nfi_ioproxy-rpc_fhandle));
	if (fh_aux == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		return -1;
	}

	bzero(fh_aux, sizeof(struct nfi_ioproxy-rpc_fhandle));
	
	server_aux = (struct nfi_ioproxy-rpc_server *) serv->private_info;
	/* create the file into the directory */
	fd = open(dir, O_CREAT|O_RDWR , attr->at_mode);
	if(fd < 0){
		//fprintf(stderr,"nfi_ioproxy-rpc_creat: Fail creat %s in server %s.\n",fh_aux->path,serv->server);
		//fprintf(stderr,"nfi_ioproxy-rpc_creat: Fail creat %s.\n",fh_aux->path);
		fprintf(stderr,"nfi_ioproxy-rpc_creat: Fail creat %s.\n",dir);
		ioproxy-rpc_err(IOPROXY_RPCERR_LOOKUP);
		free(fh_aux);
		return -1;
	}
	fh->server = serv;
	
	fh_aux->fd = fd;
	strcpy(fh_aux->path, dir);
	
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               	ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		free(fh_aux);
               	return -1;
        }
		
        strcpy(fh->url, url);
	memset (&st, 0, sizeof(struct stat));
	ret = stat(fh_aux->path, &st);
	if(ret < 0){
		fprintf(stderr,"nfi_ioproxy-rpc_creat: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		ioproxy-rpc_err(IOPROXY_RPCERR_GETATTR);
		free(fh->url);
		free(fh_aux);
		
		return -1;
	}
	
	IOPROXY_RPCtoNFIattr(attr, &st);

	return 0;
}

int nfi_ioproxy-rpc_remove(struct nfi_server *serv,  char *url)
{

	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_ioproxy-rpc_server *server_aux;
	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
			/* ioproxy-rpc_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_ioproxy-rpc_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	//ret = ParseURL(url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(ret < 0){
		//fprintf(stderr,"nfi_ioproxy-rpc_remove: url %s incorrect.\n",url);
		ioproxy-rpc_err(IOPROXY_RPCERR_URL);
		return -1;
	}
	/* remove the file into the directory */
	ret = unlink(dir);

	if(ret < 0){
		//fprintf(stderr,"nfi_ioproxy-rpc_remove: Fail remove %s in server %s.\n",dir,serv->server);
		//fprintf(stderr,"nfi_ioproxy-rpc_remove: Fail remove %s.\n", dir);
		ioproxy-rpc_err(IOPROXY_RPCERR_REMOVE);
		return -1;
	}



	return 0;
}

int nfi_ioproxy-rpc_rename(struct nfi_server *server,  char *old_url, char *new_url)
{
	/*
        struct nfi_ioproxy-rpc_server *server_aux;
        struct nfi_ioproxy-rpc_fhandle *fh_aux;
	if (server == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
			ioproxy-rpc_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif
	server_aux = (strcut nfi_ioproxy-rpc_server *)serv->private_info;
	*/

	/* ????? */
	return 0;
}

int nfi_ioproxy-rpc_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct stat st;		/* IOPROXY_RPC attributes */
        struct nfi_ioproxy-rpc_server *server_aux;
        struct nfi_ioproxy-rpc_fhandle *fh_aux;


	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
			/* ioproxy-rpc_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_ioproxy-rpc_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	//ret = ParseURL(url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_ioproxy-rpc_mkdir: url %s incorrect.\n",url);
		ioproxy-rpc_err(IOPROXY_RPCERR_URL);
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_ioproxy-rpc_fhandle *)malloc(sizeof(struct nfi_ioproxy-rpc_fhandle));
	if (fh_aux == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		return -1;
	}


	bzero(fh_aux, sizeof(struct nfi_ioproxy-rpc_fhandle));
	/* create the dir into the directory */
	ret = mkdir(dir, /*attr->at_mode*/ 0777);
	
	if((ret < 0)&&(errno != EEXIST)){
		ioproxy-rpc_err(IOPROXY_RPCERR_MKDIR);
		//fprintf(stderr,"nfi_ioproxy-rpc_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server);
		fprintf(stderr,"nfi_ioproxy-rpc_mkdir: Fail mkdir %s.\n",dir);
		free(fh_aux);
		return -1;
	}
	
	fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
	       free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

	IOPROXY_RPCtoNFIattr(attr, &st);


	return 0;
}

int nfi_ioproxy-rpc_rmdir(struct nfi_server *serv,  char *url)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;

        struct nfi_ioproxy-rpc_server *server_aux;

	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
			/* ioproxy-rpc_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_ioproxy-rpc_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	//ret = ParseURL(url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_ioproxy-rpc_rmdir: url %s incorrect.\n",url);
		ioproxy-rpc_err(IOPROXY_RPCERR_URL);
		return -1;
	}

	ret = rmdir(dir);	
	if(ret < 0){
		//fprintf(stderr,"nfi_ioproxy-rpc_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server);
		fprintf(stderr,"nfi_ioproxy-rpc_rmdir: Fail rmdir %s.\n",dir);
		ioproxy-rpc_err(IOPROXY_RPCERR_REMOVE);
		return -1;
	}

	return 0;
}

int nfi_ioproxy-rpc_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho)
{
        
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int ret;	
	struct nfi_ioproxy-rpc_server *server_aux;
	struct nfi_ioproxy-rpc_fhandle *fh_aux;


	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

	if (fho == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
			/* ioproxy-rpc_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	//ret = ParseURL(url, NULL, NULL, NULL, NULL,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_ioproxy-rpc_opendir: url %s incorrect.\n",url);
		ioproxy-rpc_err(IOPROXY_RPCERR_URL);
		return -1;
	}

	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_ioproxy-rpc_fhandle *)malloc(sizeof(struct nfi_ioproxy-rpc_fhandle));
	if (fh_aux == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_ioproxy-rpc_server *) serv->private_info;


	fh_aux->dir = opendir(dir);
	printf("nfi_ioproxy-rpc_opendir: opendir(%s)=%p\n", dir, fh_aux->dir);
	if (fh_aux->dir == NULL){
		//fprintf(stderr,"nfi_ioproxy-rpc_opendir: Fail opendir %s in server %s.\n",dir,serv->server);
		fprintf(stderr,"nfi_ioproxy-rpc_opendir: Fail opendir %s.\n",dir);
		ioproxy-rpc_err(IOPROXY_RPCERR_MEMORY);
		free(fh_aux);
		free(fho->url);
		return -1;
	}
	
	fh_aux->fd = ret;
	strcpy(fh_aux->path, dir);
	fho->type = NFIDIR;
	fho->server = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;

	return 0;

}

int nfi_ioproxy-rpc_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, char *entry, unsigned char *type)
{
	struct dirent *ent;

        struct nfi_ioproxy-rpc_server *server_aux;
        struct nfi_ioproxy-rpc_fhandle *fh_aux;


	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

	if(fh->type != NFIDIR){
		ioproxy-rpc_err(IOPROXY_RPCERR_NOTDIR);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
			/* ioproxy-rpc_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_ioproxy-rpc_server *)serv->private_info;
	fh_aux = (struct nfi_ioproxy-rpc_fhandle *)fh->priv_fh;

	entry[0] = '\0';
	printf("nfi_ioproxy-rpc_readdir: readdir(%p)\n", fh_aux->dir);
	ent = readdir(fh_aux->dir);

	if(ent == NULL){
		printf("nfi_ioproxy-rpc_readdir: ent=%p, errno=%d, EBADF=%d\n", ent, errno, EBADF);
		perror("nfi_ioproxy-rpc_readdir: readdir");
		return 1;
	}
	if(type==NULL){
		return 0;
	}
	//printf("nfi_ioproxy-rpc_readdir: Despues: ent->d_name=%p='%s' S_ISDIR(%o)=%o\n", ent->d_name, ent->d_name, ent->d_type, S_ISDIR(ent->d_type));
	
	printf("nfi_ioproxy-rpc_readdir: Antes: entry=%p='%s'\n", entry, entry);
	strcpy(entry, ent->d_name);
	printf("nfi_ioproxy-rpc_readdir: Despues: entry=%p='%s'\n", entry, entry);
	printf("ent->d_name = %s S_ISDIR(%o) = %o\n", ent->d_name, ent->d_type,S_ISDIR(ent->d_type));
	
	*type = ent->d_type;
	
	return 0;
}

int nfi_ioproxy-rpc_closedir(struct nfi_server *serv,  struct nfi_fhandle *fh)
{
	
	struct nfi_ioproxy-rpc_fhandle *fh_aux;

	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_ioproxy-rpc_fhandle *) fh->priv_fh;
		closedir(fh_aux->dir);
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;

	}

	if (fh->url != NULL){
		free(fh->url);
	}
	
	return 0;
	
}


int nfi_ioproxy-rpc_statfs(struct nfi_server *serv,  struct nfi_info *inf)
{
	/*
	struct local_info localinf;
	int ret;
        struct nfi_ioproxy-rpc_server *server_aux;


	if (serv == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}

	if (inf == NULL){
		ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_ioproxy-rpc_reconnect(serv);
		if(ret <0){
		 ioproxy-rpc_err(); not necessary 
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               ioproxy-rpc_err(IOPROXY_RPCERR_PARAM);
               return -1;
        }
#endif
	
	server_aux = (struct nfi_ioproxy-rpc_server *)serv->private_info;
	ret = local_statfs(server_aux->fh, &localinf, server_aux->cl);
	if(ret <0){
		ioproxy-rpc_err(IOPROXY_RPCERR_STATFS);
		return -1;
	}

	IOPROXY_RPCtoNFIInfo(inf, &localinf);
	*/
	return 0;
}
