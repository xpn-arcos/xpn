#include "nfi/nfi_http/http_lib.h"

/*
 * TODO:
 *  -> local_tcp, local_udp ??????????????
 *  -> local_xxx a nfi_http_xxx
 *  -> local_misc.[ch]:
 *     -> ParseURL
 *  -> http_error.[ch]:
 *     -> http_err
 *
 * -> pasar CLIENT * a servidor (es comn a todos...)
 * -> Poner todo a punteros
 * -> struct nfi_fhandle ** Paso de puntero par�etro por referencia
 *
 */


/************************************************************
 * PRIVATE FUNCTIONS TO USE local SERVERS	            *
 ************************************************************/
void NFItoHTTPattr(struct stat *att, struct nfi_attr *nfi_att)
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

void HTTPtoNFIattr(struct nfi_attr *nfi_att, struct stat *att)
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
void HTTPtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *local_inf)
{
}
*/

/************************************************************
 * PUBLIC FUNCTIONS TO USE HTTP			    *
 ************************************************************/

/************************************************************
 * Init Local 					    	    *
 ************************************************************/
int nfi_http_init(char *url, struct nfi_server *serv, __attribute__((__unused__)) struct nfi_attr_server *attr)
{
	char server[MAXPATHLEN], dir[MAXPATHLEN] ,prt [MAXPATHLEN];
	int res;
	struct nfi_http_server *server_aux;
	
	if(serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
	/* functions */
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
	if (serv->ops == NULL){
		http_err(HTTPERR_MEMORY);
		return -1;
	}
	
	bzero(serv->ops, sizeof(struct nfi_ops));
	
	serv->ops->nfi_reconnect  = nfi_http_reconnect;
	serv->ops->nfi_disconnect = nfi_http_disconnect;
	
	serv->ops->nfi_getattr	= nfi_http_getattr;
	serv->ops->nfi_setattr	= nfi_http_setattr;
	
	serv->ops->nfi_open	= nfi_http_open;
	serv->ops->nfi_close	= nfi_http_close;
	
	serv->ops->nfi_read	= nfi_http_read;
	serv->ops->nfi_write	= nfi_http_write;
	
	serv->ops->nfi_create	= nfi_http_create;
	serv->ops->nfi_remove	= nfi_http_remove;
	serv->ops->nfi_rename	= nfi_http_rename;
	
	serv->ops->nfi_mkdir	= nfi_http_mkdir;
	serv->ops->nfi_rmdir	= nfi_http_rmdir;
	serv->ops->nfi_opendir	= nfi_http_opendir;
	serv->ops->nfi_readdir	= nfi_http_readdir;
	serv->ops->nfi_closedir	= nfi_http_closedir;
	serv->ops->nfi_statfs	= nfi_http_statfs;
	
	res = ParseURL(url,  prt, NULL, NULL, server,  NULL,  dir);
	//res = ParseURL(url,  prt, NULL, NULL, NULL,  NULL,  dir);
	if(res <0 ){
		http_err(HTTPERR_URL);
		fprintf(stderr,"nfi_http_init: url %s incorrect.\n",url);
		free(serv->ops);
		return -1;
	}
	
	server_aux = (struct nfi_http_server *)malloc(sizeof(struct nfi_http_server));
	if(server_aux == NULL){
		http_err(HTTPERR_MEMORY);
		return -1;
	}

	//memset(server_aux, 0, sizeof(struct nfi_http_server));
	//strcpy(server_aux->path, dir);
	serv->private_info = (void *)server_aux;
	
	//serv->protocol = HTTP;
	
	serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1));	/* server address */
	if(serv->server == NULL){
		http_err(HTTPERR_MEMORY);
		return -1;
	}
	strcpy(serv->server, server);
	
	serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1));	/* server address */
	if(serv->url == NULL){
		http_err(HTTPERR_MEMORY);
		return -1;
	}
	
	strcpy(serv->url, url);
	serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
	memset(serv->wrk, 0, sizeof(struct nfi_worker));
	if(strcmp(prt, "httphilos") == 0){
#if defined(DEBUG_NFI)
		printf("httphilos\n");
#endif
		nfi_worker_init(serv->wrk, serv, 1);
	}
	else{
		nfi_worker_init(serv->wrk, serv, 0);
	}

	strcpy(server_aux->path, "http://");
	strcat(server_aux->path, server);

	char *tmp[] = {"fusedav", "-n", server_aux->path, "/tmp/null", NULL};
	server_aux->sess = dav_init(4, tmp);
	//printf("dav_init(server_aux->sess = %p)\n", server_aux->sess);
	
	if (server_aux->sess == NULL)
		return -1;
	
	return 0;
}

/************************************************************
 * Disconnect from the server				    *
 * **********************************************************/
int nfi_http_disconnect(struct nfi_server *serv)
{
	struct nfi_http_server *server_aux;
	
	if (serv == NULL)
		return 0;
	
	server_aux = (struct nfi_http_server *)serv->private_info;
	if(server_aux != NULL){
		if (server_aux->sess != NULL) {
			//printf("dav_destroy(server_aux->sess = %p)\n", server_aux->sess);
			dav_destroy(server_aux->sess); // after removing a free inside dav_destroy there are not any double free anymore! Why???
			server_aux->sess = NULL;
		}
		free(serv->private_info);
		serv->private_info = NULL;
	}
	
	if(serv->url != NULL){
		free(serv->url);
		serv->url = NULL;
	}
	
	if(serv->server != NULL){
		free(serv->server);
		serv->server = NULL;
	}
	
	//serv->protocol = -1;
	
	nfi_worker_end(serv->wrk);
	
	return 0;
}



/************************************************************
 * Reconnect to the HTTP server			    *
 ************************************************************/
int nfi_http_reconnect(struct nfi_server *serv)
{
	/* Don't see the serv result */
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res;
	struct nfi_http_server *server_aux;
	
	res = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
	//res = ParseURL(serv->url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(res <0 ){
		http_err(HTTPERR_URL);
		fprintf(stderr,"nfi_http_reconnect: url %s incorrect.\n",serv->url);
		return -1;
	}
	
	server_aux = (struct nfi_http_server *)malloc(sizeof(struct nfi_http_server));
	if(server_aux == NULL){		
		http_err(HTTPERR_MEMORY);
		return -1;
	}
	
	strcpy(server_aux->path, dir);
	
	serv->private_info = (void *)server_aux;
	return 0;
}

/************************************************************
 * Destroy HTTP operations				    *
 ************************************************************/
/*
// When is this called?
int nfi_http_destroy(struct nfi_server *serv)
{
	struct nfi_http_server *server_aux;
	
	if (serv == NULL)
		return 0;
	
	if(serv->ops != NULL)
		free(serv->ops);
	
	server_aux = (struct nfi_http_server *)serv->private_info;
	
	if(server_aux != NULL){
		printf("dav_destroy(server_aux->sess = %p)\n", server_aux->sess);
		dav_destroy(server_aux->sess);
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

int nfi_http_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr)
{
	int res;
	struct stat st;
	struct nfi_http_server *server_aux;
	struct nfi_http_fhandle *fh_aux;
	
	if (attr == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
	if (fh == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
	if (fh->priv_fh == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res <0){
			/* http_err(); not necessary */
			return -1;
		}
	}
#else
	
	if (serv->private_info == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
#endif
	fh_aux = (struct nfi_http_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_http_server *) serv->private_info;
	
	XPN_DEBUG_BEGIN_ARGS1(fh_aux->path)
	
	attr->at_size = 0;
	memset(&st, 0, sizeof(struct stat));
	res = dav_getattr(server_aux->sess, fh_aux->path, &st);
	/*********BORRAR********************/
	//if (res < 0){
	//	char s[255];
	//	extern int errno;
	//
	//	sprintf(s,"nfi_http_getattr: %s", fh_aux->path);
	//	perror(s);
	//	printf("%s : %d\n", s, errno);
	//}
	/*********BORRAR********************/
	if(res < 0){
#ifdef DBG_NFI
		fprintf(stderr,"nfi_http_getattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
#endif
		http_err(HTTPERR_GETATTR);
		XPN_DEBUG_END_ARGS1(fh_aux->path)
		return res;
	}
	
	HTTPtoNFIattr(attr, &st);
	XPN_DEBUG_END_ARGS1(fh_aux->path)
	return 0;
}


int nfi_http_setattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr)
{
	int res = -1;
	struct stat st;
	struct nfi_http_server *server_aux;
	struct nfi_http_fhandle *fh_aux;
	
	if (attr == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
	if (fh->priv_fh == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res <0){
			/* http_err(); not necessary */
			return -1;
		}
	}
#else
	
	if (serv->private_info == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
#endif
	
	fh_aux = (struct nfi_http_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_http_server *) serv->private_info;
	
	NFItoHTTPattr(&st, attr);
	/* no se como hacer el setattr */
	/*
	res = stat(fh_aux->path, &st);
	*/
	//res = truncate(fh_aux->path, st.st_size);
	if(res < 0){
		//fprintf(stderr,"nfi_http_setattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		fprintf(stderr,"nfi_http_setattr: Fail stat %s.\n",fh_aux->path);
		http_err(HTTPERR_GETATTR);
		return -1;
	}
	
	return 0;
}

int nfi_http_open(struct nfi_server *serv, char *url, struct nfi_fhandle *fho)
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int res;
	struct stat st;
	struct nfi_http_server *server_aux;
	struct nfi_http_fhandle *fh_aux;
	struct fuse_file_info info;
	
	XPN_DEBUG_BEGIN_ARGS1(url)
	
	if(url[strlen(url)-1] == '/'){
		res = nfi_http_opendir(serv, url, fho);
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	if (fho == NULL){
		http_err(HTTPERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res < 0){
			/* http_err(); not necessary */
			res = -1;
			XPN_DEBUG_END_ARGS1(url)
			return res;
		}
	}
#else
	if (serv->private_info == NULL){
		http_err(HTTPERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
#endif
	
	res = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(res < 0){
		fprintf(stderr,"nfi_http_open: url %s incorrect.\n",url);
		http_err(HTTPERR_URL);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		http_err(HTTPERR_MEMORY);
		res = -1;
		XPN_DEBUG_END_ARGS1(dir)
		return res;
	}
	
	strcpy(fho->url, url);
	
	fh_aux = (struct nfi_http_fhandle *)malloc(sizeof(struct nfi_http_fhandle));
	if (fh_aux == NULL){		
		http_err(HTTPERR_MEMORY);
		free(fho->url);
		res = -1;
		XPN_DEBUG_END_ARGS1(dir)
		return res;
	}
	memset(fh_aux, 0, sizeof(struct nfi_http_fhandle));
	
	server_aux = (struct nfi_http_server *) serv->private_info;
	
	info.flags = O_RDWR;
	res = dav_open(server_aux->sess, dir, &info);
	
	if (res < 0){
		info.flags = O_RDONLY;
		res = dav_open(server_aux->sess, dir, &info);
		if(res < 0){
			fprintf(stderr,"nfi_http_open: Fail open %s in server %s.\n",dir,serv->server);
			http_err(HTTPERR_MEMORY);
			free(fh_aux);
			free(fho->url);
			res = -1;
			XPN_DEBUG_END_ARGS1(dir)
			return res;
		}
	}
	
	fh_aux->fd = res;
	strcpy(fh_aux->path, dir);
	
	res = dav_getattr(server_aux->sess, fh_aux->path, &st);
	if(res < 0){
		/*
		http_err(HTTPERR_GETATTR);
		fprintf(stderr,"nfi_http_open: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		//fprintf(stderr,"nfi_http_open: Fail stat %s.\n",fh_aux->path);
		free(fh_aux);
		free(fho->url);
		res = -1;
		XPN_DEBUG_END_ARGS1(dir)
		return res;
		*/
	} else {
		if (S_ISDIR(st.st_mode)) {
			fho->type = NFIDIR;
		} else {
			if (S_ISREG(st.st_mode)) {
				fho->type = NFIFILE;
				fh_aux->size = st.st_size;
			} else {
				//fprintf(stderr,"nfi_http_open: %s isn't a FILE or FILE.\n",fh_aux->path);
				http_err(HTTPERR_GETATTR);
				free(fh_aux);
				free(fho->url);
				res = -1;
				XPN_DEBUG_END_ARGS1(dir)
				return res;
			}
		}
	}
	
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;
	
	res = 0;
	XPN_DEBUG_END_ARGS1(dir)
	return res;
}

int nfi_http_close(struct nfi_server *serv,  struct nfi_fhandle *fh)
{	
	struct nfi_http_server *server_aux;
	struct nfi_http_fhandle *fh_aux;
	
	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
	server_aux = (struct nfi_http_server *) serv->private_info;
	
	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_http_fhandle *) fh->priv_fh;
		dav_release(server_aux->sess, fh_aux->path, NULL);
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

ssize_t nfi_http_read(	struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{
	ssize_t new_size = 0;
	
	struct nfi_http_server *server_aux;
	struct nfi_http_fhandle *fh_aux;
	
	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
	if (fh == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res < 0){
			/* http_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
#endif
	
	fh_aux = (struct nfi_http_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_http_server *) serv->private_info;
	
	if ((unsigned long int)offset > fh_aux->size)
		size = 0;
	else if ((unsigned long int)offset + size > fh_aux->size)
		size = fh_aux->size - offset;
	
	if (size > 0) {
		new_size = dav_read(	server_aux->sess,
					fh_aux->path,
					buffer,
					size,
					offset,
					NULL);
	}
	XPN_DEBUG("read %s(%d) off %ld size %zu (ret:%zd)", fh->url, fh_aux->fd, (long int)offset, size, new_size)
	if(new_size < 0){
		fprintf(stderr, "nfi_http_read: Fail read %s off %ld size %zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, new_size, errno);
		errno = -new_size;
		perror("nfi_http_read");
		http_err(HTTPERR_READ);
		return -1;
	}
	
	return new_size;
}

ssize_t nfi_http_write(struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{
	ssize_t new_size = 0;
	struct nfi_http_server *server_aux;
	struct nfi_http_fhandle *fh_aux;
	
	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
	if (fh == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res < 0){
			/* http_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
#endif
	
	fh_aux = (struct nfi_http_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_http_server *) serv->private_info;
	
	if (size > 0) {
		new_size = dav_write(	server_aux->sess,
					fh_aux->path,
					buffer,
					size,
					offset,
					NULL);
	}
	XPN_DEBUG("write %s off %ld size %zu (ret:%zd)", fh->url, (long int)offset, size, new_size)
	if(new_size < 0){
		fprintf(stderr, "nfi_http_write: Fail write %s off %ld size %zu (ret:%zd) errno=%d\n",fh->url, (long int)offset, size, new_size, errno);
		errno = -new_size;
		perror("nfi_http_write");
		http_err(HTTPERR_WRITE);
		return -1;
	}
	
	return new_size;
}

int nfi_http_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res, fd;
	struct nfi_http_server *server_aux;
	struct nfi_http_fhandle *fh_aux;
	struct stat st;
	struct fuse_file_info info;
	
	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
	if (attr == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res < 0){
			/* http_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
#endif
	res = ParseURL(url,  NULL, NULL, NULL, server, NULL, dir);
	if(res < 0){
		fprintf(stderr,"nfi_http_create: url %s incorrect.\n",url);
		http_err(HTTPERR_URL);
		return -1;
	}
	/* private_info file handle */
	fh_aux = (struct nfi_http_fhandle *)malloc(sizeof(struct nfi_http_fhandle));
	if (fh_aux == NULL){
		http_err(HTTPERR_MEMORY);
		return -1;
	}
	
	bzero(fh_aux, sizeof(struct nfi_http_fhandle));
	
	server_aux = (struct nfi_http_server *) serv->private_info;
	
	/* create the file into the directory */
	info.flags = O_CREAT|O_RDWR|O_TRUNC;
	fd = dav_open(server_aux->sess, dir, &info); // FIXME: attr->mode
	//fd = open(dir, O_CREAT|O_RDWR|O_TRUNC, attr->at_mode);
	/*********BORRAR********************/
        //if (fd < 0){
        //       char s[255];
        //        extern int errno;
	//
        //        sprintf(s,"nfi_http_getattr: %s", fh_aux->path);
        //        perror(s);
        //        printf("%s : %d\n", s, errno);          
        //}
        /*********BORRAR********************/
	if(fd < 0){
		//fprintf(stderr,"nfi_http_creat: Fail creat %s in server %s.\n",fh_aux->path,serv->server);
		//fprintf(stderr,"nfi_http_creat: Fail creat %s.\n",fh_aux->path);
		fprintf(stderr,"nfi_http_creat: Fail creat %s.\n",dir);
		http_err(HTTPERR_LOOKUP);
		free(fh_aux);
		return -1;
	}
	fh->server = serv;
	
	fh_aux->fd = fd;
	strcpy(fh_aux->path, dir);
	
	fh->priv_fh = (void *)fh_aux;
	
	fh->url = (char *)malloc(strlen(url)+1);
	if(fh->url == NULL){
		http_err(HTTPERR_MEMORY);
		free(fh_aux);
		return -1;
	}
		
	strcpy(fh->url, url);
	memset (&st, 0, sizeof(struct stat));
	res = dav_getattr(server_aux->sess, fh_aux->path, &st);
	/*if(res < 0){
		fprintf(stderr,"nfi_http_creat: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		http_err(HTTPERR_GETATTR);
		free(fh->url);
		free(fh_aux);
		
		return -1;
	}*/
	
	HTTPtoNFIattr(attr, &st);
	
	return 0;
}

int nfi_http_remove(struct nfi_server *serv,  char *url)
{

	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res;
	struct nfi_http_server *server_aux;
	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res <0){
			/* http_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               http_err(HTTPERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_http_server *)serv->private_info;

	res = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	//res = ParseURL(url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(res < 0){
		//fprintf(stderr,"nfi_http_remove: url %s incorrect.\n",url);
		http_err(HTTPERR_URL);
		return -1;
	}
	/* remove the file into the directory */
	res = dav_unlink(server_aux->sess, dir);
	/*********BORRAR********************/
	//if (res < 0){
	//	char s[255];
	//	sprintf(s,"nfi_http_remove: %s", dir);
	//	perror(s);	
	//}
	/*********BORRAR********************/

	if(res < 0){
		fprintf(stderr,"nfi_http_remove: Fail remove %s in server %s\n", dir, serv->server);
		//fprintf(stderr,"nfi_http_remove: Fail remove %s\n", dir);
		http_err(HTTPERR_REMOVE);
		return -1;
	}



	return 0;
}

int nfi_http_rename(__attribute__((__unused__)) struct nfi_server *server, __attribute__((__unused__)) char *old_url, __attribute__((__unused__)) char *new_url)
{
	/*
        struct nfi_http_server *server_aux;
        struct nfi_http_fhandle *fh_aux;
	if (server == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res <0){
			http_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               http_err(HTTPERR_PARAM);
               return -1;
        }
#endif
	server_aux = (strcut nfi_http_server *)serv->private_info;
	*/

	/* ????? */
	return 0;
}

int nfi_http_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res;
	struct stat st;		/* HTTP attributes */
        struct nfi_http_server *server_aux;
        struct nfi_http_fhandle *fh_aux;


	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res <0){
			/* http_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               http_err(HTTPERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_http_server *)serv->private_info;

	res = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	//res = ParseURL(url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(res < 0){
		fprintf(stderr,"nfi_http_mkdir: url %s incorrect.\n",url);
		http_err(HTTPERR_URL);
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_http_fhandle *)malloc(sizeof(struct nfi_http_fhandle));
	if (fh_aux == NULL){
		http_err(HTTPERR_MEMORY);
		return -1;
	}


	bzero(fh_aux, sizeof(struct nfi_http_fhandle));
	/* create the dir into the directory */
	res = dav_mkdir(server_aux->sess, dir, /*attr->at_mode*/ 0777);
	
	if((res < 0)&&(errno != EEXIST)){
		http_err(HTTPERR_MKDIR);
		//fprintf(stderr,"nfi_http_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server);
		fprintf(stderr,"nfi_http_mkdir: Fail mkdir %s.\n",dir);
		free(fh_aux);
		return -1;
	}
	
	fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               http_err(HTTPERR_MEMORY);
	       free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

	HTTPtoNFIattr(attr, &st);


	return 0;
}

int nfi_http_rmdir(struct nfi_server *serv,  char *url)
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res;
	
	struct nfi_http_server *server_aux;
	
	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res <0){
			/* http_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
#endif
	
	server_aux = (struct nfi_http_server *)serv->private_info;
	
	res = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	//res = ParseURL(url,  NULL, NULL, NULL, NULL,  NULL,  dir);
	if(res < 0){
		fprintf(stderr,"nfi_http_rmdir: url %s incorrect.\n",url);
		http_err(HTTPERR_URL);
		return -1;
	}
	
	res = dav_rmdir(server_aux->sess, dir);
	if(res < 0){
		//fprintf(stderr,"nfi_http_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server);
		fprintf(stderr,"nfi_http_rmdir: Fail rmdir %s.\n",dir);
		http_err(HTTPERR_REMOVE);
		return -1;
	}
	
	return 0;
}

int nfi_http_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho)
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int res;	
	struct nfi_http_server *server_aux;
	struct nfi_http_fhandle *fh_aux;
	
	XPN_DEBUG_BEGIN_ARGS1(url)
	
	if (serv == NULL){
		//http_err(HTTPERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	if (fho == NULL){
		//http_err(HTTPERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res <0){
			// http_err(); not necessary
			res = -1;
			XPN_DEBUG_END_ARGS1(url)
			return res;
		}
	}
#else
	if (serv->private_info == NULL){
		//http_err(HTTPERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
#endif
	
	res = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	//res = ParseURL(url, NULL, NULL, NULL, NULL,  NULL,  dir);
	if(res < 0){
		fprintf(stderr,"nfi_http_opendir: url %s incorrect.\n",url);
		//http_err(HTTPERR_URL);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		//http_err(HTTPERR_MEMORY);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	strcpy(fho->url, url);
	
	fh_aux = (struct nfi_http_fhandle *)malloc(sizeof(struct nfi_http_fhandle));
	if (fh_aux == NULL){
		free(fho->url);
		//http_err(HTTPERR_MEMORY);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	server_aux = (struct nfi_http_server *) serv->private_info;
	
	fh_aux->dir = (DIR *)NULL;
	fh_aux->buf = NULL;
	fh_aux->offset = 0;
	/*
	//fh_aux->dir = opendir(dir);
	//printf("nfi_http_opendir: opendir(%s)=%p\n", dir, fh_aux->dir);
	if (fh_aux->dir == NULL){
		//fprintf(stderr,"nfi_http_opendir: Fail opendir %s in server %s.\n",dir,serv->server);
		//fprintf(stderr,"nfi_http_opendir: Fail opendir %s.\n",dir);
		free(fh_aux);
		free(fho->url);
		//http_err(HTTPERR_MEMORY);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	*/
	
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

int nfi_http_filler(void *buf, const char *name, __attribute__((__unused__)) const struct stat *stbuf, __attribute__((__unused__)) off_t off) {
	int res = -1;
	off_t offset;
	char *ret;

	offset = 0;
	while (((char *)buf)[offset] != 0) {
		offset += strlen(buf)+1;
	}

	ret = strcat(&((char *)buf)[offset], name);
	if (ret != NULL)
		res = 0;

	return res;
}

int nfi_http_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, char *entry, unsigned char *type)
{
	int res;
	//struct dirent *ent = NULL;
    struct nfi_http_server *server_aux;
    struct nfi_http_fhandle *fh_aux;


	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}

	if(fh->type != NFIDIR){
		http_err(HTTPERR_NOTDIR);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res <0){
			/* http_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               http_err(HTTPERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_http_server *)serv->private_info;
	fh_aux = (struct nfi_http_fhandle *)fh->priv_fh;

	entry[0] = 0;

	// FIXME: This might work in WebDAV servers (not tested yet), but it fails in simple HTTP servers.
	if (fh_aux->buf == NULL) {
		fh_aux->buf = calloc(1024, (NAME_MAX+1)); // FIXME: When is this freed?
		res = dav_readdir(server_aux->sess, fh_aux->path, fh_aux->buf, nfi_http_filler, (off_t)0, (struct fuse_file_info *)NULL);

		if(res != 0){
			// FIXME: We should do "GET /", and parse the result (providing it does not have an 'index.html').
			free(fh_aux->buf);
			fh_aux->buf = NULL;
			return res;
		}
	}

	if(type==NULL){
		return 0;
	}

	if (((const char *)fh_aux->buf)[fh_aux->offset] == 0) {
		free(fh_aux->buf);
		fh_aux->buf = NULL;
		return -1;
	}

	strcpy(entry, &((const char *)fh_aux->buf)[fh_aux->offset]);
	fh_aux->offset += strlen(&((const char *)fh_aux->buf)[fh_aux->offset])+1;

	return 0;
}

int nfi_http_closedir(struct nfi_server *serv,  struct nfi_fhandle *fh)
{
    //struct nfi_http_server *server_aux;
    struct nfi_http_fhandle *fh_aux;

	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_http_fhandle *) fh->priv_fh;
		//closedir(fh_aux->dir);
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;

	}

	if (fh->url != NULL){
		free(fh->url);
	}
	
	//server_aux = (struct nfi_http_server *)serv->private_info;
	fh_aux = (struct nfi_http_fhandle *)fh->priv_fh;

	if (fh_aux != NULL) {
		if (fh_aux->buf != NULL)
			free(fh_aux->buf);
		memset(fh_aux, 0, sizeof(struct nfi_http_fhandle));
	}

	return 0;
	
}


int nfi_http_statfs(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_info *inf)
{
	/*
	struct local_info localinf;
	int res;
        struct nfi_http_server *server_aux;


	if (serv == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}

	if (inf == NULL){
		http_err(HTTPERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		res = nfi_http_reconnect(serv);
		if(res <0){
		 http_err(); not necessary 
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               http_err(HTTPERR_PARAM);
               return -1;
        }
#endif
	
	server_aux = (struct nfi_http_server *)serv->private_info;
	res = local_statfs(server_aux->fh, &localinf, server_aux->cl);
	if(res <0){
		http_err(HTTPERR_STATFS);
		return -1;
	}

	HTTPtoNFIInfo(inf, &localinf);
	*/
	return 0;
}
