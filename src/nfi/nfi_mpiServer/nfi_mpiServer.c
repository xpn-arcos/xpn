
#include "nfi_mpiServer.h"

/*
 * ENVIROMENT VARIABLES:
 *
 * XPN_DNS -> node{ID}	{hostname}	{port}
 */



#ifdef DBG_NFI
  #define debug_nfi_info(...)      fprintf(stdout, __VA_ARGS__)
#else
  #define debug_nfi_info(...)
#endif

  #define debug_nfi_error(...)     fprintf(stderr, __VA_ARGS__)


/**********************************
Write the operation to realize
***********************************/
int mpiServer_write_operation(int sd, struct st_mpiServer_msg *head)
{
        int ret;

	debug_nfi_info("[NFI]ID=%s)mpiServer_write_data: begin\n", head->id);	
        ret = mpiServer_write_data(sd, (char *)&head->type, sizeof(head->type), head->id);
        if(ret == -1){
                return -1;
        }

        debug_nfi_info("[NFI]ID=%s)write_operation: %d -> \n", head->id, head->type);
        switch ( head->type )
	{
		case MPISERVER_OPEN_FILE:
			debug_nfi_info("[NFI]ID=%s)OPEN operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_open, sizeof(struct st_mpiServer_open), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_CREAT_FILE:
			debug_nfi_info("[NFI]ID=%s)CREAT operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_creat, sizeof(struct st_mpiServer_creat), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_READ_FILE:
			debug_nfi_info("[NFI]ID=%s)READ operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_read, sizeof(struct st_mpiServer_read), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_WRITE_FILE:
			debug_nfi_info("[NFI]ID=%s)WRITE operation\n", head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_write, sizeof(struct st_mpiServer_write), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_CLOSE_FILE:
			debug_nfi_info("[NFI]ID=%s)CLOSE operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_close, sizeof(struct st_mpiServer_close), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_RM_FILE:
			debug_nfi_info("[NFI]ID=%s)RM operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_rm, sizeof(struct st_mpiServer_rm), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_GETATTR_FILE:
			debug_nfi_info("[NFI]ID=%s)GETATTR operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_getattr, sizeof(struct st_mpiServer_getattr), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_MKDIR_DIR:
			debug_nfi_info("[NFI]ID=%s)MDKIR operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_mkdir, sizeof(struct st_mpiServer_mkdir), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_RMDIR_DIR:
			debug_nfi_info("[NFI]ID=%s)RMDIR operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_rmdir, sizeof(struct st_mpiServer_rmdir), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_FLUSH_FILE:
			debug_nfi_info("[NFI]ID=%s)FLUSH operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_flush, sizeof(struct st_mpiServer_flush), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_PRELOAD_FILE:
			debug_nfi_info("[NFI]ID=%s)PRELOAD operation\n",head->id);
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_preload, sizeof(struct st_mpiServer_preload), head->id);
			if(ret == -1){
				return -1;
			}
			break;
		case MPISERVER_FINALIZE:
			debug_nfi_info("[NFI]ID=%s)FINALIZE operation\n",head->id);
		/*
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_end, sizeof(struct st_mpiServer_end), head->id);
			if(ret == -1){
				return -1;
			}
		*/
			break;
		case MPISERVER_GETID:
			debug_nfi_info("[NFI]ID=%s)GETID operation\n",head->id);
		/*
			ret = mpiServer_write_data(sd, (char *)head->id, MPISERVER_ID, head->id);
			if(ret == -1){
				return -1;
			}
		*/
			break;
		case MPISERVER_END:
			debug_nfi_info("[NFI]ID=%s)END operation\n",head->id);
		/*
			ret = mpiServer_write_data(sd, (char *)&head->u_st_mpiServer_msg.op_end, sizeof(struct st_mpiServer_end), head->id);
			if(ret == -1){
				return -1;
			}
		*/
			break;
        }

        return 0;
}

/***************************************************************/

/************************************************************
 * PRIVATE FUNCTIONS TO USE mpiServer SERVERS	            *
 ************************************************************/
void NFItoMPISERVERattr(struct stat *att, struct nfi_attr *nfi_att)
{
	if(nfi_att->at_type == NFIFILE){		
		att->st_mode	= nfi_att->at_mode | S_IFREG;     /* protection */	}
	
	if(nfi_att->at_type == NFIDIR){		
		att->st_mode	= nfi_att->at_mode | S_IFDIR;     /* protection */	}
	
	att->st_size	= nfi_att->at_size;      /* size */
	att->st_uid	= nfi_att->at_uid;      /* user ID of owner */
	att->st_gid	= nfi_att->at_gid;      /* group ID of owner */
	att->st_blksize	= nfi_att->at_blksize;  /* blocksize for filesystem I/O */
	att->st_blocks	= nfi_att->at_blocks;   /* number of blocks allocated */
	att->st_atime	= nfi_att->at_atime;    /* time of last access */
	att->st_mtime	= nfi_att->at_mtime;    /* time of last modification */
	att->st_ctime	= nfi_att->at_ctime;    /* time of last change */
}

void MPISERVERtoNFIattr(struct nfi_attr *nfi_att, struct stat *att)
{
	if(S_ISREG(att->st_mode)){
		nfi_att->at_type = NFIFILE;
	}
        if(S_ISDIR(att->st_mode)){
		nfi_att->at_type = NFIDIR;
        }

	nfi_att->at_size = att->st_size;      /* size */
        nfi_att->at_mode = att->st_mode&(S_IRWXU|S_IRWXG|S_IRWXO); /* protection */
        nfi_att->at_uid = att->st_uid;      /* user ID of owner */
        nfi_att->at_gid = att->st_gid;      /* group ID of owner */
        nfi_att->at_blksize = att->st_blksize;  /* blocksize for filesystem I/O*/
        nfi_att->at_blocks = att->st_blocks;   /* number of blocks allocated */
	nfi_att->at_atime = att->st_atime;    /* time of last access */
        nfi_att->at_mtime = att->st_mtime;    /* time of last modification */
        nfi_att->at_ctime = att->st_ctime;    /* time of last change */
}

void MPISERVERtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *mpiServer_inf)
{
}

/************************************************************
 * PUBLIC FUNCTIONS TO USE MPISERVER			    *
 ************************************************************/

/************************************************************
 * Init Local 					    	    *
 ************************************************************/
int nfi_mpiServer_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr)
{
	char server[MAXPATHLEN], dir[MAXPATHLEN], prt[MAXPATHLEN];
	
	int ret;
	struct nfi_mpiServer_server *server_aux;
	struct st_mpiServer_msg msg;


	debug_nfi_info("[NFI]nfi_mpiServer_init: begin\n");
	debug_nfi_info("[NFI]nfi_mpiServer_init: sizeof(struct st_mpiServer_msg) = %d.\n", sizeof(struct st_mpiServer_msg));
	
	if(serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	/* functions */
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
	if (serv->ops == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}

	bzero(serv->ops, sizeof(struct nfi_ops));
	
	serv->ops->nfi_preload	= nfi_mpiServer_preload;
	serv->ops->nfi_flush	= nfi_mpiServer_flush;
	
	serv->ops->nfi_reconnect  = nfi_mpiServer_reconnect;
	serv->ops->nfi_disconnect = nfi_mpiServer_disconnect;

	serv->ops->nfi_getattr	= nfi_mpiServer_getattr;
	serv->ops->nfi_setattr	= nfi_mpiServer_setattr;

	serv->ops->nfi_open	= nfi_mpiServer_open;
	serv->ops->nfi_close	= nfi_mpiServer_close;

	serv->ops->nfi_read	= nfi_mpiServer_read;
	serv->ops->nfi_write	= nfi_mpiServer_write;

	serv->ops->nfi_create	= nfi_mpiServer_create;
	serv->ops->nfi_remove	= nfi_mpiServer_remove;
	serv->ops->nfi_rename	= nfi_mpiServer_rename;

	serv->ops->nfi_mkdir	= nfi_mpiServer_mkdir;
	serv->ops->nfi_rmdir	= nfi_mpiServer_rmdir;
	serv->ops->nfi_opendir	= nfi_mpiServer_opendir;
	serv->ops->nfi_readdir	= nfi_mpiServer_readdir;
	serv->ops->nfi_closedir	= nfi_mpiServer_closedir;
	serv->ops->nfi_statfs	= nfi_mpiServer_statfs;

	ret = ParseURL(url,  prt, NULL, NULL, server,  NULL,  dir);
	if(ret <0 ){
		mpiServer_err(MPISERVERERR_URL);
		debug_nfi_error("nfi_mpiServer_init: url %s incorrect.\n",url);
		free(serv->ops);
		return -1;
	}
	server_aux = (struct nfi_mpiServer_server *)malloc(sizeof(struct nfi_mpiServer_server));
	if(server_aux == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}

	/******************************************/

	server_aux->sd = mpiServer_connect(server);
	strcpy(msg.id, "GETID");
        msg.type = MPISERVER_GETID;
        mpiServer_write_operation(server_aux->sd, &msg);
	debug_nfi_info("[NFI]nfi_mpiServer_init: mpiServer_write_data\n");
	mpiServer_read_data(server_aux->sd, (char *)server_aux->id, MPISERVER_ID, msg.id);
	debug_nfi_info("[NFI]nfi_mpiServer_init: mpiServer_read_data id = %s\n",server_aux->id);
	/******************************************/
	
	serv->private_info = (void *)server_aux;

	//serv->protocol = MPISERVER;
	
	serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1));	/* server address */
	if(serv->server == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}
	strcpy(serv->server, server);

	serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1));	/* server address */
	if(serv->url == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}

	strcpy(serv->url, url);
        serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
	memset(serv->wrk, 0, sizeof(struct nfi_worker));


#ifdef _ALL_THREADS_
//	nfi_worker_init(serv->wrk, serv, 1);
//	nfi_worker_init(serv->wrk, serv, 0);
#else
	if (strcmp("mpiServer", prt) == 0)
	{
		debug_nfi_info("[NFI]mpiServer\n");
	        debug_nfi_info("[NFI]nfi_worker_init(1,ID=%s): \n",server_aux->id);
		nfi_worker_init(serv->wrk, serv, 1);
	}
	else
	{
	        debug_nfi_info("[NFI]nfi_worker_init(0,ID=%s): \n",server_aux->id);
		nfi_worker_init(serv->wrk, serv, 0);
	}
#endif	

	debug_nfi_info("[NFI]nfi_mpiServer_init(ID=%s): end\n",server_aux->id);
	return 0;
}

/************************************************************
 * Disconnect to the server				    *
 * **********************************************************/
int nfi_mpiServer_disconnect(struct nfi_server *serv)
{
        struct nfi_mpiServer_server *server_aux;


	if (serv == NULL) {
	       return 0;
	}

        server_aux = (struct nfi_mpiServer_server *)serv->private_info;
        if (server_aux != NULL) {
        	close(server_aux->sd);
			//debug_nfi_info("[NFI]close la conexion\n");
                free(serv->private_info);
	}

        if (serv->url != NULL) {
        	free(serv->url);
	}

	if (serv->server != NULL) {
		free(serv->server);
	}

	//serv->protocol = -1;

	return 0;
}

/************************************************************
 * Reconnect to the NFS server				    *
 ************************************************************/
int nfi_mpiServer_reconnect(struct nfi_server *serv)
{
	/* Don't see the serv result */
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;

	ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret <0 ){
		mpiServer_err(MPISERVERERR_URL);
		debug_nfi_error("nfi_mpiServer_reconnect: url %s incorrect.\n",serv->url);
		return -1;
	}

	server_aux = (struct nfi_mpiServer_server *)malloc(sizeof(struct nfi_mpiServer_server));
	if(server_aux == NULL){		
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}

	strcpy(server_aux->path, dir);

	serv->private_info = (void *)server_aux;
	return 0;
}

/************************************************************
 * Destroy NFS operations				    *
 ************************************************************/

int nfi_mpiServer_destroy(struct nfi_server *serv)
{

	struct nfi_mpiServer_server *server_aux;
	struct st_mpiServer_msg msg;

	if (serv == NULL)
		return 0;

	if(serv->ops != NULL)
		free(serv->ops);

	server_aux = (struct nfi_mpiServer_server *)serv->private_info;

	if(server_aux != NULL){
		//debug_nfi_info("[NFI]Cierro la conexion\n");
		msg.type = -1;
        	mpiServer_write_operation(server_aux->sd, &msg);
        	close(server_aux->sd);
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

int nfi_mpiServer_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr)
{
	struct nfi_mpiServer_server *server_aux;
	struct nfi_mpiServer_fhandle *fh_aux;
	struct st_mpiServer_msg msg;
	struct st_mpiServer_attr_req req;

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	debug_nfi_info("[NFI]nfi_mpiServer_getattr(ID=%s): begin\n",server_aux->id);

	// check params
	if (attr == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (fh->priv_fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif
	fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_mpiServer_server *) serv->private_info;

	
	/*****************************************/
	msg.type = MPISERVER_GETATTR_FILE;
	strcpy(msg.u_st_mpiServer_msg.op_getattr.path, fh_aux->path);

        mpiServer_write_operation(server_aux->sd, &msg);
	debug_nfi_info("[NFI]nfi_mpiServer_getattr(ID=%s): getattr: -> %s \n", server_aux->id, msg.u_st_mpiServer_msg.op_getattr.path);
	bzero(&req, sizeof(struct st_mpiServer_attr_req));
	mpiServer_read_data(server_aux->sd, (char *)&req, sizeof(struct st_mpiServer_attr_req), msg.id);
	debug_nfi_info("[NFI]nfi_mpiServer_getattr(ID=%s): getattr: <- %d\n", server_aux->id, req.status);
	/*****************************************/	
	
	/*
	ret = stat(fh_aux->path, &st);
	if(ret < 0){
		debug_nfi_error("nfi_mpiServer_getattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		mpiServer_err(MPISERVERERR_GETATTR);
		return -1;
	}
	*/

	MPISERVERtoNFIattr(attr, &req.attr);
	
	debug_nfi_info("[NFI]nfi_mpiServer_getattr(ID=%s): end\n",server_aux->id);
	return req.status;
}


int nfi_mpiServer_setattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){

	struct nfi_mpiServer_server *server_aux;
	struct nfi_mpiServer_fhandle *fh_aux;


	if (attr == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_mpiServer_server *) serv->private_info;

 //	NFItoMPISERVERattr(&st, attr);
	/* no se comp hacer el setattr */
	/*
	ret = stat(fh_aux->path, &st);
	*/
/*
	ret = truncate(fh_aux->path, st.st_mpiServer_size);
	if(ret < 0){
		debug_nfi_error("nfi_mpiServer_setattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		mpiServer_err(MPISERVERERR_GETATTR);
		return -1;
	}
*/	
	return 0;
}

int nfi_mpiServer_preload(struct nfi_server *serv, char *url, char *virtual_path, char *storage_path, int opt)
{
	//char dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;
	struct st_mpiServer_msg msg;
	
	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	debug_nfi_info("[NFI]nfi_mpiServer_preload(ID=%s): begin %s - %s \n", server_aux->id, virtual_path, storage_path);
	if(url == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if(url[strlen(url)-1] == '/'){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	
	if (virtual_path == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (storage_path == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	debug_nfi_info("[NFI]nfi_mpiServer_preload(ID=%s): preload %s in server %s.\n",server_aux->id,virtual_path,serv->server);
	
	/*****************************************/
	msg.type = MPISERVER_PRELOAD_FILE;
	//strcpy(msg.u_st_mpiServer_msg.op_preload.path,dir);
	strcpy(msg.u_st_mpiServer_msg.op_preload.virtual_path,virtual_path);
	strcpy(msg.u_st_mpiServer_msg.op_preload.storage_path,storage_path);
	msg.u_st_mpiServer_msg.op_preload.opt = opt;

        mpiServer_write_operation(server_aux->sd, &msg);
	debug_nfi_info("[NFI]nfi_mpiServer_preload(ID=%s): preload: -> %s \n",server_aux->id,msg.u_st_mpiServer_msg.op_preload.virtual_path);
	mpiServer_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
	debug_nfi_info("[NFI]nfi_mpiServer_preload(ID=%s): preload: <- %d \n",server_aux->id,ret);
	/*****************************************/
	
	debug_nfi_info("[NFI]nfi_mpiServer_preload(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret);
	if(ret == -1){
		debug_nfi_info("[NFI]Error en el preload\n");
	}

	return ret;
}


int nfi_mpiServer_flush(struct nfi_server *serv,  char *url, char *virtual_path, char *storage_path, int opt)
{
	//char dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;
	struct st_mpiServer_msg msg;
	
	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	debug_nfi_info("[NFI]nfi_mpiServer_flush(ID=%s): begin %s - %s \n", server_aux->id,virtual_path, storage_path);

	// check params...
	if(url == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if(url[strlen(url)-1] == '/'){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (virtual_path == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (storage_path == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	debug_nfi_info("[NFI]nfi_mpiServer_flush(ID=%s): open %s in server %s.\n",server_aux->id,virtual_path,serv->server);
	
	/*****************************************/
	msg.type = MPISERVER_FLUSH_FILE;
	//strcpy(msg.u_st_mpiServer_msg.op_flush.path,dir);
	strcpy(msg.u_st_mpiServer_msg.op_flush.virtual_path,virtual_path);
	strcpy(msg.u_st_mpiServer_msg.op_flush.storage_path,storage_path);
	msg.u_st_mpiServer_msg.op_flush.opt = opt;

        mpiServer_write_operation(server_aux->sd, &msg);
	debug_nfi_info("[NFI]nfi_mpiServer_flush(ID=%s): flush: -> %s \n",server_aux->id,msg.u_st_mpiServer_msg.op_flush.virtual_path);
	mpiServer_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
	debug_nfi_info("[NFI]nfi_mpiServer_flush(ID=%s): flush: <- %d \n",server_aux->id,ret);
	/*****************************************/
	
	debug_nfi_info("[NFI]nfi_mpiServer_flush(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret);
	return 0;
}

int nfi_mpiServer_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;
	struct nfi_mpiServer_fhandle *fh_aux;

	struct st_mpiServer_msg msg;
	
	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	debug_nfi_info("[NFI]nfi_mpiServer_open(ID=%s): begin %s\n",server_aux->id,url);
	if(url[strlen(url)-1] == '/'){
		return nfi_mpiServer_opendir(serv, url, fho);
	}
	
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	
	if (fho == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		debug_nfi_error("nfi_mpiServer_open: url %s incorrect.\n",url);
		mpiServer_err(MPISERVERERR_URL);
		return -1;
	}
	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_mpiServer_fhandle *)malloc(sizeof(struct nfi_mpiServer_fhandle));
	if (fh_aux == NULL){		
		mpiServer_err(MPISERVERERR_MEMORY);
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	
	/*****************************************/
	msg.type = MPISERVER_OPEN_FILE;
	strcpy(msg.u_st_mpiServer_msg.op_open.path,dir);
	
        mpiServer_write_operation(server_aux->sd, &msg);
	debug_nfi_info("[NFI]nfi_mpiServer_open(ID=%s): open -> %s \n",server_aux->id,msg.u_st_mpiServer_msg.op_open.path);
	mpiServer_read_data(server_aux->sd, (char *)&fh_aux->fd, sizeof(int), msg.id);
	debug_nfi_info("[NFI]nfi_mpiServer_open(ID=%s): open <- %d \n",server_aux->id,fh_aux->fd);
	strcpy(fh_aux->path, dir);
	/*****************************************/
	
	fho->type = NFIFILE;			
	
	fho->server = NULL;
	fho->priv_fh = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;

	debug_nfi_info("[NFI]nfi_mpiServer_open(ID=%s): end\n",server_aux->id);
	return 0;
}

int nfi_mpiServer_close (struct nfi_server *server,  struct nfi_fhandle *fh)
{
	struct nfi_mpiServer_fhandle *fh_aux;
	struct nfi_mpiServer_server *server_aux;
	struct st_mpiServer_msg msg;

	server_aux = (struct nfi_mpiServer_server *) server->private_info;
	strcpy(msg.id,server_aux->id);
	debug_nfi_info("[NFI]nfi_mpiServer_close(ID=%s): begin\n",server_aux->id);

	// check params...
	if (server == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	
	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
		server_aux = (struct nfi_mpiServer_server *) server->private_info;
		/*****************************************/
		msg.type = MPISERVER_CLOSE_FILE;
		msg.u_st_mpiServer_msg.op_close.fd = fh_aux->fd;
	
        	mpiServer_write_operation(server_aux->sd, &msg);
			
		debug_nfi_info("[NFI]nfi_mpiServer_close(ID=%s): close -> %d \n",server_aux->id,msg.u_st_mpiServer_msg.op_close.fd);
		/*****************************************/
		//close(fh_aux->fd);
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;
	}
	
	fh->type = NFINULL;
	fh->server = NULL;
	debug_nfi_info("[NFI]nfi_mpiServer_close(ID=%s): end\n",server_aux->id);

	return 0;
}

ssize_t nfi_mpiServer_read(	struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
			//off_t offset)
{
	int ret, cont;

	struct nfi_mpiServer_server *server_aux;
	struct nfi_mpiServer_fhandle *fh_aux;
	struct st_mpiServer_msg msg;
	struct st_mpiServer_read_req req;

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	debug_nfi_info("[NFI]nfi_mpiServer_read(%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size);

	// check params...
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	
	/*****************************************/
		
	msg.type = MPISERVER_READ_FILE;
	msg.u_st_mpiServer_msg.op_read.fd 	= fh_aux->fd;
	msg.u_st_mpiServer_msg.op_read.offset 	= offset;
	msg.u_st_mpiServer_msg.op_read.size 	= size;

	#ifdef  DBG_IO
		debug_nfi_info("[NFI]read: -> fd %d \n",msg.u_st_mpiServer_msg.op_read.fd);
		debug_nfi_info("[NFI]read: -> offset %d \n",(int)msg.u_st_mpiServer_msg.op_read.offset);
		debug_nfi_info("[NFI]read: -> size %d \n",msg.u_st_mpiServer_msg.op_read.size);
	#endif

	ret = mpiServer_write_operation(server_aux->sd, &msg);
	if(ret == -1){
		perror("ERROR: (1)nfi_mpiServer_read: Error on write operation");
		debug_nfi_error("ERROR: (1)nfi_mpiServer_read: Error on write operation\n");
		return -1;
	}

	cont = 0;
	do{
		ret = mpiServer_read_data(server_aux->sd, (char *)&req, sizeof(struct st_mpiServer_read_req), msg.id);
		debug_nfi_info("[NFI]nfi_mpiServer_read(ID=%s): (1)mpiServer_read_data = %d.\n",server_aux->id, ret);
		if(ret == -1){
			perror("ERROR: (2)nfi_mpiServer_read: Error on write operation");
			debug_nfi_error("ERROR: (2)nfi_mpiServer_read: Error on write operation\n");
			return -1;
		}


		if(req.size > 0){
		debug_nfi_info("[NFI]nfi_mpiServer_read(ID=%s): (2)mpiServer_read_data = %d. size = %d cont = %d\n",server_aux->id, ret, req.size, cont);
			ret = mpiServer_read_data(server_aux->sd, (char *)buffer+cont, req.size, msg.id);
			debug_nfi_info("[NFI]nfi_mpiServer_read(ID=%s): (2)mpiServer_read_data = %d.\n",server_aux->id, ret);
			if(ret == -1){
				perror("ERROR: (3)nfi_mpiServer_read: Error on write operation");
				debug_nfi_error("ERROR: (3)nfi_mpiServer_read: Error on read operation\n");
				return -1;
			}
			//cont += req.size;
		}
		cont += req.size;
		debug_nfi_info("[NFI]nfi_mpiServer_read(ID=%s): read(%d,%d) cont = %d\n",server_aux->id,req.size,(int)req.last,cont);	
	}while((req.size>0)&&(!req.last));
	
	/*****************************************/
	debug_nfi_info("[NFI]nfi_mpiServer_read(ID=%s): read %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);
	if(req.size < 0){
		debug_nfi_error("ERROR: nfi_mpiServer_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)req.size);
		mpiServer_err(MPISERVERERR_READ);
		return -1;
	}

	debug_nfi_info("[NFI]nfi_mpiServer_read(ID=%s): end\n",server_aux->id);
	return req.size;
}



ssize_t nfi_mpiServer_write(struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
			//off_t offset)
{

        struct nfi_mpiServer_server *server_aux;
	struct nfi_mpiServer_fhandle *fh_aux;
	struct st_mpiServer_msg msg;
	struct st_mpiServer_write_req req;

	int ret;
	
	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	debug_nfi_info("[NFI]nfi_mpiServer_write(ID=%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size);

	// check params...
	if(size == 0){
		return 0;
	}	
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif


    	fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_mpiServer_server *) serv->private_info;

	/*****************************************/
	msg.type = MPISERVER_WRITE_FILE;
	msg.u_st_mpiServer_msg.op_write.fd 	= fh_aux->fd;
	msg.u_st_mpiServer_msg.op_write.offset 	= offset;
	msg.u_st_mpiServer_msg.op_write.size 	= size;

	#ifdef  DBG_IO
		debug_nfi_info("[NFI]write: -> fd %d \n",msg.u_st_mpiServer_msg.op_write.fd);
		debug_nfi_info("[NFI]write: -> offset %d \n",(int)msg.u_st_mpiServer_msg.op_write.offset);
		debug_nfi_info("[NFI]write: -> size %d \n",msg.u_st_mpiServer_msg.op_write.size);
	#endif

	ret = mpiServer_write_operation(server_aux->sd, &msg);
	if(ret == -1){
		debug_nfi_error("(1)ERROR: nfi_mpiServer_write(ID=%s): Error on write operation\n",server_aux->id);
		return -1;
	}


	ret = mpiServer_write_data(server_aux->sd, (char *)buffer, size, msg.id);
	if(ret == -1){
		debug_nfi_error("(2)ERROR: nfi_mpiServer_read(ID=%s): Error on write operation\n",server_aux->id);
		return -1;
	}

	ret = mpiServer_read_data(server_aux->sd, (char *)&req, sizeof(struct st_mpiServer_write_req), msg.id);
	if(ret == -1){
		debug_nfi_error("(3)ERROR: nfi_mpiServer_write(ID=%s): Error on write operation\n",server_aux->id);
		return -1;
	}
	
	
	/*****************************************/
	debug_nfi_info("[NFI]nfi_mpiServer_write(ID=%s): write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);
	if(req.size < 0){
		debug_nfi_error("ERROR: nfi_mpiServer_write(ID=%s): Fail write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);
		
		mpiServer_err(MPISERVERERR_WRITE);
		return -1;
	}

	debug_nfi_info("[NFI]nfi_mpiServer_write(ID=%s): end\n",server_aux->id);
	return req.size;
}

int nfi_mpiServer_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh){

	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;
        struct nfi_mpiServer_fhandle *fh_aux;
	struct st_mpiServer_msg msg;

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	debug_nfi_info("[NFI]nfi_mpiServer_create(ID=%s): begin %s\n",server_aux->id,url);

	// check params...
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (attr == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif
	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		debug_nfi_error("ERROR: nfi_mpiServer_create: url %s incorrect.\n",url);
		mpiServer_err(MPISERVERERR_URL);
		return -1;
	}
	/* private_info file handle */
	fh_aux = (struct nfi_mpiServer_fhandle *)malloc(sizeof(struct nfi_mpiServer_fhandle));
	if (fh_aux == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}

	bzero(fh_aux, sizeof(struct nfi_mpiServer_fhandle));
	

    	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	/* create the file into the directory */

	/*****************************************/
	msg.type = MPISERVER_CREAT_FILE;
	strcpy(msg.u_st_mpiServer_msg.op_creat.path,dir);
	
    	mpiServer_write_operation(server_aux->sd, &msg);
	mpiServer_read_data(server_aux->sd, (char *)&(fh_aux->fd), sizeof(int), msg.id);
	
	strcpy(fh_aux->path, dir);
	/*****************************************/
	
	fh->type = NFIFILE;			
	
	fh->server = serv;
	
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               	mpiServer_err(MPISERVERERR_MEMORY);
		free(fh_aux);
               	return -1;
        }
		
        strcpy(fh->url, url);

	debug_nfi_info("[NFI]nfi_mpiServer_create(ID=%s): end\n",server_aux->id);
	return 0;
}

int nfi_mpiServer_remove(struct nfi_server *serv,  char *url)
{

	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_mpiServer_server *server_aux;
	struct st_mpiServer_msg msg;
	
	server_aux = (struct nfi_mpiServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
	debug_nfi_info("[NFI]nfi_mpiServer_remove(%s): begin %s\n",server_aux->id, url);
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_mpiServer_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		debug_nfi_error("nfi_mpiServer_remove: url %s incorrect.\n",url);
		mpiServer_err(MPISERVERERR_URL);
		return -1;
	}
	/* remove the file into the directory */
	//ret = unlink(dir);
	/*****************************************/
	msg.type = MPISERVER_RM_FILE;
	strcpy(msg.u_st_mpiServer_msg.op_rm.path,dir);
	
        mpiServer_write_operation(server_aux->sd, &msg);
	
	/*****************************************/
	debug_nfi_info("[NFI]nfi_mpiServer_remove(ID=%s): end \n",server_aux->id);
	return 0;
}

int nfi_mpiServer_rename(struct nfi_server *server,  char *old_url, char *new_url)
{
	/*
        struct nfi_mpiServer_server *server_aux;
        struct nfi_mpiServer_fhandle *fh_aux;
	if (server == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			mpiServer_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif
	server_aux = (strcut nfi_mpiServer_server *)serv->private_info;
	*/

	/* ????? */
	return 0;
}

int nfi_mpiServer_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
        struct nfi_mpiServer_server *server_aux;
        struct nfi_mpiServer_fhandle *fh_aux;
	struct st_mpiServer_msg msg;

	// check params...
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (attr == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_mpiServer_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		debug_nfi_error("nfi_mpiServer_mkdir: url %s incorrect.\n",url);
		mpiServer_err(MPISERVERERR_URL);
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_mpiServer_fhandle *)malloc(sizeof(struct nfi_mpiServer_fhandle));
	if (fh_aux == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}


	bzero(fh_aux, sizeof(struct nfi_mpiServer_fhandle));
	/* create the dir into the directory */

	/******************************************************/	
	msg.type = MPISERVER_MKDIR_DIR;
	strcpy(msg.u_st_mpiServer_msg.op_mkdir.path, dir);
	
    	mpiServer_write_operation(server_aux->sd, &msg);
	mpiServer_read_data(server_aux->sd, (char *)&(fh_aux->fd), sizeof(int), msg.id);
	
	strcpy(fh_aux->path, dir);
	/******************************************************/

	if((ret < 0)&&(errno != EEXIST)){
		mpiServer_err(MPISERVERERR_MKDIR);
		debug_nfi_error("nfi_mpiServer_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server);
		free(fh_aux);
		return -1;
	}
	
	fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               mpiServer_err(MPISERVERERR_MEMORY);
	       free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

	//TODO:
	//MPISERVERtoNFIattr(attr, &st);
	
	return 0;
}

int nfi_mpiServer_rmdir(struct nfi_server *serv,  char *url)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;

        struct nfi_mpiServer_server *server_aux;
	struct st_mpiServer_msg msg;

	// check params...
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_mpiServer_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		debug_nfi_error("nfi_mpiServer_rmdir: url %s incorrect.\n",url);
		mpiServer_err(MPISERVERERR_URL);
		return -1;
	}


	/******************************************************/	
	msg.type = MPISERVER_RMDIR_DIR;
	strcpy(msg.u_st_mpiServer_msg.op_rmdir.path, url);
	
    	mpiServer_write_operation(server_aux->sd, &msg);
	mpiServer_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
	
	//strcpy(fh_aux->path, dir);
	/******************************************************/

	
	//ret = rmdir(dir);	
	if(ret < 0){
		debug_nfi_error("nfi_mpiServer_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server);
		mpiServer_err(MPISERVERERR_REMOVE);
		return -1;
	}

	return 0;
}

int nfi_mpiServer_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho)
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int ret;	
	struct nfi_mpiServer_server *server_aux;
	struct nfi_mpiServer_fhandle *fh_aux;

	// check params...
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (fho == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		debug_nfi_error("nfi_mpiServer_opendir: url %s incorrect.\n",url);
		mpiServer_err(MPISERVERERR_URL);
		return -1;
	}

	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_mpiServer_fhandle *)malloc(sizeof(struct nfi_mpiServer_fhandle));
	if (fh_aux == NULL){
		mpiServer_err(MPISERVERERR_MEMORY);
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_mpiServer_server *) serv->private_info;


	fh_aux->dir = opendir(dir);
    if (ret < 0 ){
		debug_nfi_error("nfi_mpiServer_opendir: Fail opendir %s in server %s.\n",dir,serv->server);
	    mpiServer_err(MPISERVERERR_MEMORY);		
		free(fh_aux);
		free(fho->url);
		return -1;		
	}
	
	fh_aux->fd = ret;
	strcpy(fh_aux->path, dir);	
	fho->type = NFIDIR;
	
	fho->server = NULL;
	fho->priv_fh = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;

	return 0;
}

int nfi_mpiServer_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, char *entry, unsigned char *type)
{
	struct dirent *ent;
        struct nfi_mpiServer_server *server_aux;
        struct nfi_mpiServer_fhandle *fh_aux;

	// check params...
	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (fh->priv_fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if(fh->type != NFIDIR){
		mpiServer_err(MPISERVERERR_NOTDIR);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
			/* mpiServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_mpiServer_server *)serv->private_info;
	fh_aux = (struct nfi_mpiServer_fhandle *)fh->priv_fh;

	entry[0] = '\0';
	ent = readdir(fh_aux->dir);

	if(ent == NULL){
		return 1;
	}
	if(type==NULL){
		return 0;
	}
	strcpy(entry, ent->d_name);
	//debug_nfi_info("[NFI]ent->d_name = %s S_ISDIR(%o) = %o\n", ent->d_name, ent->d_type,S_ISDIR(ent->d_type));
//	*type = ent->d_type;

	return 0;
}

int nfi_mpiServer_closedir(struct nfi_server *serv,  struct nfi_fhandle *fh)
{
	struct nfi_mpiServer_fhandle *fh_aux;

	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_mpiServer_fhandle *) fh->priv_fh;
		closedir(fh_aux->dir);
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;

	}
	
	return 0;
}

int nfi_mpiServer_statfs(struct nfi_server *serv,  struct nfi_info *inf)
{/*
	
	struct mpiServer_info mpiServerinf;
	int ret;
        struct nfi_mpiServer_server *server_aux;


	if (serv == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}

	if (inf == NULL){
		mpiServer_err(MPISERVERERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_mpiServer_reconnect(serv);
		if(ret <0){
		 mpiServer_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               mpiServer_err(MPISERVERERR_PARAM);
               return -1;
        }
#endif
	
	server_aux = (struct nfi_mpiServer_server *)serv->private_info;
	ret = mpiServer_statfs(server_aux->fh, &mpiServerinf, server_aux->cl);
	if(ret <0){
		mpiServer_err(MPISERVERERR_STATFS);
		return -1;
	}

	NFStoNFIInfo(inf, &mpiServerinf);
*/	
	return 0;
}

