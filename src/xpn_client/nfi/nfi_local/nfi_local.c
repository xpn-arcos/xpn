
  /*
   *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
   *
   *  This file is part of Expand.
   *
   *  Expand is free software: you can redistribute it and/or modify
   *  it under the terms of the GNU Lesser General Public License as published by
   *  the Free Software Foundation, either version 3 of the License, or
   *  (at your option) any later version.
   *
   *  Expand is distributed in the hope that it will be useful,
   *  but WITHOUT ANY WARRANTY; without even the implied warranty of
   *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   *  GNU Lesser General Public License for more details.
   *
   *  You should have received a copy of the GNU Lesser General Public License
   *  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
   *
   */


  /* ... Include / Inclusion ........................................... */

  #include "nfi_local.h"


  /* ... Global Variable / Variable Globales ........................... */

  #define FILESYSTEM_DLSYM 1
  
  /* ... Functions / Funciones ......................................... */

  /*
   * Communication
   */

  int nfi_local_keepConnected ( struct nfi_server *serv )
  {
    if (NULL == serv){
      return -1;
    }

#ifdef NFI_DYNAMIC
    if (serv->private_info == NULL)
    {
      ret = nfi_local_reconnect(serv) ;
      if (ret <0)
      {
        serv->private_info = NULL;
        return -1;
      }
    }
#endif

    // return OK
    return (serv->private_info != NULL) ;
  }


  /*
   * PRIVATE FUNCTIONS TO USE local SERVERS
   */

  void NFItoLOCALattr ( struct stat *att, struct nfi_attr *nfi_att )
  {
    att->st_dev = nfi_att->st_dev ;
    att->st_ino = nfi_att->st_ino ;

    if (nfi_att->at_type == NFIFILE){
      att->st_mode = nfi_att->at_mode | S_IFREG; // protection
    }

    if (nfi_att->at_type == NFIDIR){
      att->st_mode = nfi_att->at_mode | S_IFDIR; // protection
    }

    att->st_nlink   = nfi_att->at_nlink;   // number of hard links
    att->st_uid     = nfi_att->at_uid;     // user ID of owner
    att->st_gid     = nfi_att->at_gid;     // group ID of owner
    att->st_size    = nfi_att->at_size;    // size
    att->st_blksize = nfi_att->at_blksize; // blocksize for filesystem I/O
    att->st_blocks  = nfi_att->at_blocks;  // number of blocks allocated
    att->st_atime   = nfi_att->at_atime;   // time of last access
    att->st_mtime   = nfi_att->at_mtime;   // time of last modification
    att->st_ctime   = nfi_att->at_ctime;   // time of last change
  }

  void LOCALtoNFIattr ( struct nfi_attr *nfi_att, struct stat *att )
  {
    nfi_att->st_dev = att->st_dev;
    nfi_att->st_ino = att->st_ino;

    if (S_ISREG(att->st_mode)) {
      nfi_att->at_type = NFIFILE;
    }
    if (S_ISDIR(att->st_mode)) {
      nfi_att->at_type = NFIDIR;
    }

    nfi_att->at_mode    = att->st_mode&(S_IRWXU|S_IRWXG|S_IRWXO) ; // protection
    nfi_att->at_nlink   = att->st_nlink;                           // number of hard links
    nfi_att->at_uid     = att->st_uid;                             // user ID of owner
    nfi_att->at_gid     = att->st_gid;                             // group ID of owner
    nfi_att->at_size    = att->st_size;                            // total size, in bytes
    nfi_att->at_blksize = att->st_blksize;                         // blocksize for filesystem I/O
    nfi_att->at_blocks  = att->st_blocks;                          // number of blocks allocated
    nfi_att->at_atime   = att->st_atime;                           // time of last access
    nfi_att->at_mtime   = att->st_mtime;                           // time of last modification
    nfi_att->at_ctime   = att->st_ctime;                           // time of last change
  }

  void LOCALtoNFIInfo( __attribute__((__unused__)) struct nfi_info *nfi_inf, __attribute__((__unused__)) struct nfi_info *local_inf )
  {
    //TODO
  }


  /*
   * PUBLIC FUNCTIONS TO USE LOCAL
   */

  /************************************************************
   * Init Local                                               *
   ************************************************************/
  int nfi_local_init ( char *url, struct nfi_server *serv, __attribute__((__unused__)) struct nfi_attr_server *attr )
  {
    int ret;
    char server[PATH_MAX], dir[PATH_MAX], prt[PATH_MAX];
    struct nfi_local_server *server_aux;

    DEBUG_BEGIN();

    // check params...
    if (serv == NULL) {
      debug_error("ERROR: serv argument is NULL.\n") ;
      return -1 ;
    }

    // Alloc for serv->ops...
    serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops)) ;
    NULL_RET_ERR(serv->ops, LOCALERR_MEMORY) ;

    // Fill serv->ops...
    bzero(serv->ops, sizeof(struct nfi_ops)) ;
    serv->ops->nfi_reconnect  = nfi_local_reconnect;
    serv->ops->nfi_disconnect = nfi_local_disconnect;

    serv->ops->nfi_open     = nfi_local_open;
    serv->ops->nfi_create   = nfi_local_create;
    serv->ops->nfi_read     = nfi_local_read;
    serv->ops->nfi_write    = nfi_local_write;
    serv->ops->nfi_close    = nfi_local_close;
    serv->ops->nfi_remove   = nfi_local_remove;
    serv->ops->nfi_rename   = nfi_local_rename;
    serv->ops->nfi_getattr  = nfi_local_getattr;
    serv->ops->nfi_setattr  = nfi_local_setattr;

    serv->ops->nfi_opendir  = nfi_local_opendir;
    serv->ops->nfi_mkdir    = nfi_local_mkdir;
    serv->ops->nfi_readdir  = nfi_local_readdir;
    serv->ops->nfi_closedir = nfi_local_closedir;
    serv->ops->nfi_rmdir    = nfi_local_rmdir;

    //serv->ops->nfi_preload        = nfi_local_preload;
    //serv->ops->nfi_flush          = nfi_local_flush;

    serv->ops->nfi_statfs   = nfi_local_statfs;

    // ParseURL...
    ret = ParseURL(url, prt, NULL, NULL, server, NULL, dir) ;
    if (ret < 0)
    {
      // local_err(LOCALERR_URL) ;
      debug_error("ERROR: nfi_local_init found incorrect URL '%s'.\n", url) ;
      FREE_AND_NULL(serv->ops) ;
      return -1;
    }

    // Alloc for serv_aux...
    server_aux = (struct nfi_local_server *)malloc(sizeof(struct nfi_local_server)) ;
    if (server_aux == NULL)
    {
      debug_error("ERROR: out of memory\n") ;
      FREE_AND_NULL(serv->ops) ;
      return -1;
    }

    // Initialize serv_aux...
    memset(server_aux, 0, sizeof(struct nfi_local_server)) ;

    serv->xpn_thread = TH_NOT ;
    char *env_thread = getenv("XPN_THREAD");
    if (env_thread != NULL)
    {
      serv->xpn_thread = atoi(env_thread) ;
    }


    // copy 'url' string...
    //serv->protocol = LOCAL; //TODO: delete
    strcpy(server_aux->path, dir) ;
    serv->private_info = (void *)server_aux;

    serv->url = STRING_MISC_StrDup(url) ; // full URL
    if (serv->url == NULL)
    {
      debug_error("ERROR: out of memory\n") ;
      return -1;
    }

    serv->server = STRING_MISC_StrDup(server) ; // URL.server
    if (serv->server == NULL)
    {
      debug_error("ERROR: out of memory\n") ;
      return -1;
    }


    // nfi_worker...
    serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker)) ;
    memset(serv->wrk, 0, sizeof(struct nfi_worker)) ;
    serv->wrk->server = serv ;

    debug_info("[NFI-LOCAL] nfiworker_init()\n") ;
    ret = nfiworker_init(serv);

    ret = nfi_local_connect(serv, url, prt, server, dir) ;
    if (ret < 0) {
      FREE_AND_NULL(serv->ops) ;
      FREE_AND_NULL(server_aux) ;
      return -1 ;
    }

    DEBUG_END();

    // Return OK
    return 0;
  }


  /************************************************************
   * Destroy to the server                                    *
   * **********************************************************/
  int nfi_local_destroy ( struct nfi_server *serv )
  {
    struct nfi_local_server *server_aux;

    DEBUG_BEGIN();

    // Check arguments
    if (serv == NULL) {
      return -1;
    }

    server_aux = (struct nfi_local_server *) (serv->private_info) ;
    if (server_aux == NULL) {
      return -1;
    }
    
    // Thread destroy...
    debug_info("[NFI-LOCAL] nfiworker_destroy()\n") ;
    nfiworker_destroy(serv);

    // free private_info, 'url' string and 'server' string...
    FREE_AND_NULL(serv->ops) ;
    FREE_AND_NULL(serv->private_info) ;
    FREE_AND_NULL(serv->url) ;
    FREE_AND_NULL(serv->server) ;

    DEBUG_END();

    // Return OK
    return 0;
  }


  /*********************************************************
   * Connect to the server                                 *
   * *******************************************************/
  int nfi_local_connect ( struct nfi_server *serv, __attribute__((__unused__)) char *url, __attribute__((__unused__)) char* prt, __attribute__((__unused__)) char* server, __attribute__((__unused__)) char* dir )
  {
    struct nfi_local_server *server_aux;

    DEBUG_BEGIN();

    // check params...
    if (serv == NULL) {
      return -1;
    }

    server_aux = (struct nfi_local_server *) (serv->private_info) ;
    if (server_aux == NULL) {
      return -1;
    }

    DEBUG_END();

    return 0;
  }


  /************************************************************
   * Disconnect to the server                                 *
   * **********************************************************/
  int nfi_local_disconnect ( struct nfi_server *serv )
  {
    struct nfi_local_server *server_aux;

    DEBUG_BEGIN();

    // Check arguments
    if (serv == NULL) {
      return -1;
    }

    server_aux = (struct nfi_local_server *) (serv->private_info) ;
    if (server_aux == NULL) {
      return -1;
    }

    FREE_AND_NULL(serv->private_info);

    DEBUG_END();

    // Return OK
    return 0;
  }


  /************************************************************
   * Reconnect to the LOCAL server                            *
   ************************************************************/
  int nfi_local_reconnect ( struct nfi_server *serv ) //TODO
  {
    // Don't see the serv retult
    int ret;
    char   dir[PATH_MAX];
    struct nfi_local_server *server_aux;

    DEBUG_BEGIN();

    // Check arguments
    if (serv == NULL) {
      return -1;
    }

    server_aux = (struct nfi_local_server *) (serv->private_info) ;
    if (server_aux == NULL) {
      return -1;
    }

    ret = ParseURL(serv->url, NULL, NULL, NULL, NULL, NULL, dir) ;
    if (ret < 0) {
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


  /*
   *  GENERIC FUNCTIONS
   */

  int nfi_local_open ( struct nfi_server *serv, char *url, struct nfi_fhandle *fho )
  {
    int ret;
    char dir[PATH_MAX] ;
    struct nfi_local_fhandle *fh_aux;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, LOCALERR_PARAM) ;
    NULL_RET_ERR(fho,  LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // from url -> server + dir
    ret = ParseURL(url, NULL, NULL, NULL, NULL, NULL, dir);
    if (ret < 0)
    {
      fprintf(stderr,"nfi_local_open: url %s incorrect.\n",url) ;
      local_err(LOCALERR_URL) ;
      return -1 ;
    }

    fho->url = strdup(url) ;
    NULL_RET_ERR(fho->url, LOCALERR_MEMORY) ;

    fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle)) ;
    if (fh_aux == NULL)
    { 
      local_err(LOCALERR_MEMORY) ;
      FREE_AND_NULL(fho->url) ;
      return -1;
    }

    // Do open
    fh_aux->fd = real_posix_open2(dir, O_RDWR, S_IRWXU) ;
    if (fh_aux->fd < 0)
    {
      debug_error("real_posix_open fails to open '%s' in server %s.\n", dir, serv->server) ;
      FREE_AND_NULL(fh_aux) ;
      FREE_AND_NULL(fho->url) ;
      return -1;
    }

    strcpy(fh_aux->path, dir) ;

    fho->type    = NFIFILE;
    fho->priv_fh = NULL;
    fho->server = serv;
    fho->priv_fh = (void *) fh_aux;

    DEBUG_END();

    return 0;
  }


  int nfi_local_create ( struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh )
  {
    int ret;
    char   dir[PATH_MAX];
    struct nfi_local_fhandle *fh_aux;
    struct stat st;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, LOCALERR_PARAM) ;
    NULL_RET_ERR(attr, LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // url -> server + dir
    ret = ParseURL(url, NULL, NULL, NULL, NULL, NULL, dir) ;
    if (ret < 0)
    {
      fprintf(stderr,"ERROR: nfi_local_create: url %s incorrect.\n",url) ;
      local_err(LOCALERR_URL) ;
      return -1;
    }

    // private_info file handle
    fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle)) ;
    NULL_RET_ERR(fh_aux, LOCALERR_MEMORY) ;
    bzero(fh_aux, sizeof(struct nfi_local_fhandle)) ;
    
    // Do creat
    fh_aux->fd = real_posix_open2(dir, O_CREAT|O_RDWR|O_TRUNC, attr->at_mode) ;
    if (fh_aux->fd < 0)
    {
      debug_error("files_posix_open fails to creat '%s' in server '%s'.\n", dir, serv->server) ;
      FREE_AND_NULL(fh_aux) ;
      return -1;
    }

    // Get stat of the file
    memset(&st, 0, sizeof(struct stat)) ;
    ret = real_posix_stat(dir, &st) ;
    if (ret < 0)
    {
      debug_error("real_posix_stat fails to stat '%s' in server '%s'.\n", fh_aux->path, serv->server) ;
      FREE_AND_NULL(fh->url) ;
      FREE_AND_NULL(fh_aux) ;
      return -1;
    }

    fh->type   = NFIFILE;
    fh->server = serv;
    fh->priv_fh = (void *)fh_aux;

    fh->url = strdup(url) ;
    if (fh->url == NULL)
    {
      local_err(LOCALERR_MEMORY) ;
      FREE_AND_NULL(fh_aux) ;
      return -1;
    }

    LOCALtoNFIattr(attr, &st) ;

    DEBUG_END();

    return 0;
  }


  ssize_t nfi_local_read ( struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size )
  {
    ssize_t ret;
    struct nfi_local_fhandle *fh_aux;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, LOCALERR_PARAM) ;
    NULL_RET_ERR(fh,   LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // private_info
    fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;

    // Do read
    real_posix_lseek(fh_aux->fd, offset, SEEK_SET) ; //TODO: check error
    ret = real_posix_read(fh_aux->fd, buffer, size) ;

    debug_info("[NFI-LOCAL] read %s(%d) off %ld size %zu (ret:%zd)", fh->url, fh_aux->fd, (long int)offset, size, ret);
    if (ret < 0)
    {
      debug_error("real_posix_read reads zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, ret, errno) ;
      return -1;
    }

    DEBUG_END();

    return ret;
  }

  ssize_t nfi_local_write ( struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size )
  {
    ssize_t ret;
    struct nfi_local_fhandle *fh_aux;

    DEBUG_BEGIN();

    // Check arguments...
    if (size == 0){
      return 0;
    }

    NULL_RET_ERR(serv, LOCALERR_PARAM) ;
    NULL_RET_ERR(fh,   LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // private_info...
    fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
    
    // Do write
    real_posix_lseek(fh_aux->fd, offset, SEEK_SET) ; //TODO: check error
    ret = real_posix_write(fh_aux->fd, buffer, size) ;
    debug_info("[NFI-LOCAL] write %s off %ld size %zu (ret:%zd)\n", fh->url, (long int)offset, size, ret);
    if (ret < 0)
    {
      debug_error("files_posix_write_buffer writes zero bytes from url:%s offset:%ld size:%zu (ret:%zd) errno=%d\n", fh->url, (long int)offset, size, ret, errno) ;
      return -1;
    }

    DEBUG_END();

    return ret;
  }


  int nfi_local_close ( struct nfi_server *serv,  struct nfi_fhandle *fh )
  {
    struct nfi_local_fhandle *fh_aux ;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, LOCALERR_PARAM) ;
    NULL_RET_ERR(fh,   LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;
    
    // private_info
    fh_aux = (struct nfi_local_fhandle *) fh->priv_fh ;

    // Do close
    if (fh_aux != NULL)
    {
      real_posix_close(fh_aux->fd) ;
    }

    // free memory
    FREE_AND_NULL(fh->priv_fh) ;
    FREE_AND_NULL(fh->url) ;
    fh->type   = NFINULL ;
    fh->server = NULL ;

    DEBUG_END();

    // return OK
    return 0;
  }


  int nfi_local_remove ( struct nfi_server *serv,  char *url )
  {
    int ret;
    char dir[PATH_MAX];

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, LOCALERR_PARAM) ;
    NULL_RET_ERR(url,  LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // from url -> server + dir
    ret = ParseURL(url, NULL, NULL, NULL, NULL, NULL, dir) ;
    if (ret < 0)
    {
      fprintf(stderr,"nfi_local_remove: url %s incorrect.\n",url) ;
      local_err(LOCALERR_URL) ;
      return -1;
    }

    // Do unlink
    ret = real_posix_unlink(dir) ;
    if (ret < 0)
    {
      debug_error("nfi_local_remove: Fail remove %s in server %s.\n", dir, serv->server) ;
      return -1;
    }

    DEBUG_END();

    // Return OK
    return 0;
  }


  int nfi_local_rename (struct nfi_server *serv, char *old_url, char *new_url )
  {
    int  ret;
    char old_path[PATH_MAX], new_path[PATH_MAX];

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv,     LOCALERR_PARAM) ;
    NULL_RET_ERR(old_url,  LOCALERR_PARAM) ;
    NULL_RET_ERR(new_url,  LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // Get fields...
    ret = ParseURL(old_url, NULL, NULL, NULL, NULL, NULL, old_path);
    if (ret < 0)
    {
      fprintf(stderr,"nfi_local_open: url %s incorrect.\n",old_url) ;
      local_err(LOCALERR_URL) ;
      return -1;
    }

    ret = ParseURL(new_url, NULL, NULL, NULL, NULL, NULL, new_path);
    if (ret < 0)
    {
      fprintf(stderr,"nfi_local_open: url %s incorrect.\n",new_path) ;
      local_err(LOCALERR_URL) ;
      return -1;
    }

    ret = real_posix_rename(old_path, new_path) ;
    if (ret < 0)
    {
      debug_error("real_posix_rename fails to rename '%s' in server %s.\n", old_path, serv->server) ;
      return -1;
    }

    DEBUG_END();

    // Return OK
    return 0;
  }


  int nfi_local_getattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
  {
    int  ret;
    char dir[PATH_MAX];
  //struct nfi_local_fhandle *fh_aux;
    struct stat st;

    DEBUG_BEGIN();

    // check arguments...
    NULL_RET_ERR(serv,            LOCALERR_PARAM) ;
    NULL_RET_ERR(fh,              LOCALERR_PARAM) ;
    NULL_RET_ERR(attr,            LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // copy private information...
    //fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;

    ret = ParseURL(fh->url, NULL, NULL, NULL, NULL,  NULL,  dir) ;
    if (ret < 0)
    {
      fprintf(stderr,"nfi_mpi_server_getattr: url %s incorrect.\n", dir) ;
      // mpi_server_err(LOCALERR_URL) ;
      return -1;
    }

    // Do stat
    ret = real_posix_stat(dir, &st) ;
    if (ret < 0)
    {
      debug_error("nfi_local_getattr: Fail stat %s.\n", dir) ;
      return ret;
    }

    LOCALtoNFIattr(attr, &st) ;

    DEBUG_END();

    // Return OK
    return 0;
  }


  int nfi_local_setattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
  {
    struct stat st;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, LOCALERR_PARAM) ;
    NULL_RET_ERR(fh,   LOCALERR_PARAM) ;
    NULL_RET_ERR(attr, LOCALERR_PARAM) ;
    NULL_RET_ERR(fh->priv_fh, LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // TODO: setattr

    NFItoLOCALattr(&st, attr) ;

    DEBUG_END();

    // Return OK
    return 0;
  }


  int nfi_local_mkdir ( struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh )
  {
    int    ret;
    char   dir[PATH_MAX];
    struct nfi_local_fhandle *fh_aux;
    struct stat st;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, LOCALERR_PARAM) ;
    NULL_RET_ERR(attr, LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // Get fields...
    ret = ParseURL(url, NULL, NULL, NULL, NULL, NULL, dir) ;
    if (ret < 0)
    {
      fprintf(stderr,"nfi_local_mkdir: url %s incorrect.\n",url) ;
      local_err(LOCALERR_URL) ;
      return -1;
    }

    // private_info file handle
    fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle)) ;
    NULL_RET_ERR(fh_aux, LOCALERR_MEMORY) ;
    bzero(fh_aux, sizeof(struct nfi_local_fhandle)) ;

    // Do mkdir
    ret = real_posix_mkdir(dir, /*attr->at_mode*/ 0777) ;
    if ((ret < 0) && (errno != EEXIST))
    {
      debug_error("nfi_local_mkdir: Fail mkdir %s.\n", dir) ;
      FREE_AND_NULL(fh_aux) ;
      return -1;
    }

    // Do stat
    ret = real_posix_stat(dir, &st) ;
    if (ret < 0)
    {
      debug_error("nfi_local_getattr: Fail stat %s.\n", dir) ;
      return ret;
    }

    fh->type = NFIDIR;
    fh->priv_fh = (void *)fh_aux;

    fh->url = STRING_MISC_StrDup(url) ;
    if (fh->url == NULL) {
      local_err(LOCALERR_MEMORY) ;
      FREE_AND_NULL(fh_aux) ;
      return -1;
    }

    LOCALtoNFIattr(attr, &st) ;

    DEBUG_END();

    // return OK
    return 0;
  }


  int nfi_local_opendir ( struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
  {
    int    ret;
    char   dir[PATH_MAX];
    struct nfi_local_fhandle *fh_aux;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, LOCALERR_PARAM) ;
    NULL_RET_ERR(url,  LOCALERR_PARAM) ;
    NULL_RET_ERR(fho,  LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // Get fields...
    ret = ParseURL(url, NULL, NULL, NULL, NULL, NULL, dir) ;
    if (ret < 0)
    {
      fprintf(stderr,"nfi_local_opendir: url %s incorrect.\n",url) ;
      local_err(LOCALERR_URL) ;
      return -1;
    }

    fho->url = strdup(url) ;
    NULL_RET_ERR(fho->url, LOCALERR_MEMORY) ;

    fh_aux = (struct nfi_local_fhandle *)malloc(sizeof(struct nfi_local_fhandle)) ;
    if (fh_aux == NULL)
    {
      local_err(LOCALERR_MEMORY) ;
      FREE_AND_NULL(fho->url) ;
      return -1;
    }

    // Do opendir
    fh_aux->dir = real_posix_opendir(dir) ;
    if (fh_aux->dir == NULL)
    {
      FREE_AND_NULL(fh_aux) ;
      FREE_AND_NULL(fho->url) ;
      debug_error("real_posix_opendir fails to open directory '%s' in server '%s'.\n", dir, serv->server) ;
      return -1;
    }

    strcpy(fh_aux->path, dir) ;
    fho->type    = NFIDIR;
    fho->server  = serv;
    fho->priv_fh = (void *) fh_aux;

    DEBUG_END();

    // Return OK
    return 0 ;
  }

  int nfi_local_readdir ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct dirent *entry )
  {
    struct nfi_local_fhandle *fh_aux;
    struct dirent *ent;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv,        LOCALERR_PARAM) ;
    NULL_RET_ERR(fh,          LOCALERR_PARAM) ;
    NULL_RET_ERR(fh->priv_fh, LOCALERR_PARAM) ;
    if (fh->type != NFIDIR) {
      local_err(LOCALERR_NOTDIR) ;
      return -1;
    }
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // private_info...
    fh_aux = (struct nfi_local_fhandle *)fh->priv_fh;

    // cleaning entry values...
    memset(entry, 0, sizeof(struct dirent)) ;

    // Do readdir
    ent = real_posix_readdir(fh_aux->dir) ;
    if (ent == NULL)
    {
      debug_error("nfi_local_readdir: readdir") ;
      return -1;
    }

    memcpy(entry, ent, sizeof(struct dirent)) ;

    // Return OK
    return 0;
  }


  int nfi_local_closedir ( struct nfi_server *serv,  struct nfi_fhandle *fh )
  {
    struct nfi_local_fhandle *fh_aux;

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, LOCALERR_PARAM) ;
    NULL_RET_ERR(fh,   LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // Do closedir
    if (fh->priv_fh != NULL){
      fh_aux = (struct nfi_local_fhandle *) fh->priv_fh;
      real_posix_closedir(fh_aux->dir) ;
    }

    // free memory
    FREE_AND_NULL(fh->priv_fh) ;
    FREE_AND_NULL(fh->url) ;

    DEBUG_END();

    // Return OK
    return 0;
  }


  int nfi_local_rmdir ( struct nfi_server *serv,  char *url )
  {
    int ret;
    char dir[PATH_MAX];

    DEBUG_BEGIN();

    // Check arguments...
    NULL_RET_ERR(serv, LOCALERR_PARAM) ;
    NULL_RET_ERR(url,  LOCALERR_PARAM) ;
    nfi_local_keepConnected(serv) ;
    NULL_RET_ERR(serv->private_info, LOCALERR_PARAM) ;

    // Get fields...
    ret = ParseURL(url, NULL, NULL, NULL, NULL, NULL, dir) ;
    if (ret < 0)
    {
      fprintf(stderr,"nfi_local_rmdir: url %s incorrect.\n", url) ;
      local_err(LOCALERR_URL) ;
      return -1;
    }

    // Do rmdir
    ret = real_posix_rmdir(dir) ;
    if (ret < 0)
    {
      debug_error(stderr,"nfi_local_rmdir: Fail rmdir %s.\n", dir) ;
      return -1;
    }

    DEBUG_END();

    // Return OK
    return 0;
  }


  int nfi_local_statfs ( __attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_info *inf )
  {
    DEBUG_BEGIN();

    /*
    struct local_info localinf;
    int ret;
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
    ret = local_statfs(server_aux->fh, &localinf, server_aux->cl) ;
    if (ret <0) {
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


  /*int nfi_local_preload(struct nfi_server *serv, char *url, char *virtual_path, char *storage_path, int opt)
  {
    
  }


  int nfi_local_flush ( struct nfi_server *serv,  char *url, char *virtual_path, char *storage_path, int opt )
  {
    
  }*/


/* ................................................................... */

