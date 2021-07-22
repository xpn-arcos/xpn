#include "nfi_gridftp/gridftp_lib.h"

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


const char *gridftp_prefix="gsiftp://";
//pthread_mutex_t gridftp_mutex = PTHREAD_MUTEX_INITIALIZER;
//globus_mutex_t gridftp_globus_mutex;

void check_result(globus_result_t ret)
{
	char * tmpstr;
	if (ret != GLOBUS_SUCCESS)
	{
		tmpstr = globus_object_printable_to_string(globus_error_get(ret));
		fprintf(stderr, "Error: %s\n", tmpstr);
		globus_free(tmpstr);
	
	}
}

void check_error(globus_object_t *error)
{
	char * tmpstr;
	if (error != GLOBUS_SUCCESS)
	{
		tmpstr = globus_error_get_short_desc(error);
		fprintf(stderr, "Error: %s %d\n", tmpstr, globus_error_get_type(error));
		globus_free(tmpstr);

		tmpstr = globus_error_print_friendly(globus_error_get_cause(error));
		fprintf(stderr, "%s\n", tmpstr);
		globus_free(tmpstr);

		tmpstr = globus_error_get_long_desc(error);
		if (tmpstr != NULL) {
			fprintf(stderr, "       %s\n", tmpstr);
			globus_free(tmpstr);
		}
	
	}
}

static void data_read_cb (void *user_arg,
	 globus_ftp_client_handle_t * handle,
	 globus_object_t * err,
	 globus_byte_t * buffer,
	 globus_size_t length,
	 __attribute__((__unused__)) globus_off_t offset,
	 globus_bool_t eof)
{
	//pthread_mutex_lock (&gridftp_mutex);
	//globus_mutex_lock (&gridftp_globus_mutex);

	globus_size_t *rbytes = (globus_size_t *)user_arg;

	XPN_DEBUG_BEGIN_CUSTOM("handle %p buffer %p offset %lu length %lu rbytes %lu eof %d", *handle, buffer, offset, length, *rbytes, eof);

	if (err != GLOBUS_SUCCESS) {
		fprintf (stderr, "data_read_cb error: %s\n", globus_object_printable_to_string (err));
		fprintf (stderr, "%s\n", globus_error_print_friendly(globus_error_get_cause(err)));
	}

	*rbytes -= length;
	if (*rbytes <= 0)
		*rbytes = 1;

	if (eof != GLOBUS_TRUE)
		globus_ftp_client_register_read (handle,
        	                                buffer+length,
                	                        *rbytes,
                        	                data_read_cb, rbytes);
	//pthread_mutex_unlock (&gridftp_mutex);
	//globus_mutex_unlock (&gridftp_globus_mutex);

	return;
}

static void data_write_cb (void *user_arg,
	 globus_ftp_client_handle_t * handle,
	 globus_object_t * err,
	 globus_byte_t * buffer,
	 globus_size_t length,
	 globus_off_t offset,
	 globus_bool_t eof)
{
	//pthread_mutex_lock (&gridftp_mutex);
	//globus_mutex_lock (&gridftp_globus_mutex);

	globus_size_t *rbytes = (globus_size_t *)user_arg;

	XPN_DEBUG_BEGIN_CUSTOM("handle %p buffer %p offset %lu length %lu rbytes %lu eof %d", *handle, buffer, offset, length, *rbytes, eof);

	if (err != GLOBUS_SUCCESS) {
		fprintf (stderr, "data_write_cb error: %s\n", globus_object_printable_to_string (err));
		fprintf (stderr, "%s\n", globus_error_print_friendly(globus_error_get_cause(err)));
	}

	*rbytes -= length;
	if (*rbytes <= 0)
		*rbytes = 1;

	if (eof != GLOBUS_TRUE)
		globus_ftp_client_register_write (handle,
        	                                  buffer+length,
                	                          *rbytes,
                	                          offset,
                	                          eof,
                        	                  data_write_cb, rbytes);
	//pthread_mutex_unlock (&gridftp_mutex);
	//globus_mutex_unlock (&gridftp_globus_mutex);

	return;
}

/********************************************************************
* done_cb: A pointer to this function is passed to the call to
* globus_ftp_client_put (and all the other high level transfer
* operations). It is called when the transfer is completely
* finished, i.e. both the data channel and control channel exchange.
* Here it simply sets a global variable (done) to true so the main
* program will exit the while loop.
********************************************************************/
static void done_cb (void *user_arg, __attribute__((__unused__)) globus_ftp_client_handle_t * handle, globus_object_t * err) {
	struct nfi_gridftp_fhandle *fh_aux = (struct nfi_gridftp_fhandle *)user_arg;

	XPN_DEBUG_BEGIN_CUSTOM("handle %p err %lu", *handle, (unsigned long)err);
	
	if (err != GLOBUS_SUCCESS) {
		fprintf (stderr, "done_cb error: %s\n", globus_object_printable_to_string (err));
		fprintf (stderr, "%s\n", globus_error_print_friendly(globus_error_get_cause(err)));
	}
	
	/*
	pthread_mutex_lock (&fh_aux->pmutex);
	fh_aux->done = GLOBUS_TRUE;
	pthread_cond_signal (&fh_aux->pcond);
	pthread_mutex_unlock (&fh_aux->pmutex);
	*/
	globus_mutex_lock (&fh_aux->gmutex);
	fh_aux->done = GLOBUS_TRUE;
	globus_cond_signal (&fh_aux->gcond);
	globus_mutex_unlock (&fh_aux->gmutex);
	
	return;
}

static void noop_cb (void *user_arg, __attribute__((__unused__)) globus_ftp_client_handle_t * handle, __attribute__((__unused__)) globus_object_t * err) {
	struct nfi_gridftp_fhandle *fh_aux = (struct nfi_gridftp_fhandle *)user_arg;

	//fprintf(stderr, "noop_cb handle %p err %lu\n", *handle, (unsigned long)err);
	
	/*
	pthread_mutex_lock (&fh_aux->pmutex);
	fh_aux->done = GLOBUS_TRUE;
	pthread_cond_signal (&fh_aux->pcond);
	pthread_mutex_unlock (&fh_aux->pmutex);
	*/
	globus_mutex_lock (&fh_aux->gmutex);
	fh_aux->done = GLOBUS_TRUE;
	globus_cond_signal (&fh_aux->gcond);
	globus_mutex_unlock (&fh_aux->gmutex);
	
	return;
}

/************************************************************
 * PRIVATE FUNCTIONS TO USE local SERVERS	            *
 ************************************************************/
void NFItoGRIDFTPattr(struct stat *att, struct nfi_attr *nfi_att)
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

void GRIDFTPtoNFIattr(struct nfi_attr *nfi_att, struct stat *att)
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

/*
void GRIDFTPtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *local_inf)
{
}
*/

/************************************************************
 * PUBLIC FUNCTIONS TO USE GRIDFTP			    *
 ************************************************************/

/************************************************************
 * Init gridftp					    	    *
 ************************************************************/
int nfi_gridftp_init(char *url, struct nfi_server *serv, __attribute__((__unused__)) struct nfi_attr_server *attr)
{
	char server[MAXPATHLEN], dir[MAXPATHLEN],prt [MAXPATHLEN];
	int ret;
	struct nfi_gridftp_server *server_aux;
	//globus_result_t res;

	//printf("nfi_gridftp_init ...\n");
	if(serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

	/* functions */
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
	if (serv->ops == NULL){
		gridftp_err(GRIDFTPERR_MEMORY);
		return -1;
	}

	serv->ops->nfi_reconnect  = nfi_gridftp_reconnect;
	serv->ops->nfi_disconnect = nfi_gridftp_disconnect;

	serv->ops->nfi_getattr	= nfi_gridftp_getattr;
	serv->ops->nfi_setattr	= nfi_gridftp_setattr;

	serv->ops->nfi_open	= nfi_gridftp_open;
	serv->ops->nfi_close	= nfi_gridftp_close;

	serv->ops->nfi_read	= nfi_gridftp_read;
	serv->ops->nfi_write	= nfi_gridftp_write;

	serv->ops->nfi_create	= nfi_gridftp_create;
	serv->ops->nfi_remove	= nfi_gridftp_remove;
	serv->ops->nfi_rename	= nfi_gridftp_rename;

	serv->ops->nfi_mkdir	= nfi_gridftp_mkdir;
	serv->ops->nfi_rmdir	= nfi_gridftp_rmdir;
	serv->ops->nfi_opendir	= nfi_gridftp_opendir;
	serv->ops->nfi_readdir	= nfi_gridftp_readdir;
	serv->ops->nfi_closedir	= nfi_gridftp_closedir;
	serv->ops->nfi_statfs	= nfi_gridftp_statfs;

	ret = ParseURL(url, prt, NULL, NULL, server, NULL, dir);
	if(ret < 0){
		gridftp_err(GRIDFTPERR_URL);
		fprintf(stderr,"nfi_gridftp_init: url %s incorrect.\n",url);
		free(serv->ops);
		return -1;
	}
	server_aux = (struct nfi_gridftp_server *)malloc(sizeof(struct nfi_gridftp_server));
	if(server_aux == NULL){
		gridftp_err(GRIDFTPERR_MEMORY);
		return -1;
	}

	strcpy(server_aux->path, gridftp_prefix);
	strcpy(server_aux->path+strlen(gridftp_prefix), server);
	//strcpy(server_aux->path+strlen(gridftp_prefix)+strlen(server), dir);
	//printf("nfi_gridftp_init: path = \"%s\"\n", server_aux->path);

	serv->private_info = (void *)server_aux;

	//serv->protocol = GRIDFTP;

	serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1));	/* server address */
	if(serv->server == NULL){
		gridftp_err(GRIDFTPERR_MEMORY);
		return -1;
	}
	strcpy(serv->server, server);

	serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1));	/* server address */
	if(serv->url == NULL){
		gridftp_err(GRIDFTPERR_MEMORY);
		return -1;
	}

	strcpy(serv->url, url);
        serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
	memset(serv->wrk, 0, sizeof(struct nfi_worker));
	if(strcmp(prt, "gridftphilos") == 0){
#if defined(DEBUG_NFI)
		printf("gridftphilos\n");
#endif
		nfi_worker_init(serv->wrk, serv, 1);
	}
	else{
		nfi_worker_init(serv->wrk, serv, 0);
	}

	// GLOBUS_THREAD_MODEL="pthread" is required since Globus 5.2
	char *env = getenv("GLOBUS_THREAD_MODEL");
	if (env == NULL)
		setenv("GLOBUS_THREAD_MODEL", "pthread", 0);

	//pthread_mutex_lock(&gridftp_mutex);

	globus_module_activate(GLOBUS_FTP_CLIENT_MODULE);
	
	//res = globus_mutex_init(&gridftp_globus_mutex, GLOBUS_NULL);
	//assert(res == GLOBUS_SUCCESS);
	
	//pthread_mutex_unlock(&gridftp_mutex);

	return 0;
}

/************************************************************
 * Disconnect to the server				    *
 * **********************************************************/
int nfi_gridftp_disconnect(struct nfi_server *serv)
{

        struct nfi_gridftp_server *server_aux;
        

	if (serv == NULL)
	       return 0;


        server_aux = (struct nfi_gridftp_server *)serv->private_info;
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
	
	globus_module_deactivate(GLOBUS_FTP_CLIENT_MODULE);

	return 0;
}



/************************************************************
 * Reconnect to the NFS server				    *
 ************************************************************/
int nfi_gridftp_reconnect(struct nfi_server *serv)
{
	/* Don't see the serv result */
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_gridftp_server *server_aux;

	ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret <0 ){
		gridftp_err(GRIDFTPERR_URL);
		fprintf(stderr,"nfi_gridftp_reconnect: url %s incorrect.\n",serv->url);
		return -1;
	}

	server_aux = (struct nfi_gridftp_server *)malloc(sizeof(struct nfi_gridftp_server));
	if(server_aux == NULL){		
		gridftp_err(GRIDFTPERR_MEMORY);
		return -1;
	}

	strcpy(server_aux->path, dir);

	serv->private_info = (void *)server_aux;
	return 0;
}

/************************************************************
 * Destroy NFS operations				    *
 ************************************************************/

int nfi_gridftp_destroy(struct nfi_server *serv)
{

	struct nfi_gridftp_server *server_aux;

	if (serv == NULL)
		return 0;

	if(serv->ops != NULL)
		free(serv->ops);

	server_aux = (struct nfi_gridftp_server *)serv->private_info;

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

	globus_module_deactivate(GLOBUS_FTP_CLIENT_MODULE);

	return 0;
}




/************************************************************
 * GENERIC FUNCTIONS 					    *
 ************************************************************/

int nfi_gridftp_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){
	struct stat st;

	struct nfi_gridftp_server *server_aux;
	struct nfi_gridftp_fhandle *fh_aux;
	
	globus_result_t res;
	char *url;
	globus_off_t size = 0;


	if (attr == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}


	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
			//gridftp_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gridftp_err(GRIDFTPERR_PARAM);
               return -1;
        }
#endif
	fh_aux = (struct nfi_gridftp_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_gridftp_server *) serv->private_info;

	/*ret = stat(fh_aux->path, &st);
	if(ret < 0){
		fprintf(stderr,"nfi_gridftp_getattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		gridftp_err(GRIDFTPERR_GETATTR);
		return -1;
	}*/
	/*
	url = malloc(strlen(server_aux->path)+strlen(fh_aux->path)+1);
	strcpy(url, server_aux->path);
	strcpy(url+strlen(server_aux->path), fh_aux->path);
	url[strlen(server_aux->path)+strlen(fh_aux->path)] = 0;
	*/
	url = malloc(strlen(fh_aux->path)+1);
	strcpy(url, fh_aux->path);
	fh_aux->done = GLOBUS_FALSE;

	attr->at_size = 0;
	memset(&st, 0, sizeof(struct stat));
	res = globus_ftp_client_size (&(fh_aux->handle), url, &(fh_aux->opattr), &size, noop_cb, fh_aux);
	if (res != GLOBUS_SUCCESS)
	{
		globus_object_t *err;
		err = globus_error_get (res);
		fprintf (stderr, "\nnfi_gridftp_getattr: %s url='%s'\n", globus_object_printable_to_string (err), url);
		fprintf (stderr, "nfi_gridftp_getattr: server_aux->path='%s'\n", server_aux->path);
		fprintf (stderr, "nfi_gridftp_getattr:     fh_aux->path='%s' = %p\n", fh_aux->path, &(fh_aux->path));
		fh_aux->done = GLOBUS_TRUE;
	}
	free(url);

	/*
	pthread_mutex_lock (&fh_aux->pmutex);
	while (!fh_aux->done)
	{
		pthread_cond_wait (&fh_aux->pcond, &fh_aux->pmutex);
	}
	pthread_mutex_unlock (&fh_aux->pmutex);
	*/
	globus_mutex_lock (&fh_aux->gmutex);
	while (!fh_aux->done)
	{
		globus_cond_wait (&fh_aux->gcond, &fh_aux->gmutex);
	}
	globus_mutex_unlock (&fh_aux->gmutex);

	st.st_size = size;
	st.st_mode = NFIFILE;

 	GRIDFTPtoNFIattr(attr, &st);

	return 0;
}


int nfi_gridftp_setattr(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_fhandle *fh, __attribute__((__unused__)) struct nfi_attr *attr){
/*
	int ret;
	struct stat st;
	struct nfi_gridftp_server *server_aux;
	struct nfi_gridftp_fhandle *fh_aux;


	if (attr == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
			//gridftp_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gridftp_err(GRIDFTPERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_gridftp_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_gridftp_server *) serv->private_info;

 	NFItoGRIDFTPattr(&st, attr);
	// no se comp hacer el setattr
	//ret = stat(fh_aux->path, &st);

	ret = truncate(fh_aux->path, st.st_size);
	if(ret < 0){
		fprintf(stderr,"nfi_gridftp_setattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		gridftp_err(GRIDFTPERR_GETATTR);
		return -1;
	}
*/	
	return 0;
}

int nfi_gridftp_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int res;
	//struct stat st;
	globus_result_t ret;
	struct nfi_gridftp_server *server_aux;
	struct nfi_gridftp_fhandle *fh_aux;
	//char *env = NULL;
	//char **end = NULL;
	//int num_streams = -1, tcp_buffer = -1;

	XPN_DEBUG_BEGIN_ARGS1(url)
	
	//printf("nfi_gridftp_open ...\n");
	if(url[strlen(url)-1] == '/'){
		return nfi_gridftp_opendir(serv, url, fho);
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	
	if (fho == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
			/* gridftp_err(); not necessary */
			res = -1;
			XPN_DEBUG_END_ARGS1(url)
			return res;
		}
	}
#else
	if (serv->private_info == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
        }
#endif

	res = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(res < 0){
		fprintf(stderr,"nfi_gridftp_open: url %s incorrect.\n",url);
		gridftp_err(GRIDFTPERR_URL);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}
	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		gridftp_err(GRIDFTPERR_MEMORY);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_gridftp_fhandle *)malloc(sizeof(struct nfi_gridftp_fhandle));
	if (fh_aux == NULL){		
		gridftp_err(GRIDFTPERR_MEMORY);
		free(fho->url);
		res = -1;
		XPN_DEBUG_END_ARGS1(url)
		return res;
	}

	server_aux = (struct nfi_gridftp_server *) serv->private_info;
	
	ret = globus_ftp_client_handle_init (&(fh_aux->handle), GLOBUS_NULL);
	assert(ret == GLOBUS_SUCCESS);

	ret = globus_ftp_client_handleattr_init(&(fh_aux->attr));
	assert(ret == GLOBUS_SUCCESS);

	ret = globus_ftp_client_operationattr_init(&(fh_aux->opattr));
	assert(ret == GLOBUS_SUCCESS);

	char *env = getenv("GLOBUS_GRIDFTP_SERVER_SUBJECT");
	//char *env = globus_libc_getenv("GLOBUS_GRIDFTP_SERVER_SUBJECT");
	if (env != NULL) {
		/*
		gss_cred_id_t cred = GSS_C_NO_CREDENTIAL;
		char *user;
		char *password;
		char *account;
		char *subject;

		ret = globus_ftp_client_operationattr_get_authorization(&(fh_aux->opattr), &cred, &user, &password, &account, &subject);
		assert(ret == GLOBUS_SUCCESS);

		fprintf(stderr, "cred = %d\n", cred);
		fprintf(stderr, "user = %s\n", user);
		fprintf(stderr, "password = %s\n", password);
		fprintf(stderr, "account = %s\n", account);
		fprintf(stderr, "subject = %s\n", subject);
		fprintf(stderr, "env = %s\n", env);
		*/

		//ret = globus_ftp_client_operationattr_set_authorization(&(fh_aux->opattr), cred, user, password, account, env);
		//ret = globus_ftp_client_operationattr_set_authorization(&(fh_aux->opattr), GSS_C_NO_CREDENTIAL, ":globus-mapping:", "", GLOBUS_NULL, env);
		//ret = globus_ftp_client_operationattr_set_authorization(&(fh_aux->opattr), GSS_C_NO_CREDENTIAL, "bberguaguerra", GLOBUS_NULL, GLOBUS_NULL, env);
		//const globus_i_ftp_client_operationattr_t *i_attr = fh_aux->opattr;
		//cred = i_attr->auth_info.credential_handle;
		//ret = globus_ftp_client_operationattr_set_authorization(&(fh_aux->opattr), cred, GLOBUS_NULL, GLOBUS_NULL, GLOBUS_NULL, env);
		ret = globus_ftp_client_operationattr_set_authorization(&(fh_aux->opattr), GSS_C_NO_CREDENTIAL, GLOBUS_NULL, GLOBUS_NULL, GLOBUS_NULL, env);
		assert(ret == GLOBUS_SUCCESS);
	}

	/*
	globus_ftp_control_tcpbuffer_t tcpbuffer;
	ret = globus_ftp_client_operationattr_get_tcp_buffer(&(fh_aux->opattr), &tcpbuffer);
	assert(ret == GLOBUS_SUCCESS);
	switch (tcpbuffer.mode) {
		case GLOBUS_FTP_CONTROL_TCPBUFFER_DEFAULT:
			fprintf(stderr, "nfi_gridftp_open: tcp_buffer.mode = GLOBUS_FTP_CONTROL_TCPBUFFER_DEFAULT\n");
			break;
		case GLOBUS_FTP_CONTROL_TCPBUFFER_FIXED:
			fprintf(stderr, "nfi_gridftp_open: tcp_buffer.mode = GLOBUS_FTP_CONTROL_TCPBUFFER_FIXED\n");
			fprintf(stderr, "nfi_gridftp_open: tcp_buffer.fixed.size = %d\n", tcpbuffer.fixed.size);
			break;
		case GLOBUS_FTP_CONTROL_TCPBUFFER_AUTOMATIC:
			fprintf(stderr, "nfi_gridftp_open: tcp_buffer.mode = GLOBUS_FTP_CONTROL_TCPBUFFER_AUTOMATIC\n");
			fprintf(stderr, "nfi_gridftp_open: tcp_buffer.automatic.initial_size = %u\n", tcpbuffer.automatic.initial_size);
			fprintf(stderr, "nfi_gridftp_open: tcp_buffer.automatic.minimum_size = %u\n", tcpbuffer.automatic.minimum_size);
			fprintf(stderr, "nfi_gridftp_open: tcp_buffer.automatic.maximum_size = %u\n", tcpbuffer.automatic.maximum_size);
			break;
		default:
			fprintf(stderr, "nfi_gridftp_create: tcp_buffer.mode = INVALID MODE\n");
			break;
	}

	tcpbuffer.fixed.mode = GLOBUS_FTP_CONTROL_TCPBUFFER_FIXED;
	//tcpbuffer.fixed.size = 64*1024;
	tcpbuffer.fixed.size = 4*1024*1024;
	ret = globus_ftp_client_operationattr_set_tcp_buffer(&(fh_aux->opattr), &tcpbuffer);
	assert(ret == GLOBUS_SUCCESS);

	ret = globus_ftp_client_operationattr_get_tcp_buffer(&(fh_aux->opattr), &tcpbuffer);
	assert(ret == GLOBUS_SUCCESS);
	fprintf(stderr, "Despues tcp_buffer_size = %u\n", tcpbuffer.fixed.size);
	*/

	//pthread_mutex_init(&fh_aux->pmutex, NULL);
	//pthread_cond_init(&fh_aux->pcond, NULL);

	ret = globus_mutex_init(&fh_aux->gmutex, GLOBUS_NULL);
	assert(ret == GLOBUS_SUCCESS);

	ret = globus_cond_init(&fh_aux->gcond, GLOBUS_NULL);
	assert(ret == GLOBUS_SUCCESS);

	fh_aux->done = GLOBUS_FALSE;

	strcpy(fh_aux->path, gridftp_prefix);
	strcpy(fh_aux->path+strlen(gridftp_prefix), server);
	strcpy(fh_aux->path+strlen(gridftp_prefix)+strlen(server), dir);
	//fprintf(stderr, "nfi_gridftp_open: fh_aux->path = \"%s\" = %p\n", fh_aux->path, &(fh_aux->path));
	
	fh_aux->offset = 0;
	/*
	ret = open(dir, O_RDWR);
#define DBG_NFI 0
#ifdef  DBG_NFI 
fprintf(stderr,"nfi_gridftp_open: open %s in server %s.\n",dir,serv->server);
#endif
	if (ret < 0 ){
		ret = open(dir, O_RDONLY);
		if(ret <0){
			fprintf(stderr,"nfi_gridftp_open: Fail open %s in server %s.\n",dir,serv->server);
		gridftp_err(GRIDFTPERR_MEMORY);
			free(fh_aux);
			free(fho->url);
			return -1;
		}
	}
	*/
	//strcpy(fh_aux->path, dir);
	/*
	ret = stat(fh_aux->path, &st);
	if(ret < 0){
		gridftp_err(GRIDFTPERR_GETATTR);
		fprintf(stderr,"nfi_gridftp_open: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
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
			fprintf(stderr,"nfi_gridftp_open: %s isn't a FILE or FILE.\n",fh_aux->path);
			gridftp_err(GRIDFTPERR_GETATTR);
			free(fh_aux);
			free(fho->url);
			return -1;
		}
	}
	*/
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;

	res = 0;
	XPN_DEBUG_END_ARGS1(dir)
	return res;
}


int nfi_gridftp_close(struct nfi_server *server,  struct nfi_fhandle *fh){
	
	struct nfi_gridftp_fhandle *fh_aux;

	if (server == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_gridftp_fhandle *) fh->priv_fh;
		
		globus_ftp_client_operationattr_destroy(&(fh_aux->opattr));

		globus_ftp_client_handleattr_destroy(&(fh_aux->attr));

		globus_ftp_client_handle_destroy(&(fh_aux->handle));

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

ssize_t nfi_gridftp_read(struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{

	struct nfi_gridftp_server *server_aux;
	struct nfi_gridftp_fhandle *fh_aux;

	globus_off_t globus_offset = offset;
	globus_size_t res;
	globus_result_t ret;

	char *url;

	XPN_DEBUG_BEGIN_CUSTOM("offset=%ld, size=%zu", (long int)offset, size);

	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
			/* gridftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gridftp_err(GRIDFTPERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_gridftp_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_gridftp_server *) serv->private_info;
	if (fh_aux->offset != globus_offset){
		//ret = globus_xio_handle_cntl(fh_aux->handle, server_aux->driver, GLOBUS_GRIDFTP_GRIDFTP_SEEK, globus_offset);
		//assert(ret == GLOBUS_SUCCESS);
		fh_aux->offset = globus_offset;
		//printf("nfi_gridftp_handle_cntl(%d) offset reposicionado\n", (int)globus_offset);
	}
	//ret = globus_xio_read(fh_aux->handle, (globus_byte_t *)buffer, size, 1, &res, NULL);
	/*
	url = malloc(strlen(server_aux->path)+strlen(fh_aux->path)+1);
	strcpy(url, server_aux->path);
	strcpy(url+strlen(server_aux->path), fh_aux->path);
	url[strlen(server_aux->path)+strlen(fh_aux->path)] = 0;
	*/
	url = malloc(strlen(fh_aux->path)+1);
	strcpy(url, fh_aux->path);

	//if (size != 4096) size = 1986;
	//printf("\nRead: offset = %d, size = %lu\n", (int)offset, size);
	fh_aux->done = GLOBUS_FALSE;
	//pthread_mutex_lock (&gridftp_mutex);
	//globus_mutex_lock (&gridftp_globus_mutex);
	//fprintf(stderr,"nfi_gridftp_read 1: handle %p read %s off %d size %d\n",fh_aux->handle,fh->url,(int)offset,(int)size);
	ret = globus_ftp_client_partial_get (&(fh_aux->handle), url, &(fh_aux->opattr), GLOBUS_NULL, offset, offset+size, done_cb, fh_aux);
	//free(url);
	//fprintf(stderr,"nfi_gridftp_read 2: handle %p read %s off %d size %d (size:%d)\n",fh_aux->handle,fh->url,(int)offset,(int)size,(int)size);
	if (ret != GLOBUS_SUCCESS)
	{
		globus_object_t *err;
		err = globus_error_get (ret);
		fprintf(stderr, "nfi_gridftp_read: %s url = '%s'\n", globus_object_printable_to_string (err), url);
		fprintf(stderr, "nfi_gridftp_read: fh_aux->path = \"%s\" = %p\n", fh_aux->path, &(fh_aux->path));
		fh_aux->done = GLOBUS_TRUE;
	}
	else
	{
		fh_aux->rbytes = size;
		globus_ftp_client_register_read (&(fh_aux->handle),
						buffer,
						size,
						data_read_cb, &(fh_aux->rbytes));
	}
	//globus_mutex_unlock (&gridftp_globus_mutex);
	//pthread_mutex_unlock (&gridftp_mutex);

	globus_mutex_lock (&fh_aux->gmutex);
	while (!fh_aux->done)
	{
		globus_cond_wait (&fh_aux->gcond, &fh_aux->gmutex);
	}
	globus_mutex_unlock (&fh_aux->gmutex);

  	res = size;
	fh_aux->offset = fh_aux->offset + size;
#define DBG_NFI 0
#ifdef  DBG_NFI 
	//fprintf(stderr,"nfi_gridftp_read: read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)res);
#endif
	/*
	if(res < 0){
		fprintf(stderr,"nfi_gridftp_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)res);
		gridftp_err(GRIDFTPERR_READ);
		return -1;
	}
	*/

	XPN_DEBUG_END;
	return res;
}

ssize_t nfi_gridftp_write(struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{

        struct nfi_gridftp_server *server_aux;
        struct nfi_gridftp_fhandle *fh_aux;

	globus_off_t globus_offset = offset;
	globus_size_t res;
	globus_result_t ret;
	
	char *url;

	XPN_DEBUG_BEGIN_CUSTOM("offset=%ld, size=%zu", (long int)offset, size);

	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
			/* gridftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gridftp_err(GRIDFTPERR_PARAM);
               return -1;
        }
#endif


        fh_aux = (struct nfi_gridftp_fhandle *) fh->priv_fh;
        server_aux = (struct nfi_gridftp_server *) serv->private_info;
	if (fh_aux->offset != globus_offset){
		//ret = globus_xio_handle_cntl(fh_aux->handle, server_aux->driver, GLOBUS_GRIDFTP_GRIDFTP_SEEK, globus_offset);
		//assert(ret == GLOBUS_SUCCESS);
		fh_aux->offset = globus_offset;
		//printf("nfi_gridftp_handle_cntl(%d) offset reposicionado\n", (int)globus_offset);
	}
	//printf("nfi_gridftp_write: globus_xio_write(size=%d)\n", size);
	//ret = globus_xio_write(fh_aux->handle, (globus_byte_t *)buffer, size, 1, &res, NULL);
//fprintf (stderr, "server_aux->path = %s\n", server_aux->path);
//fprintf (stderr, "fh_aux->path = %s\n", fh_aux->path);
/*
	url = malloc(strlen(server_aux->path)+strlen(fh_aux->path)+1);
	strcpy(url, server_aux->path);
	strcpy(url+strlen(server_aux->path), fh_aux->path);
	url[strlen(server_aux->path)+strlen(fh_aux->path)] = 0;
*/
  url = malloc(strlen(fh_aux->path)+1);
  strcpy(url, fh_aux->path);
  fh_aux->done = GLOBUS_FALSE;
  ret = globus_ftp_client_partial_put (&(fh_aux->handle), url, &(fh_aux->opattr), GLOBUS_NULL, offset, offset+size, done_cb, fh_aux);
  if (ret != GLOBUS_SUCCESS)
    {
      globus_object_t *err;
      err = globus_error_get (ret);
      fprintf (stderr, "\nnfi_gridftp_write: %s url='%s'\n", globus_object_printable_to_string (err), url);
      fprintf (stderr, "nfi_gridftp_write: server_aux->path = '%s'\n", server_aux->path);
      fprintf (stderr, "nfi_gridftp_write:     fh_aux->path = '%s' %p\n", fh_aux->path, &(fh_aux->path));
      fh_aux->done = GLOBUS_TRUE;
    }
  else
    {
      fh_aux->rbytes = size;
      globus_ftp_client_register_write (&(fh_aux->handle),
					buffer,
					size,
					offset,
					GLOBUS_TRUE,
					data_write_cb, &(fh_aux->rbytes));
    }
  globus_mutex_lock (&fh_aux->gmutex);
  while (!fh_aux->done)
    {
      globus_cond_wait (&fh_aux->gcond, &fh_aux->gmutex);
    }
  globus_mutex_unlock (&fh_aux->gmutex);
	res = size;
//printf("nfi_gridftp_write: NO LLEGA\n");
	fh_aux->offset = fh_aux->offset + res;
	//assert(ret == GLOBUS_SUCCESS);
#define DBG_NFI 0
#ifdef  DBG_NFI 
	//fprintf(stderr,"nfi_gridftp_write: write %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)res);
#endif
	/*
	if(res < 0){
		fprintf(stderr,"nfi_gridftp_write: Fail write %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)res);
		
		gridftp_err(GRIDFTPERR_WRITE);
		return -1;
	}
	*/

	XPN_DEBUG_END;
	return res;
}

int nfi_gridftp_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh){

	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int res;
	globus_result_t ret;
	struct nfi_gridftp_server *server_aux;
        struct nfi_gridftp_fhandle *fh_aux;
	//struct stat st;

	XPN_DEBUG_BEGIN_ARGS1(url)
	
	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
			/* gridftp_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gridftp_err(GRIDFTPERR_PARAM);
               return -1;
        }
#endif
	res = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(res < 0){
		fprintf(stderr,"nfi_gridftp_create: url %s incorrect.\n",url);
		gridftp_err(GRIDFTPERR_URL);
		return -1;
	}
	/* private_info file handle */
	fh_aux = (struct nfi_gridftp_fhandle *)malloc(sizeof(struct nfi_gridftp_fhandle));
	if (fh_aux == NULL){
		gridftp_err(GRIDFTPERR_MEMORY);
		return -1;
	}

	bzero(fh_aux, sizeof(struct nfi_gridftp_fhandle));
	
	server_aux = (struct nfi_gridftp_server *) serv->private_info;

	ret = globus_ftp_client_handle_init (&(fh_aux->handle), GLOBUS_NULL);
	assert(ret == GLOBUS_SUCCESS);

	ret = globus_ftp_client_handleattr_init(&(fh_aux->attr));
	assert(ret == GLOBUS_SUCCESS);

	ret = globus_ftp_client_operationattr_init(&(fh_aux->opattr));
	assert(ret == GLOBUS_SUCCESS);

	char *env = getenv("GLOBUS_GRIDFTP_SERVER_SUBJECT");
	//char *env = globus_libc_getenv("GLOBUS_GRIDFTP_SERVER_SUBJECT");
	if (env != NULL) {
		/*
		gss_cred_id_t cred = GSS_C_NO_CREDENTIAL;
		char *user;
		char *password;
		char *account;
		char *subject;

		ret = globus_ftp_client_operationattr_get_authorization(&(fh_aux->opattr), &cred, &user, &password, &account, &subject);
		assert(ret == GLOBUS_SUCCESS);

		fprintf(stderr, "cred = %d\n", cred);
		fprintf(stderr, "user = %s\n", user);
		fprintf(stderr, "password = %s\n", password);
		fprintf(stderr, "account = %s\n", account);
		fprintf(stderr, "subject = %s\n", subject);
		fprintf(stderr, "env = %s\n", env);
		*/

		//ret = globus_ftp_client_operationattr_set_authorization(&(fh_aux->opattr), cred, user, password, account, env);
		//ret = globus_ftp_client_operationattr_set_authorization(&(fh_aux->opattr), GSS_C_NO_CREDENTIAL, ":globus-mapping:", "", GLOBUS_NULL, env);
		//ret = globus_ftp_client_operationattr_set_authorization(&(fh_aux->opattr), GSS_C_NO_CREDENTIAL, "bberguaguerra", GLOBUS_NULL, GLOBUS_NULL, env);
		//const globus_i_ftp_client_operationattr_t *i_attr = fh_aux->opattr;
		//cred = i_attr->auth_info.credential_handle;
		//ret = globus_ftp_client_operationattr_set_authorization(&(fh_aux->opattr), cred, GLOBUS_NULL, GLOBUS_NULL, GLOBUS_NULL, env);
		ret = globus_ftp_client_operationattr_set_authorization(&(fh_aux->opattr), GSS_C_NO_CREDENTIAL, GLOBUS_NULL, GLOBUS_NULL, GLOBUS_NULL, env);
		assert(ret == GLOBUS_SUCCESS);
	}

	/*
	globus_ftp_control_tcpbuffer_t tcpbuffer;
	ret = globus_ftp_client_operationattr_get_tcp_buffer(&(fh_aux->opattr), &tcpbuffer);
	assert(ret == GLOBUS_SUCCESS);
	switch (tcpbuffer.mode) {
		case GLOBUS_FTP_CONTROL_TCPBUFFER_DEFAULT:
			fprintf(stderr, "nfi_gridftp_create: tcp_buffer.mode = GLOBUS_FTP_CONTROL_TCPBUFFER_DEFAULT\n");
			break;
		case GLOBUS_FTP_CONTROL_TCPBUFFER_FIXED:
			fprintf(stderr, "nfi_gridftp_create: tcp_buffer.mode = GLOBUS_FTP_CONTROL_TCPBUFFER_FIXED\n");
			fprintf(stderr, "nfi_gridftp_create: tcp_buffer.fixed.size = %d\n", tcpbuffer.fixed.size);
			break;
		case GLOBUS_FTP_CONTROL_TCPBUFFER_AUTOMATIC:
			fprintf(stderr, "nfi_gridftp_create: tcp_buffer.mode = GLOBUS_FTP_CONTROL_TCPBUFFER_AUTOMATIC\n");
			fprintf(stderr, "nfi_gridftp_create: tcp_buffer.automatic.initial_size = %u\n", tcpbuffer.automatic.initial_size);
			fprintf(stderr, "nfi_gridftp_create: tcp_buffer.automatic.minimum_size = %u\n", tcpbuffer.automatic.minimum_size);
			fprintf(stderr, "nfi_gridftp_create: tcp_buffer.automatic.maximum_size = %u\n", tcpbuffer.automatic.maximum_size);
			break;
		default:
			fprintf(stderr, "nfi_gridftp_create: tcp_buffer.mode = INVALID MODE\n");
			break;
	}

	tcpbuffer.fixed.mode = GLOBUS_FTP_CONTROL_TCPBUFFER_FIXED;
	//tcpbuffer.fixed.size = 64*1024;
	tcpbuffer.fixed.size = 4*1024*1024;
	ret = globus_ftp_client_operationattr_set_tcp_buffer(&(fh_aux->opattr), &tcpbuffer);
	assert(ret == GLOBUS_SUCCESS);

	ret = globus_ftp_client_operationattr_get_tcp_buffer(&(fh_aux->opattr), &tcpbuffer);
	assert(ret == GLOBUS_SUCCESS);
	fprintf(stderr, "Despues tcp_buffer_size = %u\n", tcpbuffer.fixed.size);
	*/

	//pthread_mutex_init(&fh_aux->pmutex, NULL);
	//pthread_cond_init(&fh_aux->pcond, NULL);

	ret = globus_mutex_init(&fh_aux->gmutex, GLOBUS_NULL);
	assert(ret == GLOBUS_SUCCESS);

	ret = globus_cond_init(&fh_aux->gcond, GLOBUS_NULL);
	assert(ret == GLOBUS_SUCCESS);

	fh_aux->done = GLOBUS_FALSE;

	strcpy(fh_aux->path, gridftp_prefix);
	strcpy(fh_aux->path+strlen(gridftp_prefix), server);
	strcpy(fh_aux->path+strlen(gridftp_prefix)+strlen(server), dir);
	//fprintf(stderr, "nfi_gridftp_create: gridftp_prefix = \"%s\"\n", gridftp_prefix);
	//fprintf(stderr, "nfi_gridftp_create:         server = \"%s\"\n", server);
	//fprintf(stderr, "nfi_gridftp_create:            dir = \"%s\"\n", dir);
	//fprintf(stderr, "nfi_gridftp_create:   fh_aux->path = \"%s\" = %p\n", fh_aux->path, &(fh_aux->path));

	fh_aux->offset = 0;

	res = globus_ftp_client_delete(&(fh_aux->handle), fh_aux->path, GLOBUS_NULL, noop_cb, fh_aux);
	assert(res == GLOBUS_SUCCESS);

	globus_mutex_lock (&fh_aux->gmutex);
	while (!fh_aux->done)
	{
		globus_cond_wait (&fh_aux->gcond, &fh_aux->gmutex);
	}
	globus_mutex_unlock (&fh_aux->gmutex);

	/* create the file into the directory */
	
	/*fd = open(dir, O_CREAT|O_RDWR , attr->at_mode);
	if(fd < 0){
		fprintf(stderr,"nfi_gridftp_creat: Fail creat %s in server %s.\n",fh_aux->path,serv->server);
		gridftp_err(GRIDFTPERR_LOOKUP);
		free(fh_aux);
		return -1;
	}*/
	fh->server = serv;
	
	//strcpy(fh_aux->path, dir);
	//fprintf(stderr, "nfi_gridftp_create:   fh_aux->path = \"%s\" = %p\n", fh_aux->path, &(fh_aux->path));
	
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               	gridftp_err(GRIDFTPERR_MEMORY);
		free(fh_aux);
               	return -1;
        }
		
        strcpy(fh->url, url);
	/*memset (&st, 0, sizeof(struct stat));
	ret = stat(fh_aux->path, &st);
	if(ret < 0){
#if defined(DEBUG_NFI)
		printf("stat mal\n");
#endif
		fprintf(stderr,"nfi_gridftp_creat: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		gridftp_err(GRIDFTPERR_GETATTR);
		free(fh->url);
		free(fh_aux);
		
		return -1;
	}
	
	GRIDFTPtoNFIattr(attr, &st);*/

	res = 0;
	XPN_DEBUG_END_ARGS1(dir)
	return res;
}

int nfi_gridftp_remove(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) char *url)
{
/*
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_gridftp_server *server_aux;
	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
			//gridftp_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gridftp_err(GRIDFTPERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_gridftp_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_gridftp_remove: url %s incorrect.\n",url);
		gridftp_err(GRIDFTPERR_URL);
		return -1;
	}
	// remove the file into the directory
	ret = unlink(dir);

	if(ret < 0){
		fprintf(stderr,"nfi_gridftp_remove: Fail remove %s in server %s.\n",dir,serv->server);
		gridftp_err(GRIDFTPERR_REMOVE);
		return -1;
	}
*/
	return 0;
}

int nfi_gridftp_rename(__attribute__((__unused__)) struct nfi_server *server, __attribute__((__unused__)) char *old_url, __attribute__((__unused__)) char *new_url)
{
	/*
        struct nfi_gridftp_server *server_aux;
        struct nfi_gridftp_fhandle *fh_aux;
	if (server == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
			gridftp_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gridftp_err(GRIDFTPERR_PARAM);
               return -1;
        }
#endif
	server_aux = (strcut nfi_gridftp_server *)serv->private_info;
	*/

	/* ????? */
	return 0;
}

int nfi_gridftp_mkdir(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) char *url, __attribute__((__unused__)) struct nfi_attr *attr, __attribute__((__unused__)) struct nfi_fhandle *fh)
{
/*
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct stat st;		// GRIDFTP attributes
        struct nfi_gridftp_server *server_aux;
        struct nfi_gridftp_fhandle *fh_aux;


	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
			// gridftp_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gridftp_err(GRIDFTPERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_gridftp_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_gridftp_mkdir: url %s incorrect.\n",url);
		gridftp_err(GRIDFTPERR_URL);
		return -1;
	}

	// private_info file handle
	fh_aux = (struct nfi_gridftp_fhandle *)malloc(sizeof(struct nfi_gridftp_fhandle));
	if (fh_aux == NULL){
		gridftp_err(GRIDFTPERR_MEMORY);
		return -1;
	}


	bzero(fh_aux, sizeof(struct nfi_gridftp_fhandle));
	// create the dir into the directory
	//ret = mkdir(dir, attr->at_mode);
	ret = mkdir(dir, 0777);
	
	if((ret < 0)&&(errno != EEXIST)){
		gridftp_err(GRIDFTPERR_MKDIR);
		fprintf(stderr,"nfi_gridftp_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server);
		free(fh_aux);
		return -1;
	}
	
	fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               gridftp_err(GRIDFTPERR_MEMORY);
	       free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

	GRIDFTPtoNFIattr(attr, &st);
*/
	return 0;
}

int nfi_gridftp_rmdir(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) char *url)
{
/*
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;

        struct nfi_gridftp_server *server_aux;

	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
			// gridftp_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gridftp_err(GRIDFTPERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_gridftp_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_gridftp_rmdir: url %s incorrect.\n",url);
		gridftp_err(GRIDFTPERR_URL);
		return -1;
	}

	ret = rmdir(dir);	
	if(ret < 0){
		fprintf(stderr,"nfi_gridftp_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server);
		gridftp_err(GRIDFTPERR_REMOVE);
		return -1;
	}
*/
	return 0;
}

int nfi_gridftp_opendir(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) char *url, __attribute__((__unused__)) struct nfi_fhandle *fho)
{
/*      
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int ret;	
	struct nfi_gridftp_server *server_aux;
	struct nfi_gridftp_fhandle *fh_aux;


	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

	if (fho == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
			// gridftp_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gridftp_err(GRIDFTPERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_gridftp_opendir: url %s incorrect.\n",url);
		gridftp_err(GRIDFTPERR_URL);
		return -1;
	}

	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		gridftp_err(GRIDFTPERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_gridftp_fhandle *)malloc(sizeof(struct nfi_gridftp_fhandle));
	if (fh_aux == NULL){
		gridftp_err(GRIDFTPERR_MEMORY);
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_gridftp_server *) serv->private_info;


	fh_aux->dir = opendir(dir);
    if (ret < 0 ){
		fprintf(stderr,"nfi_gridftp_opendir: Fail opendir %s in server %s.\n",dir,serv->server);
	    gridftp_err(GRIDFTPERR_MEMORY);		
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
*/
	return 0;
}

int nfi_gridftp_readdir(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_fhandle *fh, __attribute__((__unused__)) char *entry, __attribute__((__unused__)) unsigned char *type)
{
/*
	struct dirent *ent;

        struct nfi_gridftp_server *server_aux;
        struct nfi_gridftp_fhandle *fh_aux;


	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

	if(fh->type != NFIDIR){
		gridftp_err(GRIDFTPERR_NOTDIR);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
			// gridftp_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gridftp_err(GRIDFTPERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_gridftp_server *)serv->private_info;
	fh_aux = (struct nfi_gridftp_fhandle *)fh->priv_fh;

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
*/
	return 0;
}

int nfi_gridftp_closedir(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_fhandle *fh)
{
/*	
	struct nfi_gridftp_fhandle *fh_aux;

	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_gridftp_fhandle *) fh->priv_fh;
		closedir(fh_aux->dir);
		// free memory
		free(fh->priv_fh);
		fh->priv_fh = NULL;

	}
	
	if (fh->url != NULL){
		free(fh->url);
	}
*/
	
	return 0;
}


int nfi_gridftp_statfs(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_info *inf)
{
	/*
	struct local_info localinf;
	int ret;
        struct nfi_gridftp_server *server_aux;


	if (serv == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}

	if (inf == NULL){
		gridftp_err(GRIDFTPERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_gridftp_reconnect(serv);
		if(ret <0){
		 gridftp_err(); not necessary 
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               gridftp_err(GRIDFTPERR_PARAM);
               return -1;
        }
#endif
	
	server_aux = (struct nfi_gridftp_server *)serv->private_info;
	ret = xio_statfs(server_aux->fh, &localinf, server_aux->cl);

	NFStoNFIInfo(inf, &localinf);
	*/
	return 0;
}
