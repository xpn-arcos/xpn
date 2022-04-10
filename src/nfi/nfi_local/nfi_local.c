
#include "nfi/nfi_local/local_lib.h"

/*
 * TODO:
 *  -> local_tcp, local_udp ??????????????
 *  -> local_xxx a nfi_local_xxx
 *  -> local_misc.[ch]:
 *     -> ParseURL
 *
 * -> pasar CLIENT * a servidor (es comn a todos...)
 * -> Poner todo a punteros
 * -> struct nfi_fhandle ** Paso de puntero par�etro por referencia
 *
 */


/************************************************************
 * PRIVATE FUNCTIONS TO USE local SERVERS	            *
 ************************************************************/

void NFItoLOCALattr ( struct stat *att, struct nfi_attr *nfi_att )
{
	if (nfi_att->at_type == NFIFILE) {
		att->st_mode	= nfi_att->at_mode | S_IFREG;	/* protection */
	}

	if (nfi_att->at_type == NFIDIR) {
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

void LOCALtoNFIattr ( struct nfi_attr *nfi_att, struct stat *att )
{
	if (S_ISREG(att->st_mode)) {
		nfi_att->at_type = NFIFILE;
	}
	if (S_ISDIR(att->st_mode)) {
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


 // 
 // PUBLIC FUNCTIONS
 //

/************************************************************
 * Init Local 					    	    *
 ************************************************************/
int nfi_local_init ( char *url, struct nfi_server *serv, __attribute__((__unused__)) struct nfi_attr_server *attr )
{
	int res;
	char server[MAXPATHLEN], dir[MAXPATHLEN], prt[MAXPATHLEN];
	struct nfi_local_server *server_aux;

	DEBUG_BEGIN();

	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1 ;
	}

	/* functions */
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops)) ;
	if (serv->ops == NULL) {
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
	}

	bzero(serv->ops, sizeof(struct nfi_ops)) ;

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

	res = ParseURL(url,  prt, NULL, NULL, server, NULL, dir) ;
	if (res < 0) {
	    debug_error("nfi_local_init: incorrect URL '%s'.\n", url) ;
	    free(serv->ops) ;
	    return -1;
	}

	server_aux = (struct nfi_local_server *)malloc(sizeof(struct nfi_local_server)) ;
	if (server_aux == NULL) {
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
	}

	strcpy(server_aux->path, dir) ;
	serv->private_info = (void *)server_aux;

	//serv->protocol = LOCAL;

	serv->server = STRING_MISC_StrDup(server) ; /* server address */
	if (serv->server == NULL) {
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
	}

	serv->url = STRING_MISC_StrDup(url) ; /* server address */
	if (serv->url == NULL) {
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
	}

	serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker)) ;
	memset(serv->wrk, 0, sizeof(struct nfi_worker)) ;
	if (strcmp(prt, "filehilos") == 0)
	{
	    debug_info("filehilos in ptr.\n") ;
	    nfi_worker_init(serv->wrk, serv, 1) ;
	}
	else
	{
	    nfi_worker_init(serv->wrk, serv, 0) ;
	}

	DEBUG_END();

	// Return OK
	return 0;
}


/************************************************************
 * Reconnect to the LOCAL server			    *
 ************************************************************/
int nfi_local_reconnect ( struct nfi_server *serv )
{
	/* Don't see the serv result */
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res;
	struct nfi_local_server *server_aux;

	DEBUG_BEGIN();

	res = ParseURL(serv->url,  NULL, NULL, NULL, server, NULL, dir) ;
	if (res < 0) {
	    debug_error("nfi_local_reconnect: incorrect '%s' URL.\n", serv->url) ;
	    return -1;
	}

	server_aux = (struct nfi_local_server *)malloc(sizeof(struct nfi_local_server)) ;
	if (server_aux == NULL) {
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
	}

	strcpy(server_aux->path, dir) ;
	serv->private_info = (void *)server_aux;

	DEBUG_END();

	// Return OK
	return 0;
}


/************************************************************
 * Disconnect from the server				    *
 * **********************************************************/
int nfi_local_disconnect ( struct nfi_server *serv )
{
	DEBUG_BEGIN();

	// Check arguments
	if (serv == NULL) {
	    return 0;
	}

	// Free and null
	FREE_AND_NULL(serv->private_info) ;
	FREE_AND_NULL(serv->url) ;
	FREE_AND_NULL(serv->server) ;

	nfi_worker_end(serv->wrk) ;

	DEBUG_END();

	// Return OK
	return 0;
}


/************************************************************
 * Destroy LOCAL operations				    *
 ************************************************************/

int nfi_local_destroy ( struct nfi_server *serv )
{
	DEBUG_BEGIN();

// TODO: When is this called?

/*
	// Check arguments
	if (serv == NULL) {
	    return 0;
	}

	// Free and null
 	FREE_AND_NULL(serv->ops) ;
	FREE_AND_NULL(serv->private_info) ;
 	FREE_AND_NULL(serv->url) ;
	FREE_AND_NULL(serv->server) ;

	//serv->protocol = -1;

	nfi_worker_end(serv->wrk) ;
*/

	DEBUG_END();

	// Return OK
	return 0;
}


/************************************************************
 * GENERIC FUNCTIONS 					    *
 ************************************************************/

int nfi_local_getattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
{
	int res;
	struct stat st;
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;

	DEBUG_BEGIN();

	// Check arguments
	if (attr == NULL) {
	    debug_error("attr argument is NULL.\n") ;
	    return -1;
	}
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}
	if (fh == NULL) {
	    debug_error("fh argument is NULL.\n") ;
	    return -1;
	}

	if (fh->priv_fh == NULL) {
	    debug_error("fh->priv_fh argument is NULL.\n") ;
	    return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res <0) {
		    return -1;
		}
	}
#else
	if (serv->private_info == NULL) {
	    debug_error("serv->private_info argument is NULL.\n") ;
	    return -1;
	}
#endif
	fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_local_server *) serv->private_info;

	res = stat(fh_aux->path, &st) ;
	if (res < 0) {
	    debug_error("nfi_local_getattr: Fail stat %s.\n", fh_aux->path) ;
	    return res;
	}

	LOCALtoNFIattr(attr, &st) ;

	DEBUG_END();

	// Return OK
	return 0;
}

int nfi_local_setattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
{
	int res;
	struct stat st;
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;

	DEBUG_BEGIN();

	// Check arguments
	if (attr == NULL) {
	    debug_error("attr argument is NULL.\n") ;
	    return -1;
	}
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}

	if (fh->priv_fh == NULL) {
	    debug_error("fh->priv_fh argument is NULL.\n") ;
	    return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res <0) {
		    return -1;
		}
	}
#else

	if (serv->private_info == NULL) {
	    debug_error("serv->private_info field is NULL.\n") ;
	    return -1;
	}
#endif

	fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_local_server *) serv->private_info;

	NFItoLOCALattr(&st, attr) ;
	/* no se comp hacer el setattr */
	/*
	res = stat(fh_aux->path, &st) ;
	*/
	res = truncate(fh_aux->path, st.st_size) ;
	if (res < 0) {
		debug_error("nfi_local_setattr: Fail stat %s.\n", fh_aux->path) ;
		return -1;
	}

	DEBUG_END();

	// Return OK
	return 0;
}

int nfi_local_open ( struct nfi_server *serv, char *url, struct nfi_fhandle *fho )
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int res;
	struct stat st;
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;

	DEBUG_BEGIN();

	// Check arguments...
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}
	if (fho == NULL) {
	    debug_error("fho argument is NULL.\n") ;
	    return -1;
	}

	if (url[strlen(url)-1] == '/') {
		res = nfi_local_opendir(serv, url, fho) ;
		return res;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res < 0) {
			res = -1;
			return res;
		}
	}
#else
	if (serv->private_info == NULL) {
	    debug_error("serv->private_info field is NULL.\n") ;
	    return -1;
	}
#endif

	res = ParseURL(url, NULL, NULL, NULL, server, NULL, dir) ;
	if (res < 0) {
	    debug_error("LOCALERR_URL.\n") ;
	    return -1 ;
	}

	fho->url = STRING_MISC_StrDup(url) ;
	if (fho->url == NULL) {
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
	}

	fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle)) ;
	if (fh_aux == NULL) {	 
	    free(fho->url) ;
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
	}

	server_aux = (struct nfi_local_server *) serv->private_info;

	// Open file...
	res = files_posix_open(dir, O_RDWR) ;
	if (res < 0)
	{
	    debug_error("files_posix_open fails to open '%s' in server %s.\n", dir, serv->server) ;
	    free(fh_aux) ;
	    free(fho->url) ;
	    return -1;
	}

	fh_aux->fd = res;
	strcpy(fh_aux->path, dir) ;

	res = stat(fh_aux->path, &st) ;
	if (res < 0) {
	    debug_error("LOCALERR_GETATTR.\n") ;
	    free(fh_aux) ;
	    free(fho->url) ;
	    return -1 ;
	}

	if (S_ISDIR(st.st_mode)) {
		fho->type = NFIDIR;
	} else {
		if (S_ISREG(st.st_mode)) {
			fho->type = NFIFILE;
		} else {
	                debug_error("LOCALERR_GETATTR.\n") ;
			free(fh_aux) ;
			free(fho->url) ;
			return -1;
		}
	}

	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;
	res = 0;

	DEBUG_END();

	return res;
}

int nfi_local_close ( struct nfi_server *serv,  struct nfi_fhandle *fh )
{
	struct nfi_local_fhandle *fh_aux ;

	DEBUG_BEGIN();

	// Check params
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}
	if (fh == NULL) {
	    debug_error("fh argument is NULL.\n") ;
	    return -1;
	}

	fh_aux = (struct nfi_local_fhandle *) fh->priv_fh ;
	if (fh_aux != NULL) {
	    files_posix_close(fh_aux->fd) ;
	}

	/* free memory */
	FREE_AND_NULL(fh->priv_fh) ;
	FREE_AND_NULL(fh->url) ;

	fh->type   = NFINULL ;
	fh->server = NULL ;

	DEBUG_END();

	// return OK
	return 0;
}

ssize_t nfi_local_read ( struct nfi_server *serv,
			 struct nfi_fhandle *fh,
			 void *buffer,
			 off_t offset,
			 size_t size )
{
	ssize_t new_size;
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;

	DEBUG_BEGIN();

	// Check params
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}
	if (fh == NULL) {
	    debug_error("fh argument is NULL.\n") ;
	    return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res < 0) {
			return -1;
		}
	}
#else
	if (serv->private_info == NULL) {
	    debug_error("serv->private_info field is NULL.\n") ;
	    return -1;
	}
#endif

	fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_local_server *) serv->private_info;

	if (fh_aux == NULL) {
	    debug_error("nfi_local_read: fh_aux == NULL\n") ;
	    return -1;
	}

	real_posix_lseek(fh_aux->fd, offset, SEEK_SET) ;
	new_size = files_posix_read_buffer(fh_aux->fd, buffer, size) ;
	debug_info("read %s(%d) off %ld size %zu (ret:%zd)", fh->url, fh_aux->fd, (long int)offset, size, new_size)
	if (new_size < 0) {
	    debug_error("files_posix_read_buffer reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, new_size, errno) ;
	    return -1;
	}

	DEBUG_END();

	return new_size;
}

ssize_t nfi_local_write ( struct nfi_server *serv,
			  struct nfi_fhandle *fh,
			  void *buffer,
			  off_t offset,
			  size_t size )
{
	ssize_t new_size;
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;

	DEBUG_BEGIN();

	// Check params
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}
	if (fh == NULL) {
	    debug_error("fh argument is NULL.\n") ;
	    return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res < 0) {
			return -1;
		}
	}
#else
	if (serv->private_info == NULL) {
	    debug_error("serv->private_info field is NULL.\n") ;
	    return -1;
	}
#endif

	fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_local_server *) serv->private_info;

	real_posix_lseek(fh_aux->fd, offset, SEEK_SET) ;
	new_size = files_posix_write_buffer(fh_aux->fd, buffer, size) ;
	debug_info("write %s off %ld size %zu (ret:%zd)", fh->url, (long int)offset, size, new_size)
	if (new_size < 0) {
	    debug_error("files_posix_write_buffer writes zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, new_size, errno) ;
	    return -1;
	}

	DEBUG_END();

	return new_size;
}

int nfi_local_create ( struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh )
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res, fd;
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;
	struct stat st;

	DEBUG_BEGIN();

	// Check params
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}
	if (attr == NULL) {
	    debug_error("attr argument is NULL.\n") ;
	    return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res < 0) {
			return -1;
		}
	}
#else
	if (serv->private_info == NULL) {
	    debug_error("serv->private_info field is NULL.\n") ;
	    return -1;
	}
#endif
	res = ParseURL(url,  NULL, NULL, NULL, server, NULL, dir) ;
	if (res < 0) {
	    debug_error("nfi_local_create: url '%s' incorrect.\n", url) ;
	    return -1;
	}
	/* private_info file handle */
	fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle)) ;
	if (fh_aux == NULL) {
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
	}

	bzero(fh_aux, sizeof(struct nfi_local_fhandle)) ;

	server_aux = (struct nfi_local_server *) serv->private_info;

	/* create the file into the directory */
	fd = files_posix_open(dir, O_CREAT|O_RDWR|O_TRUNC, attr->at_mode) ;
	if (fd < 0) {
	    debug_error("files_posix_open fails to creat '%s' in server '%s'.\n", dir, serv->server) ;
	    free(fh_aux) ;
	    return -1;
	}
	fh->server = serv;
	fh_aux->fd = fd;
	strcpy(fh_aux->path, dir) ;
	fh->priv_fh = (void *)fh_aux;

	fh->url = STRING_MISC_StrDup(url) ;
	if (fh->url == NULL) {
	    free(fh_aux) ;
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
	}
	
	memset(&st, 0, sizeof(struct stat)) ;
	res = real_posix_stat(fh_aux->path, &st) ;
	if (res < 0) {
	    debug_error("real_posix_stat fails to stat '%s' in server '%s'.\n", fh_aux->path, serv->server) ;
	    free(fh->url) ;
	    free(fh_aux) ;
	    return -1;
	}

	LOCALtoNFIattr(attr, &st) ;

	DEBUG_END();

	return fd;
}

int nfi_local_remove ( struct nfi_server *serv,  char *url )
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res;
	struct nfi_local_server *server_aux;

	DEBUG_BEGIN();

	// Check arguments
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res <0) {
			return -1;
		}
	}
#else
	if (serv->private_info == NULL) {
	    debug_error("serv->private_info field is NULL.\n") ;
	    return -1;
        }
#endif

	server_aux = (struct nfi_local_server *)serv->private_info;

	res = ParseURL(url,  NULL, NULL, NULL, server, NULL, dir) ;
	if (res < 0) {
		debug_error("nfi_local_remove: url '%s' incorrect.\n", url) ;
		return -1;
	}
	/* remove the file into the directory */
	res = real_posix_unlink(dir) ;
	if (res < 0) {
		debug_error("nfi_local_remove: Fail remove %s in server %s.\n", dir, serv->server) ;
		return -1;
	}

	DEBUG_END();

	// Return OK
	return 0;
}

int nfi_local_rename (__attribute__((__unused__)) struct nfi_server *server, __attribute__((__unused__)) char *old_url, __attribute__((__unused__)) char *new_url )
{
	DEBUG_BEGIN();

	/*
        struct nfi_local_server *server_aux;
        struct nfi_local_fhandle *fh_aux;

	// Check arguments
	if (server == NULL) {
	    debug_error("server argument is NULL.\n") ;
	    return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res <0) {
			return -1;
		}
	}
#else
	if (serv->private_info == NULL) {
	    debug_error("serv->private_info field is NULL.\n") ;
	    return -1;
        }
#endif
	server_aux = (strcut nfi_local_server *)serv->private_info;
	*/

	// TODO !!

	DEBUG_END();

	// Return OK
	return 0;
}

int nfi_local_mkdir ( struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh )
{
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res;
	struct stat st;		/* LOCAL attributes */
        struct nfi_local_server *server_aux;
        struct nfi_local_fhandle *fh_aux;

	DEBUG_BEGIN();

	// Check arguments...
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}
	if (attr == NULL) {
	    debug_error("attr argument is NULL.\n") ;
	    return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res <0) {
			return -1;
		}
	}
#else
	if (serv->private_info == NULL) {
	    debug_error("serv->private_info field is NULL.\n") ;
	    return -1;
        }
#endif

	server_aux = (struct nfi_local_server *)serv->private_info;

	res = ParseURL(url,  NULL, NULL, NULL, server, NULL, dir) ;
	if (res < 0) {
		debug_error("nfi_local_mkdir: url '%s' incorrect.\n", url) ;
		return -1;
	}

	/* private_info file handle */
	fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle)) ;
	if (fh_aux == NULL) {
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
	}

	bzero(fh_aux, sizeof(struct nfi_local_fhandle)) ;

	/* create the dir into the directory */
	res = real_posix_mkdir(dir, /*attr->at_mode*/ 0777) ;
	if ((res < 0) && (errno != EEXIST))
	{
		debug_error("nfi_local_mkdir: Fail mkdir %s.\n", dir) ;
		free(fh_aux) ;
		return -1;
	}

	fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

	fh->url = STRING_MISC_StrDup(url) ;
        if (fh->url == NULL) {
	    free(fh_aux) ;
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
        }

	LOCALtoNFIattr(attr, &st) ;

	DEBUG_END();

        // return OK
	return 0;
}

int nfi_local_rmdir ( struct nfi_server *serv,  char *url )
{
	int res;
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	struct nfi_local_server *server_aux;

	DEBUG_BEGIN();

	// Check params
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res <0) {
			return -1;
		}
	}
#else
	if (serv->private_info == NULL) {
	    debug_error("serv->private_info field is NULL.\n") ;
	    return -1;
	}
#endif

	server_aux = (struct nfi_local_server *)serv->private_info;

	res = ParseURL(url,  NULL, NULL, NULL, server, NULL, dir) ;
	if (res < 0) {
		debug_error("nfi_local_rmdir: url '%s' incorrect.\n", url) ;
		return -1;
	}

	res = real_posix_rmdir(dir) ;
	if (res < 0)
	{
		debug_error(stderr,"nfi_local_rmdir: Fail rmdir %s.\n", dir) ;
		return -1;
	}

	DEBUG_END();

	// Return OK
	return 0;
}

int nfi_local_opendir ( struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int res;
	struct nfi_local_server *server_aux;
	struct nfi_local_fhandle *fh_aux;

	DEBUG_BEGIN();

	// Check params
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}
	if (fho == NULL) {
	    debug_error("fho argument is NULL.\n") ;
	    return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res <0) {
			res = -1;
			return res;
		}
	}
#else
	if (serv->private_info == NULL) {
	    debug_error("serv->private_info field is NULL.\n") ;
	    return -1;
	}
#endif

	res = ParseURL(url, NULL, NULL, NULL, server, NULL, dir) ;
	if (res < 0) {
		debug_error("nfi_local_opendir: url %s incorrect.\n", url) ;
		return -1 ;
	}

	fho->url = STRING_MISC_StrDup(url) ;
	if (fho->url == NULL) {
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
	}

	fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle)) ;
	if (fh_aux == NULL) {
	    free(fho->url) ;
	    debug_error("LOCALERR_MEMORY\n") ;
	    return -1;
	}

	server_aux = (struct nfi_local_server *) serv->private_info;

	fh_aux->dir = real_posix_opendir(dir) ;
	if (fh_aux->dir == NULL) {
	    free(fh_aux) ;
	    free(fho->url) ;
	    debug_error("real_posix_opendir fails to open directory '%s' in server '%s'.\n", dir, serv->server) ;
	    return -1;
	}

	fh_aux->fd = res;
	strcpy(fh_aux->path, dir) ;
	fho->type = NFIDIR;
	fho->server = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;
	res = 0;

	DEBUG_END();

	return res;
}

int nfi_local_readdir ( struct nfi_server *serv,  struct nfi_fhandle *fh, char *entry, unsigned char *type )
{
	struct dirent *ent;
        struct nfi_local_server *server_aux;
        struct nfi_local_fhandle *fh_aux;

	DEBUG_BEGIN();

	// Check params
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}
	if (fh == NULL) {
	    debug_error("fh argument is NULL.\n") ;
	    return -1;
	}

	if (fh->priv_fh == NULL) {
	    debug_error("fh->priv_fh field is NULL.\n") ;
	    return -1;
	}
	if (fh->type != NFIDIR) {
	    debug_error("fh->type field is NULL.\n") ;
	    return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res <0) {
			return -1;
		}
	}
#else
	if (serv->private_info == NULL) {
	    debug_error("fh->private_info field is NULL.\n") ;
	    return -1;
        }
#endif

	server_aux = (struct nfi_local_server *)serv->private_info;
	fh_aux = (struct nfi_local_fhandle *)fh->priv_fh;

	entry[0] = '\0';
	ent = real_posix_readdir(fh_aux->dir) ;

	if (ent == NULL) {
		//printf("nfi_local_readdir: ent=%p, errno=%d, EBADF=%d\n", ent, errno, EBADF) ;
		//perror("nfi_local_readdir: readdir") ;
		return 1;
	}
	if (type==NULL) {
		return 0;
	}

	strcpy(entry, ent->d_name) ;
	*type = ent->d_type;

	// Return OK
	return 0;
}

int nfi_local_closedir ( struct nfi_server *serv,  struct nfi_fhandle *fh )
{
	struct nfi_local_fhandle *fh_aux;

	DEBUG_BEGIN();

	// Check arguments
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}
	if (fh == NULL) {
	    debug_error("fh argument is NULL.\n") ;
	    return -1;
	}

	fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
	if (fh_aux != NULL) {
	    real_posix_closedir(fh_aux->dir) ;
	}

        /* free memory */
	FREE_AND_NULL(fh->priv_fh) ;
	FREE_AND_NULL(fh->url) ;

	DEBUG_END();

	// Return OK
	return 0;
}

int nfi_local_statfs ( __attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_info *inf )
{
	DEBUG_BEGIN();

	/*
	struct local_info localinf;
	int res;
        struct nfi_local_server *server_aux;

	// Check params
	if (serv == NULL) {
	    debug_error("serv argument is NULL.\n") ;
	    return -1;
	}

	if (inf == NULL) {
	    debug_error("inf argument is NULL.\n") ;
	    return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL) {
		res = nfi_local_reconnect(serv) ;
		if (res <0) {
			return -1;
		}
	}
#else
	if (serv->private_info == NULL) {
	    debug_error("serv->private_info field is NULL.\n") ;
	    return -1;
        }
#endif

	server_aux = (struct nfi_local_server *)serv->private_info;
	res = local_statfs(server_aux->fh, &localinf, server_aux->cl) ;
	if (res <0) {
		debug_error("LOCALERR_STATFS") ;
		return -1;
	}

	LOCALtoNFIInfo(inf, &localinf) ;
	*/

	// TODO

	DEBUG_END();

	// Return OK
	return 0;
}

