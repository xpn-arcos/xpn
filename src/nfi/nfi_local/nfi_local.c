#include "nfi/nfi_local/local_lib.h"

/*
 * TODO:
 *  -> local_tcp, local_udp ??????????????
 *  -> local_xxx a nfi_local_xxx
 *  -> local_misc.[ch]:
 *     -> ParseURL
 *  -> local_error.[ch]:
 *     -> local_err
 *
 * -> pasar CLIENT * a servidor (es comn a todos...)
 * -> Poner todo a punteros
 * -> struct nfi_fhandle ** Paso de puntero par�etro por referencia
 *
 */


/************************************************************
 * PRIVATE FUNCTIONS TO USE local SERVERS	            *
 ************************************************************/
void NFItoLOCALattr(struct stat *att, struct nfi_attr *nfi_att)
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

void LOCALtoNFIattr(struct nfi_attr *nfi_att, struct stat *att)
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

/*
void LOCALtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *local_inf)
{
}
*/

/************************************************************
 * PUBLIC FUNCTIONS TO USE LOCAL			    *
 ************************************************************/

/************************************************************
 * Init Local 					    	    *
 ************************************************************/
int nfi_local_init(char *url, struct nfi_server *serv, __attribute__((__unused__)) struct nfi_attr_server *attr)
{
	char server[MAXPATHLEN], dir[MAXPATHLEN] ,prt [MAXPATHLEN];
	int res;
	struct nfi_local_server *server_aux;
	
	if(serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
	/* functions */
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
	if (serv->ops == NULL){
		local_err(LOCALERR_MEMORY);
		return -1;
	}
	
	bzero(serv->ops, sizeof(struct nfi_ops));
	
	serv->ops->nfi_reconnect  = nfi_local_reconnect;
	serv->ops->nfi_disconnect = nfi_local_disconnect;
	//serv->ops->nfi_destroy = nfi_http_destroy;
	
	serv->ops->nfi_getattr	= nfi_local_getattr;
	serv->ops->nfi_setattr	= nfi_local_setattr;
	
	serv->ops->nfi_open	= nfi_local_open;
	serv->ops->nfi_close	= nfi_local_close;
	
	serv->ops->nfi_read	= nfi_local_read;
	serv->ops->nfi_write	= nfi_local_write;
	
	serv->ops->nfi_create	= nfi_local_create;
	serv->ops->nfi_remove	= nfi_local_remove;
	serv->ops->nfi_rename	= nfi_local_rename;
	
	serv->ops->nfi_mkdir	= nfi_local_mkdir;
	serv->ops->nfi_rmdir	= nfi_local_rmdir;
	serv->ops->nfi_opendir	= nfi_local_opendir;
	serv->ops->nfi_readdir	= nfi_local_readdir;
	serv->ops->nfi_closedir	= nfi_local_closedir;
	serv->ops->nfi_statfs	= nfi_local_statfs;
	
	res = ParseURL(url,  prt, NULL, NULL, server,  NULL,  dir);
	//res = ParseURL(url,  prt, NULL, NULL, NULL,  NULL,  dir);
	if(res <0 ){
		local_err(LOCALERR_URL);
		fprintf(stderr,"nfi_local_init: url %s incorrect.\n",url);
		free(serv->ops);
		return -1;
	}
	
	server_aux = (struct nfi_local_server *)malloc(sizeof(struct nfi_local_server));
	if(server_aux == NULL){
		local_err(LOCALERR_MEMORY);
		return -1;
	}
	
	strcpy(server_aux->path, dir);
	serv->private_info = (void *)server_aux;
	
	//serv->protocol = LOCAL;
	
	serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1));	/* server address */
	if(serv->server == NULL){
		local_err(LOCALERR_MEMORY);
		return -1;
	}
	strcpy(serv->server, server);
	
	serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1));	/* server address */
	if(serv->url == NULL){
		local_err(LOCALERR_MEMORY);
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
 * Reconnect to the LOCAL server			    *
 ************************************************************/
int nfi_local_reconnect(struct nfi_server *serv)
{
	/* Don't see the serv result */
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res;
	struct nfi_local_server *server_aux;

	res = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
	//res = ParseURL(serv->url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(res <0 ){
		local_err(LOCALERR_URL);
		fprintf(stderr,"nfi_local_reconnect: url %s incorrect.\n",serv->url);
		return -1;
	}

	server_aux = (struct nfi_local_server *)malloc(sizeof(struct nfi_local_server));
	if(server_aux == NULL){
		local_err(LOCALERR_MEMORY);
		return -1;
	}

	strcpy(server_aux->path, dir);

	serv->private_info = (void *)server_aux;
	return 0;
}


/************************************************************
 * Disconnect from the server				    *
 * **********************************************************/
int nfi_local_disconnect(struct nfi_server *serv)
{
	// Check arguments
	if (serv == NULL) {
	    return 0;
	}
	
	// Free and null
	FREE_AND_NULL(serv->private_info) ;
	FREE_AND_NULL(serv->url) ;
	FREE_AND_NULL(serv->server) ;
	
	nfi_worker_end(serv->wrk);
	
	// Return OOK
	return 0;
}


/************************************************************
 * Destroy LOCAL operations				    *
 ************************************************************/
/*
// When is this called?
int nfi_local_destroy(struct nfi_server *serv)
{
	struct nfi_local_server *server_aux;
	
	if (serv == NULL)
		return 0;
	
	if(serv->ops != NULL)
		free(serv->ops);
	
	server_aux = (struct nfi_local_server *)serv->private_info;
	
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
*/



/************************************************************
 * GENERIC FUNCTIONS 					    *
 ************************************************************/

int nfi_local_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr)
{
	int res;
	struct stat st;
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;
	
	if (attr == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
	if (fh == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
	if (fh->priv_fh == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res <0){
			/* local_err(); not necessary */
			return -1;
		}
	}
#else
	
	if (serv->private_info == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
#endif
	fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_local_server *) serv->private_info;
	
	XPN_DEBUG_BEGIN_ARGS1(fh_aux->path)
	
	res = stat(fh_aux->path, &st);
	if(res < 0){
		//fprintf(stderr,"nfi_local_getattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		fprintf(stderr,"nfi_local_getattr: Fail stat %s.\n",fh_aux->path);
		local_err(LOCALERR_GETATTR);
		XPN_DEBUG_END_ARGS1(fh_aux->path)
		return res;
	}
	
	LOCALtoNFIattr(attr, &st);
	XPN_DEBUG_END_ARGS1(fh_aux->path)
	return 0;
}


int nfi_local_setattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr)
{
	int res;
	struct stat st;
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;
	
	if (attr == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
	if (fh->priv_fh == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res <0){
			/* local_err(); not necessary */
			return -1;
		}
	}
#else
	
	if (serv->private_info == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
#endif
	
	fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_local_server *) serv->private_info;
	
	NFItoLOCALattr(&st, attr);
	/* no se comp hacer el setattr */
	/*
	res = stat(fh_aux->path, &st);
	*/
	res = truncate(fh_aux->path, st.st_size);
	if(res < 0){
		//fprintf(stderr,"nfi_local_setattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		fprintf(stderr,"nfi_local_setattr: Fail stat %s.\n",fh_aux->path);
		local_err(LOCALERR_GETATTR);
		return -1;
	}
	
	return 0;
}

int nfi_local_open ( struct nfi_server *serv, char *url, struct nfi_fhandle *fho )
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int res;
	struct stat st;
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;
	
	XPN_DEBUG_BEGIN_ARGS1(url)
	
	if(url[strlen(url)-1] == '/'){
		res = nfi_local_opendir(serv, url, fho);
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	if (fho == NULL){
		local_err(LOCALERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res < 0){
			/* local_err(); not necessary */
			res = -1;
			XPN_DEBUG_END_ARGS1(url)
			return res;
		}
	}
#else
	if (serv->private_info == NULL){
		local_err(LOCALERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
#endif
	
	res = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(res < 0){
		XPN_DEBUG("nfi_local_open: url %s incorrect.\n",url);
		local_err(LOCALERR_URL);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}

	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		local_err(LOCALERR_MEMORY);
		res = -1;
		XPN_DEBUG_END_ARGS1(dir)
		return res;
	}
	
	strcpy(fho->url, url);
	
	fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle));
	if (fh_aux == NULL){		
		local_err(LOCALERR_MEMORY);
		free(fho->url);
		res = -1;
		XPN_DEBUG_END_ARGS1(dir)
		return res;
	}
	
	server_aux = (struct nfi_local_server *) serv->private_info;
	
	res = open(dir, O_RDWR);
	if (res < 0)
	{
		res = open(dir, O_RDONLY);
		if(res < 0){
			XPN_DEBUG("nfi_local_open: Fail open %s in server %s.\n",dir,serv->server);
			local_err(LOCALERR_MEMORY);
			free(fh_aux);
			free(fho->url);
			res = -1;
			XPN_DEBUG_END_ARGS1(dir)
			return res;
		}
	}
	
	fh_aux->fd = res;
	strcpy(fh_aux->path, dir);
	
	res = stat(fh_aux->path, &st);
	if(res < 0){
		local_err(LOCALERR_GETATTR);
		//XPN_DEBUG("nfi_local_open: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		//XPN_DEBUG("nfi_local_open: Fail stat %s.\n",fh_aux->path);
		free(fh_aux);
		free(fho->url);
		res = -1;
		XPN_DEBUG_END_ARGS1(dir)
		return res;
	}
	
	if (S_ISDIR(st.st_mode)) {
		fho->type = NFIDIR;
	} else {
		if (S_ISREG(st.st_mode)) {
			fho->type = NFIFILE;
		} else {
			//XPN_DEBUG("nfi_local_open: %s isn't a FILE nor DIR.\n",fh_aux->path);
			local_err(LOCALERR_GETATTR);
			free(fh_aux);
			free(fho->url);
			res = -1;
			XPN_DEBUG_END_ARGS1(dir)
			return res;
		}
	}
	
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;
	
	res = 0;
	XPN_DEBUG_END_ARGS1(dir)
	return res;
}

int nfi_local_close ( struct nfi_server *serv,  struct nfi_fhandle *fh )
{	
	struct nfi_local_fhandle *fh_aux;
	
	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
		close(fh_aux->fd);
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;
	
	}
	
	if (fh->url != NULL){
		free(fh->url);
		fh->url = NULL;
	}
	
	fh->type = NFINULL;
	fh->server = NULL;
	
	return 0;
}

ssize_t nfi_local_read(	struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{
	ssize_t new_size;
	
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;
	
	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
	if (fh == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res < 0){
			/* local_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
#endif
	
	fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_local_server *) serv->private_info;
	
	if(fh_aux == NULL){
		errno = EBADF;
		fprintf(stderr,"nfi_local_read: fh_aux == NULL\n");
		perror("nfi_local_read");
		local_err(LOCALERR_PARAM);
		return -1;
	}

	lseek(fh_aux->fd, offset, SEEK_SET);
	new_size = read(fh_aux->fd,
					buffer,
					size);
	XPN_DEBUG("read %s(%d) off %ld size %zu (ret:%zd)", fh->url, fh_aux->fd, (long int)offset, size, new_size)
	if(new_size < 0){
		fprintf(stderr, "nfi_local_read: Fail read %s off %ld size %zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, new_size, errno);
		perror("nfi_local_read");
		local_err(LOCALERR_READ);
		return -1;
	}
	
	return new_size;
}

ssize_t nfi_local_write(struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{
	ssize_t new_size;
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;
	
	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
	if (fh == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res < 0){
			/* local_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
#endif
	
	fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_local_server *) serv->private_info;
	
	lseek(fh_aux->fd, offset, SEEK_SET);
	new_size = write(fh_aux->fd,
					 buffer,
					 size);
	XPN_DEBUG("write %s off %ld size %zu (ret:%zd)", fh->url, (long int)offset, size, new_size)
	if(new_size < 0){
		fprintf(stderr, "nfi_local_write: Fail write %s off %ld size %zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, new_size, errno);
		perror("nfi_local_write");
		local_err(LOCALERR_WRITE);
		return -1;
	}
	
	return new_size;
}

int nfi_local_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res, fd;
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;
	struct stat st;
	
	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
	if (attr == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res < 0){
			/* local_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
#endif
	res = ParseURL(url,  NULL, NULL, NULL, server, NULL, dir);
	if(res < 0){
		fprintf(stderr,"nfi_local_create: url %s incorrect.\n",url);
		local_err(LOCALERR_URL);
		return -1;
	}
	/* private_info file handle */
	fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle));
	if (fh_aux == NULL){
		local_err(LOCALERR_MEMORY);
		return -1;
	}
	
	bzero(fh_aux, sizeof(struct nfi_local_fhandle));
	
	server_aux = (struct nfi_local_server *) serv->private_info;
	
	/* create the file into the directory */
	fd = open(dir, O_CREAT|O_RDWR|O_TRUNC, attr->at_mode);
	/*********BORRAR********************/
        //if (fd < 0){
        //       char s[255];
        //        extern int errno;
	//
        //        sprintf(s,"nfi_local_getattr: %s", fh_aux->path);
        //        perror(s);
        //        printf("%s : %d\n", s, errno);          
        //}
        /*********BORRAR********************/
	if(fd < 0){
		//fprintf(stderr,"nfi_local_creat: Fail creat %s in server %s.\n",fh_aux->path,serv->server);
		//fprintf(stderr,"nfi_local_creat: Fail creat %s.\n",fh_aux->path);
		fprintf(stderr,"nfi_local_creat: Fail creat %s.\n",dir);
		local_err(LOCALERR_LOOKUP);
		free(fh_aux);
		return -1;
	}
	fh->server = serv;
	
	fh_aux->fd = fd;
	strcpy(fh_aux->path, dir);
	
	fh->priv_fh = (void *)fh_aux;
	
	fh->url = (char *)malloc(strlen(url)+1);
	if(fh->url == NULL){
		local_err(LOCALERR_MEMORY);
		free(fh_aux);
		return -1;
	}
		
	strcpy(fh->url, url);
	memset (&st, 0, sizeof(struct stat));
	res = stat(fh_aux->path, &st);
	if(res < 0){
		fprintf(stderr,"nfi_local_creat: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		local_err(LOCALERR_GETATTR);
		free(fh->url);
		free(fh_aux);
		
		return -1;
	}
	
	LOCALtoNFIattr(attr, &st);
	
	return 0;
}

int nfi_local_remove(struct nfi_server *serv,  char *url)
{

	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res;
	struct nfi_local_server *server_aux;
	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res <0){
			/* local_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               local_err(LOCALERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_local_server *)serv->private_info;

	res = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	//res = ParseURL(url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(res < 0){
		//fprintf(stderr,"nfi_local_remove: url %s incorrect.\n",url);
		local_err(LOCALERR_URL);
		return -1;
	}
	/* remove the file into the directory */
	res = unlink(dir);
	/*********BORRAR********************/
	//if (res < 0){
	//	char s[255];
	//	sprintf(s,"nfi_local_remove: %s", dir);
	//	perror(s);	
	//}
	/*********BORRAR********************/

	if(res < 0){
		//fprintf(stderr,"nfi_local_remove: Fail remove %s in server %s.\n",dir,serv->server);
		//fprintf(stderr,"nfi_local_remove: Fail remove %s.\n", dir);
		local_err(LOCALERR_REMOVE);
		return -1;
	}



	return 0;
}

int nfi_local_rename(__attribute__((__unused__)) struct nfi_server *server, __attribute__((__unused__)) char *old_url, __attribute__((__unused__)) char *new_url)
{
	/*
        struct nfi_local_server *server_aux;
        struct nfi_local_fhandle *fh_aux;
	if (server == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res <0){
			local_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               local_err(LOCALERR_PARAM);
               return -1;
        }
#endif
	server_aux = (strcut nfi_local_server *)serv->private_info;
	*/

	/* ????? */
	return 0;
}

int nfi_local_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res;
	struct stat st;		/* LOCAL attributes */
        struct nfi_local_server *server_aux;
        struct nfi_local_fhandle *fh_aux;


	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res <0){
			/* local_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               local_err(LOCALERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_local_server *)serv->private_info;

	res = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	//res = ParseURL(url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(res < 0){
		fprintf(stderr,"nfi_local_mkdir: url %s incorrect.\n",url);
		local_err(LOCALERR_URL);
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle));
	if (fh_aux == NULL){
		local_err(LOCALERR_MEMORY);
		return -1;
	}


	bzero(fh_aux, sizeof(struct nfi_local_fhandle));
	/* create the dir into the directory */
	res = mkdir(dir, /*attr->at_mode*/ 0777);
	
	if((res < 0)&&(errno != EEXIST)){
		local_err(LOCALERR_MKDIR);
		//fprintf(stderr,"nfi_local_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server);
		fprintf(stderr,"nfi_local_mkdir: Fail mkdir %s.\n",dir);
		free(fh_aux);
		return -1;
	}
	
	fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               local_err(LOCALERR_MEMORY);
	       free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

	LOCALtoNFIattr(attr, &st);


	return 0;
}

int nfi_local_rmdir(struct nfi_server *serv,  char *url)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res;
	
	struct nfi_local_server *server_aux;
	
	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res <0){
			/* local_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
#endif
	
	server_aux = (struct nfi_local_server *)serv->private_info;
	
	res = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	//res = ParseURL(url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(res < 0){
		fprintf(stderr,"nfi_local_rmdir: url %s incorrect.\n",url);
		local_err(LOCALERR_URL);
		return -1;
	}
	
	res = rmdir(dir);
	if(res < 0){
		//fprintf(stderr,"nfi_local_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server);
		fprintf(stderr,"nfi_local_rmdir: Fail rmdir %s.\n",dir);
		local_err(LOCALERR_REMOVE);
		return -1;
	}
	
	return 0;
}

int nfi_local_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho)
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int res;	
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;
	
	
	XPN_DEBUG_BEGIN_ARGS1(url)
	
	if (serv == NULL){
		//local_err(LOCALERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	if (fho == NULL){
		//local_err(LOCALERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res <0){
			/* local_err(); not necessary */
			res = -1;
			XPN_DEBUG_END_ARGS1(url)
			return res;
		}
	}
#else
	if (serv->private_info == NULL){
		//local_err(LOCALERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
#endif
	
	res = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	//res = ParseURL(url, NULL, NULL, NULL, NULL,  NULL,  dir);
	if(res < 0){
		fprintf(stderr,"nfi_local_opendir: url %s incorrect.\n",url);
		//local_err(LOCALERR_URL);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		//local_err(LOCALERR_MEMORY);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	strcpy(fho->url, url);
	
	fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle));
	if (fh_aux == NULL){
		free(fho->url);
		//local_err(LOCALERR_MEMORY);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	server_aux = (struct nfi_local_server *) serv->private_info;
	
	fh_aux->dir = opendir(dir);
	//printf("nfi_local_opendir: opendir(%s)=%p\n", dir, fh_aux->dir);
	if (fh_aux->dir == NULL){
		//fprintf(stderr,"nfi_local_opendir: Fail opendir %s in server %s.\n",dir,serv->server);
		//fprintf(stderr,"nfi_local_opendir: Fail opendir %s.\n",dir);
		free(fh_aux);
		free(fho->url);
		//local_err(LOCALERR_MEMORY);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	fh_aux->fd = res;
	strcpy(fh_aux->path, dir);
	fho->type = NFIDIR;
	fho->server = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;
	
	res = 0;
	XPN_DEBUG_END_ARGS1(url)
	return res;
}

int nfi_local_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, char *entry, unsigned char *type)
{
	struct dirent *ent;

        struct nfi_local_server *server_aux;
        struct nfi_local_fhandle *fh_aux;


	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}

	if(fh->type != NFIDIR){
		local_err(LOCALERR_NOTDIR);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res <0){
			/* local_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               local_err(LOCALERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_local_server *)serv->private_info;
	fh_aux = (struct nfi_local_fhandle *)fh->priv_fh;

	entry[0] = '\0';
	ent = readdir(fh_aux->dir);

	if(ent == NULL){
		//printf("nfi_local_readdir: ent=%p, errno=%d, EBADF=%d\n", ent, errno, EBADF);
		//perror("nfi_local_readdir: readdir");
		return 1;
	}
	if(type==NULL){
		return 0;
	}
	
	strcpy(entry, ent->d_name);
	
	*type = ent->d_type;
	
	return 0;
}

int nfi_local_closedir(struct nfi_server *serv,  struct nfi_fhandle *fh)
{
	
	struct nfi_local_fhandle *fh_aux;

	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
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


int nfi_local_statfs(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_info *inf)
{
	/*
	struct local_info localinf;
	int res;
        struct nfi_local_server *server_aux;


	if (serv == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}

	if (inf == NULL){
		local_err(LOCALERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_local_reconnect(serv);
		if(res <0){
		 local_err(); not necessary 
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               local_err(LOCALERR_PARAM);
               return -1;
        }
#endif
	
	server_aux = (struct nfi_local_server *)serv->private_info;
	res = local_statfs(server_aux->fh, &localinf, server_aux->cl);
	if(res <0){
		local_err(LOCALERR_STATFS);
		return -1;
	}

	LOCALtoNFIInfo(inf, &localinf);
	*/
	return 0;
}
