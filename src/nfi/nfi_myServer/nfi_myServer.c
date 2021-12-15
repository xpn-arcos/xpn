#include "nfi_myServer.h"

/*
 * ENVIROMENT VARIABLES:
 *
 * XPN_DNS -> node{ID}	{hostname}	{port}
 */


/**********************************
Write the operation to realize
***********************************/
int myServer_write_operation(int sd, struct st_myServer_msg *head){
        int ret;


#ifdef DBG_NFI 
	printf("[NFI]ID=%s)myServer_write_data: begin\n", head->id);	
#endif
        ret = myServer_write_data(sd, (char *)&head->type, sizeof(head->type), head->id);
        if(ret == -1){
                return -1;
        }



#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)write_operation: %d -> \n", head->id, head->type);
#endif
        switch(head->type){
case MYSERVER_OPEN_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)OPEN operation\n",head->id);
#endif
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_open, sizeof(struct st_myServer_open), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case MYSERVER_CREAT_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)CREAT operation\n",head->id);
#endif
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_creat, sizeof(struct st_myServer_creat), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case MYSERVER_READ_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)READ operation\n",head->id);
#endif
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_read, sizeof(struct st_myServer_read), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case MYSERVER_WRITE_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)WRITE operation\n", head->id);
#endif
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_write, sizeof(struct st_myServer_write), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case MYSERVER_CLOSE_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)CLOSE operation\n",head->id);
#endif
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_close, sizeof(struct st_myServer_close), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case MYSERVER_RM_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)RM operation\n",head->id);
#endif
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_rm, sizeof(struct st_myServer_rm), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case MYSERVER_GETATTR_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)GETATTR operation\n",head->id);
#endif
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_getattr, sizeof(struct st_myServer_getattr), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case MYSERVER_MKDIR_DIR:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)MDKIR operation\n",head->id);
#endif
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_mkdir, sizeof(struct st_myServer_mkdir), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case MYSERVER_RMDIR_DIR:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)RMDIR operation\n",head->id);
#endif
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_rmdir, sizeof(struct st_myServer_rmdir), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case MYSERVER_FLUSH_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)FLUSH operation\n",head->id);
#endif
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_flush, sizeof(struct st_myServer_flush), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case MYSERVER_PRELOAD_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)PRELOAD operation\n",head->id);
#endif
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_preload, sizeof(struct st_myServer_preload), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case MYSERVER_FINALIZE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)FINALIZE operation\n",head->id);
#endif
/*
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_end, sizeof(struct st_myServer_end), head->id);
        if(ret == -1){
                return -1;
        }
*/
        break;
case MYSERVER_GETID:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)GETID operation\n",head->id);
#endif
/*
        ret = myServer_write_data(sd, (char *)head->id, MYSERVER_ID, head->id);
        if(ret == -1){
                return -1;
        }
*/
        break;
case MYSERVER_END:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)END operation\n",head->id);
#endif
/*
        ret = myServer_write_data(sd, (char *)&head->u_st_myServer_msg.op_end, sizeof(struct st_myServer_end), head->id);
        if(ret == -1){
                return -1;
        }
*/
        break;
        }
        return 0;
        //return head->type;
}

/***************************************************************/

/************************************************************
 * PRIVATE FUNCTIONS TO USE myServer SERVERS	            *
 ************************************************************/
void NFItoMYSERVERattr(struct stat *att, struct nfi_attr *nfi_att)
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

void MYSERVERtoNFIattr(struct nfi_attr *nfi_att, struct stat *att)
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

void MYSERVERtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *myServer_inf)
{
}

/************************************************************
 * PUBLIC FUNCTIONS TO USE MYSERVER			    *
 ************************************************************/

/************************************************************
 * Init Local 					    	    *
 ************************************************************/
int nfi_myServer_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr)
{
	char server[MAXPATHLEN], dir[MAXPATHLEN], prt[MAXPATHLEN];
	
	int ret;
	struct nfi_myServer_server *server_aux;
	struct st_myServer_msg msg;        


#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_init: begin\n");
#endif	

#ifdef DBG_NFI 
	printf("[NFI]nfi_myServer_init: sizeof(struct st_myServer_msg) = %d.\n", sizeof(struct st_myServer_msg));
#endif	
	
	if(serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	/* functions */
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
	if (serv->ops == NULL){
		myServer_err(MYSERVERERR_MEMORY);
		return -1;
	}

	bzero(serv->ops, sizeof(struct nfi_ops));
	
	serv->ops->nfi_preload	= nfi_myServer_preload;
	serv->ops->nfi_flush	= nfi_myServer_flush;
	
	serv->ops->nfi_reconnect  = nfi_myServer_reconnect;
	serv->ops->nfi_disconnect = nfi_myServer_disconnect;

	serv->ops->nfi_getattr	= nfi_myServer_getattr;
	serv->ops->nfi_setattr	= nfi_myServer_setattr;

	serv->ops->nfi_open	= nfi_myServer_open;
	serv->ops->nfi_close	= nfi_myServer_close;

	serv->ops->nfi_read	= nfi_myServer_read;
	serv->ops->nfi_write	= nfi_myServer_write;

	serv->ops->nfi_create	= nfi_myServer_create;
	serv->ops->nfi_remove	= nfi_myServer_remove;
	serv->ops->nfi_rename	= nfi_myServer_rename;

	serv->ops->nfi_mkdir	= nfi_myServer_mkdir;
	serv->ops->nfi_rmdir	= nfi_myServer_rmdir;
	serv->ops->nfi_opendir	= nfi_myServer_opendir;
	serv->ops->nfi_readdir	= nfi_myServer_readdir;
	serv->ops->nfi_closedir	= nfi_myServer_closedir;
	serv->ops->nfi_statfs	= nfi_myServer_statfs;

	ret = ParseURL(url,  prt, NULL, NULL, server,  NULL,  dir);
	if(ret <0 ){
		myServer_err(MYSERVERERR_URL);
		fprintf(stderr,"nfi_myServer_init: url %s incorrect.\n",url);
		free(serv->ops);
		return -1;
	}
	server_aux = (struct nfi_myServer_server *)malloc(sizeof(struct nfi_myServer_server));
	if(server_aux == NULL){
		myServer_err(MYSERVERERR_MEMORY);
		return -1;
	}

	/******************************************/

	server_aux->sd = myServer_connect(server);
	strcpy(msg.id, "GETID");
        msg.type = MYSERVER_GETID;                                                                                   
        myServer_write_operation(server_aux->sd, &msg);                                                                     
#ifdef  DBG_NFI
	printf("[NFI]nfi_myServer_init: myServer_write_data\n");
#endif
	myServer_read_data(server_aux->sd, (char *)server_aux->id, MYSERVER_ID, msg.id);  
#ifdef  DBG_NFI
	printf("[NFI]nfi_myServer_init: myServer_read_data id = %s\n",server_aux->id);
#endif
	/******************************************/
	
	serv->private_info = (void *)server_aux;

	//serv->protocol = MYSERVER;
	
	serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1));	/* server address */
	if(serv->server == NULL){
		myServer_err(MYSERVERERR_MEMORY);
		return -1;
	}
	strcpy(serv->server, server);

	serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1));	/* server address */
	if(serv->url == NULL){
		myServer_err(MYSERVERERR_MEMORY);
		return -1;
	}

	strcpy(serv->url, url);
        serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
	memset(serv->wrk, 0, sizeof(struct nfi_worker));


#ifdef _ALL_THREADS_
//	nfi_worker_init(serv->wrk, serv, 1);
//	nfi_worker_init(serv->wrk, serv, 0);

#else
	if(strcmp("myServer", prt) == 0){
#if defined(DEBUG_NFI)
		printf("[NFI]myServer\n");
#endif
#ifdef  DBG_NFI 
	printf("[NFI]nfi_worker_init(1,ID=%s): \n",server_aux->id);
#endif
		nfi_worker_init(serv->wrk, serv, 1);
	}
	else{
#ifdef  DBG_NFI 
	printf("[NFI]nfi_worker_init(0,ID=%s): \n",server_aux->id);
#endif
		nfi_worker_init(serv->wrk, serv, 0);
	}
#endif	
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_init(ID=%s): end\n",server_aux->id);
#endif
	return 0;
}

/************************************************************
 * Disconnect to the server				    *
 * **********************************************************/
int nfi_myServer_disconnect(struct nfi_server *serv)
{

        struct nfi_myServer_server *server_aux;
        

	if (serv == NULL)
	       return 0;


        server_aux = (struct nfi_myServer_server *)serv->private_info;
        if(server_aux != NULL){
        	close(server_aux->sd);
			//printf("[NFI]close la conexion\n");
                free(serv->private_info);
	}

    if(serv->url != NULL){
        	free(serv->url);
	}

	if(serv->server != NULL){
		free(serv->server);
	}

	//serv->protocol = -1;

	return 0;
}



/************************************************************
 * Reconnect to the NFS server				    *
 ************************************************************/
int nfi_myServer_reconnect(struct nfi_server *serv)
{
	/* Don't see the serv result */
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_myServer_server *server_aux;

	ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret <0 ){
		myServer_err(MYSERVERERR_URL);
		fprintf(stderr,"nfi_myServer_reconnect: url %s incorrect.\n",serv->url);
		return -1;
	}

	server_aux = (struct nfi_myServer_server *)malloc(sizeof(struct nfi_myServer_server));
	if(server_aux == NULL){		
		myServer_err(MYSERVERERR_MEMORY);
		return -1;
	}

	strcpy(server_aux->path, dir);

	serv->private_info = (void *)server_aux;
	return 0;
}

/************************************************************
 * Destroy NFS operations				    *
 ************************************************************/

int nfi_myServer_destroy(struct nfi_server *serv)
{

	struct nfi_myServer_server *server_aux;
	struct st_myServer_msg msg;

	if (serv == NULL)
		return 0;

	if(serv->ops != NULL)
		free(serv->ops);

	server_aux = (struct nfi_myServer_server *)serv->private_info;

	if(server_aux != NULL){
		//printf("[NFI]Cierro la conexion\n");
		msg.type = -1;
        	myServer_write_operation(server_aux->sd, &msg);
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

int nfi_myServer_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){


	struct nfi_myServer_server *server_aux;
	struct nfi_myServer_fhandle *fh_aux;
	struct st_myServer_msg msg;
	struct st_myServer_attr_req req;

	server_aux = (struct nfi_myServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_getattr(ID=%s): begin\n",server_aux->id);
#endif	
	if (attr == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}


	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif
	fh_aux = (struct nfi_myServer_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_myServer_server *) serv->private_info;

	
	
	/*****************************************/

	msg.type = MYSERVER_GETATTR_FILE; 
	strcpy(msg.u_st_myServer_msg.op_getattr.path, fh_aux->path);

        myServer_write_operation(server_aux->sd, &msg);
#ifdef DBG_NFI 
	printf("[NFI]nfi_myServer_getattr(ID=%s): getattr: -> %s \n", server_aux->id, msg.u_st_myServer_msg.op_getattr.path);
#endif
	bzero(&req, sizeof(struct st_myServer_attr_req));
	myServer_read_data(server_aux->sd, (char *)&req, sizeof(struct st_myServer_attr_req), msg.id);
#ifdef DBG_NFI 
	printf("[NFI]nfi_myServer_getattr(ID=%s): getattr: <- %d\n", server_aux->id, req.status);
#endif
	/*****************************************/	
	
	/*
	ret = stat(fh_aux->path, &st);
	if(ret < 0){
		fprintf(stderr,"nfi_myServer_getattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		myServer_err(MYSERVERERR_GETATTR);
		return -1;
	}
	*/

	MYSERVERtoNFIattr(attr, &req.attr);
	
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_getattr(ID=%s): end\n",server_aux->id);
#endif
	return req.status;
}


int nfi_myServer_setattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){

	struct nfi_myServer_server *server_aux;
	struct nfi_myServer_fhandle *fh_aux;


	if (attr == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_myServer_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_myServer_server *) serv->private_info;

 //	NFItoMYSERVERattr(&st, attr);
	/* no se comp hacer el setattr */
	/*
	ret = stat(fh_aux->path, &st);
	*/
/*
	ret = truncate(fh_aux->path, st.st_myServer_size);
	if(ret < 0){
		fprintf(stderr,"nfi_myServer_setattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		myServer_err(MYSERVERERR_GETATTR);
		return -1;
	}
*/	
	return 0;
}


int nfi_myServer_preload(struct nfi_server *serv, char *url, char *virtual_path, char *storage_path, int opt)
{
	//char dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_myServer_server *server_aux;
	struct st_myServer_msg msg;
	
	server_aux = (struct nfi_myServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_preload(ID=%s): begin %s - %s \n", server_aux->id, virtual_path, storage_path);
#endif
	if(url == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if(url[strlen(url)-1] == '/'){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}
	
	if (virtual_path == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if (storage_path == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_myServer_server *) serv->private_info;
#ifdef DBG_NFI 
	printf("[NFI]nfi_myServer_preload(ID=%s): preload %s in server %s.\n",server_aux->id,virtual_path,serv->server);
#endif
	
	/*****************************************/
	msg.type = MYSERVER_PRELOAD_FILE; 
	//strcpy(msg.u_st_myServer_msg.op_preload.path,dir);
	strcpy(msg.u_st_myServer_msg.op_preload.virtual_path,virtual_path);
	strcpy(msg.u_st_myServer_msg.op_preload.storage_path,storage_path);
	msg.u_st_myServer_msg.op_preload.opt = opt;

        myServer_write_operation(server_aux->sd, &msg);
#ifdef DBG_NFI 
	printf("[NFI]nfi_myServer_preload(ID=%s): preload: -> %s \n",server_aux->id,msg.u_st_myServer_msg.op_preload.virtual_path);
#endif
	myServer_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
#ifdef DBG_NFI 
	printf("[NFI]nfi_myServer_preload(ID=%s): preload: <- %d \n",server_aux->id,ret);
#endif
	/*****************************************/
	
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_preload(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret);
#endif
	if(ret == -1){
		printf("[NFI]Error en el preload\n");
	}

	return ret;
}


int nfi_myServer_flush(struct nfi_server *serv,  char *url, char *virtual_path, char *storage_path, int opt)
{
	//char dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_myServer_server *server_aux;

	struct st_myServer_msg msg;
	
	server_aux = (struct nfi_myServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_flush(ID=%s): begin %s - %s \n", server_aux->id,virtual_path, storage_path);
#endif
	if(url == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if(url[strlen(url)-1] == '/'){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}
	
	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}
	
	if (virtual_path == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if (storage_path == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_myServer_server *) serv->private_info;
#ifdef DBG_NFI 
	printf("[NFI]nfi_myServer_flush(ID=%s): open %s in server %s.\n",server_aux->id,virtual_path,serv->server);
#endif
	
	/*****************************************/
	msg.type = MYSERVER_FLUSH_FILE; 
	//strcpy(msg.u_st_myServer_msg.op_flush.path,dir);
	strcpy(msg.u_st_myServer_msg.op_flush.virtual_path,virtual_path);
	strcpy(msg.u_st_myServer_msg.op_flush.storage_path,storage_path);
	msg.u_st_myServer_msg.op_flush.opt = opt;

        myServer_write_operation(server_aux->sd, &msg);
#ifdef DBG_NFI 
	printf("[NFI]nfi_myServer_flush(ID=%s): flush: -> %s \n",server_aux->id,msg.u_st_myServer_msg.op_flush.virtual_path);
#endif
	myServer_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
#ifdef DBG_NFI 
	printf("[NFI]nfi_myServer_flush(ID=%s): flush: <- %d \n",server_aux->id,ret);
#endif
	/*****************************************/
	
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_flush(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret);
#endif
	return 0;
}



int nfi_myServer_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int ret;
	struct nfi_myServer_server *server_aux;
	struct nfi_myServer_fhandle *fh_aux;

	struct st_myServer_msg msg;
	
	server_aux = (struct nfi_myServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_open(ID=%s): begin %s\n",server_aux->id,url);
#endif
	if(url[strlen(url)-1] == '/'){
		return nfi_myServer_opendir(serv, url, fho);
	}
	
	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}
	
	if (fho == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_myServer_open: url %s incorrect.\n",url);
		myServer_err(MYSERVERERR_URL);
		return -1;
	}
	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		myServer_err(MYSERVERERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_myServer_fhandle *)malloc(sizeof(struct nfi_myServer_fhandle));
	if (fh_aux == NULL){		
		myServer_err(MYSERVERERR_MEMORY);
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_myServer_server *) serv->private_info;
	
	/*****************************************/
	msg.type = MYSERVER_OPEN_FILE; 
	strcpy(msg.u_st_myServer_msg.op_open.path,dir);
	
        myServer_write_operation(server_aux->sd, &msg);
#ifdef DBG_NFI 
	printf("[NFI]nfi_myServer_open(ID=%s): open -> %s \n",server_aux->id,msg.u_st_myServer_msg.op_open.path);
#endif
	myServer_read_data(server_aux->sd, (char *)&fh_aux->fd, sizeof(int), msg.id);
#ifdef DBG_NFI 
	printf("[NFI]nfi_myServer_open(ID=%s): open <- %d \n",server_aux->id,fh_aux->fd);
#endif
	strcpy(fh_aux->path, dir);
	/*****************************************/
	
	fho->type = NFIFILE;			
	
	fho->server = NULL;
	fho->priv_fh = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;

#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_open(ID=%s): end\n",server_aux->id);
#endif
	return 0;

}





int nfi_myServer_close(struct nfi_server *server,  struct nfi_fhandle *fh){
	
	struct nfi_myServer_fhandle *fh_aux;
	struct nfi_myServer_server *server_aux;
	struct st_myServer_msg msg;

	server_aux = (struct nfi_myServer_server *) server->private_info;
	strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_close(ID=%s): begin\n",server_aux->id);
#endif
	if (server == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}
	
	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_myServer_fhandle *) fh->priv_fh;
		server_aux = (struct nfi_myServer_server *) server->private_info;
		/*****************************************/
		msg.type = MYSERVER_CLOSE_FILE; 
		msg.u_st_myServer_msg.op_close.fd = fh_aux->fd;
	
        	myServer_write_operation(server_aux->sd, &msg);
			
#ifdef  DBG_NFI 
		printf("[NFI]nfi_myServer_close(ID=%s): close -> %d \n",server_aux->id,msg.u_st_myServer_msg.op_close.fd);
#endif
		/*****************************************/
		//close(fh_aux->fd);
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;
	}
	
	fh->type = NFINULL;
	fh->server = NULL;
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_close(ID=%s): end\n",server_aux->id);
#endif

	return 0;
}

ssize_t nfi_myServer_read(	struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
			//off_t offset)
{

	int ret, cont;

	struct nfi_myServer_server *server_aux;
	struct nfi_myServer_fhandle *fh_aux;
	struct st_myServer_msg msg;
	struct st_myServer_read_req req;

	server_aux = (struct nfi_myServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_read(%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size);
#endif
	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_myServer_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_myServer_server *) serv->private_info;
	
	/*****************************************/
		
	msg.type = MYSERVER_READ_FILE; 
	msg.u_st_myServer_msg.op_read.fd 	= fh_aux->fd;
	msg.u_st_myServer_msg.op_read.offset 	= offset;
	msg.u_st_myServer_msg.op_read.size 	= size;

	#ifdef  DBG_IO
		printf("[NFI]read: -> fd %d \n",msg.u_st_myServer_msg.op_read.fd);
		printf("[NFI]read: -> offset %d \n",(int)msg.u_st_myServer_msg.op_read.offset);
		printf("[NFI]read: -> size %d \n",msg.u_st_myServer_msg.op_read.size);
	#endif  

	ret = myServer_write_operation(server_aux->sd, &msg);
	if(ret == -1){
		perror("ERROR: (1)nfi_myServer_read: Error on write operation");
		fprintf(stderr,"ERROR: (1)nfi_myServer_read: Error on write operation\n");
		return -1;
	}

	cont = 0;
	do{
		ret = myServer_read_data(server_aux->sd, (char *)&req, sizeof(struct st_myServer_read_req), msg.id);
#ifdef DBG_NFI 
		printf("[NFI]nfi_myServer_read(ID=%s): (1)myServer_read_data = %d.\n",server_aux->id, ret);
#endif		
		if(ret == -1){
			perror("ERROR: (2)nfi_myServer_read: Error on write operation");
			fprintf(stderr,"ERROR: (2)nfi_myServer_read: Error on write operation\n");
			return -1;
		}


		if(req.size > 0){
#ifdef DBG_NFI 
		printf("[NFI]nfi_myServer_read(ID=%s): (2)myServer_read_data = %d. size = %d cont = %d\n",server_aux->id, ret, req.size, cont);
#endif		
			ret = myServer_read_data(server_aux->sd, (char *)buffer+cont, req.size, msg.id);
#ifdef DBG_NFI 
			printf("[NFI]nfi_myServer_read(ID=%s): (2)myServer_read_data = %d.\n",server_aux->id, ret);
#endif		
			if(ret == -1){
				perror("ERROR: (3)nfi_myServer_read: Error on write operation");
				fprintf(stderr,"ERROR: (3)nfi_myServer_read: Error on read operation\n");
				return -1;
			}
			//cont += req.size;
		}
		cont += req.size;
#ifdef  DBG_NFI 
		printf("[NFI]nfi_myServer_read(ID=%s): read(%d,%d) cont = %d\n",server_aux->id,req.size,(int)req.last,cont);	
#endif
	}while((req.size>0)&&(!req.last));
	
	/*****************************************/
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_read(ID=%s): read %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);
#endif
	if(req.size < 0){
		fprintf(stderr,"ERROR: nfi_myServer_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)req.size);
		myServer_err(MYSERVERERR_READ);
		return -1;
	}

#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_read(ID=%s): end\n",server_aux->id);
#endif
	return req.size;
}



ssize_t nfi_myServer_write(struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
			//off_t offset)
{

        struct nfi_myServer_server *server_aux;
	struct nfi_myServer_fhandle *fh_aux;
	struct st_myServer_msg msg;
	struct st_myServer_write_req req;

	int ret;
	
	server_aux = (struct nfi_myServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI
	printf("[NFI]nfi_myServer_write(ID=%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size);
#endif
	if(size == 0){
		return 0;
	}	

	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif


    	fh_aux = (struct nfi_myServer_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_myServer_server *) serv->private_info;

	/*****************************************/
	msg.type = MYSERVER_WRITE_FILE; 
	msg.u_st_myServer_msg.op_write.fd 	= fh_aux->fd;
	msg.u_st_myServer_msg.op_write.offset 	= offset;
	msg.u_st_myServer_msg.op_write.size 	= size;

	#ifdef  DBG_IO
		printf("[NFI]write: -> fd %d \n",msg.u_st_myServer_msg.op_write.fd);
		printf("[NFI]write: -> offset %d \n",(int)msg.u_st_myServer_msg.op_write.offset);
		printf("[NFI]write: -> size %d \n",msg.u_st_myServer_msg.op_write.size);
	#endif  

	ret = myServer_write_operation(server_aux->sd, &msg);
	if(ret == -1){
		fprintf(stderr,"(1)ERROR: nfi_myServer_write(ID=%s): Error on write operation\n",server_aux->id);
		return -1;
	}


	ret = myServer_write_data(server_aux->sd, (char *)buffer, size, msg.id);
	if(ret == -1){
		fprintf(stderr,"(2)ERROR: nfi_myServer_read(ID=%s): Error on write operation\n",server_aux->id);
		return -1;
	}

	ret = myServer_read_data(server_aux->sd, (char *)&req, sizeof(struct st_myServer_write_req), msg.id);
	if(ret == -1){
		fprintf(stderr,"(3)ERROR: nfi_myServer_write(ID=%s): Error on write operation\n",server_aux->id);
		return -1;
	}
	
	
	/*****************************************/
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_write(ID=%s): write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);
#endif
	if(req.size < 0){
		fprintf(stderr,"ERROR: nfi_myServer_write(ID=%s): Fail write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);
		
		myServer_err(MYSERVERERR_WRITE);
		return -1;
	}

#ifdef  DBG_NFI
	printf("[NFI]nfi_myServer_write(ID=%s): end\n",server_aux->id);
#endif
	return req.size;
}

int nfi_myServer_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh){

	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_myServer_server *server_aux;
        struct nfi_myServer_fhandle *fh_aux;
	struct st_myServer_msg msg;

	server_aux = (struct nfi_myServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_create(ID=%s): begin %s\n",server_aux->id,url);
#endif
	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif
	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"ERROR: nfi_myServer_create: url %s incorrect.\n",url);
		myServer_err(MYSERVERERR_URL);
		return -1;
	}
	/* private_info file handle */
	fh_aux = (struct nfi_myServer_fhandle *)malloc(sizeof(struct nfi_myServer_fhandle));
	if (fh_aux == NULL){
		myServer_err(MYSERVERERR_MEMORY);
		return -1;
	}

	bzero(fh_aux, sizeof(struct nfi_myServer_fhandle));
	

    	server_aux = (struct nfi_myServer_server *) serv->private_info;
	/* create the file into the directory */

	/*****************************************/
	msg.type = MYSERVER_CREAT_FILE; 
	strcpy(msg.u_st_myServer_msg.op_creat.path,dir);
	
    	myServer_write_operation(server_aux->sd, &msg);
	myServer_read_data(server_aux->sd, (char *)&(fh_aux->fd), sizeof(int), msg.id);
	
	strcpy(fh_aux->path, dir);
	/*****************************************/
	
	fh->type = NFIFILE;			
	
	fh->server = serv;
	
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               	myServer_err(MYSERVERERR_MEMORY);
		free(fh_aux);
               	return -1;
        }
		
        strcpy(fh->url, url);

#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_create(ID=%s): end\n",server_aux->id);
#endif
	return 0;
}

int nfi_myServer_remove(struct nfi_server *serv,  char *url)
{

	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_myServer_server *server_aux;
	struct st_myServer_msg msg;
	
	server_aux = (struct nfi_myServer_server *) serv->private_info;
	strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_remove(%s): begin %s\n",server_aux->id, url);
#endif	
	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_myServer_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_myServer_remove: url %s incorrect.\n",url);
		myServer_err(MYSERVERERR_URL);
		return -1;
	}
	/* remove the file into the directory */
	//ret = unlink(dir);
	/*****************************************/
	msg.type = MYSERVER_RM_FILE; 
	strcpy(msg.u_st_myServer_msg.op_rm.path,dir);
	
        myServer_write_operation(server_aux->sd, &msg);
	
	/*****************************************/
#ifdef  DBG_NFI 
	printf("[NFI]nfi_myServer_remove(ID=%s): end \n",server_aux->id);
#endif
	return 0;
}

int nfi_myServer_rename(struct nfi_server *server,  char *old_url, char *new_url)
{
	/*
        struct nfi_myServer_server *server_aux;
        struct nfi_myServer_fhandle *fh_aux;
	if (server == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			myServer_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif
	server_aux = (strcut nfi_myServer_server *)serv->private_info;
	*/

	/* ????? */
	return 0;
}

int nfi_myServer_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
        struct nfi_myServer_server *server_aux;
        struct nfi_myServer_fhandle *fh_aux;
	struct st_myServer_msg msg;

	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_myServer_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_myServer_mkdir: url %s incorrect.\n",url);
		myServer_err(MYSERVERERR_URL);
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_myServer_fhandle *)malloc(sizeof(struct nfi_myServer_fhandle));
	if (fh_aux == NULL){
		myServer_err(MYSERVERERR_MEMORY);
		return -1;
	}


	bzero(fh_aux, sizeof(struct nfi_myServer_fhandle));
	/* create the dir into the directory */

	/******************************************************/	
	msg.type = MYSERVER_MKDIR_DIR; 
	strcpy(msg.u_st_myServer_msg.op_mkdir.path, dir);
	
    	myServer_write_operation(server_aux->sd, &msg);
	myServer_read_data(server_aux->sd, (char *)&(fh_aux->fd), sizeof(int), msg.id);
	
	strcpy(fh_aux->path, dir);
	/******************************************************/

	if((ret < 0)&&(errno != EEXIST)){
		myServer_err(MYSERVERERR_MKDIR);
		fprintf(stderr,"nfi_myServer_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server);
		free(fh_aux);
		return -1;
	}
	
	fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               myServer_err(MYSERVERERR_MEMORY);
	       free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

	//TODO:
	//MYSERVERtoNFIattr(attr, &st);
	
	return 0;
}

int nfi_myServer_rmdir(struct nfi_server *serv,  char *url)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;

        struct nfi_myServer_server *server_aux;
	struct st_myServer_msg msg;

	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_myServer_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_myServer_rmdir: url %s incorrect.\n",url);
		myServer_err(MYSERVERERR_URL);
		return -1;
	}


	/******************************************************/	
	msg.type = MYSERVER_RMDIR_DIR; 
	strcpy(msg.u_st_myServer_msg.op_rmdir.path, url);
	
    	myServer_write_operation(server_aux->sd, &msg);
	myServer_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
	
	//strcpy(fh_aux->path, dir);
	/******************************************************/

	
	//ret = rmdir(dir);	
	if(ret < 0){
		fprintf(stderr,"nfi_myServer_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server);
		myServer_err(MYSERVERERR_REMOVE);
		return -1;
	}

	return 0;
}

int nfi_myServer_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho)
{
        
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int ret;	
	struct nfi_myServer_server *server_aux;
	struct nfi_myServer_fhandle *fh_aux;


	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if (fho == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_myServer_opendir: url %s incorrect.\n",url);
		myServer_err(MYSERVERERR_URL);
		return -1;
	}

	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		myServer_err(MYSERVERERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_myServer_fhandle *)malloc(sizeof(struct nfi_myServer_fhandle));
	if (fh_aux == NULL){
		myServer_err(MYSERVERERR_MEMORY);
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_myServer_server *) serv->private_info;


	fh_aux->dir = opendir(dir);
    if (ret < 0 ){
		fprintf(stderr,"nfi_myServer_opendir: Fail opendir %s in server %s.\n",dir,serv->server);
	    myServer_err(MYSERVERERR_MEMORY);		
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

int nfi_myServer_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, char *entry, unsigned char *type)
{
	struct dirent *ent;

        struct nfi_myServer_server *server_aux;
        struct nfi_myServer_fhandle *fh_aux;


	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if(fh->type != NFIDIR){
		myServer_err(MYSERVERERR_NOTDIR);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
			/* myServer_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_myServer_server *)serv->private_info;
	fh_aux = (struct nfi_myServer_fhandle *)fh->priv_fh;

	entry[0] = '\0';
	ent = readdir(fh_aux->dir);

	if(ent == NULL){
		return 1;
	}
	if(type==NULL){
		return 0;
	}
	strcpy(entry, ent->d_name);
	//printf("[NFI]ent->d_name = %s S_ISDIR(%o) = %o\n", ent->d_name, ent->d_type,S_ISDIR(ent->d_type));
//	*type = ent->d_type;

	return 0;
}

int nfi_myServer_closedir(struct nfi_server *serv,  struct nfi_fhandle *fh)
{
	
	struct nfi_myServer_fhandle *fh_aux;

	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_myServer_fhandle *) fh->priv_fh;
		closedir(fh_aux->dir);
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;

	}
	
	return 0;
	
}


int nfi_myServer_statfs(struct nfi_server *serv,  struct nfi_info *inf)
{/*
	
	struct myServer_info myServerinf;
	int ret;
        struct nfi_myServer_server *server_aux;


	if (serv == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}

	if (inf == NULL){
		myServer_err(MYSERVERERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_myServer_reconnect(serv);
		if(ret <0){
		 myServer_err(); not necessary 
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               myServer_err(MYSERVERERR_PARAM);
               return -1;
        }
#endif
	
	server_aux = (struct nfi_myServer_server *)serv->private_info;
	ret = myServer_statfs(server_aux->fh, &myServerinf, server_aux->cl);
	if(ret <0){
		myServer_err(MYSERVERERR_STATFS);
		return -1;
	}

	NFStoNFIInfo(inf, &myServerinf);
*/	
	return 0;
}
