#include "nfi_xio/xio_lib.h"

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


const char *xio_gridftp_prefix="gsiftp://";

static globus_mutex_t xio_lock;
static globus_cond_t xio_cond;
static globus_bool_t xio_done;

// From nfi_gridftp.c
void check_result(globus_result_t ret);
void check_error(globus_object_t *error);

/*static void xio_data_cb (void *user_arg,
	 globus_ftp_client_handle_t * handle,
	 globus_object_t * err,
	 globus_byte_t * buffer,
	 globus_size_t length, globus_off_t offset, globus_bool_t eof)
{
	//printf("\nDATA!!!\n");
  if (err)
    {
      fprintf (stderr, "%s", globus_object_printable_to_string (err));
    }
	return;
}
*/
/********************************************************************
* done_cb: A pointer to this function is passed to the call to
* globus_ftp_client_put (and all the other high level transfer
* operations). It is called when the transfer is completely
* finished, i.e. both the data channel and control channel exchange.
* Here it simply sets a global variable (done) to true so the main
* program will exit the while loop.
********************************************************************/
static void xio_done_cb (__attribute__((__unused__)) void *user_arg, __attribute__((__unused__)) globus_ftp_client_handle_t * handle, globus_object_t * err) {
	//printf("\nDONE!!!\n");
	
	if (err) {
		fprintf (stderr, "%s", globus_object_printable_to_string (err));
	}
	
	globus_mutex_lock (&xio_lock);
	xio_done = GLOBUS_TRUE;
	globus_cond_signal (&xio_cond);
	globus_mutex_unlock (&xio_lock);
	
	return;
}


void xio_check_error(globus_result_t ret)
{
	char * tmpstr;
	if (ret != GLOBUS_SUCCESS)
	{
		tmpstr = globus_object_printable_to_string(globus_error_get(ret));
		printf("Error: %s\n", tmpstr);
		globus_free(tmpstr);
	
	}
}

/************************************************************
 * PRIVATE FUNCTIONS TO USE xio SERVERS		            *
 ************************************************************/
void NFItoXIOattr(struct stat *att, struct nfi_attr *nfi_att)
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

void XIOtoNFIattr(struct nfi_attr *nfi_att, struct stat *att)
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

/*void XIOtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *local_inf)
{
}*/

/************************************************************
 * PUBLIC FUNCTIONS TO USE XIO				    *
 ************************************************************/

/************************************************************
 * Init xio 					    	    *
 ************************************************************/
int nfi_xio_init(char *url, struct nfi_server *serv, __attribute__((__unused__)) struct nfi_attr_server *attr)
{
	char server[MAXPATHLEN], dir[MAXPATHLEN] ,prt [MAXPATHLEN];
	int ret;
	globus_result_t res;
	struct nfi_xio_server *server_aux;

	//printf("nfi_xio_init ...\n");
	if(serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

	/* functions */
	serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
	if (serv->ops == NULL){
		xio_err(XIOERR_MEMORY);
		return -1;
	}

	serv->ops->nfi_reconnect  = nfi_xio_reconnect;
	serv->ops->nfi_disconnect = nfi_xio_disconnect;

	serv->ops->nfi_getattr	= nfi_xio_getattr;
	serv->ops->nfi_setattr	= nfi_xio_setattr;

	serv->ops->nfi_open	= nfi_xio_open;
	serv->ops->nfi_close	= nfi_xio_close;

	serv->ops->nfi_read	= nfi_xio_read;
	serv->ops->nfi_write	= nfi_xio_write;

	serv->ops->nfi_create	= nfi_xio_create;
	serv->ops->nfi_remove	= nfi_xio_remove;
	serv->ops->nfi_rename	= nfi_xio_rename;

	serv->ops->nfi_mkdir	= nfi_xio_mkdir;
	serv->ops->nfi_rmdir	= nfi_xio_rmdir;
	serv->ops->nfi_opendir	= nfi_xio_opendir;
	serv->ops->nfi_readdir	= nfi_xio_readdir;
	serv->ops->nfi_closedir	= nfi_xio_closedir;
	serv->ops->nfi_statfs	= nfi_xio_statfs;

	ret = ParseURL(url, prt, NULL, NULL, server, NULL, dir);
	if(ret <0 ){
		xio_err(XIOERR_URL);
		fprintf(stderr,"nfi_xio_init: url %s incorrect.\n",url);
		free(serv->ops);
		return -1;
	}
	server_aux = (struct nfi_xio_server *)malloc(sizeof(struct nfi_xio_server));
	if(server_aux == NULL){
		xio_err(XIOERR_MEMORY);
		return -1;
	}

	strcpy(server_aux->path, xio_gridftp_prefix);
	strcpy(server_aux->path+strlen(xio_gridftp_prefix), server);
	//strcpy(server_aux->path+strlen(xio_gridftp_prefix)+strlen(server), dir);
	//printf("nfi_xio_init: path = \"%s\"\n", server_aux->path);
	//strcpy(server_aux->path, dir); // old local

	serv->private_info = (void *)server_aux;

	//serv->protocol = XIO;

	serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1));	/* server address */
	if(serv->server == NULL){
		xio_err(XIOERR_MEMORY);
		return -1;
	}
	strcpy(serv->server, server);

	serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1));	/* server address */
	if(serv->url == NULL){
		xio_err(XIOERR_MEMORY);
		return -1;
	}

	strcpy(serv->url, url);
        serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
	memset(serv->wrk, 0, sizeof(struct nfi_worker));
	if(strcmp(prt, "xiohilos") == 0){
#if defined(DEBUG_NFI)
		printf("xiohilos\n");
#endif
		nfi_worker_init(serv->wrk, serv, 1);
	}
	else{
		nfi_worker_init(serv->wrk, serv, 0);
	}

/*
struct nfi_xio_server{
        char path[NFIMAXPATHLEN];
        globus_xio_driver_t             driver;
        globus_xio_stack_t              stack;
};
*/
	// GLOBUS_THREAD_MODEL="pthread" is required since Globus 5.2
	char *env = getenv("GLOBUS_THREAD_MODEL");
	if (env == NULL)
		setenv("GLOBUS_THREAD_MODEL", "pthread", 0);

	globus_module_activate(GLOBUS_XIO_MODULE);

	res = globus_xio_driver_load("gridftp", &(server_aux->driver));
	assert(res == GLOBUS_SUCCESS);

	res = globus_xio_stack_init(&(server_aux->stack), NULL);
	assert(res == GLOBUS_SUCCESS);

	res = globus_xio_stack_push_driver(server_aux->stack, server_aux->driver);
	assert(res == GLOBUS_SUCCESS);

	globus_module_activate(GLOBUS_FTP_CLIENT_MODULE);
	
	globus_mutex_init (&xio_lock, GLOBUS_NULL);
	globus_cond_init (&xio_cond, GLOBUS_NULL);
	
	return 0;
}

/************************************************************
 * Disconnect to the server				    *
 * **********************************************************/
int nfi_xio_disconnect(struct nfi_server *serv)
{

        struct nfi_xio_server *server_aux;
        

	if (serv == NULL)
	       return 0;


        server_aux = (struct nfi_xio_server *)serv->private_info;
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
 * Reconnect to the NFS server				    *
 ************************************************************/
int nfi_xio_reconnect(struct nfi_server *serv)
{
	/* Don't see the serv result */
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_xio_server *server_aux;

	ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret <0 ){
		xio_err(XIOERR_URL);
		fprintf(stderr,"nfi_xio_reconnect: url %s incorrect.\n",serv->url);
		return -1;
	}

	server_aux = (struct nfi_xio_server *)malloc(sizeof(struct nfi_xio_server));
	if(server_aux == NULL){		
		xio_err(XIOERR_MEMORY);
		return -1;
	}

	strcpy(server_aux->path, dir);

	serv->private_info = (void *)server_aux;
	return 0;
}

/************************************************************
 * Destroy NFS operations				    *
 ************************************************************/

int nfi_xio_destroy(struct nfi_server *serv)
{

	struct nfi_xio_server *server_aux;

	if (serv == NULL)
		return 0;

	if(serv->ops != NULL)
		free(serv->ops);

	server_aux = (struct nfi_xio_server *)serv->private_info;

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

	globus_module_deactivate(GLOBUS_XIO_MODULE);
	globus_module_deactivate(GLOBUS_FTP_CLIENT_MODULE);

	return 0;
}




/************************************************************
 * GENERIC FUNCTIONS 					    *
 ************************************************************/

int nfi_xio_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){
	struct stat st;

	struct nfi_xio_server *server_aux;
	struct nfi_xio_fhandle *fh_aux;

	globus_result_t res;
	char *url;
	globus_off_t size;

	if (attr == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}


	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
			//xio_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif
	fh_aux = (struct nfi_xio_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_xio_server *) serv->private_info;

	/*ret = stat(fh_aux->path, &st);
	if(ret < 0){
		fprintf(stderr,"nfi_xio_getattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		xio_err(XIOERR_GETATTR);
		return -1;
	}*/
	url = malloc(strlen(server_aux->path)+strlen(fh_aux->path)+1);
	strcpy(url, server_aux->path);
	strcpy(url+strlen(server_aux->path), fh_aux->path);
	url[strlen(server_aux->path)+strlen(fh_aux->path)] = 0;

	res = globus_ftp_client_handle_init (&(fh_aux->handle_g), GLOBUS_NULL);
	assert(res == GLOBUS_SUCCESS);

	res = globus_ftp_client_handleattr_init(&(fh_aux->attr_g));
	assert(res == GLOBUS_SUCCESS);

	attr->at_size = 0;
	memset(&st, 0, sizeof(struct stat));
  xio_done = GLOBUS_FALSE;
  res = globus_ftp_client_size (&(fh_aux->handle_g), url, GLOBUS_NULL, &size, xio_done_cb, 0);
  if (res != GLOBUS_SUCCESS)
    {
      globus_object_t *err;
      err = globus_error_get (res);
      fprintf (stderr, "\n%s\n", globus_object_printable_to_string (err));
      xio_done = GLOBUS_TRUE;
    }
  globus_mutex_lock (&xio_lock);
  while (!xio_done)
    {
      globus_cond_wait (&xio_cond, &xio_lock);
    }
  globus_mutex_unlock (&xio_lock);
  st.st_mode = 0;
  st.st_size = size;

 	XIOtoNFIattr(attr, &st);

	return 0;
}


int nfi_xio_setattr(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_fhandle *fh, __attribute__((__unused__)) struct nfi_attr *attr){
/*
	int ret;
	struct stat st;
	struct nfi_xio_server *server_aux;
	struct nfi_xio_fhandle *fh_aux;


	if (attr == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
			//xio_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_xio_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_xio_server *) serv->private_info;

 	NFItoXIOattr(&st, attr);
	// no se comp hacer el setattr
	//ret = stat(fh_aux->path, &st);

	ret = truncate(fh_aux->path, st.st_size);
	if(ret < 0){
		fprintf(stderr,"nfi_xio_setattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		xio_err(XIOERR_GETATTR);
		return -1;
	}
*/	
	return 0;
}

int nfi_xio_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int ret;
	//struct stat st;
	globus_result_t res;
	struct nfi_xio_server *server_aux;
	struct nfi_xio_fhandle *fh_aux;
	//char *env = NULL;
	//char **end = NULL;
	//int num_streams = -1, tcp_buffer = -1;
	//globus_l_xio_gridftp_protection_t c;

	//printf("nfi_xio_open ...\n");
	if(url[strlen(url)-1] == '/'){
		return nfi_xio_opendir(serv, url, fho);
	}
	
	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}
	
	if (fho == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}
#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
			/* xio_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_xio_open: url %s incorrect.\n",url);
		xio_err(XIOERR_URL);
		return -1;
	}
	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		xio_err(XIOERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

/*
struct nfi_xio_fhandle{
        char path[NFIMAXPATHLEN];
        char contac_string[NFIMAXPATHLEN];
        globus_xio_handle_t             handle;
        globus_xio_attr_t               attr;
        globus_mutex_t                  mutex;
        globus_cond_t                   cond;
	globus_bool_t			done;
        globus_ftp_client_handle_t      handle_g;
        globus_ftp_client_handleattr_t  attr_g;
        globus_off_t                    offset;
        globus_result_t                 fd;
};
*/
	fh_aux = (struct nfi_xio_fhandle *)malloc(sizeof(struct nfi_xio_fhandle));
	if (fh_aux == NULL){		
		xio_err(XIOERR_MEMORY);
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_xio_server *) serv->private_info;
	
	res = globus_xio_handle_create(&(fh_aux->handle), server_aux->stack);
	assert(res == GLOBUS_SUCCESS);
	//fprintf(stderr, "globus_xio_handle_create(%p, %p)\n", fh_aux->handle, server_aux->stack);

	globus_xio_attr_init(&(fh_aux->attr));
	globus_mutex_init(&(fh_aux->mutex), GLOBUS_NULL);
	globus_cond_init(&(fh_aux->cond), GLOBUS_NULL);
	fh_aux->done = GLOBUS_FALSE;

	//TODO: getenv GLOBUS_GRIDFTP_SERVER_SUBJECT and set it with xio_gridftp_attr_cntl(..., GLOBUS_XIO_GRIDFTP_SET_AUTH, ...)

	/*
	//res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_PARTIAL_TRANSFER, GLOBUS_TRUE);
	//assert(res == GLOBUS_SUCCESS);
	
	//printf("Antes de petar\n");
	res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_GET_NUM_STREAMS, &num_streams);
	//printf("Depues de petar\n");
	assert(res == GLOBUS_SUCCESS);
	printf("El numero de streams configurado por defecto es: %d\n", num_streams);
	
	env = getenv("GLOBUS_XIO_GRIDFTP_SET_NUM_STREAMS");
	if (env != NULL) {
		end = (char **)malloc(256);
		num_streams = (int)strtol(env,end,10);
		if ((*env != '\0') && (**end == '\0')){
			//res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_NUM_STREAMS, 1);
			res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_NUM_STREAMS, num_streams);
			assert(res == GLOBUS_SUCCESS);
			num_streams = -1;
			res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_GET_NUM_STREAMS, &num_streams);
			assert(res == GLOBUS_SUCCESS);
			printf("nfi_xio_open: Atributo GLOBUS_XIO_GRIDFTP_SET_NUM_STREAMS = %d\n", num_streams);
		}
		free(end);
	}

	env = getenv("GLOBUS_XIO_GRIDFTP_SET_TCP_BUFFER");
	if (env != NULL) {
		end = (char **)malloc(256);
		tcp_buffer = (int)strtol(env,end,10);
		if ((*env != '\0') && (**end == '\0')){
			//res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_TCP_BUFFER, 64*1024);
			res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_TCP_BUFFER, tcp_buffer);
			assert(res == GLOBUS_SUCCESS);
			fprintf(stderr, "nfi_xio_open: Attribute GLOBUS_XIO_GRIDFTP_SET_TCP_BUFFER = %d\n", tcp_buffer);
		}
		free(end);
	}
	*/
	
	strcpy(fh_aux->contact_string, xio_gridftp_prefix);
	strcpy(fh_aux->contact_string+strlen(xio_gridftp_prefix), server);
	strcpy(fh_aux->contact_string+strlen(xio_gridftp_prefix)+strlen(server), dir);
	//fprintf(stderr, "nfi_xio_open: contact_string = \"%s\"\n", fh_aux->contact_string);
	
	res = globus_xio_open(fh_aux->handle, fh_aux->contact_string, fh_aux->attr);
	xio_check_error(res);
	if (res != GLOBUS_SUCCESS){
		printf("res = %d\n", res);
		nfi_xio_close(serv, fho);
		nfi_xio_destroy(serv);
	}
	assert(res == GLOBUS_SUCCESS);
	fh_aux->offset = 0;
	/*
	ret = open(dir, O_RDWR);
#define DBG_NFI 0
#ifdef  DBG_NFI 
fprintf(stderr,"nfi_xio_open: open %s in server %s.\n",dir,serv->server);
#endif
	if (ret < 0 ){
		ret = open(dir, O_RDONLY);
		if(ret <0){
			fprintf(stderr,"nfi_xio_open: Fail open %s in server %s.\n",dir,serv->server);
		xio_err(XIOERR_MEMORY);
			free(fh_aux);
			free(fho->url);
			return -1;
		}
	}
	*/
	fh_aux->fd = res;
	strcpy(fh_aux->path, dir);
	/*
	ret = stat(fh_aux->path, &st);
	if(ret < 0){
		xio_err(XIOERR_GETATTR);
		fprintf(stderr,"nfi_xio_open: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
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
			fprintf(stderr,"nfi_xio_open: %s isn't a FILE or FILE.\n",fh_aux->path);
			xio_err(XIOERR_GETATTR);
			free(fh_aux);
			free(fho->url);
			return -1;
		}
	}
	*/
	fho->server = NULL;
	fho->priv_fh = NULL;
	fho->server = serv;
	fho->priv_fh = (void *) fh_aux;

	//res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_DATA_PROTECTION, GLOBUS_XIO_GRIDFTP_PROTECTION_SAFE);
	//assert(res == GLOBUS_SUCCESS);
	//res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_DATA_PROTECTION, GLOBUS_XIO_GRIDFTP_PROTECTION_CONFIDENTIAL);
	//assert(res == GLOBUS_SUCCESS);
	//res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_CONTROL_PROTECTION, GLOBUS_XIO_GRIDFTP_PROTECTION_PRIVATE);
	//assert(res == GLOBUS_SUCCESS);
	//res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_GET_DATA_PROTECTION, &c);
	//assert(res == GLOBUS_SUCCESS);
	//printf("Antes: la proteccion es %c", c);
	//res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_DATA_PROTECTION, GLOBUS_XIO_GRIDFTP_PROTECTION_PRIVATE);
	//assert(res == GLOBUS_SUCCESS);
	//res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_GET_DATA_PROTECTION, &c);
	//assert(res == GLOBUS_SUCCESS);
	//printf("Despues: la proteccion es %c", c);
	
	return 0;
}


int nfi_xio_close(__attribute__((__unused__)) struct nfi_server *server,  struct nfi_fhandle *fh){
	
	struct nfi_xio_fhandle *fh_aux;

	int res = -1;
	XPN_DEBUG_BEGIN

	if (server == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_xio_fhandle *) fh->priv_fh;
		XPN_DEBUG_BEGIN_CUSTOM("fd=%d", fh_aux->fd)
		
		globus_xio_attr_destroy(fh_aux->attr);

		globus_xio_close(fh_aux->handle, NULL);
		//close(fh_aux->fd);
		/* free memory */
		free(fh->priv_fh);
		fh->priv_fh = NULL;

	}
	
	if (fh->url != NULL){
		free(fh->url);
	}
	
	fh->type = NFINULL;
	fh->server = NULL;

	XPN_DEBUG_END_CUSTOM("fd=%d", fh_aux->fd);

	return 0;
}

void nfi_xio_read_cb (
		globus_xio_handle_t	handle,
		globus_result_t		result,
		globus_byte_t *		buffer,
		globus_size_t		len,
		globus_size_t		nbytes,
		globus_xio_data_descriptor_t	data_desc,
		void *			user_arg
		)
{
	struct nfi_xio_fhandle *fh_aux = (struct nfi_xio_fhandle *)user_arg;

	fprintf(stderr, "nfi_xio_read_cb(handle=%p, result=%d, buffer=%p, len=%zu, nbytes=%zu data_desc=%p, user_arg=%p)\n",handle,result,buffer,len,nbytes,data_desc,user_arg);

	//if (len == nbytes) {
		globus_mutex_lock(&(fh_aux->mutex));
		fh_aux->done = GLOBUS_TRUE;
		globus_cond_signal(&(fh_aux->cond));
		globus_mutex_unlock(&(fh_aux->mutex));
	//}
}

globus_result_t globus_xio_read2 (
		globus_xio_handle_t  	handle,
		globus_byte_t *  	buffer,
		globus_size_t  		buffer_length,
		globus_size_t  		waitforbytes,
		globus_size_t *  	nbytes,
		globus_xio_data_descriptor_t  	data_desc,
		void *  		user_arg
		)
{
	globus_result_t res;
	struct nfi_xio_fhandle *fh_aux = (struct nfi_xio_fhandle *)user_arg;
	/*
	globus_result_t globus_xio_register_read (
		globus_xio_handle_t  	handle,
		globus_byte_t *  	buffer,
		globus_size_t  	buffer_length,
		globus_size_t  	waitforbytes,
		globus_xio_data_descriptor_t  	data_desc,
		globus_xio_data_callback_t  	cb,
		void *  	user_arg
	)
	*/
	fh_aux->done = GLOBUS_FALSE;

	res = globus_xio_register_read (handle, buffer, buffer_length, waitforbytes, data_desc, nfi_xio_read_cb, user_arg);
	
	globus_mutex_lock(&(fh_aux->mutex));
	while(!fh_aux->done)
	{
		globus_cond_wait(&(fh_aux->cond), &(fh_aux->mutex));
	}
	globus_mutex_unlock(&(fh_aux->mutex));

	*nbytes = buffer_length;
  	//res = size;
	//fh_aux->offset = fh_aux->offset + size;

	return res;
}

ssize_t nfi_xio_read(	struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{

	struct nfi_xio_server *server_aux;
	struct nfi_xio_fhandle *fh_aux;

	globus_off_t globus_offset = offset;
	globus_size_t new_size;
	globus_result_t res;

	XPN_DEBUG_BEGIN_CUSTOM("offset=%ld, size=%zu", offset, size)

	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
			/* xio_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif


	fh_aux = (struct nfi_xio_fhandle *) fh->priv_fh;
	server_aux = (struct nfi_xio_server *) serv->private_info;
	///////////////////////////////
	// Create, open and close a new handle every time, instead of using fh_aux->handle as it should be,
	// because if I reuse the same handle, globus_xio_read blocks in the second call.
	//globus_xio_attr_t attr;
	globus_xio_handle_t handle;
	res = globus_xio_handle_create(&handle, server_aux->stack);
	assert(res == GLOBUS_SUCCESS);
	//fprintf(stderr, "fh_aux->contact_string = %s\n", fh_aux->contact_string);
	res = globus_xio_open(handle, fh_aux->contact_string, fh_aux->attr);
	xio_check_error(res);
	assert(res == GLOBUS_SUCCESS);
	///////////////////////////////
	if (fh_aux->offset != globus_offset){
		//res = globus_xio_handle_cntl(fh_aux->handle, server_aux->driver, GLOBUS_XIO_GRIDFTP_SEEK, globus_offset);
		res = globus_xio_handle_cntl(handle, server_aux->driver, GLOBUS_XIO_GRIDFTP_SEEK, globus_offset);
		assert(res == GLOBUS_SUCCESS);
		fh_aux->offset = globus_offset;
		//fprintf(stderr, "nfi_xio_read 0: read %s handle %p off %d repositioned\n", fh->url,fh_aux->handle, (int)globus_offset);
	}
	//fprintf(stderr,"nfi_xio_read 1: read %s handle %p off %d size %d.\n",fh->url,fh_aux->handle,(int)offset,(int)size);
	//res = globus_xio_read (fh_aux->handle, (globus_byte_t *)buffer, size, 1, &new_size, NULL); // 4th parameter (waitforbytes) must be 1. Don't know why. Cannot find a reference to the documentation
	res = globus_xio_read (handle, (globus_byte_t *)buffer, size, 1, &new_size, NULL); // 4th parameter (waitforbytes) must be 1. Don't know why. Cannot find a reference to the documentation
	/*
	if (res != GLOBUS_SUCCESS) {
		fprintf(stderr,"nfi_xio_read Error: read %s handle %p off %d size %d (new_size=%d) res=%d.\n",fh->url,fh_aux->handle,(int)offset,(int)size,(int)new_size,res);
		//fprintf(stderr,"nfi_xio_read Error: filse size is %d\n", fh->);
		check_result(res);
		xio_check_error(res);
	}
	//assert(res == GLOBUS_SUCCESS);
	*/
	///////////////////////////////
	//res = globus_xio_close(handle, fh_aux->attr);
	//assert(res == GLOBUS_SUCCESS);
	///////////////////////////////
	//res = globus_xio_read2 (fh_aux->handle, (globus_byte_t *)buffer, size, 1, &new_size, NULL, fh_aux);
	//fprintf(stderr,"nfi_xio_read 2: read %s handle %p off %d size %d (new_size=%d).\n",fh->url,fh_aux->handle,(int)offset,(int)size,(int)new_size);
	//fprintf(stderr,"nfi_xio_read 3: read %s handle %p off %d size %d (new_size=%d) 1st byte='%c'.\n",fh->url,fh_aux->handle,(int)offset,(int)size,(int)new_size, ((char *)buffer)[0]);
	fh_aux->offset = fh_aux->offset + new_size;
#define DBG_NFI 0
#ifdef  DBG_NFI 
	//fprintf(stderr,"nfi_xio_read: read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
#endif
	/*
	if(new_size < (globus_size_t)0){
		fprintf(stderr,"nfi_xio_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
		xio_err(XIOERR_READ);
		return -1;
	}
	*/

	XPN_DEBUG_END_CUSTOM("offset=%ld, size=%zu, new_size=%zu", offset, size, new_size)

	return new_size;
}

ssize_t nfi_xio_write(struct nfi_server *serv,
			struct nfi_fhandle *fh,
			void *buffer,
			off_t offset,
			size_t size)
{

        struct nfi_xio_server *server_aux;
        struct nfi_xio_fhandle *fh_aux;

	globus_off_t globus_offset = offset;
	globus_size_t new_size;
	globus_result_t res;

	globus_l_xio_gridftp_protection_t c;

	//printf("nfi_xio_write(offset=%d, size=%d)\n", offset, size);
	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
			/* xio_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif


        fh_aux = (struct nfi_xio_fhandle *) fh->priv_fh;
        server_aux = (struct nfi_xio_server *) serv->private_info;

	res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_GET_DATA_PROTECTION, &c);
	assert(res == GLOBUS_SUCCESS);
	//printf("La proteccion es %d\n", c);
	
	if (fh_aux->offset != globus_offset){
		res = globus_xio_handle_cntl(fh_aux->handle, server_aux->driver, GLOBUS_XIO_GRIDFTP_SEEK, globus_offset);
		assert(res == GLOBUS_SUCCESS);
		fh_aux->offset = globus_offset;
		//printf("nfi_xio_handle_cntl(%d) offset reposicionado\n", (int)globus_offset);
	}
	//printf("nfi_xio_write: globus_xio_write(size=%d)\n", size);
	res = globus_xio_write(fh_aux->handle, (globus_byte_t *)buffer, size, 1, &new_size, NULL);
	//printf("nfi_xio_write: NO LLEGA\n");
	assert(res == GLOBUS_SUCCESS);
	fh_aux->offset = fh_aux->offset + new_size;
	//assert(res == GLOBUS_SUCCESS);
#define DBG_NFI 0
#ifdef  DBG_NFI 
	//fprintf(stderr,"nfi_xio_write: write %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
#endif
	/*
	if(new_size < (globus_size_t)0){
		fprintf(stderr,"nfi_xio_write: Fail write %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)new_size);
		
		xio_err(XIOERR_WRITE);
		return -1;
	}
	*/

	return new_size;
}

int nfi_xio_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh){

	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret,fd=-1;
	struct nfi_xio_server *server_aux;
        struct nfi_xio_fhandle *fh_aux;
	//struct stat st;
	//globus_result_t res;
	//globus_l_xio_gridftp_protection_t c;

	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
			/* xio_err(); not necessary */
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif
	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_xio_create: url %s incorrect.\n",url);
		xio_err(XIOERR_URL);
		return -1;
	}
	/* private_info file handle */
	fh_aux = (struct nfi_xio_fhandle *)malloc(sizeof(struct nfi_xio_fhandle));
	if (fh_aux == NULL){
		xio_err(XIOERR_MEMORY);
		return -1;
	}

	bzero(fh_aux, sizeof(struct nfi_xio_fhandle));
	

    server_aux = (struct nfi_xio_server *) serv->private_info;
	/* create the file into the directory */
	ret = globus_xio_handle_create(&(fh_aux->handle), server_aux->stack);
	assert(ret == GLOBUS_SUCCESS);

	globus_xio_attr_init(&(fh_aux->attr));
	
	//res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_CONTROL_PROTECTION, GLOBUS_XIO_GRIDFTP_PROTECTION_PRIVATE);
	//assert(res == GLOBUS_SUCCESS);

	// Descomentar esto para cifrar
	/*
	printf("GLOBUS_XIO_GRIDFTP_PROTECTION_CLEAR es %d\n", GLOBUS_XIO_GRIDFTP_PROTECTION_CLEAR);
	printf("GLOBUS_XIO_GRIDFTP_PROTECTION_SAFE es %d\n", GLOBUS_XIO_GRIDFTP_PROTECTION_SAFE);
	printf("GLOBUS_XIO_GRIDFTP_PROTECTION_CONFIDENTIAL es %d\n", GLOBUS_XIO_GRIDFTP_PROTECTION_CONFIDENTIAL);
	printf("GLOBUS_XIO_GRIDFTP_PROTECTION_PRIVATE es %d\n", GLOBUS_XIO_GRIDFTP_PROTECTION_PRIVATE);
	res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_GET_DATA_PROTECTION, &c);
	assert(res == GLOBUS_SUCCESS);
	printf("Antes: la proteccion es %d\n", c);
	*/
	
	/*res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_CONTROL_PROTECTION, GLOBUS_XIO_GRIDFTP_PROTECTION_SAFE);
	assert(res == GLOBUS_SUCCESS);
	res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_DATA_PROTECTION, GLOBUS_XIO_GRIDFTP_PROTECTION_SAFE);
	assert(res == GLOBUS_SUCCESS);*/
	

	// Descomentar esto para cifrar
	/*
	res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_CONTROL_PROTECTION, GLOBUS_XIO_GRIDFTP_PROTECTION_PRIVATE);
	assert(res == GLOBUS_SUCCESS);
	res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_SET_DATA_PROTECTION, GLOBUS_XIO_GRIDFTP_PROTECTION_PRIVATE);
	assert(res == GLOBUS_SUCCESS);
	
	res = globus_xio_attr_cntl(fh_aux->attr, server_aux->driver, GLOBUS_XIO_GRIDFTP_GET_DATA_PROTECTION, &c);
	assert(res == GLOBUS_SUCCESS);
	printf("Despues: la proteccion es %d\n", c);
	*/

	strcpy(fh_aux->contact_string, xio_gridftp_prefix);
	strcpy(fh_aux->contact_string+strlen(xio_gridftp_prefix), server);
	strcpy(fh_aux->contact_string+strlen(xio_gridftp_prefix)+strlen(server), dir);
	//printf("nfi_xio_create: contact_string = \"%s\"\n", fh_aux->contact_string);
	
	ret = globus_xio_open(fh_aux->handle, fh_aux->contact_string, fh_aux->attr);
	//assert(ret == GLOBUS_SUCCESS);
	xio_check_error(ret);
	if (ret != GLOBUS_SUCCESS){
		nfi_xio_close(serv, fh);
		nfi_xio_destroy(serv);
	}
	
	/*fd = open(dir, O_CREAT|O_RDWR , attr->at_mode);
	if(fd < 0){
		fprintf(stderr,"nfi_xio_creat: Fail creat %s in server %s.\n",fh_aux->contact_string,serv->server);
		xio_err(XIOERR_LOOKUP);
		free(fh_aux);
		return -1;
	}*/
	fh->server = serv;
	
	fh_aux->fd = fd;
	strcpy(fh_aux->path, dir);
	
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               	xio_err(XIOERR_MEMORY);
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
		fprintf(stderr,"nfi_xio_creat: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
		xio_err(XIOERR_GETATTR);
		free(fh->url);
		free(fh_aux);
		
		return -1;
	}
	
	XIOtoNFIattr(attr, &st);*/

	return 0;
}

int nfi_xio_remove(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) char *url)
{
/*
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct nfi_xio_server *server_aux;
	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
			//xio_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_xio_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_xio_remove: url %s incorrect.\n",url);
		xio_err(XIOERR_URL);
		return -1;
	}
	// remove the file into the directory
	ret = unlink(dir);

	if(ret < 0){
		fprintf(stderr,"nfi_xio_remove: Fail remove %s in server %s.\n",dir,serv->server);
		xio_err(XIOERR_REMOVE);
		return -1;
	}
*/
	return 0;
}

int nfi_xio_rename(__attribute__((__unused__)) struct nfi_server *server, __attribute__((__unused__)) char *old_url, __attribute__((__unused__)) char *new_url)
{
	/*
        struct nfi_xio_server *server_aux;
        struct nfi_xio_fhandle *fh_aux;
	if (server == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
			xio_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif
	server_aux = (strcut nfi_xio_server *)serv->private_info;
	*/

	/* ????? */
	return 0;
}

int nfi_xio_mkdir(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) char *url, __attribute__((__unused__)) struct nfi_attr *attr, __attribute__((__unused__)) struct nfi_fhandle *fh)
{
/*
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;
	struct stat st;		// XIO attributes
        struct nfi_xio_server *server_aux;
        struct nfi_xio_fhandle *fh_aux;


	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

	if (attr == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
			// xio_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_xio_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_xio_mkdir: url %s incorrect.\n",url);
		xio_err(XIOERR_URL);
		return -1;
	}

	// private_info file handle
	fh_aux = (struct nfi_xio_fhandle *)malloc(sizeof(struct nfi_xio_fhandle));
	if (fh_aux == NULL){
		xio_err(XIOERR_MEMORY);
		return -1;
	}


	bzero(fh_aux, sizeof(struct nfi_xio_fhandle));
	// create the dir into the directory
	//ret = mkdir(dir, attr->at_mode);
	ret = mkdir(dir, 0777);
	
	if((ret < 0)&&(errno != EEXIST)){
		xio_err(XIOERR_MKDIR);
		fprintf(stderr,"nfi_xio_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server);
		free(fh_aux);
		return -1;
	}
	
	fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               xio_err(XIOERR_MEMORY);
	       free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

	XIOtoNFIattr(attr, &st);
*/
	return 0;
}

int nfi_xio_rmdir(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) char *url)
{
/*
	char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
	int ret;

        struct nfi_xio_server *server_aux;

	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
			// xio_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif


	server_aux = (struct nfi_xio_server *)serv->private_info;

	ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_xio_rmdir: url %s incorrect.\n",url);
		xio_err(XIOERR_URL);
		return -1;
	}

	ret = rmdir(dir);	
	if(ret < 0){
		fprintf(stderr,"nfi_xio_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server);
		xio_err(XIOERR_REMOVE);
		return -1;
	}
*/
	return 0;
}

int nfi_xio_opendir(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) char *url, __attribute__((__unused__)) struct nfi_fhandle *fho)
{
/*      
	char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
	int ret;	
	struct nfi_xio_server *server_aux;
	struct nfi_xio_fhandle *fh_aux;


	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

	if (fho == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
			// xio_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif

	ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
	if(ret < 0){
		fprintf(stderr,"nfi_xio_opendir: url %s incorrect.\n",url);
		xio_err(XIOERR_URL);
		return -1;
	}

	fho->url = (char *)malloc(strlen(url)+1);
	if(fho->url == NULL){
		xio_err(XIOERR_MEMORY);
		return -1;
	}

	strcpy(fho->url, url);

	fh_aux = (struct nfi_xio_fhandle *)malloc(sizeof(struct nfi_xio_fhandle));
	if (fh_aux == NULL){
		xio_err(XIOERR_MEMORY);
		free(fho->url);
		return -1;
	}

	server_aux = (struct nfi_xio_server *) serv->private_info;


	fh_aux->dir = opendir(dir);
    if (ret < 0 ){
		fprintf(stderr,"nfi_xio_opendir: Fail opendir %s in server %s.\n",dir,serv->server);
	    xio_err(XIOERR_MEMORY);		
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

int nfi_xio_readdir(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_fhandle *fh, __attribute__((__unused__)) char *entry, __attribute__((__unused__)) unsigned char *type)
{
/*
	struct dirent *ent;

        struct nfi_xio_server *server_aux;
        struct nfi_xio_fhandle *fh_aux;


	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}


	if (fh == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

	if (fh->priv_fh == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

	if(fh->type != NFIDIR){
		xio_err(XIOERR_NOTDIR);
		return -1;
	}

#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
			// xio_err(); not necessary
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif

	server_aux = (struct nfi_xio_server *)serv->private_info;
	fh_aux = (struct nfi_xio_fhandle *)fh->priv_fh;

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

int nfi_xio_closedir(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_fhandle *fh)
{
/*	
	struct nfi_xio_fhandle *fh_aux;

	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}
	if (fh == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

	if (fh->priv_fh != NULL){
		fh_aux = (struct nfi_xio_fhandle *) fh->priv_fh;
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


int nfi_xio_statfs(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_info *inf)
{
	/*
	struct xio_info localinf;
	int ret;
        struct nfi_xio_server *server_aux;


	if (serv == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}

	if (inf == NULL){
		xio_err(XIOERR_PARAM);
		return -1;
	}


#ifdef NFI_DYNAMIC
	if (serv->private_info == NULL){
		ret = nfi_xio_reconnect(serv);
		if(ret <0){
		 xio_err(); not necessary 
			return -1;
		}
	}
#else
	if (serv->private_info == NULL){
               xio_err(XIOERR_PARAM);
               return -1;
        }
#endif
	
	server_aux = (struct nfi_xio_server *)serv->private_info;
	ret = xio_statfs(server_aux->fh, &localinf, server_aux->cl);
	if(ret <0){
		xio_err(XIOERR_STATFS);
		return -1;
	}

	NFStoNFIInfo(inf, &localinf);
	*/
	return 0;
}
