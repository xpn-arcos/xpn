
/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

  #include "nfi_tcp_server.h"


  /* ... Global Variable / Variable Globales ........................... */

/*
 * ENVIROMENT VARIABLES:
 * XPN_DNS -> node{ID}  {hostname}  {port}
 */


  /* ... Functions / Funciones ......................................... */

  /*
   * Communication
   */

  int tcp_server_write_operation(int sd, struct st_tcp_server_msg *head)
  {
      int ret;

      debug_info("[NFI-TCP] (ID=%s) tcp_server_write_data: begin\n", head->id);
      ret = tcp_server_write_data(sd, (char *)&head->type, sizeof(head->type), head->id);
      if (ret == -1){
          debug_warning("Server[?]: tcp_server_write_data fails :-(") ;
          return -1;
      }

      debug_info("[NFI-TCP] (ID=%s)write_operation: %d -> \n", head->id, head->type);
      switch(head->type)
      {
	// File API
	case TCP_SERVER_OPEN_FILE:
		debug_info("[NFI-TCP] (ID=%s)OPEN operation\n",head->id);
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_open, sizeof(struct st_tcp_server_open), head->id);
		if(ret == -1){
			return -1;
		}
		break;
	case TCP_SERVER_CREAT_FILE:
		debug_info("[NFI-TCP] (ID=%s)CREAT operation\n",head->id);
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_creat, sizeof(struct st_tcp_server_creat), head->id);
		if(ret == -1){
			return -1;
		}
		break;
	case TCP_SERVER_READ_FILE:
		debug_info("[NFI-TCP] (ID=%s)READ operation\n",head->id);
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_read, sizeof(struct st_tcp_server_read), head->id);
		if(ret == -1){
			return -1;
		}
		break;
	case TCP_SERVER_WRITE_FILE:
		debug_info("[NFI-TCP] (ID=%s)WRITE operation\n", head->id);
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_write, sizeof(struct st_tcp_server_write), head->id);
		if(ret == -1){
			return -1;
		}
		break;
	case TCP_SERVER_CLOSE_FILE:
		debug_info("[NFI-TCP] (ID=%s)CLOSE operation\n",head->id);
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_close, sizeof(struct st_tcp_server_close), head->id);
		if(ret == -1){
			return -1;
		}
		break;
	case TCP_SERVER_RM_FILE:
		debug_info("[NFI-TCP] (ID=%s)RM operation\n",head->id);
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_rm, sizeof(struct st_tcp_server_rm), head->id);
		if(ret == -1){
			return -1;
		}
		break;
	case TCP_SERVER_GETATTR_FILE:
		debug_info("[NFI-TCP] (ID=%s)GETATTR operation\n",head->id);
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_getattr, sizeof(struct st_tcp_server_getattr), head->id);
		if(ret == -1){
			return -1;
		}
		break;

	// Directory API
	case TCP_SERVER_MKDIR_DIR:
		debug_info("[NFI-TCP] (ID=%s)MDKIR operation\n",head->id);
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_mkdir, sizeof(struct st_tcp_server_mkdir), head->id);
		if(ret == -1){
			return -1;
		}
		break;
	case TCP_SERVER_RMDIR_DIR:
		debug_info("[NFI-TCP] (ID=%s)RMDIR operation\n",head->id);
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_rmdir, sizeof(struct st_tcp_server_rmdir), head->id);
		if(ret == -1){
			return -1;
		}
		break;

	// Optimization API
	case TCP_SERVER_FLUSH_FILE:
		debug_info("[NFI-TCP] (ID=%s)FLUSH operation\n",head->id);
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_flush, sizeof(struct st_tcp_server_flush), head->id);
		if(ret == -1){
			return -1;
		}
		break;
	case TCP_SERVER_PRELOAD_FILE:
		debug_info("[NFI-TCP] (ID=%s)PRELOAD operation\n",head->id);
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_preload, sizeof(struct st_tcp_server_preload), head->id);
		if(ret == -1){
			return -1;
		}
		break;

	// Filesystem API
	case TCP_SERVER_FINALIZE:
		debug_info("[NFI-TCP] (ID=%s)FINALIZE operation\n",head->id);
		/*
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_end, sizeof(struct st_tcp_server_end), head->id);
		if(ret == -1){
			return -1;
		}
		*/
		break;
	case TCP_SERVER_GETID:
		debug_info("[NFI-TCP] (ID=%s)GETID operation\n",head->id);
		/*
		ret = tcp_server_write_data(sd, (char *)head->id, TCP_SERVER_ID, head->id);
		if(ret == -1){
			return -1;
		}
		*/
		break;
	case TCP_SERVER_END:
		debug_info("[NFI-TCP] (ID=%s)END operation\n",head->id);
		/*
		ret = tcp_server_write_data(sd, (char *)&head->u_st_tcp_server_msg.op_end, sizeof(struct st_tcp_server_end), head->id);
		if(ret == -1){
			return -1;
		}
		*/
		break;
        }

        return ret;
  }


/************************************************************
 * PRIVATE FUNCTIONS TO USE tcp_server SERVERS             *
 ************************************************************/
void NFItoTCP_SERVERattr(struct stat *att, struct nfi_attr *nfi_att)
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

void TCP_SERVERtoNFIattr (struct nfi_attr *nfi_att, struct stat *att)
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

void TCP_SERVERtoNFIInfo ( __attribute__((__unused__)) struct nfi_info *nfi_inf, __attribute__((__unused__)) struct nfi_info *tcp_server_inf )
{
	// TODO
}

/************************************************************
 * PUBLIC FUNCTIONS TO USE TCP_SERVER          *
 ************************************************************/

/************************************************************
 * Init Local                     *
 ************************************************************/
int nfi_tcp_server_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr)
{
  int ret;
  struct nfi_tcp_server_server *server_aux;
  struct st_tcp_server_msg msg;
  char server[PATH_MAX], dir[PATH_MAX], prt[PATH_MAX];


  debug_info("[NFI-TCP] nfi_tcp_server_init: begin\n");
  debug_info("[NFI-TCP] nfi_tcp_server_init: sizeof(struct st_tcp_server_msg) = %lu.\n", (unsigned long)sizeof(struct st_tcp_server_msg));

  if(serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  /* functions */
  serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
  if (serv->ops == NULL){
    tcp_server_err(TCP_SERVERERR_MEMORY);
    return -1;
  }

  bzero(serv->ops, sizeof(struct nfi_ops));

  serv->ops->nfi_preload  = nfi_tcp_server_preload;
  serv->ops->nfi_flush  = nfi_tcp_server_flush;

  serv->ops->nfi_reconnect  = nfi_tcp_server_reconnect;
  serv->ops->nfi_disconnect = nfi_tcp_server_disconnect;

  serv->ops->nfi_getattr  = nfi_tcp_server_getattr;
  serv->ops->nfi_setattr  = nfi_tcp_server_setattr;

  serv->ops->nfi_open = nfi_tcp_server_open;
  serv->ops->nfi_close  = nfi_tcp_server_close;

  serv->ops->nfi_read = nfi_tcp_server_read;
  serv->ops->nfi_write  = nfi_tcp_server_write;

  serv->ops->nfi_create = nfi_tcp_server_create;
  serv->ops->nfi_remove = nfi_tcp_server_remove;
  serv->ops->nfi_rename = nfi_tcp_server_rename;

  serv->ops->nfi_mkdir  = nfi_tcp_server_mkdir;
  serv->ops->nfi_rmdir  = nfi_tcp_server_rmdir;
  serv->ops->nfi_opendir  = nfi_tcp_server_opendir;
  serv->ops->nfi_readdir  = nfi_tcp_server_readdir;
  serv->ops->nfi_closedir = nfi_tcp_server_closedir;
  serv->ops->nfi_statfs = nfi_tcp_server_statfs;

  ret = ParseURL(url,  prt, NULL, NULL, server,  NULL,  dir);
  if(ret <0 ){
    tcp_server_err(TCP_SERVERERR_URL);
    fprintf(stderr,"nfi_tcp_server_init: url %s incorrect.\n",url);
    free(serv->ops);
    return -1;
  }
  server_aux = (struct nfi_tcp_server_server *)malloc(sizeof(struct nfi_tcp_server_server));
  if(server_aux == NULL){
    tcp_server_err(TCP_SERVERERR_MEMORY);
    return -1;
  }

  /******************************************/
  server_aux->sd = tcp_server_connect(server);
  strcpy(msg.id, "GETID");
        msg.type = TCP_SERVER_GETID;
        tcp_server_write_operation(server_aux->sd, &msg);
  debug_info("[NFI-TCP] nfi_tcp_server_init: tcp_server_write_data\n");
  tcp_server_read_data(server_aux->sd, (char *)server_aux->id, TCP_SERVER_ID, msg.id);
  debug_info("[NFI-TCP] nfi_tcp_server_init: tcp_server_read_data id = %s\n",server_aux->id);
  /******************************************/

  serv->private_info = (void *)server_aux;
  //serv->protocol = TCP_SERVER;

  serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1)); /* server address */
  if(serv->server == NULL){
    tcp_server_err(TCP_SERVERERR_MEMORY);
    return -1;
  }
  strcpy(serv->server, server);

  serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1)); /* server address */
  if(serv->url == NULL){
    tcp_server_err(TCP_SERVERERR_MEMORY);
    return -1;
  }

  strcpy(serv->url, url);
  serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
  memset(serv->wrk, 0, sizeof(struct nfi_worker));


  if(strcmp("tcp_server", prt) == 0)
  {
    debug_info("[NFI-TCP] tcp_server\n");
    debug_info("[NFI-TCP] nfi_worker_init(1,ID=%s): \n",server_aux->id);
    nfi_worker_init(serv->wrk, serv, 1);
  }
  else{
    debug_info("[NFI-TCP] nfi_worker_init(0,ID=%s): \n",server_aux->id);
    nfi_worker_init(serv->wrk, serv, 0);
  }

  debug_info("[NFI-TCP] nfi_tcp_server_init(ID=%s): end\n",server_aux->id);
  return 0;
}

/************************************************************
 * Disconnect to the server           *
 * **********************************************************/
int nfi_tcp_server_disconnect(struct nfi_server *serv)
{
  struct nfi_tcp_server_server *server_aux;

  if (serv == NULL)
      return 0;

  server_aux = (struct nfi_tcp_server_server *)serv->private_info;
  if(server_aux != NULL){
       close(server_aux->sd);
       debug_info("[NFI-TCP] close connection\n");
       free(serv->private_info);
  }

  if(serv->url != NULL){
     free(serv->url);
     serv->url = NULL;
  }

  if(serv->server != NULL){
    free(serv->server);
    serv->server = NULL ;
  }

  //serv->protocol = -1;

  return 0;
}


/************************************************************
 * Reconnect to the NFS server            *
 ************************************************************/
int nfi_tcp_server_reconnect(struct nfi_server *serv)
{
  /* Don't see the serv result */
  char server[PATH_MAX], dir[PATH_MAX];
  int ret;
  struct nfi_tcp_server_server *server_aux;

  ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
  if(ret <0 ){
    tcp_server_err(TCP_SERVERERR_URL);
    fprintf(stderr,"nfi_tcp_server_reconnect: url %s incorrect.\n",serv->url);
    return -1;
  }

  server_aux = (struct nfi_tcp_server_server *)malloc(sizeof(struct nfi_tcp_server_server));
  if(server_aux == NULL){
    tcp_server_err(TCP_SERVERERR_MEMORY);
    return -1;
  }

  strcpy(server_aux->path, dir);

  serv->private_info = (void *)server_aux;

  return 0;
}


/************************************************************
 * Destroy NFS operations           *
 ************************************************************/

int nfi_tcp_server_destroy(struct nfi_server *serv)
{
  struct nfi_tcp_server_server *server_aux;
  struct st_tcp_server_msg msg;

  if (serv == NULL)
    return 0;

  if(serv->ops != NULL) {
    free(serv->ops);
    serv->ops = NULL ;
  }

  server_aux = (struct nfi_tcp_server_server *)serv->private_info;

  if (server_aux != NULL)
  {
    debug_info("[NFI-TCP] close connection\n");
    msg.type = (char) -1;
    tcp_server_write_operation(server_aux->sd, &msg);
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
 * GENERIC FUNCTIONS                                        *
 ************************************************************/

int nfi_tcp_server_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr)
{
  struct nfi_tcp_server_server *server_aux;
  struct nfi_tcp_server_fhandle *fh_aux;
  struct st_tcp_server_msg msg;
  struct st_tcp_server_attr_req req;

  debug_info("[NFI-TCP] nfi_tcp_server_getattr(ID=%s): begin\n",server_aux->id);

  if (attr == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (fh == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (fh->priv_fh == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  server_aux = (struct nfi_tcp_server_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL)
  {
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      /* tcp_server_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL) {
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
  }
#endif

  fh_aux = (struct nfi_tcp_server_fhandle *) fh->priv_fh;
  server_aux = (struct nfi_tcp_server_server *) serv->private_info;

  /*****************************************/
  msg.type = TCP_SERVER_GETATTR_FILE;
  strcpy(msg.u_st_tcp_server_msg.op_getattr.path, fh_aux->path);

  tcp_server_write_operation(server_aux->sd, &msg);
  debug_info("[NFI-TCP] nfi_tcp_server_getattr(ID=%s): getattr: -> %s \n", server_aux->id, msg.u_st_tcp_server_msg.op_getattr.path);
  bzero(&req, sizeof(struct st_tcp_server_attr_req));
  tcp_server_read_data(server_aux->sd, (char *)&req, sizeof(struct st_tcp_server_attr_req), msg.id);
  debug_info("[NFI-TCP] nfi_tcp_server_getattr(ID=%s): getattr: <- %d\n", server_aux->id, req.status);
  /*****************************************/

  /*
  ret = stat(fh_aux->path, &st);
  if(ret < 0){
    fprintf(stderr,"nfi_tcp_server_getattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
    tcp_server_err(TCP_SERVERERR_GETATTR);
    return -1;
  }
  */

  TCP_SERVERtoNFIattr(attr, &req.attr);

  debug_info("[NFI-TCP] nfi_tcp_server_getattr(ID=%s): end\n",server_aux->id);

  return req.status;
}


int nfi_tcp_server_setattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr)
{
  struct nfi_tcp_server_server *server_aux;
  struct nfi_tcp_server_fhandle *fh_aux;

  if (attr == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (fh->priv_fh == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL)
  {
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      /* tcp_server_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
  }
#endif

  fh_aux = (struct nfi_tcp_server_fhandle *) fh->priv_fh;
  server_aux = (struct nfi_tcp_server_server *) serv->private_info;

 // NFItoTCP_SERVERattr(&st, attr);
  /* no se comp hacer el setattr */
  /*
  ret = stat(fh_aux->path, &st);
  */
/*
  ret = truncate(fh_aux->path, st.st_tcp_server_size);
  if(ret < 0){
    fprintf(stderr,"nfi_tcp_server_setattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
    tcp_server_err(TCP_SERVERERR_GETATTR);
    return -1;
  }
*/

  return 0;
}


int nfi_tcp_server_preload(struct nfi_server *serv, char *url, char *virtual_path, char *storage_path, int opt)
{
  //char dir[PATH_MAX];
  int ret;
  struct nfi_tcp_server_server *server_aux;
  struct st_tcp_server_msg msg;

  debug_info("[NFI-TCP] nfi_tcp_server_preload(ID=%s): begin %s - %s \n", server_aux->id, virtual_path, storage_path);

  if(url == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if(url[strlen(url)-1] == '/'){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (virtual_path == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (storage_path == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  server_aux = (struct nfi_tcp_server_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL)
  {
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      /* tcp_server_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
  }
#endif

  server_aux = (struct nfi_tcp_server_server *) serv->private_info;
  debug_info("[NFI-TCP] nfi_tcp_server_preload(ID=%s): preload %s in server %s.\n",server_aux->id,virtual_path,serv->server);

  /*****************************************/
  msg.type = TCP_SERVER_PRELOAD_FILE;
  //strcpy(msg.u_st_tcp_server_msg.op_preload.path,dir);
  strcpy(msg.u_st_tcp_server_msg.op_preload.virtual_path,virtual_path);
  strcpy(msg.u_st_tcp_server_msg.op_preload.storage_path,storage_path);
  msg.u_st_tcp_server_msg.op_preload.opt = opt;

  tcp_server_write_operation(server_aux->sd, &msg);
  debug_info("[NFI-TCP] nfi_tcp_server_preload(ID=%s): preload: -> %s \n",server_aux->id,msg.u_st_tcp_server_msg.op_preload.virtual_path);
  tcp_server_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
  debug_info("[NFI-TCP] nfi_tcp_server_preload(ID=%s): preload: <- %d \n",server_aux->id,ret);
  /*****************************************/

  debug_info("[NFI-TCP] nfi_tcp_server_preload(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret);
  if(ret == -1){
    printf("[NFI-TCP] Error en el preload\n");
  }

  return ret;
}


int nfi_tcp_server_flush(struct nfi_server *serv,  char *url, char *virtual_path, char *storage_path, int opt)
{
  //char dir[PATH_MAX];
  int ret;
  struct nfi_tcp_server_server *server_aux;
  struct st_tcp_server_msg msg;

  debug_info("[NFI-TCP] nfi_tcp_server_flush(ID=%s): begin %s - %s \n", server_aux->id,virtual_path, storage_path);

  if(url == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if(url[strlen(url)-1] == '/'){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (virtual_path == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (storage_path == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  server_aux = (struct nfi_tcp_server_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL)
  {
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      /* tcp_server_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
  }
#endif

  server_aux = (struct nfi_tcp_server_server *) serv->private_info;
  debug_info("[NFI-TCP] nfi_tcp_server_flush(ID=%s): open %s in server %s.\n",server_aux->id,virtual_path,serv->server);

  /*****************************************/
  msg.type = TCP_SERVER_FLUSH_FILE;
  //strcpy(msg.u_st_tcp_server_msg.op_flush.path,dir);
  strcpy(msg.u_st_tcp_server_msg.op_flush.virtual_path,virtual_path);
  strcpy(msg.u_st_tcp_server_msg.op_flush.storage_path,storage_path);
  msg.u_st_tcp_server_msg.op_flush.opt = opt;

        tcp_server_write_operation(server_aux->sd, &msg);
  debug_info("[NFI-TCP] nfi_tcp_server_flush(ID=%s): flush: -> %s \n",server_aux->id,msg.u_st_tcp_server_msg.op_flush.virtual_path);
  tcp_server_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
  debug_info("[NFI-TCP] nfi_tcp_server_flush(ID=%s): flush: <- %d \n",server_aux->id,ret);
  /*****************************************/

  debug_info("[NFI-TCP] nfi_tcp_server_flush(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret);

  return 0;
}


int nfi_tcp_server_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
  char dir[PATH_MAX], server[PATH_MAX];
  int ret;
  struct nfi_tcp_server_server *server_aux;
  struct nfi_tcp_server_fhandle *fh_aux;
  struct st_tcp_server_msg msg;

  debug_info("[NFI-TCP] nfi_tcp_server_open(ID=%s): begin %s\n",server_aux->id,url);
  if(url[strlen(url)-1] == '/'){
    return nfi_tcp_server_opendir(serv, url, fho);
  }

  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (fho == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  server_aux = (struct nfi_tcp_server_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL)
  {
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      /* tcp_server_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL)
  {
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
  }
#endif

  ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0){
    fprintf(stderr,"nfi_tcp_server_open: url %s incorrect.\n",url);
    tcp_server_err(TCP_SERVERERR_URL);
    return -1;
  }
  fho->url = (char *)malloc(strlen(url)+1);
  if(fho->url == NULL){
    tcp_server_err(TCP_SERVERERR_MEMORY);
    return -1;
  }

  strcpy(fho->url, url);

  fh_aux = (struct nfi_tcp_server_fhandle *)malloc(sizeof(struct nfi_tcp_server_fhandle));
  if (fh_aux == NULL){
    tcp_server_err(TCP_SERVERERR_MEMORY);
    free(fho->url);
    return -1;
  }

  server_aux = (struct nfi_tcp_server_server *) serv->private_info;

  /*****************************************/
  msg.type = TCP_SERVER_OPEN_FILE;
  strcpy(msg.u_st_tcp_server_msg.op_open.path,dir);

  tcp_server_write_operation(server_aux->sd, &msg);
  debug_info("[NFI-TCP] nfi_tcp_server_open(ID=%s): open -> %s \n",server_aux->id,msg.u_st_tcp_server_msg.op_open.path);
  tcp_server_read_data(server_aux->sd, (char *)&fh_aux->fd, sizeof(int), msg.id);
  debug_info("[NFI-TCP] nfi_tcp_server_open(ID=%s): open <- %d \n",server_aux->id,fh_aux->fd);
  strcpy(fh_aux->path, dir);
  /*****************************************/

  fho->type = NFIFILE;

  fho->server = NULL;
  fho->priv_fh = NULL;
  fho->server = serv;
  fho->priv_fh = (void *) fh_aux;

  debug_info("[NFI-TCP] nfi_tcp_server_open(ID=%s): end\n",server_aux->id);

  return 0;
}


int nfi_tcp_server_close(struct nfi_server *server,  struct nfi_fhandle *fh)
{
  struct nfi_tcp_server_fhandle *fh_aux;
  struct nfi_tcp_server_server *server_aux;
  struct st_tcp_server_msg msg;

  debug_info("[NFI-TCP] nfi_tcp_server_close(ID=%s): begin\n",server_aux->id);

  if (server == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }
  if (fh == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  server_aux = (struct nfi_tcp_server_server *) server->private_info;
  strcpy(msg.id,server_aux->id);

  if (fh->priv_fh != NULL)
  {
    fh_aux = (struct nfi_tcp_server_fhandle *) fh->priv_fh;
    server_aux = (struct nfi_tcp_server_server *) server->private_info;

    /*****************************************/
    msg.type = TCP_SERVER_CLOSE_FILE;
    msg.u_st_tcp_server_msg.op_close.fd = fh_aux->fd;

    tcp_server_write_operation(server_aux->sd, &msg);
    debug_info("[NFI-TCP] nfi_tcp_server_close(ID=%s): close -> %d \n",server_aux->id,msg.u_st_tcp_server_msg.op_close.fd);
    /*****************************************/

    //close(fh_aux->fd);
    /* free memory */
    free(fh->priv_fh);
    fh->priv_fh = NULL;
  }

  fh->type = NFINULL;
  fh->server = NULL;

  debug_info("[NFI-TCP] nfi_tcp_server_close(ID=%s): end\n",server_aux->id);

  return 0;
}

ssize_t nfi_tcp_server_read( struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size)
{
  int ret, cont, diff;
  struct nfi_tcp_server_server *server_aux;
  struct nfi_tcp_server_fhandle *fh_aux;
  struct st_tcp_server_msg msg;
  struct st_tcp_server_read_req req;

  debug_info("[NFI-TCP] nfi_tcp_server_read(%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size);

  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (fh == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  server_aux = (struct nfi_tcp_server_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);


#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL)
  {
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      /* tcp_server_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL)
  {
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
  }
#endif

  fh_aux = (struct nfi_tcp_server_fhandle *) fh->priv_fh;
  server_aux = (struct nfi_tcp_server_server *) serv->private_info;

  /*****************************************/
  msg.type = TCP_SERVER_READ_FILE;
  msg.u_st_tcp_server_msg.op_read.fd       = fh_aux->fd;
  msg.u_st_tcp_server_msg.op_read.offset   = offset;
  msg.u_st_tcp_server_msg.op_read.size   = size;

  #ifdef  DBG_IO
    printf("[NFI-TCP] read: -> fd %d \n",msg.u_st_tcp_server_msg.op_read.fd);
    printf("[NFI-TCP] read: -> offset %d \n",(int)msg.u_st_tcp_server_msg.op_read.offset);
    printf("[NFI-TCP] read: -> size %d \n",msg.u_st_tcp_server_msg.op_read.size);
  #endif

  ret = tcp_server_write_operation(server_aux->sd, &msg);
  if(ret == -1){
    perror("ERROR: (1)nfi_tcp_server_read: Error on write operation");
    fprintf(stderr,"ERROR: (1)nfi_tcp_server_read: Error on write operation\n");
    return -1;
  }

  cont = 0;
  do
  {
    ret = tcp_server_read_data(server_aux->sd, (char *)&req, sizeof(struct st_tcp_server_read_req), msg.id);
    if(ret == -1){
      perror("ERROR: (2)nfi_tcp_server_read: Error on write operation");
      fprintf(stderr,"ERROR: (2)nfi_tcp_server_read: Error on write operation\n");
      return -1;
    }

    if(req.size > 0){
      ret = tcp_server_read_data(server_aux->sd, (char *)buffer+cont, req.size, msg.id);
      if(ret == -1){
        perror("ERROR: (3)nfi_tcp_server_read: Error on write operation");
        fprintf(stderr,"ERROR: (3)nfi_tcp_server_read: Error on read operation\n");
        return -1;
      }
    }

    cont = cont + req.size ;
    diff = msg.u_st_tcp_server_msg.op_read.size - cont;

  } while ((diff > 0) && (req.size != 0));

  if(req.size < 0){
    fprintf(stderr,"ERROR: nfi_tcp_server_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)req.size);
    tcp_server_err(TCP_SERVERERR_READ);
    return -1;
  }

  return cont;
}


ssize_t nfi_tcp_server_write(struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size)
{
    struct nfi_tcp_server_server *server_aux;
    struct nfi_tcp_server_fhandle *fh_aux;
    struct st_tcp_server_msg msg;
    struct st_tcp_server_write_req req;
    int ret, diff, cont;


    debug_info("[NFI-TCP] nfi_tcp_server_write(ID=%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size);

    if(size == 0){
      return 0;
    }

    if (serv == NULL){
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
    }

    if (fh == NULL){
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
    }

    server_aux = (struct nfi_tcp_server_server *) serv->private_info;
    strcpy(msg.id,server_aux->id);

#ifdef NFI_DYNAMIC
    if (serv->private_info == NULL)
    {
      ret = nfi_tcp_server_reconnect(serv);
      if(ret <0){
        /* tcp_server_err(); not necessary */
        return -1;
      }
    }
#else
    if (serv->private_info == NULL)
    {
               tcp_server_err(TCP_SERVERERR_PARAM);
               return -1;
    }
#endif


    fh_aux = (struct nfi_tcp_server_fhandle *) fh->priv_fh;
    server_aux = (struct nfi_tcp_server_server *) serv->private_info;

    /*****************************************/
    msg.type = TCP_SERVER_WRITE_FILE;
    msg.u_st_tcp_server_msg.op_write.fd  = fh_aux->fd;
    msg.u_st_tcp_server_msg.op_write.offset  = offset;
    msg.u_st_tcp_server_msg.op_write.size  = size;

    #ifdef  DBG_IO
      printf("[NFI-TCP] write: -> fd %d \n",msg.u_st_tcp_server_msg.op_write.fd);
      printf("[NFI-TCP] write: -> offset %d \n",(int)msg.u_st_tcp_server_msg.op_write.offset);
      printf("[NFI-TCP] write: -> size %d \n",msg.u_st_tcp_server_msg.op_write.size);
    #endif

    ret = tcp_server_write_operation(server_aux->sd, &msg);
    if(ret == -1){
      fprintf(stderr,"(1)ERROR: nfi_tcp_server_write(ID=%s): Error on write operation\n",server_aux->id);
      return -1;
    }

    diff = size;
    cont = 0;

    int buffer_size = size;
    if (buffer_size > (MAX_BUFFER_SIZE)) {
        buffer_size = MAX_BUFFER_SIZE;
    }

    do
    {
      if (diff > buffer_size)
      {
        ret = tcp_server_write_data(server_aux->sd, (char *)buffer + cont, buffer_size, msg.id);
        if(ret == -1){
          fprintf(stderr,"(2)ERROR: nfi_tcp_server_read(ID=%s): Error on write operation\n",server_aux->id);
          return -1;
        }
      }
      else{
        ret = tcp_server_write_data(server_aux->sd, (char *)buffer + cont, diff, msg.id);
        if(ret == -1){
          fprintf(stderr,"(2)ERROR: nfi_tcp_server_read(ID=%s): Error on write operation\n",server_aux->id);
          return -1;
        }
      }

      cont = cont + ret; //Send bytes
      diff = size - cont;

    //} while((diff > 0) || (ret == 0));
    } while ((diff > 0) && (ret != 0));

    ret = tcp_server_read_data(server_aux->sd, (char *)&req, sizeof(struct st_tcp_server_write_req), msg.id);
    if(ret == -1){
      fprintf(stderr,"(3)ERROR: nfi_tcp_server_write(ID=%s): Error on write operation\n",server_aux->id);
      return -1;
    }

    /*****************************************/
    debug_info("[NFI-TCP] nfi_tcp_server_write(ID=%s): write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);
    if(req.size < 0){
      fprintf(stderr,"ERROR: nfi_tcp_server_write(ID=%s): Fail write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);

      tcp_server_err(TCP_SERVERERR_WRITE);
      return -1;
    }

    debug_info("[NFI-TCP] nfi_tcp_server_write(ID=%s): end\n",server_aux->id);

    return cont;
}


int nfi_tcp_server_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh)
{
  char server[PATH_MAX], dir[PATH_MAX];
  int ret;
  struct nfi_tcp_server_server *server_aux;
  struct nfi_tcp_server_fhandle *fh_aux;
  struct st_tcp_server_msg msg;

  debug_info("[NFI-TCP] nfi_tcp_server_create(ID=%s): begin %s\n",server_aux->id,url);

  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (attr == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  server_aux = (struct nfi_tcp_server_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      /* tcp_server_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL)
  {
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
  }
#endif

  ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0){
    fprintf(stderr,"ERROR: nfi_tcp_server_create: url %s incorrect.\n",url);
    tcp_server_err(TCP_SERVERERR_URL);
    return -1;
  }

  /* private_info file handle */
  fh_aux = (struct nfi_tcp_server_fhandle *)malloc(sizeof(struct nfi_tcp_server_fhandle));
  if (fh_aux == NULL){
    tcp_server_err(TCP_SERVERERR_MEMORY);
    return -1;
  }

  bzero(fh_aux, sizeof(struct nfi_tcp_server_fhandle));
  server_aux = (struct nfi_tcp_server_server *) serv->private_info;
  /* create the file into the directory */

  /*****************************************/
  msg.type = TCP_SERVER_CREAT_FILE;
  strcpy(msg.u_st_tcp_server_msg.op_creat.path,dir);

  tcp_server_write_operation(server_aux->sd, &msg);
  tcp_server_read_data(server_aux->sd, (char *)&(fh_aux->fd), sizeof(int), msg.id);

  strcpy(fh_aux->path, dir);
  /*****************************************/

  fh->type = NFIFILE;
  fh->server = serv;

  fh->priv_fh = (void *)fh_aux;

  fh->url = (char *)malloc(strlen(url)+1);
  if(fh->url == NULL){
     tcp_server_err(TCP_SERVERERR_MEMORY);
     free(fh_aux);
     return -1;
  }

  strcpy(fh->url, url);

  debug_info("[NFI-TCP] nfi_tcp_server_create(ID=%s): end\n",server_aux->id);

  return 0;
}


int nfi_tcp_server_remove(struct nfi_server *serv,  char *url)
{
  char server[PATH_MAX], dir[PATH_MAX];
  int ret;
  struct nfi_tcp_server_server *server_aux;
  struct st_tcp_server_msg msg;

  debug_info("[NFI-TCP] nfi_tcp_server_remove(%s): begin %s\n",server_aux->id, url);

  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  server_aux = (struct nfi_tcp_server_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL)
  {
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      /* tcp_server_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL)
  {
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
  }
#endif

  server_aux = (struct nfi_tcp_server_server *)serv->private_info;

  ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0){
    fprintf(stderr,"nfi_tcp_server_remove: url %s incorrect.\n",url);
    tcp_server_err(TCP_SERVERERR_URL);
    return -1;
  }
  /* remove the file into the directory */
  //ret = unlink(dir);
  //
  /*****************************************/
  msg.type = TCP_SERVER_RM_FILE;
  strcpy(msg.u_st_tcp_server_msg.op_rm.path,dir);

  tcp_server_write_operation(server_aux->sd, &msg);
  /*****************************************/

  debug_info("[NFI-TCP] nfi_tcp_server_remove(ID=%s): end \n",server_aux->id);

  return 0;
}


int nfi_tcp_server_rename(struct nfi_server *server,  char *old_url, char *new_url)
{
  /*
        struct nfi_tcp_server_server *server_aux;
        struct nfi_tcp_server_fhandle *fh_aux;
  if (server == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      tcp_server_err(); not necessary
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcp_server_err(TCP_SERVERERR_PARAM);
               return -1;
        }
#endif
  server_aux = (strcut nfi_tcp_server_server *)serv->private_info;
  */

  /* ????? */
  return 0;
}


int nfi_tcp_server_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
{
  char server[PATH_MAX], dir[PATH_MAX];
  int ret;
  struct nfi_tcp_server_server *server_aux;
  struct nfi_tcp_server_fhandle *fh_aux;
  struct st_tcp_server_msg msg;

  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (attr == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      /* tcp_server_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
  }
#endif

  server_aux = (struct nfi_tcp_server_server *)serv->private_info;

  ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0){
    fprintf(stderr,"nfi_tcp_server_mkdir: url %s incorrect.\n",url);
    tcp_server_err(TCP_SERVERERR_URL);
    return -1;
  }

  /* private_info file handle */
  fh_aux = (struct nfi_tcp_server_fhandle *)malloc(sizeof(struct nfi_tcp_server_fhandle));
  if (fh_aux == NULL){
    tcp_server_err(TCP_SERVERERR_MEMORY);
    return -1;
  }

  bzero(fh_aux, sizeof(struct nfi_tcp_server_fhandle));
  /* create the dir into the directory */

  /******************************************************/
  msg.type = TCP_SERVER_MKDIR_DIR;
  strcpy(msg.u_st_tcp_server_msg.op_mkdir.path, dir);

  tcp_server_write_operation(server_aux->sd, &msg);
  tcp_server_read_data(server_aux->sd, (char *)&(fh_aux->fd), sizeof(int), msg.id);

  strcpy(fh_aux->path, dir);
  /******************************************************/

  if((ret < 0)&&(errno != EEXIST)){
    tcp_server_err(TCP_SERVERERR_MKDIR);
    fprintf(stderr,"nfi_tcp_server_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server);
    free(fh_aux);
    return -1;
  }

  fh->type = NFIDIR;
  fh->priv_fh = (void *)fh_aux;

  fh->url = (char *)malloc(strlen(url)+1);
  if (fh->url == NULL){
      tcp_server_err(TCP_SERVERERR_MEMORY);
      free(fh_aux);
      return -1;
  }
  strcpy(fh->url, url);

  //TODO:
  //TCP_SERVERtoNFIattr(attr, &st);

  return 0;
}


int nfi_tcp_server_rmdir(struct nfi_server *serv,  char *url)
{
  int ret;
  struct nfi_tcp_server_server *server_aux;
  struct st_tcp_server_msg msg;
  char server[PATH_MAX], dir[PATH_MAX];

  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL)
  {
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      /* tcp_server_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL)
  {
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
  }
#endif

  server_aux = (struct nfi_tcp_server_server *)serv->private_info;

  ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0){
    fprintf(stderr,"nfi_tcp_server_rmdir: url %s incorrect.\n",url);
    tcp_server_err(TCP_SERVERERR_URL);
    return -1;
  }


  /******************************************************/
  msg.type = TCP_SERVER_RMDIR_DIR;
  strcpy(msg.u_st_tcp_server_msg.op_rmdir.path, url);

  tcp_server_write_operation(server_aux->sd, &msg);
  tcp_server_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);

  //strcpy(fh_aux->path, dir);
  /******************************************************/


  //ret = rmdir(dir);
  if(ret < 0){
    fprintf(stderr,"nfi_tcp_server_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server);
    tcp_server_err(TCP_SERVERERR_REMOVE);
    return -1;
  }

  return 0;
}


int nfi_tcp_server_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho)
{
  int ret;
  struct nfi_tcp_server_server *server_aux;
  struct nfi_tcp_server_fhandle *fh_aux;
  char dir[PATH_MAX], server[PATH_MAX];


  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (fho == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL)
  {
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      /* tcp_server_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
      tcp_server_err(TCP_SERVERERR_PARAM);
      return -1;
  }
#endif

  ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0){
    fprintf(stderr,"nfi_tcp_server_opendir: url %s incorrect.\n",url);
    tcp_server_err(TCP_SERVERERR_URL);
    return -1;
  }

  fho->url = (char *)malloc(strlen(url)+1);
  if(fho->url == NULL){
    tcp_server_err(TCP_SERVERERR_MEMORY);
    return -1;
  }

  strcpy(fho->url, url);

  fh_aux = (struct nfi_tcp_server_fhandle *)malloc(sizeof(struct nfi_tcp_server_fhandle));
  if (fh_aux == NULL){
    tcp_server_err(TCP_SERVERERR_MEMORY);
    free(fho->url);
    return -1;
  }

  server_aux = (struct nfi_tcp_server_server *) serv->private_info;

  fh_aux->dir = opendir(dir);
  if (ret < 0 ){
      fprintf(stderr,"nfi_tcp_server_opendir: Fail opendir %s in server %s.\n",dir,serv->server);
      tcp_server_err(TCP_SERVERERR_MEMORY);
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


int nfi_tcp_server_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, struct dirent *entry)
{
  struct dirent *ent;
  struct nfi_tcp_server_server *server_aux;
  struct nfi_tcp_server_fhandle *fh_aux;


  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (fh == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (fh->priv_fh == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if(fh->type != NFIDIR){
    tcp_server_err(TCP_SERVERERR_NOTDIR);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
      /* tcp_server_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcp_server_err(TCP_SERVERERR_PARAM);
               return -1;
        }
#endif

  server_aux = (struct nfi_tcp_server_server *)serv->private_info;
  fh_aux = (struct nfi_tcp_server_fhandle *)fh->priv_fh;

  entry->d_name[0] = '\0';
  ent = readdir(fh_aux->dir);

  if(ent == NULL){
    return 1;
  }

  // TODO
  // memcpy(entry, ent, sizeof(struct dirent)) ;
 
  return 0;
}


int nfi_tcp_server_closedir(struct nfi_server *serv,  struct nfi_fhandle *fh)
{
  struct nfi_tcp_server_fhandle *fh_aux;

  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }
  if (fh == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (fh->priv_fh != NULL)
  {
    fh_aux = (struct nfi_tcp_server_fhandle *) fh->priv_fh;
    closedir(fh_aux->dir);
    /* free memory */
    free(fh->priv_fh);
    fh->priv_fh = NULL;
  }

  return 0;
}


int nfi_tcp_server_statfs(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__))  struct nfi_info *inf)
{
/*
  int ret;
  struct nfi_tcp_server_server *server_aux;
  struct tcp_server_info tcp_serverinf;


  if (serv == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }

  if (inf == NULL){
    tcp_server_err(TCP_SERVERERR_PARAM);
    return -1;
  }


#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcp_server_reconnect(serv);
    if(ret <0){
     tcp_server_err(); not necessary
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcp_server_err(TCP_SERVERERR_PARAM);
               return -1;
        }
#endif

  server_aux = (struct nfi_tcp_server_server *)serv->private_info;
  ret = tcp_server_statfs(server_aux->fh, &tcp_serverinf, server_aux->cl);
  if(ret <0){
    tcp_server_err(TCP_SERVERERR_STATFS);
    return -1;
  }

  NFStoNFIInfo(inf, &tcp_serverinf);
*/

  return 0;
}


