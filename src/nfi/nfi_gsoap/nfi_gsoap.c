#include "nfi/nfi_gsoap/gsoap_lib.h"

/*
 * TODO:
 *  -> local_tcp, local_udp ??????????????
 *  -> local_xxx a nfi_gsoap_xxx
 *  -> local_misc.[ch]:
 *     -> ParseURL
 *  -> gsoap_error.[ch]:
 *     -> gsoap_err
 *
 * -> pasar CLIENT * a servidor (es comn a todos...)
 * -> Poner todo a punteros
 * -> struct nfi_fhandle ** Paso de puntero par�etro por referencia
 *
 */


/************************************************************
 * PRIVATE FUNCTIONS TO USE local SERVERS	            *
 ************************************************************/
void NFItoGSOAPattr(struct stat *att, struct nfi_attr *nfi_att)
{
	if(nfi_att->at_type == NFIFILE){
		att->st_mode	= nfi_att->at_mode | S_IFREG;     /* protection */
	}
	
	if(nfi_att->at_type == NFIDIR){
		att->st_mode	= nfi_att->at_mode | S_IFDIR;     /* protection */
	}
	
	att->st_size	= nfi_att->at_size;     /* size */
	att->st_uid	= nfi_att->at_uid;      /* user ID of owner */
	att->st_gid	= nfi_att->at_gid;      /* group ID of owner */
	att->st_blksize	= nfi_att->at_blksize;  /* blocksize for filesystem I/O */
	att->st_blocks	= nfi_att->at_blocks;   /* number of blocks allocated */
	att->st_atime	= nfi_att->at_atime;    /* time of last access */
	att->st_mtime	= nfi_att->at_mtime;    /* time of last modification */
	att->st_ctime	= nfi_att->at_ctime;    /* time of last change */
}

void GSOAPtoNFIattr(struct nfi_attr *nfi_att, struct stat *att)
{
	if(S_ISREG(att->st_mode)){
		nfi_att->at_type = NFIFILE;
	}
	
	if(S_ISDIR(att->st_mode)){
		nfi_att->at_type = NFIDIR;
	}
	
	nfi_att->at_size	= att->st_size;     /* size */
	nfi_att->at_mode	= att->st_mode&(S_IRWXU|S_IRWXG|S_IRWXO); /* protection */
	nfi_att->at_uid		= att->st_uid;      /* user ID of owner */
	nfi_att->at_gid		= att->st_gid;      /* group ID of owner */
	nfi_att->at_blksize	= att->st_blksize;  /* blocksize for filesystem I/O*/
	nfi_att->at_blocks	= att->st_blocks;   /* number of blocks allocated */
	nfi_att->at_atime	= att->st_atime;    /* time of last access */
	nfi_att->at_mtime	= att->st_mtime;    /* time of last modification */
	nfi_att->at_ctime	= att->st_ctime;    /* time of last change */
}

void GSOAPtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *local_inf)
{
}

/****************************************************************
 * PUBLIC FUNCTIONS TO USE GSOAP				*
 ****************************************************************/

/****************************************************************
 * Init Local 							*
 ****************************************************************/
int nfi_gsoap_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr)
{
	char protocol[MAXPATHLEN], server[MAXPATHLEN], port[MAXPATHLEN], dir[MAXPATHLEN];
	int ret, p;
	//char server2[MAXPATHLEN];
	//char *http_prefix="http://";
	char *https_prefix="https://";
	struct nfi_gsoap_server *server_aux;
	
	if(serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
	/* functions */
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
	if (serv->ops == NULL){
		gsoap_err(GSOAPERR_MEMORY);
		return -1;
	}
	
	serv->ops->nfi_reconnect  = nfi_gsoap_reconnect;
	serv->ops->nfi_disconnect = nfi_gsoap_disconnect;
	
	serv->ops->nfi_getattr	= nfi_gsoap_getattr;
	serv->ops->nfi_setattr	= nfi_gsoap_setattr;
	
	serv->ops->nfi_open	= nfi_gsoap_open;
	serv->ops->nfi_close	= nfi_gsoap_close;
	
	serv->ops->nfi_read	= nfi_gsoap_read;
	serv->ops->nfi_write	= nfi_gsoap_write;
	
	serv->ops->nfi_create	= nfi_gsoap_create;
	serv->ops->nfi_remove	= nfi_gsoap_remove;
	serv->ops->nfi_rename	= nfi_gsoap_rename;
	
	serv->ops->nfi_mkdir	= nfi_gsoap_mkdir;
	serv->ops->nfi_rmdir	= nfi_gsoap_rmdir;
	serv->ops->nfi_opendir	= nfi_gsoap_opendir;
	serv->ops->nfi_readdir	= nfi_gsoap_readdir;
	serv->ops->nfi_closedir	= nfi_gsoap_closedir;
	serv->ops->nfi_statfs	= nfi_gsoap_statfs;
	
	ret = ParseURL(url, protocol, NULL, NULL, server, port, dir);
	if (ret < 0){
		gsoap_err(GSOAPERR_URL);
		fprintf(stderr,"nfi_gsoap_init: url %s incorrect.\n",url);
		free(serv->ops);
		return -1;
	}
	
	p = (int)strtol(port, NULL, 10);
	if (p <= 0){
		gsoap_err(GSOAPERR_URL);
		fprintf(stderr,"nfi_gsoap_init: port %s incorrect.\n",port);
		free(serv->ops);
		return -1;
	}
	
	server_aux = (struct nfi_gsoap_server *)malloc(sizeof(struct nfi_gsoap_server));
	if(server_aux == NULL){
		gsoap_err(GSOAPERR_MEMORY);
		return -1;
	}
	
	strcpy(server_aux->path, https_prefix);	
	strcpy(server_aux->path+strlen(https_prefix), server);
#if defined(DEBUG_NFI)
	printf("nfi_gsoap: server_aux = %s\n", server_aux->path);
#endif
	strcpy(server_aux->path+strlen(https_prefix)+strlen(server), ":");
	strcpy(server_aux->path+strlen(https_prefix)+strlen(server)+1, port);
#if defined(DEBUG_NFI)
	printf("nfi_gsoap: server_aux = %s\n", server_aux->path);
#endif
	server_aux->port = p;
	
	serv->private_info = (void *)server_aux;
	
	//serv->protocol = GSOAP;
	
	serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1));	/* server address */
	if(serv->server == NULL){
		gsoap_err(GSOAPERR_MEMORY);
		return -1;
	}
	strcpy(serv->server, server);
	
	serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1));	/* server address */
	if(serv->url == NULL){
		gsoap_err(GSOAPERR_MEMORY);
		return -1;
	}
	
	strcpy(serv->url, url);
	serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
	memset(serv->wrk, 0, sizeof(struct nfi_worker));
	if(strcmp(protocol, "gsoaphilos") == 0){
#if defined(DEBUG_NFI)
		printf("gsoaphilos\n");
#endif
		nfi_worker_init(serv->wrk, serv, 1);
	}
	else{
		nfi_worker_init(serv->wrk, serv, 0);
	}
	
#if defined(DEBUG_NFI)
	printf("nfi_gsoap: init(%s, %d)\n", server, p);
#endif
	/*
	bzero(server2, MAXPATHLEN);
	strcpy(server2, https_prefix);
	strcpy(server2+strlen(https_prefix), server);
	printf("server2 es %s\n", server2);
	*/
	/*
	memset(server2, 0, MAXPATHLEN);
	sprintf(server2, "%s%s", http_prefix, server);
	printf("server2 es %s\n", server2);
	init(&(server_aux->ios), server2, p, 0, 0, NULL, NULL, NULL);
	*/
	//printf("server es %s\n", server2);
	server_aux->ios = posix_ws_init(server, p, 0, 0, NULL, NULL, NULL, 3, 0);
//printf("Thread %lu: nfi_gsoap_init: server_aux->ios = posix_ws_init(%s, %d, ...) = %p\n", pthread_self(), server, p, server_aux->ios);
	//init(&(server_aux->ios), server, p, 0, 0, NULL, NULL, NULL);
	//init(&(server_aux->ios), server, p, 1, 0, "/home/bbergua/prueba/byteIO/CA/client.pem", "password", "/home/bbergua/prueba/byteIO/CA/cacert.pem");
	//init(&(server_aux->ios), "https://localhost", p, 1, 0, "/home/bbergua/expand-2.0/src/nfi/nfi_gsoap/IOService/CA-orig/client.pem", "password", "/home/bbergua/expand-2.0/src/nfi/nfi_gsoap/IOService/CA-orig/cacert.pem");
	//init (&(server_aux->ios), server2, p, 1, 0, "/home/bbergua/expand-2.0/src/nfi/nfi_gsoap/IOService/CA-orig/client.pem", "password", "/home/bbergua/expand-2.0/src/nfi/nfi_gsoap/IOService/CA-orig/cacert.pem");
	//init (&(server_aux->ios), server2, p, 1, 0, "/home/bbergua/expand-2.0/src/nfi/nfi_gsoap/IOService/CA/client.pem", "prueba", "/home/bbergua/expand-2.0/src/nfi/nfi_gsoap/IOService/CA/cacert.pem");
	//printf("Despues de init\n");
	
	return 0;
}

/************************************************************
 * Disconnect to the server				    *
 * **********************************************************/
int nfi_gsoap_disconnect(struct nfi_server *serv)
{
	struct nfi_gsoap_server *server_aux;
	
	if (serv == NULL)
	       return 0;
	
	server_aux = (struct nfi_gsoap_server *)serv->private_info;
	if(server_aux != NULL){
		posix_ws_end(server_aux->ios);
		free(serv->private_info);
	}
	
	if(serv->url != NULL){
		free(serv->url);
	}
	
	if(serv->server != NULL){
		free(serv->server);
	}
	
	nfi_worker_end(serv->wrk);
	
	return 0;
}



/************************************************************
 * Reconnect to the GSOAP server			    *
 ************************************************************/
int nfi_gsoap_reconnect(struct nfi_server *serv)
{
	/* Don't see the serv result */
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_gsoap_server *server_aux;

	ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret <0 ){
		gsoap_err(GSOAPERR_URL);
		fprintf(stderr,"nfi_gsoap_reconnect: url %s incorrect.\n",serv->url);
		return -1;
	}

	server_aux = (struct nfi_gsoap_server *)malloc(sizeof(struct nfi_gsoap_server));
	if(server_aux == NULL){
		gsoap_err(GSOAPERR_MEMORY);
		return -1;
	}

	strcpy(server_aux->path, dir);

	serv->private_info = (void *)server_aux;
	return 0;
}

/************************************************************
 * Destroy GSOAP operations				    *
 ************************************************************/

int nfi_gsoap_destroy(struct nfi_server *serv)
{

	struct nfi_gsoap_server *server_aux;

	if (serv == NULL)
		return 0;

	if(serv->ops != NULL)
		free(serv->ops);

	server_aux = (struct nfi_gsoap_server *)serv->private_info;

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

int nfi_gsoap_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr)
{
	int res;
	struct stat st;
	
	struct nfi_gsoap_server *server_aux;
	struct nfi_gsoap_fhandle *fh_aux;
	
	if (attr == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
	if (fh == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
	if (fh->priv_fh == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gsoap_reconnect(serv);
		if(ret <0){
			/* gsoap_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
#endif
	fh_aux = (struct nfi_gsoap_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_gsoap_server *) serv->private_info;
	
	res = posix_ws_stat(server_aux->ios, fh_aux->path, &st);
	if(res < 0){
		fprintf(stderr,"nfi_gsoap_getattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		gsoap_err(GSOAPERR_GETATTR);
		return -1;
	}
	
	GSOAPtoNFIattr(attr, &st);
	return 0;
}


int nfi_gsoap_setattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){

	int ret;
	struct stat st;
	struct nfi_gsoap_server *server_aux;
	struct nfi_gsoap_fhandle *fh_aux;


	if (attr == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}

	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gsoap_reconnect(serv);
		if(ret <0){
			/* gsoap_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gsoap_err(GSOAPERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_gsoap_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_gsoap_server *) serv->private_info;

 	NFItoGSOAPattr(&st, attr);
	/* no se comp hacer el setattr */
	/*
	ret = stat(fh_aux->path, &st);
	*/
	ret = truncate(fh_aux->path, st.st_size);
	if(ret < 0){
		fprintf(stderr,"nfi_gsoap_setattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		gsoap_err(GSOAPERR_GETATTR);
		return -1;
	}
	
	return 0;
}

int nfi_gsoap_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int res;
	struct stat st;
	struct nfi_gsoap_server *server_aux;
	struct nfi_gsoap_fhandle *fh_aux;
	
	XPN_DEBUG_BEGIN_ARGS1(url)
	
	if(url[strlen(url)-1] == '/'){
		res = nfi_gsoap_opendir(serv, url, fho);
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	if (fho == NULL){
		gsoap_err(GSOAPERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_gsoap_reconnect(serv);
		if(res < 0){
			// gsoap_err(); not necessary
			res = -1;
			XPN_DEBUG_END_ARGS1(url)
			return res;
		}
	}
#else
	if (serv->private_info == NULL){
		gsoap_err(GSOAPERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
#endif
	
	res = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(res < 0){
		fprintf(stderr,"nfi_gsoap_open: url %s incorrect.\n",url);
		gsoap_err(GSOAPERR_URL);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		gsoap_err(GSOAPERR_MEMORY);
		res = -1;
		XPN_DEBUG_END_ARGS1(dir)
		return res;
	}
	
	strcpy(fho->url, url);
	
	fh_aux = (struct nfi_gsoap_fhandle *)malloc(sizeof(struct nfi_gsoap_fhandle));
	if (fh_aux == NULL){		
		gsoap_err(GSOAPERR_MEMORY);
		free(fho->url);
		res = -1;
		XPN_DEBUG_END_ARGS1(dir)
		return res;
	}
	
	server_aux = (struct nfi_gsoap_server *) serv->private_info;
	
	res = posix_ws_open(server_aux->ios, dir, O_RDWR);
#ifdef  DBG_NFI
	fprintf(stderr,"nfi_gsoap_open: open %s in server %s.\n",dir,serv->server);
#endif
	if (res < 0 ){
		res = open(dir, O_RDONLY);
		if(res < 0){
			fprintf(stderr,"nfi_gsoap_open: Fail open %s in server %s.\n",dir,serv->server);
			gsoap_err(GSOAPERR_MEMORY);
			free(fh_aux);
			free(fho->url);
			res = -1;
			XPN_DEBUG_END_ARGS1(dir)
			return res;
		}
	}
	
	fh_aux->fd = res;
	strcpy(fh_aux->path, dir);
	
	res = posix_ws_stat(server_aux->ios, fh_aux->path, &st);
	if(res < 0){
		gsoap_err(GSOAPERR_GETATTR);
		fprintf(stderr,"nfi_gsoap_open: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
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
			fprintf(stderr,"nfi_gsoap_open: %s isn't a FILE or FILE.\n",fh_aux->path);
	        	gsoap_err(GSOAPERR_GETATTR);
			free(fh_aux);
			free(fho->url);
			return -1;
		}
	}
	
	fho->server = NULL;
	fho->priv_fh = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;
	
	res = 0;
	XPN_DEBUG_END_ARGS1(dir)
	return res;
}


int nfi_gsoap_close(struct nfi_server *serv,  struct nfi_fhandle *fh)
{
	struct nfi_gsoap_server *server_aux;
	struct nfi_gsoap_fhandle *fh_aux;
	
	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
	if (fh->priv_fh != NULL){
		server_aux = (struct nfi_gsoap_server *) serv->private_info;
		fh_aux = (struct nfi_gsoap_fhandle *) fh->priv_fh;
		posix_ws_close(server_aux->ios, fh_aux->fd);
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;
	}
	
	if (fh->url != NULL){
		free(fh->url);
	}
	
	fh->type = NFINULL;
	fh->server = NULL;
	
	return 0;
}

ssize_t nfi_gsoap_read(	struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{
	ssize_t new_size;
	
	struct nfi_gsoap_server *server_aux;
	struct nfi_gsoap_fhandle *fh_aux;
	
	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
	if (fh == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_gsoap_reconnect(serv);
		if(res < 0){
			/* gsoap_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
#endif
	
	fh_aux = (struct nfi_gsoap_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_gsoap_server *) serv->private_info;
	
	posix_ws_lseek(server_aux->ios, fh_aux->fd, offset, SEEK_SET);
	new_size = posix_ws_read(server_aux->ios,
				 fh_aux->fd,
				 buffer,
				 size);
#ifdef  DBG_NFI
	fprintf(stderr,"nfi_gsoap_read: read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
#endif
	if(new_size < 0){
		fprintf(stderr,"nfi_gsoap_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
		gsoap_err(GSOAPERR_READ);
		return -1;
	}
	
	return new_size;
}

ssize_t nfi_gsoap_write(struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{
	ssize_t new_size;
	struct nfi_gsoap_server *server_aux;
	struct nfi_gsoap_fhandle *fh_aux;
	
	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
	if (fh == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gsoap_reconnect(serv);
		if(ret < 0){
			/* gsoap_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
#endif
	
	fh_aux = (struct nfi_gsoap_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_gsoap_server *) serv->private_info;
	
	posix_ws_lseek(server_aux->ios, fh_aux->fd, offset, SEEK_SET);
	new_size = posix_ws_write(server_aux->ios,
				  fh_aux->fd,
				  buffer,
				  size);
#ifdef  DBG_NFI 
	//fprintf(stderr,"nfi_gsoap_write: write %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)data.__size);
#endif
	if(new_size < 0){
		fprintf(stderr,"nfi_gsoap_write: Fail write %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
		gsoap_err(GSOAPERR_WRITE);
		return -1;
	}
	
	return new_size;
}

int nfi_gsoap_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res, fd;
	struct nfi_gsoap_server *server_aux;
	struct nfi_gsoap_fhandle *fh_aux;
	struct stat st;
	
	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
	if (attr == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_gsoap_reconnect(serv);
		if(res < 0){
			/* gsoap_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
#endif
	res = ParseURL(url,  NULL, NULL, NULL, server, NULL, dir);
	if(res < 0){
		fprintf(stderr,"nfi_gsoap_create: url %s incorrect.\n",url);
		gsoap_err(GSOAPERR_URL);
		return -1;
	}
	/* private_info file handle */
	fh_aux = (struct nfi_gsoap_fhandle *)malloc(sizeof(struct nfi_gsoap_fhandle));
	if (fh_aux == NULL){
		gsoap_err(GSOAPERR_MEMORY);
		return -1;
	}
	
	bzero(fh_aux, sizeof(struct nfi_gsoap_fhandle));
	
	server_aux = (struct nfi_gsoap_server *) serv->private_info;
	
	/* create the file into the directory */
//printf("Thread %lu: nfi_gsoap_create: posix_ws_open(%p=%s, %s, %d, %d)\n", pthread_self(), server_aux->ios, server_aux->ios->server, dir, O_CREAT|O_RDWR|O_TRUNC, attr->at_mode);
	fd = posix_ws_open(server_aux->ios, dir, O_CREAT|O_RDWR|O_TRUNC, attr->at_mode);
//printf("Thread %lu: nfi_gsoap_create: posix_ws_open(%p=%s, %s, %d, %d) = %d\n", pthread_self(), server_aux->ios, server_aux->ios->server, dir, O_CREAT|O_RDWR|O_TRUNC, attr->at_mode, fd);
	//printf("nfi_gsoap_create: Antes de createFile(%s,%s)\n", server_aux->path, dir);
	//fd = createFile(server_aux->ios, dir);
	//printf("nfi_gsoap_create: Despues de createFile(%s,%s)=%d\n", server_aux->path, dir, fd);
	if(fd < 0){
		fprintf(stderr,"nfi_gsoap_create: Fail create %s in server %s.\n",fh_aux->path,serv->server);
		gsoap_err(GSOAPERR_LOOKUP);
		free(fh_aux);
		return -1;
	}
	fh->server = serv;
	
	fh_aux->fd = fd;
	strcpy(fh_aux->path, dir);
	
	fh->priv_fh = (void *)fh_aux;
	
	fh->url = (char *)malloc(strlen(url)+1);
	if(fh->url == NULL){
		gsoap_err(GSOAPERR_MEMORY);
		free(fh_aux);
		return -1;
	}
		
	strcpy(fh->url, url);
	memset (&st, 0, sizeof(struct stat));
	res = posix_ws_stat(server_aux->ios, fh_aux->path, &st);
	if(res < 0){
#if defined(DEBUG_NFI)
		printf("posix_ws_stat failed\n");
#endif
		fprintf(stderr,"nfi_gsoap_create: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		gsoap_err(GSOAPERR_GETATTR);
		free(fh->url);
		free(fh_aux);
		
		return -1;
	}
	
	GSOAPtoNFIattr(attr, &st);
	
	return 0;
}

int nfi_gsoap_remove(struct nfi_server *serv,  char *url)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_gsoap_server *server_aux;
	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gsoap_reconnect(serv);
		if(ret <0){
			/* gsoap_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
#endif
	
	server_aux = (struct nfi_gsoap_server *)serv->private_info;
	
	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_gsoap_remove: url %s incorrect.\n",url);
		gsoap_err(GSOAPERR_URL);
		return -1;
	}
	/* remove the file into the directory */
	//ret = posix_ws_unlink(dir);
	
	if(ret < 0){
		fprintf(stderr,"nfi_gsoap_remove: Fail remove %s in server %s.\n",dir,serv->server);
		gsoap_err(GSOAPERR_REMOVE);
		return -1;
	}
	
	return 0;
}

int nfi_gsoap_rename(struct nfi_server *server,  char *old_url, char *new_url)
{
	/*
        struct nfi_gsoap_server *server_aux;
        struct nfi_gsoap_fhandle *fh_aux;
	if (server == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gsoap_reconnect(serv);
		if(ret <0){
			gsoap_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gsoap_err(GSOAPERR_PARAM);
               return -1;
        }
#endif
	server_aux = (strcut nfi_gsoap_server *)serv->private_info;
	*/

	/* ????? */
	return 0;
}

int nfi_gsoap_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct stat st;		/* GSOAP attributes */
        struct nfi_gsoap_server *server_aux;
        struct nfi_gsoap_fhandle *fh_aux;


	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gsoap_reconnect(serv);
		if(ret <0){
			/* gsoap_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gsoap_err(GSOAPERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_gsoap_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_gsoap_mkdir: url %s incorrect.\n",url);
		gsoap_err(GSOAPERR_URL);
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_gsoap_fhandle *)malloc(sizeof(struct nfi_gsoap_fhandle));
	if (fh_aux == NULL){
		gsoap_err(GSOAPERR_MEMORY);
		return -1;
	}


	bzero(fh_aux, sizeof(struct nfi_gsoap_fhandle));
	/* create the dir into the directory */
	ret = mkdir(dir, /*attr->at_mode*/ 0777);
	
	if((ret < 0)&&(errno != EEXIST)){
		gsoap_err(GSOAPERR_MKDIR);
		fprintf(stderr,"nfi_gsoap_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server);
		free(fh_aux);
		return -1;
	}
	
	fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               gsoap_err(GSOAPERR_MEMORY);
	       free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

	GSOAPtoNFIattr(attr, &st);


	return 0;
}

int nfi_gsoap_rmdir(struct nfi_server *serv,  char *url)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;

        struct nfi_gsoap_server *server_aux;

	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gsoap_reconnect(serv);
		if(ret <0){
			/* gsoap_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gsoap_err(GSOAPERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_gsoap_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_gsoap_rmdir: url %s incorrect.\n",url);
		gsoap_err(GSOAPERR_URL);
		return -1;
	}

	ret = rmdir(dir);	
	if(ret < 0){
		fprintf(stderr,"nfi_gsoap_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server);
		gsoap_err(GSOAPERR_REMOVE);
		return -1;
	}

	return 0;
}

int nfi_gsoap_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho)
{
        
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int ret;	
	struct nfi_gsoap_server *server_aux;
	struct nfi_gsoap_fhandle *fh_aux;


	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}

	if (fho == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gsoap_reconnect(serv);
		if(ret <0){
			/* gsoap_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gsoap_err(GSOAPERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_gsoap_opendir: url %s incorrect.\n",url);
		gsoap_err(GSOAPERR_URL);
		return -1;
	}

	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		gsoap_err(GSOAPERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_gsoap_fhandle *)malloc(sizeof(struct nfi_gsoap_fhandle));
	if (fh_aux == NULL){
		gsoap_err(GSOAPERR_MEMORY);
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_gsoap_server *) serv->private_info;


	fh_aux->dir = opendir(dir);
    if (ret < 0 ){
		fprintf(stderr,"nfi_gsoap_opendir: Fail opendir %s in server %s.\n",dir,serv->server);
	    gsoap_err(GSOAPERR_MEMORY);		
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

int nfi_gsoap_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, char *entry, unsigned char *type)
{
	struct dirent *ent;

        struct nfi_gsoap_server *server_aux;
        struct nfi_gsoap_fhandle *fh_aux;


	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}

	if(fh->type != NFIDIR){
		gsoap_err(GSOAPERR_NOTDIR);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gsoap_reconnect(serv);
		if(ret <0){
			/* gsoap_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gsoap_err(GSOAPERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_gsoap_server *)serv->private_info;
	fh_aux = (struct nfi_gsoap_fhandle *)fh->priv_fh;

	entry[0] = '\0';
	ent = readdir(fh_aux->dir);

	if(ent == NULL){
		return 1;
	}
	if(type==NULL){
		return 0;
	}
	strcpy(entry, ent->d_name);
	//printf("ent->d_name = %s S_ISDIR(%o) = %o\n", ent->d_name, ent->d_type,S_ISDIR(ent->d_type));
	*type = ent->d_type;

	return 0;
}

int nfi_gsoap_closedir(struct nfi_server *serv,  struct nfi_fhandle *fh)
{
	
	struct nfi_gsoap_fhandle *fh_aux;

	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_gsoap_fhandle *) fh->priv_fh;
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


int nfi_gsoap_statfs(struct nfi_server *serv,  struct nfi_info *inf)
{
	/*
	struct local_info localinf;
	int ret;
        struct nfi_gsoap_server *server_aux;


	if (serv == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}

	if (inf == NULL){
		gsoap_err(GSOAPERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gsoap_reconnect(serv);
		if(ret <0){
		 gsoap_err(); not necessary 
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gsoap_err(GSOAPERR_PARAM);
               return -1;
        }
#endif
	
	server_aux = (struct nfi_gsoap_server *)serv->private_info;
	ret = local_statfs(server_aux->fh, &localinf, server_aux->cl);
	if(ret <0){
		gsoap_err(GSOAPERR_STATFS);
		return -1;
	}

	GSOAPtoNFIInfo(inf, &localinf);
	*/
	return 0;
}
