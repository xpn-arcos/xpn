
#include "nfi/nfi_local/local_lib.h"


/************************************************************
 * PRIVATE FUNCTIONS TO USE local SERVERS             *
 ************************************************************/

void NFItoLOCALattr ( struct stat *att, struct nfi_attr *nfi_att )
{
  if (nfi_att->at_type == NFIFILE) {
    att->st_mode = nfi_att->at_mode | S_IFREG;  /* protection */
  }

  if (nfi_att->at_type == NFIDIR) {
    att->st_mode = nfi_att->at_mode | S_IFDIR;  /* protection */
  }

  att->st_nlink = nfi_att->at_nlink;  /* number of hard links   */
  att->st_uid = nfi_att->at_uid;  /* user ID of owner   */
  att->st_gid = nfi_att->at_gid;  /* group ID of owner    */
  att->st_size  = nfi_att->at_size; /* total size, in bytes   */
  att->st_blksize = nfi_att->at_blksize;  /* blocksize for filesystem I/O */
  att->st_blocks  = nfi_att->at_blocks; /* number of blocks allocated */
  att->st_atime = nfi_att->at_atime;  /* time of last access    */
  att->st_mtime = nfi_att->at_mtime;  /* time of last modification  */
  att->st_ctime = nfi_att->at_ctime;  /* time of last status change */
}

void LOCALtoNFIattr ( struct nfi_attr *nfi_att, struct stat *att )
{
  if (S_ISREG(att->st_mode)) {
    nfi_att->at_type = NFIFILE;
  }
  if (S_ISDIR(att->st_mode)) {
    nfi_att->at_type = NFIDIR;
  }

  nfi_att->at_mode  = att->st_mode/*&(S_IRWXU|S_IRWXG|S_IRWXO)*/;   /* protection   */
  nfi_att->at_nlink = att->st_nlink;  /* number of hard links   */
  nfi_att->at_uid   = att->st_uid;    /* user ID of owner   */
  nfi_att->at_gid   = att->st_gid;    /* group ID of owner    */
  nfi_att->at_size  = att->st_size;   /* total size, in bytes   */
  nfi_att->at_blksize = att->st_blksize;  /* blocksize for filesystem I/O */
  nfi_att->at_blocks  = att->st_blocks; /* number of blocks allocated */
  nfi_att->at_atime = att->st_atime;  /* time of last access    */
  nfi_att->at_mtime = att->st_mtime;  /* time of last modification  */
  nfi_att->at_ctime = att->st_ctime;  /* time of last status change */
}

/*
void LOCALtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *local_inf)
{
}
*/

      int nfi_local_keepConnected ( struct nfi_server *serv )
      {
#ifdef NFI_DYNAMIC
    if (serv->private_info == NULL)
          {
        res = nfi_local_reconnect(serv) ;
        if (res <0) {
                  serv->private_info = NULL;
      return -1;
        }
    }
#endif

          // return OK
          return (serv->private_info != NULL) ;
      }


 //
 // PUBLIC FUNCTIONS
 //

/************************************************************
 * Init Local                     *
 ************************************************************/
int nfi_local_init ( char *url, struct nfi_server *serv, __attribute__((__unused__)) struct nfi_attr_server *attr )
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX], prt[PATH_MAX];
  struct nfi_local_server *server_aux;

  DEBUG_BEGIN();

  // Check arguments
  if (NULL == serv) {
      debug_error("ERROR: serv argument is NULL.\n") ;
      return -1 ;
  }

  // Alloc for serv->ops...
  serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops)) ;
  if (NULL == serv->ops) {
      debug_error("ERROR: out of memory\n") ;
      return -1;
  }

  // Fill serv->ops...
  bzero(serv->ops, sizeof(struct nfi_ops)) ;

  serv->ops->nfi_reconnect  = nfi_local_reconnect;
  serv->ops->nfi_disconnect = nfi_local_disconnect;
//  serv->ops->nfi_destroy    = nfi_local_destroy; //TODO: when nfi_local_destroy is called?

  serv->ops->nfi_getattr  = nfi_local_getattr;
  serv->ops->nfi_setattr  = nfi_local_setattr;

  serv->ops->nfi_open = nfi_local_open;
  serv->ops->nfi_close  = nfi_local_close;

  serv->ops->nfi_read = nfi_local_read;
  serv->ops->nfi_write  = nfi_local_write;

  serv->ops->nfi_create = nfi_local_create;
  serv->ops->nfi_remove = nfi_local_remove;
  serv->ops->nfi_rename = nfi_local_rename;

  serv->ops->nfi_mkdir  = nfi_local_mkdir;
  serv->ops->nfi_rmdir  = nfi_local_rmdir;
  serv->ops->nfi_opendir  = nfi_local_opendir;
  serv->ops->nfi_readdir  = nfi_local_readdir;
  serv->ops->nfi_closedir = nfi_local_closedir;
  serv->ops->nfi_statfs = nfi_local_statfs;

  // ParseURL...
  ret = ParseURL(url,  prt, NULL, NULL, server, NULL, dir) ;
  if (ret < 0) {
      debug_error("ERROR: nfi_local_init found incorrect URL '%s'.\n", url) ;
      FREE_AND_NULL(serv->ops) ;
      return -1;
  }

  // Alloc for serv_aux...
  server_aux = (struct nfi_local_server *)malloc(sizeof(struct nfi_local_server)) ;
  if (server_aux == NULL) {
      debug_error("ERROR: out of memory\n") ;
      return -1;
  }

  // Fill serv_aux...
  bzero(server_aux, sizeof(struct nfi_local_server)) ;

  /*serv->protocol = LOCAL;*/
  strcpy(server_aux->path, dir) ;
  serv->private_info = (void *)server_aux;

  serv->server = STRING_MISC_StrDup(server) ; /* server address */
  if (serv->server == NULL) {
      debug_error("ERROR: out of memory\n") ;
      return -1;
  }

  serv->url = STRING_MISC_StrDup(url) ; /* server address */
  if (serv->url == NULL) {
      debug_error("ERROR: out of memory\n") ;
      return -1;
  }

        // <thread checking>
  /*
        serv->xpn_thread = 0 ;
        server_aux->params.xpn_thread = 0 ;
        server_aux->params.xpn_thread_mode = TH_POOL ;

        char *env_thread = getenv("XPN_THREAD");
        if (env_thread != NULL)
        {
          serv->xpn_thread = atoi(env_thread) ;
          server_aux->params.xpn_thread      = serv->xpn_thread ;
          server_aux->params.xpn_thread_mode = serv->xpn_thread ;
        }
  */
        // </thread checking>

  // nfi_worker...
  serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker)) ;
  memset(serv->wrk, 0, sizeof(struct nfi_worker)) ;
  serv->wrk->server = serv ;    

  ret = nfi_local_connect(serv, url, prt, server, dir) ;

  DEBUG_END();

  // Return OK
  return 0;
}


  /*********************************************************
   * Connect to the server                                 *
   * *******************************************************/
  int nfi_local_connect ( struct nfi_server *serv, char *url, char* prt, char* server, char* dir )
  {
        int ret ;
        struct nfi_local_server *server_aux;

        // check params...
        if (serv == NULL) {
          return 0;
        }
        server_aux = (struct nfi_local_server *) (serv->private_info) ;
        if (server_aux == NULL) {
          return 0;
        }

        // <OLD CODE>
        debug_info("filehilos = %d\n", prt) ;
        if (strcmp(prt, "filehilos") == 0) {
            nfi_worker_init(serv->wrk, serv, 1) ;
        }
        else {
            nfi_worker_init(serv->wrk, serv, 0) ;
        }
        // </OLD CODE>

        // <NEW CODE>
        /*
        debug_info("[NFI_LOCAL] workers_init()\n") ;
        ret = workers_init ( &(serv->wrk->wb), server_aux->params.xpn_thread );
        */
        // </NEW CODE>

        debug_info("[NFI] nfi_local_connect(): end\n") ;

    return ret;
  }


/************************************************************
 * Disconnect from the server           *
 * **********************************************************/
int nfi_local_disconnect ( struct nfi_server *serv )
{
  struct nfi_local_server *server_aux;

  DEBUG_BEGIN();

  // Check arguments
  if (serv == NULL) {
    return 0;
  }

  server_aux = (struct nfi_local_server *) (serv->private_info) ;
  if (server_aux == NULL) {
    return 0;
  }

  // <OLD CODE>
  nfi_worker_end(serv->wrk) ;
  // </OLD CODE>

  // <NEW CODE>
        // Thread destroy...
  /*
  if (server_aux->params.xpn_thread != TH_NOT)
  {
    debug_info("[NFI] workers_destroy()\n") ;
    workers_destroy ( &(serv->wrk->wb) );
  }
  */
  // </NEW CODE>

  DEBUG_END();

  // Return OK
  return 0;
}


/************************************************************
 * Reconnect to the LOCAL server          *
 ************************************************************/
int nfi_local_reconnect ( struct nfi_server *serv )
{
  /* Don't see the serv result */
  //char server[PATH_MAX], dir[PATH_MAX];
  //int res;
  struct nfi_local_server *server_aux;

  DEBUG_BEGIN();

  // Check arguments
  if (serv == NULL) {
    return 0;
  }
        server_aux = (struct nfi_local_server *) (serv->private_info) ;
        if (server_aux == NULL) {
          return 0;
        }

  /*
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
  */

  DEBUG_END();

  // Return OK
  return 0;
}


/************************************************************
 * Destroy LOCAL operations           *
 ************************************************************/

int nfi_local_destroy ( struct nfi_server *serv )
{
  int ret ;

  DEBUG_BEGIN();

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

  ret = nfi_local_disconnect(serv) ;

  DEBUG_END();

  // Return OK
  return 0;
}


/************************************************************
 * GENERIC FUNCTIONS              *
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

  // Check fields...
  if (fh->priv_fh == NULL) {
      debug_error("fh->priv_fh argument is NULL.\n") ;
      return -1;
  }
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  // Do stat
  fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
  server_aux = (struct nfi_local_server *) serv->private_info;
  if (server_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }

  res = filesystem_stat(fh_aux->path, &st) ;
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
  struct stat st;
  struct nfi_local_server *server_aux;

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

  // CHeck fields...
  if (fh->priv_fh == NULL) {
      debug_error("fh->priv_fh argument is NULL.\n") ;
      return -1;
  }
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  // Do stat
  server_aux = (struct nfi_local_server *) serv->private_info;
  if (server_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }

  NFItoLOCALattr(&st, attr) ;

  /* TODO: hacer el setattr */

  DEBUG_END();

  // Return OK
  return 0;
}

int nfi_local_open ( struct nfi_server *serv, char *url, struct nfi_fhandle *fho )
{
  char dir[PATH_MAX], server[PATH_MAX];
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

  // do local opendir...
  if (url[strlen(url)-1] == '/') {
    res = nfi_local_opendir(serv, url, fho) ;
    return res;
  }

  // Check fields...
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  // Get fields...
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
  if (server_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }

  // Open file...
  res = filesystem_open(dir, O_RDWR) ;
  if (res < 0)
  {
      debug_error("filesystem_open fails to open '%s' in server %s.\n", dir, serv->server) ;
      free(fh_aux) ;
      free(fho->url) ;
      return -1;
  }

  fh_aux->fd = res;
  strcpy(fh_aux->path, dir) ;

  res = filesystem_stat(fh_aux->path, &st) ;
  if (res < 0) {
      debug_error("LOCALERR_GETATTR.\n") ;
      free(fh_aux) ;
      free(fho->url) ;
      return -1 ;
  }

  if (S_ISDIR(st.st_mode)) {
      fho->type = NFIDIR ;
  }
  else if (S_ISREG(st.st_mode)) {
      fho->type = NFIFILE ;
  } else {
      debug_error("LOCALERR_GETATTR.\n") ;
      free(fh_aux) ;
      free(fho->url) ;
      return -1;
  }

  fho->server = serv;
  fho->priv_fh = (void *) fh_aux;

  DEBUG_END();

  return 0;
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
  
  // Do close
  fh_aux = (struct nfi_local_fhandle *) fh->priv_fh ;
  if (fh_aux != NULL) {
      filesystem_close(fh_aux->fd) ;
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

  // Check fields...
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  // Do read
  fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
  server_aux = (struct nfi_local_server *) serv->private_info;

  if (fh_aux == NULL) {
      debug_error("nfi_local_read: fh_aux == NULL\n") ;
      return -1;
  }
  if (server_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }

  filesystem_lseek(fh_aux->fd, offset, SEEK_SET) ;
  new_size = filesystem_read(fh_aux->fd, buffer, size) ;
  debug_info("read %s(%d) off %ld size %zu (ret:%zd)", fh->url, fh_aux->fd, (long int)offset, size, new_size)
  if (new_size < 0) {
      debug_error("filesystem_read reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, new_size, errno) ;
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

  // Check fields...
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  // Do write
  fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
  server_aux = (struct nfi_local_server *) serv->private_info;
  if (server_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }

  filesystem_lseek(fh_aux->fd, offset, SEEK_SET) ;
  new_size = filesystem_write(fh_aux->fd, buffer, size) ;
  debug_info("write %s off %ld size %zu (ret:%zd)\n", fh->url, (long int)offset, size, new_size);
  if (new_size < 0) {
      debug_error("files_posix_write_buffer writes zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, new_size, errno) ;
      return -1;
  }

  DEBUG_END();

  return new_size;
}

int nfi_local_create ( struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh )
{
  char server[PATH_MAX], dir[PATH_MAX];
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

  // Check fields...
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  // Get fields...
  res = ParseURL(url,  NULL, NULL, NULL, server, NULL, dir) ;
  if (res < 0) {
      debug_error("nfi_local_create: url '%s' incorrect.\n", url) ;
      return -1;
  }

  fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle)) ;
  if (fh_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }

  bzero(fh_aux, sizeof(struct nfi_local_fhandle)) ;
  server_aux = (struct nfi_local_server *) serv->private_info;
  if (server_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }

  // Do create
  fd = filesystem_open2(dir, O_CREAT|O_RDWR|O_TRUNC, attr->at_mode) ;
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
  res = filesystem_stat(fh_aux->path, &st) ;
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
  char server[PATH_MAX], dir[PATH_MAX];
  int res;
  struct nfi_local_server *server_aux;

  DEBUG_BEGIN();

  // Check arguments
  if (serv == NULL) {
      debug_error("serv argument is NULL.\n") ;
      return -1;
  }

  // Check fields...
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  server_aux = (struct nfi_local_server *)serv->private_info;
  if (server_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }

  res = ParseURL(url,  NULL, NULL, NULL, server, NULL, dir) ;
  if (res < 0) {
    debug_error("nfi_local_remove: url '%s' incorrect.\n", url) ;
    return -1;
  }

  // Do unlink
  res = filesystem_unlink(dir) ;
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

  // Check fields...
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  server_aux = (strcut nfi_local_server *)serv->private_info;
  if (server_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }
  */

  // TODO !!

  DEBUG_END();

  // Return OK
  return 0;
}

int nfi_local_mkdir ( struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh )
{
  char server[PATH_MAX], dir[PATH_MAX];
  int res;
  struct stat st;   /* LOCAL attributes */
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

  // Check fields...
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  // Get fields...
  res = ParseURL(url,  NULL, NULL, NULL, server, NULL, dir) ;
  if (res < 0) {
    debug_error("nfi_local_mkdir: url '%s' incorrect.\n", url) ;
    return -1;
  }

  fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle)) ;
  if (fh_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }

  bzero(fh_aux, sizeof(struct nfi_local_fhandle)) ;

  // Do mkdir
  res = filesystem_mkdir(dir, /*attr->at_mode*/ 0777) ;
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
  char server[PATH_MAX], dir[PATH_MAX];

  DEBUG_BEGIN();

  // Check params
  if (serv == NULL) {
      debug_error("serv argument is NULL.\n") ;
      return -1;
  }

  // Check fields...
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  // Get fields...
  res = ParseURL(url,  NULL, NULL, NULL, server, NULL, dir) ;
  if (res < 0) {
    debug_error("nfi_local_rmdir: url '%s' incorrect.\n", url) ;
    return -1;
  }

  // Do rmdir
  res = filesystem_rmdir(dir) ;
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
  char dir[PATH_MAX], server[PATH_MAX];
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

  // Check fields...
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  // Get fields...
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
  if (server_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }

  // Do opendir
  fh_aux->dir = filesystem_opendir(dir) ;
  if (fh_aux->dir == NULL) {
      free(fh_aux) ;
      free(fho->url) ;
      debug_error("real_posix_opendir fails to open directory '%s' in server '%s'.\n", dir, serv->server) ;
      return -1;
  }

  fh_aux->fd   = res;
  strcpy(fh_aux->path, dir) ;
  fho->type    = NFIDIR;
  fho->server  = NULL;
  fho->server  = serv;
  fho->priv_fh = (void *) fh_aux;

  DEBUG_END();

  // Return OK
  return 0 ;
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

  // Check fields...
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  fh_aux = (struct nfi_local_fhandle *)fh->priv_fh;
  server_aux = (struct nfi_local_server *)serv->private_info;
  if (server_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }

  entry[0] = '\0';
  ent = filesystem_readdir(fh_aux->dir) ;

  if (ent == NULL) {
    debug_error("nfi_local_readdir: readdir") ;
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

  // Do closedir
  fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
  if (fh_aux != NULL) {
      filesystem_closedir(fh_aux->dir) ;
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

  // Check fields...
        nfi_local_keepConnected(serv) ;
  if (serv->private_info == NULL) {
      debug_error("serv->private_info field is NULL.\n") ;
      return -1;
  }

  server_aux = (struct nfi_local_server *)serv->private_info;
  if (server_aux == NULL) {
      debug_error("LOCALERR_MEMORY\n") ;
      return -1;
  }
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

