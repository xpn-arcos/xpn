#include "nfi_tcpServer.h"

/*
 * ENVIROMENT VARIABLES:
 *
 * XPN_DNS -> node{ID}  {hostname}  {port}
 */


/**********************************
Write the operation to realize
***********************************/
int tcpServer_write_operation(int sd, struct st_tcpServer_msg *head){
        int ret;


#ifdef DBG_NFI 
  printf("[NFI]ID=%s)tcpServer_write_data: begin\n", head->id); 
#endif
        ret = tcpServer_write_data(sd, (char *)&head->type, sizeof(head->type), head->id);
        if(ret == -1){
                return -1;
        }



#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)write_operation: %d -> \n", head->id, head->type);
#endif
        switch(head->type){
case TCPSERVER_OPEN_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)OPEN operation\n",head->id);
#endif
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_open, sizeof(struct st_tcpServer_open), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case TCPSERVER_CREAT_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)CREAT operation\n",head->id);
#endif
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_creat, sizeof(struct st_tcpServer_creat), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case TCPSERVER_READ_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)READ operation\n",head->id);
#endif
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_read, sizeof(struct st_tcpServer_read), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case TCPSERVER_WRITE_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)WRITE operation\n", head->id);
#endif
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_write, sizeof(struct st_tcpServer_write), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case TCPSERVER_CLOSE_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)CLOSE operation\n",head->id);
#endif
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_close, sizeof(struct st_tcpServer_close), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case TCPSERVER_RM_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)RM operation\n",head->id);
#endif
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_rm, sizeof(struct st_tcpServer_rm), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case TCPSERVER_GETATTR_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)GETATTR operation\n",head->id);
#endif
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_getattr, sizeof(struct st_tcpServer_getattr), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case TCPSERVER_MKDIR_DIR:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)MDKIR operation\n",head->id);
#endif
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_mkdir, sizeof(struct st_tcpServer_mkdir), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case TCPSERVER_RMDIR_DIR:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)RMDIR operation\n",head->id);
#endif
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_rmdir, sizeof(struct st_tcpServer_rmdir), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case TCPSERVER_FLUSH_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)FLUSH operation\n",head->id);
#endif
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_flush, sizeof(struct st_tcpServer_flush), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case TCPSERVER_PRELOAD_FILE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)PRELOAD operation\n",head->id);
#endif
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_preload, sizeof(struct st_tcpServer_preload), head->id);
        if(ret == -1){
                return -1;
        }
        break;
case TCPSERVER_FINALIZE:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)FINALIZE operation\n",head->id);
#endif
/*
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_end, sizeof(struct st_tcpServer_end), head->id);
        if(ret == -1){
                return -1;
        }
*/
        break;
case TCPSERVER_GETID:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)GETID operation\n",head->id);
#endif
/*
        ret = tcpServer_write_data(sd, (char *)head->id, TCPSERVER_ID, head->id);
        if(ret == -1){
                return -1;
        }
*/
        break;
case TCPSERVER_END:
#ifdef DBG_NFI_OPS
        printf("[NFI]ID=%s)END operation\n",head->id);
#endif
/*
        ret = tcpServer_write_data(sd, (char *)&head->u_st_tcpServer_msg.op_end, sizeof(struct st_tcpServer_end), head->id);
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
 * PRIVATE FUNCTIONS TO USE tcpServer SERVERS             *
 ************************************************************/
void NFItoTCPSERVERattr(struct stat *att, struct nfi_attr *nfi_att)
{
  if(nfi_att->at_type == NFIFILE){    
    att->st_mode  = nfi_att->at_mode | S_IFREG;     /* protection */  }
  
  if(nfi_att->at_type == NFIDIR){   
    att->st_mode  = nfi_att->at_mode | S_IFDIR;     /* protection */  }
  
  att->st_size  = nfi_att->at_size;      /* size */
  att->st_uid = nfi_att->at_uid;      /* user ID of owner */
  att->st_gid = nfi_att->at_gid;      /* group ID of owner */
  att->st_blksize = nfi_att->at_blksize;  /* blocksize for filesystem I/O */
  att->st_blocks  = nfi_att->at_blocks;   /* number of blocks allocated */
  att->st_atime = nfi_att->at_atime;    /* time of last access */
  att->st_mtime = nfi_att->at_mtime;    /* time of last modification */
  att->st_ctime = nfi_att->at_ctime;    /* time of last change */
}

void TCPSERVERtoNFIattr(struct nfi_attr *nfi_att, struct stat *att)
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

void TCPSERVERtoNFIInfo(struct nfi_info *nfi_inf, struct nfi_info *tcpServer_inf)
{
}

/************************************************************
 * PUBLIC FUNCTIONS TO USE TCPSERVER          *
 ************************************************************/

/************************************************************
 * Init Local                     *
 ************************************************************/
int nfi_tcpServer_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr)
{
  char server[MAXPATHLEN], dir[MAXPATHLEN], prt[MAXPATHLEN];
  
  int ret;
  struct nfi_tcpServer_server *server_aux;
  struct st_tcpServer_msg msg;        


#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_init: begin\n");
#endif  

#ifdef DBG_NFI 
  printf("[NFI]nfi_tcpServer_init: sizeof(struct st_tcpServer_msg) = %d.\n", sizeof(struct st_tcpServer_msg));
#endif  
  
  if(serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  /* functions */
  serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
  if (serv->ops == NULL){
    tcpServer_err(TCPSERVERERR_MEMORY);
    return -1;
  }

  bzero(serv->ops, sizeof(struct nfi_ops));
  
  serv->ops->nfi_preload  = nfi_tcpServer_preload;
  serv->ops->nfi_flush  = nfi_tcpServer_flush;
  
  serv->ops->nfi_reconnect  = nfi_tcpServer_reconnect;
  serv->ops->nfi_disconnect = nfi_tcpServer_disconnect;

  serv->ops->nfi_getattr  = nfi_tcpServer_getattr;
  serv->ops->nfi_setattr  = nfi_tcpServer_setattr;

  serv->ops->nfi_open = nfi_tcpServer_open;
  serv->ops->nfi_close  = nfi_tcpServer_close;

  serv->ops->nfi_read = nfi_tcpServer_read;
  serv->ops->nfi_write  = nfi_tcpServer_write;

  serv->ops->nfi_create = nfi_tcpServer_create;
  serv->ops->nfi_remove = nfi_tcpServer_remove;
  serv->ops->nfi_rename = nfi_tcpServer_rename;

  serv->ops->nfi_mkdir  = nfi_tcpServer_mkdir;
  serv->ops->nfi_rmdir  = nfi_tcpServer_rmdir;
  serv->ops->nfi_opendir  = nfi_tcpServer_opendir;
  serv->ops->nfi_readdir  = nfi_tcpServer_readdir;
  serv->ops->nfi_closedir = nfi_tcpServer_closedir;
  serv->ops->nfi_statfs = nfi_tcpServer_statfs;

  ret = ParseURL(url,  prt, NULL, NULL, server,  NULL,  dir);
  if(ret <0 ){
    tcpServer_err(TCPSERVERERR_URL);
    fprintf(stderr,"nfi_tcpServer_init: url %s incorrect.\n",url);
    free(serv->ops);
    return -1;
  }
  server_aux = (struct nfi_tcpServer_server *)malloc(sizeof(struct nfi_tcpServer_server));
  if(server_aux == NULL){
    tcpServer_err(TCPSERVERERR_MEMORY);
    return -1;
  }

  /******************************************/

  server_aux->sd = tcpServer_connect(server);
  strcpy(msg.id, "GETID");
        msg.type = TCPSERVER_GETID;                                                                                   
        tcpServer_write_operation(server_aux->sd, &msg);                                                                     
#ifdef  DBG_NFI
  printf("[NFI]nfi_tcpServer_init: tcpServer_write_data\n");
#endif
  tcpServer_read_data(server_aux->sd, (char *)server_aux->id, TCPSERVER_ID, msg.id);  
#ifdef  DBG_NFI
  printf("[NFI]nfi_tcpServer_init: tcpServer_read_data id = %s\n",server_aux->id);
#endif
  /******************************************/
  
  serv->private_info = (void *)server_aux;

  //serv->protocol = TCPSERVER;
  
  serv->server = (char *)malloc(sizeof(char)*(strlen(server)+1)); /* server address */
  if(serv->server == NULL){
    tcpServer_err(TCPSERVERERR_MEMORY);
    return -1;
  }
  strcpy(serv->server, server);

  serv->url = (char *)malloc(sizeof(char)*(strlen(url)+1)); /* server address */
  if(serv->url == NULL){
    tcpServer_err(TCPSERVERERR_MEMORY);
    return -1;
  }

  strcpy(serv->url, url);
        serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
  memset(serv->wrk, 0, sizeof(struct nfi_worker));


#ifdef _ALL_THREADS_
//  nfi_worker_init(serv->wrk, serv, 1);
//  nfi_worker_init(serv->wrk, serv, 0);

#else
  if(strcmp("tcpServer", prt) == 0){
#if defined(DEBUG_NFI)
    printf("[NFI]tcpServer\n");
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
  printf("[NFI]nfi_tcpServer_init(ID=%s): end\n",server_aux->id);
#endif
  return 0;
}

/************************************************************
 * Disconnect to the server           *
 * **********************************************************/
int nfi_tcpServer_disconnect(struct nfi_server *serv)
{

        struct nfi_tcpServer_server *server_aux;
        

  if (serv == NULL)
         return 0;


        server_aux = (struct nfi_tcpServer_server *)serv->private_info;
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
 * Reconnect to the NFS server            *
 ************************************************************/
int nfi_tcpServer_reconnect(struct nfi_server *serv)
{
  /* Don't see the serv result */
  char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
  int ret;
  struct nfi_tcpServer_server *server_aux;

  ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
  if(ret <0 ){
    tcpServer_err(TCPSERVERERR_URL);
    fprintf(stderr,"nfi_tcpServer_reconnect: url %s incorrect.\n",serv->url);
    return -1;
  }

  server_aux = (struct nfi_tcpServer_server *)malloc(sizeof(struct nfi_tcpServer_server));
  if(server_aux == NULL){   
    tcpServer_err(TCPSERVERERR_MEMORY);
    return -1;
  }

  strcpy(server_aux->path, dir);

  serv->private_info = (void *)server_aux;
  return 0;
}

/************************************************************
 * Destroy NFS operations           *
 ************************************************************/

int nfi_tcpServer_destroy(struct nfi_server *serv)
{

  struct nfi_tcpServer_server *server_aux;
  struct st_tcpServer_msg msg;

  if (serv == NULL)
    return 0;

  if(serv->ops != NULL)
    free(serv->ops);

  server_aux = (struct nfi_tcpServer_server *)serv->private_info;

  if(server_aux != NULL){
    //printf("[NFI]Cierro la conexion\n");
    msg.type = -1;
          tcpServer_write_operation(server_aux->sd, &msg);
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
 * GENERIC FUNCTIONS              *
 ************************************************************/

int nfi_tcpServer_getattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){


  struct nfi_tcpServer_server *server_aux;
  struct nfi_tcpServer_fhandle *fh_aux;
  struct st_tcpServer_msg msg;
  struct st_tcpServer_attr_req req;

  server_aux = (struct nfi_tcpServer_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_getattr(ID=%s): begin\n",server_aux->id);
#endif  
  if (attr == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }


  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }


  if (fh == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if (fh->priv_fh == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      /* tcpServer_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif
  fh_aux = (struct nfi_tcpServer_fhandle *) fh->priv_fh;
  server_aux = (struct nfi_tcpServer_server *) serv->private_info;

  
  
  /*****************************************/

  msg.type = TCPSERVER_GETATTR_FILE; 
  strcpy(msg.u_st_tcpServer_msg.op_getattr.path, fh_aux->path);

        tcpServer_write_operation(server_aux->sd, &msg);
#ifdef DBG_NFI 
  printf("[NFI]nfi_tcpServer_getattr(ID=%s): getattr: -> %s \n", server_aux->id, msg.u_st_tcpServer_msg.op_getattr.path);
#endif
  bzero(&req, sizeof(struct st_tcpServer_attr_req));
  tcpServer_read_data(server_aux->sd, (char *)&req, sizeof(struct st_tcpServer_attr_req), msg.id);
#ifdef DBG_NFI 
  printf("[NFI]nfi_tcpServer_getattr(ID=%s): getattr: <- %d\n", server_aux->id, req.status);
#endif
  /*****************************************/ 
  
  /*
  ret = stat(fh_aux->path, &st);
  if(ret < 0){
    fprintf(stderr,"nfi_tcpServer_getattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
    tcpServer_err(TCPSERVERERR_GETATTR);
    return -1;
  }
  */

  TCPSERVERtoNFIattr(attr, &req.attr);
  
#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_getattr(ID=%s): end\n",server_aux->id);
#endif
  return req.status;
}


int nfi_tcpServer_setattr(struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr){

  struct nfi_tcpServer_server *server_aux;
  struct nfi_tcpServer_fhandle *fh_aux;


  if (attr == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if (fh->priv_fh == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      /* tcpServer_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif


  fh_aux = (struct nfi_tcpServer_fhandle *) fh->priv_fh;
  server_aux = (struct nfi_tcpServer_server *) serv->private_info;

 // NFItoTCPSERVERattr(&st, attr);
  /* no se comp hacer el setattr */
  /*
  ret = stat(fh_aux->path, &st);
  */
/*
  ret = truncate(fh_aux->path, st.st_tcpServer_size);
  if(ret < 0){
    fprintf(stderr,"nfi_tcpServer_setattr: Fail stat %s in server %s.\n",fh_aux->path,serv->server);
    tcpServer_err(TCPSERVERERR_GETATTR);
    return -1;
  }
*/  
  return 0;
}


int nfi_tcpServer_preload(struct nfi_server *serv, char *url, char *virtual_path, char *storage_path, int opt)
{
  //char dir[NFIMAXPATHLEN];
  int ret;
  struct nfi_tcpServer_server *server_aux;
  struct st_tcpServer_msg msg;
  
  server_aux = (struct nfi_tcpServer_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_preload(ID=%s): begin %s - %s \n", server_aux->id, virtual_path, storage_path);
#endif
  if(url == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if(url[strlen(url)-1] == '/'){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }
  
  if (virtual_path == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if (storage_path == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }
#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      /* tcpServer_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif

  server_aux = (struct nfi_tcpServer_server *) serv->private_info;
#ifdef DBG_NFI 
  printf("[NFI]nfi_tcpServer_preload(ID=%s): preload %s in server %s.\n",server_aux->id,virtual_path,serv->server);
#endif
  
  /*****************************************/
  msg.type = TCPSERVER_PRELOAD_FILE; 
  //strcpy(msg.u_st_tcpServer_msg.op_preload.path,dir);
  strcpy(msg.u_st_tcpServer_msg.op_preload.virtual_path,virtual_path);
  strcpy(msg.u_st_tcpServer_msg.op_preload.storage_path,storage_path);
  msg.u_st_tcpServer_msg.op_preload.opt = opt;

        tcpServer_write_operation(server_aux->sd, &msg);
#ifdef DBG_NFI 
  printf("[NFI]nfi_tcpServer_preload(ID=%s): preload: -> %s \n",server_aux->id,msg.u_st_tcpServer_msg.op_preload.virtual_path);
#endif
  tcpServer_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
#ifdef DBG_NFI 
  printf("[NFI]nfi_tcpServer_preload(ID=%s): preload: <- %d \n",server_aux->id,ret);
#endif
  /*****************************************/
  
#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_preload(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret);
#endif
  if(ret == -1){
    printf("[NFI]Error en el preload\n");
  }

  return ret;
}


int nfi_tcpServer_flush(struct nfi_server *serv,  char *url, char *virtual_path, char *storage_path, int opt)
{
  //char dir[NFIMAXPATHLEN];
  int ret;
  struct nfi_tcpServer_server *server_aux;

  struct st_tcpServer_msg msg;
  
  server_aux = (struct nfi_tcpServer_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_flush(ID=%s): begin %s - %s \n", server_aux->id,virtual_path, storage_path);
#endif
  if(url == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if(url[strlen(url)-1] == '/'){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }
  
  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }
  
  if (virtual_path == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if (storage_path == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }
#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      /* tcpServer_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif

  server_aux = (struct nfi_tcpServer_server *) serv->private_info;
#ifdef DBG_NFI 
  printf("[NFI]nfi_tcpServer_flush(ID=%s): open %s in server %s.\n",server_aux->id,virtual_path,serv->server);
#endif
  
  /*****************************************/
  msg.type = TCPSERVER_FLUSH_FILE; 
  //strcpy(msg.u_st_tcpServer_msg.op_flush.path,dir);
  strcpy(msg.u_st_tcpServer_msg.op_flush.virtual_path,virtual_path);
  strcpy(msg.u_st_tcpServer_msg.op_flush.storage_path,storage_path);
  msg.u_st_tcpServer_msg.op_flush.opt = opt;

        tcpServer_write_operation(server_aux->sd, &msg);
#ifdef DBG_NFI 
  printf("[NFI]nfi_tcpServer_flush(ID=%s): flush: -> %s \n",server_aux->id,msg.u_st_tcpServer_msg.op_flush.virtual_path);
#endif
  tcpServer_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
#ifdef DBG_NFI 
  printf("[NFI]nfi_tcpServer_flush(ID=%s): flush: <- %d \n",server_aux->id,ret);
#endif
  /*****************************************/
  
#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_flush(ID=%s): end %s - %s = %d\n", server_aux->id,virtual_path, storage_path, ret);
#endif
  return 0;
}



int nfi_tcpServer_open(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho )
{
  char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
  int ret;
  struct nfi_tcpServer_server *server_aux;
  struct nfi_tcpServer_fhandle *fh_aux;

  struct st_tcpServer_msg msg;
  
  server_aux = (struct nfi_tcpServer_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_open(ID=%s): begin %s\n",server_aux->id,url);
#endif
  if(url[strlen(url)-1] == '/'){
    return nfi_tcpServer_opendir(serv, url, fho);
  }
  
  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }
  
  if (fho == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }
#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      /* tcpServer_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif

  ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0){
    fprintf(stderr,"nfi_tcpServer_open: url %s incorrect.\n",url);
    tcpServer_err(TCPSERVERERR_URL);
    return -1;
  }
  fho->url = (char *)malloc(strlen(url)+1);
  if(fho->url == NULL){
    tcpServer_err(TCPSERVERERR_MEMORY);
    return -1;
  }

  strcpy(fho->url, url);

  fh_aux = (struct nfi_tcpServer_fhandle *)malloc(sizeof(struct nfi_tcpServer_fhandle));
  if (fh_aux == NULL){    
    tcpServer_err(TCPSERVERERR_MEMORY);
    free(fho->url);
    return -1;
  }

  server_aux = (struct nfi_tcpServer_server *) serv->private_info;
  
  /*****************************************/
  msg.type = TCPSERVER_OPEN_FILE; 
  strcpy(msg.u_st_tcpServer_msg.op_open.path,dir);
  
        tcpServer_write_operation(server_aux->sd, &msg);
#ifdef DBG_NFI 
  printf("[NFI]nfi_tcpServer_open(ID=%s): open -> %s \n",server_aux->id,msg.u_st_tcpServer_msg.op_open.path);
#endif
  tcpServer_read_data(server_aux->sd, (char *)&fh_aux->fd, sizeof(int), msg.id);
#ifdef DBG_NFI 
  printf("[NFI]nfi_tcpServer_open(ID=%s): open <- %d \n",server_aux->id,fh_aux->fd);
#endif
  strcpy(fh_aux->path, dir);
  /*****************************************/
  
  fho->type = NFIFILE;      
  
  fho->server = NULL;
  fho->priv_fh = NULL;
  fho->server = serv;
  fho->priv_fh = (void *) fh_aux;

#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_open(ID=%s): end\n",server_aux->id);
#endif
  return 0;

}





int nfi_tcpServer_close(struct nfi_server *server,  struct nfi_fhandle *fh){
  
  struct nfi_tcpServer_fhandle *fh_aux;
  struct nfi_tcpServer_server *server_aux;
  struct st_tcpServer_msg msg;

  server_aux = (struct nfi_tcpServer_server *) server->private_info;
  strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_close(ID=%s): begin\n",server_aux->id);
#endif
  if (server == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }
  if (fh == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }
  
  if (fh->priv_fh != NULL){
    fh_aux = (struct nfi_tcpServer_fhandle *) fh->priv_fh;
    server_aux = (struct nfi_tcpServer_server *) server->private_info;
    /*****************************************/
    msg.type = TCPSERVER_CLOSE_FILE; 
    msg.u_st_tcpServer_msg.op_close.fd = fh_aux->fd;
  
          tcpServer_write_operation(server_aux->sd, &msg);
      
#ifdef  DBG_NFI 
    printf("[NFI]nfi_tcpServer_close(ID=%s): close -> %d \n",server_aux->id,msg.u_st_tcpServer_msg.op_close.fd);
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
  printf("[NFI]nfi_tcpServer_close(ID=%s): end\n",server_aux->id);
#endif

  return 0;
}

ssize_t nfi_tcpServer_read( struct nfi_server *serv,
      struct nfi_fhandle *fh,
      void *buffer,
      off_t offset,
      size_t size)
      //off_t offset)
{

  int ret, cont, diff;
  struct nfi_tcpServer_server *server_aux;
  struct nfi_tcpServer_fhandle *fh_aux;
  struct st_tcpServer_msg msg;
  struct st_tcpServer_read_req req;

  server_aux = (struct nfi_tcpServer_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);

#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_read(%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size);
#endif
  
  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }


  if (fh == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }


#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      /* tcpServer_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif


  fh_aux = (struct nfi_tcpServer_fhandle *) fh->priv_fh;
  server_aux = (struct nfi_tcpServer_server *) serv->private_info;
  
  /*****************************************/
    
  msg.type = TCPSERVER_READ_FILE; 
  msg.u_st_tcpServer_msg.op_read.fd       = fh_aux->fd;
  msg.u_st_tcpServer_msg.op_read.offset   = offset;
  msg.u_st_tcpServer_msg.op_read.size   = size;

  #ifdef  DBG_IO
    printf("[NFI]read: -> fd %d \n",msg.u_st_tcpServer_msg.op_read.fd);
    printf("[NFI]read: -> offset %d \n",(int)msg.u_st_tcpServer_msg.op_read.offset);
    printf("[NFI]read: -> size %d \n",msg.u_st_tcpServer_msg.op_read.size);
  #endif  

  ret = tcpServer_write_operation(server_aux->sd, &msg);
  if(ret == -1){
    perror("ERROR: (1)nfi_tcpServer_read: Error on write operation");
    fprintf(stderr,"ERROR: (1)nfi_tcpServer_read: Error on write operation\n");
    return -1;
  }

  cont = 0;

  do{

    ret = tcpServer_read_data(server_aux->sd, (char *)&req, sizeof(struct st_tcpServer_read_req), msg.id);  
    if(ret == -1){
      perror("ERROR: (2)nfi_tcpServer_read: Error on write operation");
      fprintf(stderr,"ERROR: (2)nfi_tcpServer_read: Error on write operation\n");
      return -1;
    }

    if(req.size > 0){
      ret = tcpServer_read_data(server_aux->sd, (char *)buffer+cont, req.size, msg.id);
      if(ret == -1){
        perror("ERROR: (3)nfi_tcpServer_read: Error on write operation");
        fprintf(stderr,"ERROR: (3)nfi_tcpServer_read: Error on read operation\n");
        return -1;
      }
    }

    cont = cont + req.size ;
        diff = msg.u_st_tcpServer_msg.op_read.size - cont;

  }while((diff > 0) || (req.size == 0));

  if(req.size < 0){
    fprintf(stderr,"ERROR: nfi_tcpServer_read: Fail read %s off %d size %d (err:%d).\n",fh->url,(int)offset,(int)size,(int)req.size);
    tcpServer_err(TCPSERVERERR_READ);
    return -1;
  }

  return cont;
}



  ssize_t nfi_tcpServer_write(struct nfi_server *serv,
        struct nfi_fhandle *fh,
        void *buffer,
        off_t offset,
        size_t size)
        //off_t offset)
  {

    struct nfi_tcpServer_server *server_aux;
    struct nfi_tcpServer_fhandle *fh_aux;
    struct st_tcpServer_msg msg;
    struct st_tcpServer_write_req req;
    int ret, diff, cont;

    
    server_aux = (struct nfi_tcpServer_server *) serv->private_info;
    strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI
    printf("[NFI]nfi_tcpServer_write(ID=%s): begin off %d size %d\n",server_aux->id,(int)offset, (int)size);
#endif
    if(size == 0){
      return 0;
    } 

    if (serv == NULL){
      tcpServer_err(TCPSERVERERR_PARAM);
      return -1;
    }


    if (fh == NULL){
      tcpServer_err(TCPSERVERERR_PARAM);
      return -1;
    }

#ifdef NFI_DYNAMIC
    if (serv->private_info == NULL){
      ret = nfi_tcpServer_reconnect(serv);
      if(ret <0){
        /* tcpServer_err(); not necessary */
        return -1;
      }
    }
#else
    if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif


      fh_aux = (struct nfi_tcpServer_fhandle *) fh->priv_fh;
    server_aux = (struct nfi_tcpServer_server *) serv->private_info;

    /*****************************************/
    msg.type = TCPSERVER_WRITE_FILE; 
    msg.u_st_tcpServer_msg.op_write.fd  = fh_aux->fd;
    msg.u_st_tcpServer_msg.op_write.offset  = offset;
    msg.u_st_tcpServer_msg.op_write.size  = size;

    #ifdef  DBG_IO
      printf("[NFI]write: -> fd %d \n",msg.u_st_tcpServer_msg.op_write.fd);
      printf("[NFI]write: -> offset %d \n",(int)msg.u_st_tcpServer_msg.op_write.offset);
      printf("[NFI]write: -> size %d \n",msg.u_st_tcpServer_msg.op_write.size);
    #endif  

    ret = tcpServer_write_operation(server_aux->sd, &msg);
    if(ret == -1){
      fprintf(stderr,"(1)ERROR: nfi_tcpServer_write(ID=%s): Error on write operation\n",server_aux->id);
      return -1;
    }


    diff = size;
        cont = 0;

        int buffer_size = size;

        if (buffer_size > (1 * 1024 * 1024)) // > 1MB
        {
            buffer_size = 1 * 1024 * 1024; // 1MB
        }

        do{

          if (diff > buffer_size)
            {
              ret = tcpServer_write_data(server_aux->sd, (char *)buffer + cont, buffer_size, msg.id);
        if(ret == -1){
          fprintf(stderr,"(2)ERROR: nfi_tcpServer_read(ID=%s): Error on write operation\n",server_aux->id);
          return -1;
        }
            }
            else{
              ret = tcpServer_write_data(server_aux->sd, (char *)buffer + cont, diff, msg.id);
        if(ret == -1){
          fprintf(stderr,"(2)ERROR: nfi_tcpServer_read(ID=%s): Error on write operation\n",server_aux->id);
          return -1;
        }
            }

      cont = cont + ret; //Send bytes
            diff = size - cont;

        } while((diff > 0) || (ret == 0));

    ret = tcpServer_read_data(server_aux->sd, (char *)&req, sizeof(struct st_tcpServer_write_req), msg.id);
    if(ret == -1){
      fprintf(stderr,"(3)ERROR: nfi_tcpServer_write(ID=%s): Error on write operation\n",server_aux->id);
      return -1;
    }
  
  
  /*****************************************/
#ifdef  DBG_NFI 
    printf("[NFI]nfi_tcpServer_write(ID=%s): write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);
#endif
    if(req.size < 0){
      fprintf(stderr,"ERROR: nfi_tcpServer_write(ID=%s): Fail write %s off %d size %d (err:%d).\n",server_aux->id,fh->url,(int)offset,(int)size,(int)req.size);
      
      tcpServer_err(TCPSERVERERR_WRITE);
      return -1;
    }

#ifdef  DBG_NFI
    printf("[NFI]nfi_tcpServer_write(ID=%s): end\n",server_aux->id);
#endif
  return cont;
}

int nfi_tcpServer_create(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle  *fh){

  char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
  int ret;
  struct nfi_tcpServer_server *server_aux;
        struct nfi_tcpServer_fhandle *fh_aux;
  struct st_tcpServer_msg msg;

  server_aux = (struct nfi_tcpServer_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_create(ID=%s): begin %s\n",server_aux->id,url);
#endif
  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if (attr == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      /* tcpServer_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif
  ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0){
    fprintf(stderr,"ERROR: nfi_tcpServer_create: url %s incorrect.\n",url);
    tcpServer_err(TCPSERVERERR_URL);
    return -1;
  }
  /* private_info file handle */
  fh_aux = (struct nfi_tcpServer_fhandle *)malloc(sizeof(struct nfi_tcpServer_fhandle));
  if (fh_aux == NULL){
    tcpServer_err(TCPSERVERERR_MEMORY);
    return -1;
  }

  bzero(fh_aux, sizeof(struct nfi_tcpServer_fhandle));
  

      server_aux = (struct nfi_tcpServer_server *) serv->private_info;
  /* create the file into the directory */

  /*****************************************/
  msg.type = TCPSERVER_CREAT_FILE; 
  strcpy(msg.u_st_tcpServer_msg.op_creat.path,dir);
  
      tcpServer_write_operation(server_aux->sd, &msg);
  tcpServer_read_data(server_aux->sd, (char *)&(fh_aux->fd), sizeof(int), msg.id);
  
  strcpy(fh_aux->path, dir);
  /*****************************************/
  
  fh->type = NFIFILE;     
  
  fh->server = serv;
  
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
                tcpServer_err(TCPSERVERERR_MEMORY);
    free(fh_aux);
                return -1;
        }
    
        strcpy(fh->url, url);

#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_create(ID=%s): end\n",server_aux->id);
#endif
  return 0;
}

int nfi_tcpServer_remove(struct nfi_server *serv,  char *url)
{

  char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
  int ret;
  struct nfi_tcpServer_server *server_aux;
  struct st_tcpServer_msg msg;
  
  server_aux = (struct nfi_tcpServer_server *) serv->private_info;
  strcpy(msg.id,server_aux->id);
#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_remove(%s): begin %s\n",server_aux->id, url);
#endif  
  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      /* tcpServer_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif


  server_aux = (struct nfi_tcpServer_server *)serv->private_info;

  ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0){
    fprintf(stderr,"nfi_tcpServer_remove: url %s incorrect.\n",url);
    tcpServer_err(TCPSERVERERR_URL);
    return -1;
  }
  /* remove the file into the directory */
  //ret = unlink(dir);
  /*****************************************/
  msg.type = TCPSERVER_RM_FILE; 
  strcpy(msg.u_st_tcpServer_msg.op_rm.path,dir);
  
        tcpServer_write_operation(server_aux->sd, &msg);
  
  /*****************************************/
#ifdef  DBG_NFI 
  printf("[NFI]nfi_tcpServer_remove(ID=%s): end \n",server_aux->id);
#endif
  return 0;
}

int nfi_tcpServer_rename(struct nfi_server *server,  char *old_url, char *new_url)
{
  /*
        struct nfi_tcpServer_server *server_aux;
        struct nfi_tcpServer_fhandle *fh_aux;
  if (server == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      tcpServer_err(); not necessary
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif
  server_aux = (strcut nfi_tcpServer_server *)serv->private_info;
  */

  /* ????? */
  return 0;
}

int nfi_tcpServer_mkdir(struct nfi_server *serv,  char *url, struct nfi_attr *attr, struct nfi_fhandle *fh)
{
  char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
  int ret;
        struct nfi_tcpServer_server *server_aux;
        struct nfi_tcpServer_fhandle *fh_aux;
  struct st_tcpServer_msg msg;

  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if (attr == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      /* tcpServer_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif

  server_aux = (struct nfi_tcpServer_server *)serv->private_info;

  ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0){
    fprintf(stderr,"nfi_tcpServer_mkdir: url %s incorrect.\n",url);
    tcpServer_err(TCPSERVERERR_URL);
    return -1;
  }

  /* private_info file handle */
  fh_aux = (struct nfi_tcpServer_fhandle *)malloc(sizeof(struct nfi_tcpServer_fhandle));
  if (fh_aux == NULL){
    tcpServer_err(TCPSERVERERR_MEMORY);
    return -1;
  }


  bzero(fh_aux, sizeof(struct nfi_tcpServer_fhandle));
  /* create the dir into the directory */

  /******************************************************/  
  msg.type = TCPSERVER_MKDIR_DIR; 
  strcpy(msg.u_st_tcpServer_msg.op_mkdir.path, dir);
  
      tcpServer_write_operation(server_aux->sd, &msg);
  tcpServer_read_data(server_aux->sd, (char *)&(fh_aux->fd), sizeof(int), msg.id);
  
  strcpy(fh_aux->path, dir);
  /******************************************************/

  if((ret < 0)&&(errno != EEXIST)){
    tcpServer_err(TCPSERVERERR_MKDIR);
    fprintf(stderr,"nfi_tcpServer_mkdir: Fail mkdir %s in server %s.\n",dir,serv->server);
    free(fh_aux);
    return -1;
  }
  
  fh->type = NFIDIR;
        fh->priv_fh = (void *)fh_aux;

        fh->url = (char *)malloc(strlen(url)+1);
        if(fh->url == NULL){
               tcpServer_err(TCPSERVERERR_MEMORY);
         free(fh_aux);
               return -1;
        }
        strcpy(fh->url, url);

  //TODO:
  //TCPSERVERtoNFIattr(attr, &st);
  
  return 0;
}

int nfi_tcpServer_rmdir(struct nfi_server *serv,  char *url)
{
  char server[NFIMAXPATHLEN], dir[NFIMAXPATHLEN];
  int ret;

        struct nfi_tcpServer_server *server_aux;
  struct st_tcpServer_msg msg;

  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }


#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      /* tcpServer_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif


  server_aux = (struct nfi_tcpServer_server *)serv->private_info;

  ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0){
    fprintf(stderr,"nfi_tcpServer_rmdir: url %s incorrect.\n",url);
    tcpServer_err(TCPSERVERERR_URL);
    return -1;
  }


  /******************************************************/  
  msg.type = TCPSERVER_RMDIR_DIR; 
  strcpy(msg.u_st_tcpServer_msg.op_rmdir.path, url);
  
      tcpServer_write_operation(server_aux->sd, &msg);
  tcpServer_read_data(server_aux->sd, (char *)&ret, sizeof(int), msg.id);
  
  //strcpy(fh_aux->path, dir);
  /******************************************************/

  
  //ret = rmdir(dir); 
  if(ret < 0){
    fprintf(stderr,"nfi_tcpServer_rmdir: Fail rmdir %s in server %s.\n",dir,serv->server);
    tcpServer_err(TCPSERVERERR_REMOVE);
    return -1;
  }

  return 0;
}

int nfi_tcpServer_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho)
{
        
  char dir[NFIMAXPATHLEN], server[NFIMAXPATHLEN];
  int ret;  
  struct nfi_tcpServer_server *server_aux;
  struct nfi_tcpServer_fhandle *fh_aux;


  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if (fho == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      /* tcpServer_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif

  ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0){
    fprintf(stderr,"nfi_tcpServer_opendir: url %s incorrect.\n",url);
    tcpServer_err(TCPSERVERERR_URL);
    return -1;
  }

  fho->url = (char *)malloc(strlen(url)+1);
  if(fho->url == NULL){
    tcpServer_err(TCPSERVERERR_MEMORY);
    return -1;
  }

  strcpy(fho->url, url);

  fh_aux = (struct nfi_tcpServer_fhandle *)malloc(sizeof(struct nfi_tcpServer_fhandle));
  if (fh_aux == NULL){
    tcpServer_err(TCPSERVERERR_MEMORY);
    free(fho->url);
    return -1;
  }

  server_aux = (struct nfi_tcpServer_server *) serv->private_info;


  fh_aux->dir = opendir(dir);
    if (ret < 0 ){
    fprintf(stderr,"nfi_tcpServer_opendir: Fail opendir %s in server %s.\n",dir,serv->server);
      tcpServer_err(TCPSERVERERR_MEMORY);   
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

int nfi_tcpServer_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, char *entry, unsigned char *type)
{
  struct dirent *ent;

        struct nfi_tcpServer_server *server_aux;
        struct nfi_tcpServer_fhandle *fh_aux;


  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }


  if (fh == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if (fh->priv_fh == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if(fh->type != NFIDIR){
    tcpServer_err(TCPSERVERERR_NOTDIR);
    return -1;
  }

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
      /* tcpServer_err(); not necessary */
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif

  server_aux = (struct nfi_tcpServer_server *)serv->private_info;
  fh_aux = (struct nfi_tcpServer_fhandle *)fh->priv_fh;

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
//  *type = ent->d_type;

  return 0;
}

int nfi_tcpServer_closedir(struct nfi_server *serv,  struct nfi_fhandle *fh)
{
  
  struct nfi_tcpServer_fhandle *fh_aux;

  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }
  if (fh == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if (fh->priv_fh != NULL){
    fh_aux = (struct nfi_tcpServer_fhandle *) fh->priv_fh;
    closedir(fh_aux->dir);
    /* free memory */
    free(fh->priv_fh);
    fh->priv_fh = NULL;

  }
  
  return 0;
  
}


int nfi_tcpServer_statfs(struct nfi_server *serv,  struct nfi_info *inf)
{/*
  
  struct tcpServer_info tcpServerinf;
  int ret;
        struct nfi_tcpServer_server *server_aux;


  if (serv == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }

  if (inf == NULL){
    tcpServer_err(TCPSERVERERR_PARAM);
    return -1;
  }


#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL){
    ret = nfi_tcpServer_reconnect(serv);
    if(ret <0){
     tcpServer_err(); not necessary 
      return -1;
    }
  }
#else
  if (serv->private_info == NULL){
               tcpServer_err(TCPSERVERERR_PARAM);
               return -1;
        }
#endif
  
  server_aux = (struct nfi_tcpServer_server *)serv->private_info;
  ret = tcpServer_statfs(server_aux->fh, &tcpServerinf, server_aux->cl);
  if(ret <0){
    tcpServer_err(TCPSERVERERR_STATFS);
    return -1;
  }

  NFStoNFIInfo(inf, &tcpServerinf);
*/  
  return 0;
}
