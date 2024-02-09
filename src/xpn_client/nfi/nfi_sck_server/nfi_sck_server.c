
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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

#include "nfi_sck_server.h"


/* ... Const / Const ................................................. */

#define FILESYSTEM_DLSYM 1


/* ... Global variables / Variables globales ........................ */


/* ... Auxiliar Functions / Funciones Auxiliares ..................... */

//Communication
int sck_server_write_operation ( int sd, struct st_sck_server_msg * head )
{
  int ret;

  debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] >> Begin\n", head->id);

  debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] Send operation\n", head->id);

  ret = sck_client_write_operation(sd, (char *)&(head->type), 1, head->id);
  if (ret < 0)
  {
    printf("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] ERROR: sck_client_write_operation fails\n", head->id);
    return -1;
  }

  debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] Execute operation: %d -> \n", head->id, head->type);

  switch (head->type)
  {
    //File API
    case SCK_SERVER_OPEN_FILE_WS:
    case SCK_SERVER_OPEN_FILE_WOS:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] OPEN operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_open), sizeof(struct st_sck_server_open), head->id);
      break;
    case SCK_SERVER_CREAT_FILE_WS:
    case SCK_SERVER_CREAT_FILE_WOS:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] CREAT operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_creat), sizeof(struct st_sck_server_creat), head->id);
      break;
    case SCK_SERVER_READ_FILE_WS:
    case SCK_SERVER_READ_FILE_WOS:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] READ operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_read), sizeof(struct st_sck_server_read), head->id);
      break;
    case SCK_SERVER_WRITE_FILE_WS:
    case SCK_SERVER_WRITE_FILE_WOS:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] WRITE operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_write), sizeof(struct st_sck_server_write), head->id);
      break;
    case SCK_SERVER_CLOSE_FILE_WS:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] CLOSE operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_close), sizeof(struct st_sck_server_close), head->id);
      break;
    case SCK_SERVER_RM_FILE:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] RM operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_rm), sizeof(struct st_sck_server_rm), head->id);
      break;
    case SCK_SERVER_RM_FILE_ASYNC:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] RM_ASYNC operation\n", head->id);
      ret = sck_client_write_data(sd, (char *)&(head->u_st_sck_server_msg.op_rm), sizeof(struct st_sck_server_rm), head->id);
      break;
    case SCK_SERVER_RENAME_FILE:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] RENAME operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_rename), sizeof(struct st_sck_server_rename), head->id);
      break;
    case SCK_SERVER_GETATTR_FILE:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] GETATTR operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_getattr), sizeof(struct st_sck_server_getattr), head->id);
      break;

    //Directory API
    case SCK_SERVER_MKDIR_DIR:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] MDKIR operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_mkdir), sizeof(struct st_sck_server_mkdir), head->id);
      break;
    case SCK_SERVER_OPENDIR_DIR:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] OPENDIR operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_opendir), sizeof(struct st_sck_server_opendir), head->id);
      break;
    case SCK_SERVER_READDIR_DIR:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] READDIR operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_readdir), sizeof(struct st_sck_server_readdir), head->id);
      break;
    case SCK_SERVER_CLOSEDIR_DIR:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] CLOSEDIR operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_closedir), sizeof(struct st_sck_server_closedir), head->id);
      break;
    case SCK_SERVER_RMDIR_DIR:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] RMDIR operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_rmdir), sizeof(struct st_sck_server_rmdir), head->id);
      break;
    case SCK_SERVER_RMDIR_DIR_ASYNC:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] RMDIR_ASYNC operation\n", head->id);
      ret = sck_client_write_data(sd, (char *)&(head->u_st_sck_server_msg.op_rmdir), sizeof(struct st_sck_server_rmdir), head->id);
      break;

    //Optimization API
    case SCK_SERVER_FLUSH_FILE:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] FLUSH operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_flush), sizeof(struct st_sck_server_flush), head->id);
      break;
    case SCK_SERVER_PRELOAD_FILE:
      debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] PRELOAD operation\n", head->id);
      ret = sck_client_write_data(sd, (char * )&(head->u_st_sck_server_msg.op_preload), sizeof(struct st_sck_server_preload), head->id);
      break;
  }

  debug_info("[SERV_ID=%s] [NFI_SCK] [sck_server_write_operation] >> End\n", head->id);

  return ret;
}

int nfi_sck_server_do_request ( struct nfi_sck_server_server * server_aux, struct st_sck_server_msg * msg, char * req, int req_size )
{
  ssize_t ret;

  debug_info("[SERV_ID=%s] [NFI_SCK] [nfi_sck_server_do_request] >> Begin\n", server_aux->id);

  // send request...
  debug_info("[SERV_ID=%s] [NFI_SCK] [nfi_sck_server_do_request] Send operation: %s\n", server_aux->id, msg->id);

  ret = sck_server_write_operation(server_aux->params.server, msg);
  if (ret < 0) {
    return -1;
  }

  // read response...
  debug_info("[SERV_ID=%s] [NFI_SCK] [nfi_sck_server_do_request] Response operation: %s\n", server_aux->id, msg->id);

  ret = sck_client_read_data(server_aux->params.server, req, req_size, msg->id);
  if (ret < 0) {
    return -1;
  }

  debug_info("[SERV_ID=%s] [NFI_SCK] [nfi_sck_server_do_request] >> End\n", server_aux->id);

  return 0;
}

int nfi_sck_server_keepConnected ( struct nfi_server * serv )
{
  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_sck_server_keep_connected] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_keep_connected] >> Begin\n", serv->id);

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL)
  {
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_keep_connected] Server reconnect\n", server_aux->id);

    int ret = nfi_sck_server_reconnect(serv);
    if (ret < 0)
    {
      serv->private_info = NULL;
      return -1;
    }
  }
#endif

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_keep_connected] >> End\n", serv->id);

  return (serv->private_info != NULL);
}

void nfi_2_sck_server_attr ( struct stat * att, struct nfi_attr * nfi_att )
{
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_2_sck_server_attr] >> Begin\n", -1);

  att->st_dev = nfi_att->st_dev;
  att->st_ino = nfi_att->st_ino;

  if (nfi_att->at_type == NFIFILE) {
    att->st_mode = nfi_att->at_mode | S_IFREG; // protection
  }

  if (nfi_att->at_type == NFIDIR) {
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

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_2_sck_server_attr] >> End\n", -1);
}

void sck_server_2_nfi_attr ( struct nfi_attr * nfi_att, struct stat * att )
{
  debug_info("[SERV_ID=%d] [NFI_SCK] [sck_server_2_nfi_attr] >> Begin\n", -1);

  nfi_att->st_dev = att->st_dev;
  nfi_att->st_ino = att->st_ino;

  if (S_ISREG(att->st_mode)) {
    nfi_att->at_type = NFIFILE;
  }
  if (S_ISDIR(att->st_mode)) {
    nfi_att->at_type = NFIDIR;
  }

  nfi_att->at_mode    = att->st_mode & (S_IRWXU | S_IRWXG | S_IRWXO); // protection
  nfi_att->at_nlink   = att->st_nlink;                                // number of hard links
  nfi_att->at_uid     = att->st_uid;                                  // user ID of owner
  nfi_att->at_gid     = att->st_gid;                                  // group ID of owner
  nfi_att->at_size    = att->st_size;                                 // total size, in bytes
  nfi_att->at_blksize = att->st_blksize;                              // blocksize for filesystem I/O
  nfi_att->at_blocks  = att->st_blocks;                               // number of blocks allocated
  nfi_att->at_atime   = att->st_atime;                                // time of last access
  nfi_att->at_mtime   = att->st_mtime;                                // time of last modification
  nfi_att->at_ctime   = att->st_ctime;                                // time of last change

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_2_sck_server_attr] >> End\n", -1);
}

void SCK_SERVERtoNFIInfo(__attribute__((__unused__)) struct nfi_info * nfi_inf, __attribute__((__unused__)) struct nfi_info * sck_server_inf)
{
  debug_info("[SERV_ID=%d] [NFI_SCK] [sck_server_2_nfi_info] >> Begin\n", -1);

  //TODO

  debug_info("[SERV_ID=%d] [NFI_SCK] [sck_server_2_nfi_info] >> End\n", -1);
}


/* ... Functions / Funciones ......................................... */

int nfi_sck_server_init ( char * url, struct nfi_server * serv, __attribute__((__unused__)) struct nfi_attr_server * attr )
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX], prt[PATH_MAX];
  struct nfi_sck_server_server * server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_sck_server_init] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] >> Begin\n", serv->id);

  // new nfi_ops with sck_server functions...
  serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
  NULL_RET_ERR(serv->ops, SCK_SERVER_ERR_MEMORY);

  // Fill serv->ops...
  bzero(serv->ops, sizeof(struct nfi_ops));
  serv->ops->nfi_reconnect = nfi_sck_server_reconnect;
  serv->ops->nfi_disconnect = nfi_sck_server_disconnect;

  serv->ops->nfi_open = nfi_sck_server_open;
  serv->ops->nfi_create = nfi_sck_server_create;
  serv->ops->nfi_read = nfi_sck_server_read;
  serv->ops->nfi_write = nfi_sck_server_write;
  serv->ops->nfi_close = nfi_sck_server_close;
  serv->ops->nfi_remove = nfi_sck_server_remove;
  serv->ops->nfi_rename = nfi_sck_server_rename;
  serv->ops->nfi_getattr = nfi_sck_server_getattr;
  serv->ops->nfi_setattr = nfi_sck_server_setattr;

  serv->ops->nfi_opendir = nfi_sck_server_opendir;
  serv->ops->nfi_mkdir = nfi_sck_server_mkdir;
  serv->ops->nfi_readdir = nfi_sck_server_readdir;
  serv->ops->nfi_closedir = nfi_sck_server_closedir;
  serv->ops->nfi_rmdir = nfi_sck_server_rmdir;

  serv->ops->nfi_preload = nfi_sck_server_preload;
  serv->ops->nfi_flush = nfi_sck_server_flush;

  serv->ops->nfi_statfs = nfi_sck_server_statfs;

  // parse url...
  ret = ParseURL(url, prt, NULL, NULL, server, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] ERROR: incorrect url '%s'.\n", serv->id, url);
    sck_server_err(SCK_SERVER_ERR_URL);
    FREE_AND_NULL(serv->ops);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  // new nfi_sckserver_server...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] Copy nfi_sck_server_server structure\n", serv->id);

  server_aux = (struct nfi_sck_server_server * )malloc(sizeof(struct nfi_sck_server_server));
  if (server_aux == NULL)
  {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] ERROR: out of memory.\n", serv->id);
    FREE_AND_NULL(serv->ops);
    return -1;
  }

  // private_info...
  serv->private_info = (void *)server_aux;

  // Initialize params
  memset( & (server_aux->params), 0, sizeof(sck_client_param_st));

  // thread mode
  server_aux->params.xpn_thread = TH_NOT;
  serv->xpn_thread = TH_NOT;
  char * env_thread = getenv("XPN_THREAD");
  if (env_thread != NULL)
  {
    server_aux->params.xpn_thread = atoi(env_thread);
    serv->xpn_thread = atoi(env_thread);
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] Thread mode: %d\n", serv->id, serv->xpn_thread);

  // Session mode
  server_aux->params.xpn_session = 0;
  char * env_session = getenv("XPN_SESSION");
  if (env_session != NULL) {
    server_aux->params.xpn_session = atoi(env_session);
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] Sesion mode: %d\n", serv->id, server_aux->params.xpn_session);

  // Locality mode
  server_aux->params.xpn_locality = 1;
  char * env_locality = getenv("XPN_LOCALITY");
  if (env_locality != NULL) {
    server_aux->params.xpn_locality = atoi(env_locality);
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] Locality enable: %d\n", serv->id, server_aux->params.xpn_locality);

  // Initialize SCK Client communication side...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] Initialize SCK Client communication side\n", serv->id);

  ret = sck_client_comm_init(&(server_aux->params));
  if (ret < 0)
  {
    FREE_AND_NULL(serv->ops);
    FREE_AND_NULL(server_aux);
    return -1;
  }

  // copy 'url' string...
  serv->url = strdup(url);
  NULL_RET_ERR(serv->url, SCK_SERVER_ERR_MEMORY);

  // new server wrk...
  serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
  memset(serv->wrk, 0, sizeof(struct nfi_worker));
  serv->wrk->server = serv;

  // Initialize workers
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] Initialize workers\n", serv->id);

  ret = nfiworker_init(serv);

  // Server conection
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] Server conection\n", serv->id);

  ret = nfi_sck_server_connect(serv, url, prt, server, dir);
  if (ret < 0) 
  {
    FREE_AND_NULL(serv->ops);
    FREE_AND_NULL(server_aux);
    return -1;
  }

  // Data locality
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] Data locality\n", serv->id);

  ret = sck_client_comm_locality( & (server_aux->params));
  if (ret < 0)
  {
    FREE_AND_NULL(serv->ops);
    FREE_AND_NULL(server_aux);
    return -1;
  }
  
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] << End\n", serv->id);

  return 0;
}

int nfi_sck_server_destroy ( struct nfi_server * serv )
{
  int ret;
  struct nfi_sck_server_server * server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_sck_server_destroy] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_destroy] >> Begin\n", serv->id);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_destroy] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL) {
    return 0;
  }

  // Workers destroy...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_destroy] Destroy workers\n", serv->id);

  nfiworker_destroy(serv);

  // SCK Finalize...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_destroy] Destroy SCK Client communication\n", serv->id);

  ret = sck_client_comm_destroy( & (server_aux->params));
  if (ret < 0) {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_init] ERROR: sck_client_comm_destroy fails\n", serv->id);
  }

  // free private_info, 'url' string and 'server' string...
  FREE_AND_NULL(serv->ops);
  FREE_AND_NULL(serv->private_info);
  FREE_AND_NULL(serv->url);
  FREE_AND_NULL(serv->server);

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_destroy] << End\n", serv->id);

  return 0;
}

int nfi_sck_server_connect ( struct nfi_server * serv, __attribute__((__unused__)) char * url, __attribute__((__unused__)) char * prt, char * server, __attribute__((__unused__)) char * dir )
{
  int ret;
  struct nfi_sck_server_server * server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_sck_server_connect] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_connect] >> Begin\n", serv->id);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_connect] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server * )(serv->private_info);
  if (server_aux == NULL) {
    return -1;
  }

  // Copy server name
  strcpy(server_aux->params.srv_name, server);

  // SCK Connection...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_connect] SCK server conection\n", serv->id);

  ret = sck_client_comm_connect( & (server_aux->params));
  if (ret < 0) {
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_connect] << End\n", serv->id);

  return 0;
}

int nfi_sck_server_disconnect ( struct nfi_server * serv )
{
  int ret;
  struct nfi_sck_server_server * server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_sck_server_disconnect] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_disconnect] >> Begin\n", serv->id);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_disconnect] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL) {
    return 0;
  }

  // SCK Disconnect...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_disconnect] SCK server disconnect\n", serv->id);

  ret = sck_client_comm_disconnect( & (server_aux->params));
  if (ret < 0) {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_reconnect] ERROR: sck_client_comm_disconnect fails\n", serv->id);
  }

  FREE_AND_NULL(serv->private_info);

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_disconnect] << End\n", serv->id);

  return 0;
}

int nfi_sck_server_reconnect ( struct nfi_server * serv )
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX];
  struct nfi_sck_server_server * server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_LOCAL] [nfi_sck_server_reconnect] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_reconnect] >> Begin\n", serv->id);

  // ParseURL...
  ret = ParseURL(serv->url, NULL, NULL, NULL, server, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_reconnect] ERROR: url %s incorrect.\n", serv->id, serv->url);
    sck_server_err(SCK_SERVER_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_reconnect] ParseURL(%s)= %s; %s\n", serv->id, serv->url, server, dir);

  // new nfi_sckserver_server...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_reconnect] Copy nfi_sck_server_server structure\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)malloc(sizeof(struct nfi_sck_server_server));
  NULL_RET_ERR(server_aux, SCK_SERVER_ERR_MEMORY);

  strcpy(server_aux->path, dir);
  serv->private_info = (void *)server_aux;

  // reconnect...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_reconnect] SCK server conection\n", serv->id);

  ret = sck_client_comm_connect( & (server_aux->params));
  if (ret < 0)
  {
    debug_error("nfi_sck_server_reconnect: sck_client_comm_connect fails.\n");
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_reconnect] << End\n", serv->id);

  return 0;
}

// File API
int nfi_sck_server_open ( struct nfi_server * serv, char * url, struct nfi_fhandle * fho )
{
  int    ret;
  char   dir[PATH_MAX], server[PATH_MAX];
  struct nfi_sck_server_server * server_aux;
  struct nfi_sck_server_fhandle * fh_aux;
  struct st_sck_server_msg msg;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_open] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(fho, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_open] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // from url->server + dir
  ret = ParseURL(url, NULL, NULL, NULL, server, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_open] ERROR: incorrect url '%s'.\n", serv->id, url);
    sck_server_err(SCK_SERVER_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_open] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  //Copy url
  fho->url = strdup(url);
  NULL_RET_ERR(fho->url, SCK_SERVER_ERR_MEMORY);

  fh_aux = (struct nfi_sck_server_fhandle *)malloc(sizeof(struct nfi_sck_server_fhandle));
  NULL_RET_ERR(fh_aux, SCK_SERVER_ERR_MEMORY);
  bzero(fh_aux, sizeof(struct nfi_sck_server_fhandle));

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_open] nfi_sck_server_open(%s)\n", serv->id, dir);

  /************** LOCAL *****************/
  if (server_aux->params.locality)
  {
    char path [PATH_MAX];

    strcpy(path, server_aux->params.dirbase);
    strcat(path, "/");
    strcat(path, dir);

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_open] real_posix_open2(%s)\n", serv->id, path);

    fh_aux->fd = real_posix_open2(path, O_RDWR, S_IRWXU);
    if (fh_aux->fd < 0)
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_open] ERROR: real_posix_open2 fails to open '%s' in server %s.\n", serv->id, path, serv->server);
      FREE_AND_NULL(fh_aux);
      FREE_AND_NULL(fho->url);
      return -1;
    }

    if (server_aux->params.xpn_session == 0) {
      real_posix_close(fh_aux->fd);
    }

    strcpy(fh_aux->path, dir);

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_open] real_posix_open2(%s)=%d\n", serv->id, path, fh_aux->fd);
  }
  /************** REMOTE ****************/
  else
  {
    //bzero(&msg, sizeof(struct st_sck_server_msg));

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_open] xpn_open(%s)\n", serv->id, dir);

    if (server_aux->params.xpn_session) {
      msg.type = SCK_SERVER_OPEN_FILE_WS;
    } 
    else {
      msg.type = SCK_SERVER_OPEN_FILE_WOS;
    }

    memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
    memccpy(msg.u_st_sck_server_msg.op_open.path, dir, 0, PATH_MAX - 1);

    nfi_sck_server_do_request(server_aux, & msg, (char *)& (fh_aux->fd), sizeof(int));
    if (fh_aux->fd < 0)
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_open] ERROR: remote open fails to open '%s' in server %s.\n", serv->id, dir, serv->server);
      FREE_AND_NULL(fh_aux);
      FREE_AND_NULL(fho->url);
      return -1;
    }

    memccpy(fh_aux->path, dir, 0, PATH_MAX - 1);

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_open] xpn_open(%s)=%d\n", serv->id, dir, fh_aux->fd);
  }
  /*****************************************/

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_open] nfi_sck_server_open(%s)=%d\n", serv->id, dir, fh_aux->fd);

  fho->type = NFIFILE;
  fho->priv_fh = NULL;
  fho->server = serv;
  fho->priv_fh = (void *)fh_aux;

  debug_info("[SERV_ID=%d] [NFI_LOCAL] [nfi_local_open] nfi_local_open(%s)=%d\n", serv->id, dir, fh_aux->fd);

  return 0;
}

int nfi_sck_server_create ( struct nfi_server * serv, char * url, struct nfi_attr * attr, struct nfi_fhandle * fh )
{
  int    ret;
  char   server[PATH_MAX], dir[PATH_MAX];
  struct nfi_sck_server_server * server_aux;
  struct nfi_sck_server_fhandle * fh_aux;
  struct st_sck_server_msg msg;
  struct st_sck_server_attr_req req;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(attr, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // url->server + dir
  ret = ParseURL(url, NULL, NULL, NULL, server, NULL, dir);
  if (ret < 0)
  {
    debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] ERROR: incorrect url '%s'.\n", serv->id, url);
    sck_server_err(SCK_SERVER_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  // private_info file handle
  fh_aux = (struct nfi_sck_server_fhandle *)malloc(sizeof(struct nfi_sck_server_fhandle));
  NULL_RET_ERR(fh_aux, SCK_SERVER_ERR_MEMORY);
  bzero(fh_aux, sizeof(struct nfi_sck_server_fhandle));

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] nfi_sck_server_create(%s)\n", serv->id, dir);

  /************** LOCAL *****************/
  if (server_aux->params.locality)
  {
    char path [PATH_MAX];

    strcpy(path, server_aux->params.dirbase);
    strcat(path, "/");
    strcat(path, dir);

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] real_posix_open2(%s)\n", serv->id, path);

    fh_aux->fd = real_posix_open2(path, O_CREAT|O_RDWR|O_TRUNC, attr->at_mode);
    if (fh_aux->fd < 0)
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] ERROR: real_posix_open2 fails to open '%s' in server %s.\n", serv->id, path, serv->server);
      FREE_AND_NULL(fh_aux);
      return -1;
    }

    //Get stat
    ret = real_posix_stat(path, & (req.attr));
    if (ret < 0)
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] ERROR: real_posix_stat fails to stat '%s' in server %s.\n", serv->id, path, serv->server);
      FREE_AND_NULL(fh_aux);
      return ret;
    }

    if (server_aux->params.xpn_session == 0) {
      real_posix_close(fh_aux->fd);
    }

    strcpy(fh_aux->path, dir);

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] real_posix_open2(%s)=%d\n", serv->id, path, fh_aux->fd);
  }
  /************** REMOTE ****************/
  else 
  {
    //bzero(&msg, sizeof(struct st_sck_server_msg));

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] xpn_creat(%s)\n", serv->id, dir);

    if (server_aux->params.xpn_session) {
      msg.type = SCK_SERVER_CREAT_FILE_WS;
    }
    else {
      msg.type = SCK_SERVER_CREAT_FILE_WOS;
    }
    memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
    memccpy(msg.u_st_sck_server_msg.op_creat.path, dir, 0, PATH_MAX - 1);

    nfi_sck_server_do_request(server_aux, & msg, (char *)& (fh_aux->fd), sizeof(int));
    //TODO
    /*
    if (fh_aux->fd < 0)
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] ERROR: remote open fails to creat '%s' in server %s.\n", serv->id, dir, serv->server);
      FREE_AND_NULL(fh_aux);
      DEBUG_END();
      return -1;
    }
    */

    memccpy(fh_aux->path, dir, 0, PATH_MAX - 1);

    // Get stat
    //bzero(&msg, sizeof(struct st_sck_server_msg));
    msg.type = SCK_SERVER_GETATTR_FILE;
    memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
    memccpy(msg.u_st_sck_server_msg.op_getattr.path, dir, 0, PATH_MAX - 1);

    nfi_sck_server_do_request(server_aux, & msg, (char *)& req, sizeof(struct st_sck_server_attr_req));

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] xpn_creat(%s)=%d\n", serv->id, dir, fh_aux->fd);
  }
  /*****************************************/

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] nfi_sck_server_create(%s)=%d\n", serv->id, url, fh_aux->fd);

  fh->type = NFIFILE;
  fh->server = serv;
  fh->priv_fh = (void *)fh_aux;

  fh->url = strdup(url);
  if (fh->url == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_MEMORY);
    FREE_AND_NULL(fh_aux);
    return -1;
  }

  sck_server_2_nfi_attr(attr, & req.attr);

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_create] >> End\n", serv->id);

  return 0;
}

ssize_t nfi_sck_server_read ( struct nfi_server * serv, struct nfi_fhandle * fh, void * buffer, off_t offset, size_t size )
{
  int    ret, cont, diff;
  struct nfi_sck_server_server * server_aux;
  struct nfi_sck_server_fhandle * fh_aux;
  struct st_sck_server_msg msg;
  struct st_sck_server_read_req req;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(fh, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // private_info file handle
  fh_aux = (struct nfi_sck_server_fhandle *)fh->priv_fh;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] nfi_sck_server_read(%d, %ld, %ld)\n", serv->id, fh_aux->fd, offset, size);

  /************** LOCAL *****************/
  if (server_aux->params.locality)
  {
    if (server_aux->params.xpn_session)
    {
      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] real_posix_read(%d, %ld, %ld)\n", serv->id, fh_aux->fd, offset, size);

      real_posix_lseek(fh_aux->fd, offset, SEEK_SET); //TODO: check error

      //if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
      ret = real_posix_read(fh_aux->fd, buffer, size);
      //if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);
      if (ret < 0)
      {
        debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] ERROR: real_posix_read reads zero bytes from '%d' in server %d\n", serv->id, fh_aux->fd, serv->server);
        return -1;
      }

      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] real_posix_read(%d, %ld, %ld)=%d\n", serv->id, fh_aux->fd, offset, size, ret);
    } 
    else
    {
      int fd;
      char path [PATH_MAX];

      strcpy(path, server_aux->params.dirbase);
      strcat(path, "/");
      strcat(path, fh_aux->path);

      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] real_posix_read(%s, %ld, %ld)\n", serv->id, path, offset, size);

      fd = real_posix_open(path, O_RDONLY);
      if (fd < 0)
      {
        debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] ERROR: real_posix_open fails to open '%d' in server %s.\n", serv->id, fh_aux->fd, serv->server);
        return -1;
      }

      real_posix_lseek(fd, offset, SEEK_SET); //TODO: check error

      //if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
      ret = real_posix_read(fd, buffer, size);
      //if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);

      real_posix_close(fd);

      if (ret < 0)
      {
        debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] ERROR: real_posix_read reads zero bytes from '%d' in server %d\n", serv->id, fh_aux->fd, serv->server);        return -1;
      }

      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] real_posix_read(%s, %ld, %ld)=%d\n", serv->id, path, offset, size, ret);
    }
  }
  /************** REMOTE ****************/
  else
  {
    //Debug
    if (server_aux->params.xpn_session){
      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] xpn_read(%d, %ld, %ld)\n", serv->id, fh_aux->fd, offset, size);
    }
    else {
      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] xpn_read(%s, %ld, %ld)\n", serv->id, msg.u_st_sck_server_msg.op_read.path, offset, size);
    }

    //bzero(&msg, sizeof(struct st_sck_server_msg));

    if (server_aux->params.xpn_session) 
    {
      msg.type = SCK_SERVER_READ_FILE_WS;
      msg.u_st_sck_server_msg.op_read.fd = fh_aux->fd;
    } 
    else 
    {
      msg.type = SCK_SERVER_READ_FILE_WOS;
      memccpy(msg.u_st_sck_server_msg.op_read.path, fh_aux->path, 0, PATH_MAX);
    }
    memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
    msg.u_st_sck_server_msg.op_read.offset = offset;
    msg.u_st_sck_server_msg.op_read.size = size;

    //ret = sck_server_write_operation(server_aux->sd, &msg);
    ret = sck_server_write_operation(server_aux->params.server, & msg);
    if (ret < 0)
    {
      printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] ERROR: sck_server_write_operation fails\n", serv->id);
      return -1;
    }

    // read n times: number of bytes + read data (n bytes)
    cont = 0;
    do
    {
      ret = sck_client_read_data(server_aux->params.server, (char *)& req, sizeof(struct st_sck_server_read_req), msg.id);
      if (ret < 0)
      {
        printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] ERROR: sck_client_read_data fails\n", serv->id);
        return -1;
      }

      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] sck_client_read_data=%d.\n",serv->id, ret);

      if (req.size > 0)
      {
        debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] sck_client_read_data(%ld)\n",serv->id, req.size);

        ret = sck_client_read_data(server_aux->params.server, (char *)buffer + cont, req.size, msg.id);
        if (ret < 0) {
          printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] ERROR: sck_client_read_data fails\n", serv->id);
        }

        debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] sck_client_read_data(%ld)=%d\n",serv->id, req.size, ret);
      }

      cont = cont + req.size;
      diff = msg.u_st_sck_server_msg.op_read.size - cont;

    } while ((diff > 0) && (req.size != 0));

    if (req.size < 0) {
      printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] ERROR: nfi_sck_server_read reads zero bytes from '%d' in server %s\n", serv->id, fh_aux->fd, serv->server);
      sck_server_err(SCK_SERVER_ERR_READ);
      return -1;
    }

    ret = cont;
  }
  /*****************************************/

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] nfi_sck_server_read(%d, %ld, %ld)=%d\n", serv->id, fh_aux->fd, offset, size, ret);
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_read] >> End\n", serv->id);

  return ret;
}

ssize_t nfi_sck_server_write ( struct nfi_server * serv, struct nfi_fhandle * fh, void * buffer, off_t offset, size_t size )
{
  int ret, diff, cont;
  struct nfi_sck_server_server * server_aux;
  struct nfi_sck_server_fhandle * fh_aux;
  struct st_sck_server_msg msg;
  struct st_sck_server_write_req req;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] >> Begin\n", serv->id);

  // Check arguments...
  if (size == 0) {
    return 0;
  }

  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(fh, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // private_info file handle
  fh_aux = (struct nfi_sck_server_fhandle *)fh->priv_fh;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] nfi_sck_server_write(%d, %ld, %ld)\n", serv->id, fh_aux->fd, offset, size);

  /************** LOCAL *****************/
  if (server_aux->params.locality) 
  {
    if (server_aux->params.xpn_session) 
    {
      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] real_posix_write(%d, %ld, %ld)\n", serv->id, fh_aux->fd, offset, size);

      real_posix_lseek(fh_aux->fd, offset, SEEK_SET); //TODO: check error

      //if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
      ret = real_posix_write(fh_aux->fd, buffer, size);
      //if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);
      if (ret < 0)
      {
        debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] ERROR: real_posix_write writes zero bytes from '%d' in server %d\n", serv->id, fh_aux->fd, serv->server);
        return -1;
      }

      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] real_posix_write(%d, %ld, %ld)=%d\n", serv->id, fh_aux->fd, offset, size, ret);
    } 
    else 
    {
      int fd;
      char path [PATH_MAX];

      strcpy(path, server_aux->params.dirbase);
      strcat(path, "/");
      strcat(path, fh_aux->path);

      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] real_posix_write(%s, %ld, %ld)\n", serv->id, path, offset, size);

      fd = real_posix_open(path, O_WRONLY); // WOS
      if (fd < 0)
      {
        debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] ERROR: real_posix_open fails to open '%d' in server %s.\n", serv->id, fh_aux->fd, serv->server);
        return -1;
      }

      real_posix_lseek(fd, offset, SEEK_SET); //TODO: check error

      //if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
      ret = real_posix_write(fd, buffer, size);
      //if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);

      real_posix_close(fd); // WOS

      if (ret < 0)
      {
        debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] ERROR: real_posix_write writes zero bytes from '%d' in server %d\n", serv->id, fh_aux->fd, serv->server);        return -1;
        return -1;
      }

      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] real_posix_read(%s, %ld, %ld)=%d\n", serv->id, path, offset, size, ret);
    }
  }
  /************** REMOTE ****************/
  else 
  {
    //Debug
    if (server_aux->params.xpn_session){
      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] xpn_write(%d, %ld, %ld)\n", serv->id, fh_aux->fd, offset, size);
    }
    else {
      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] xpn_write(%s, %ld, %ld)\n", serv->id, msg.u_st_sck_server_msg.op_read.path, offset, size);
    }

    //bzero(&msg, sizeof(struct st_sck_server_msg));

    if (server_aux->params.xpn_session) 
    {
      msg.type = SCK_SERVER_WRITE_FILE_WS;
      msg.u_st_sck_server_msg.op_write.fd = fh_aux->fd;
    } 
    else 
    {
      msg.type = SCK_SERVER_WRITE_FILE_WOS;
      memccpy(msg.u_st_sck_server_msg.op_write.path, fh_aux->path, 0, PATH_MAX - 1);
    }
    memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
    msg.u_st_sck_server_msg.op_write.offset = offset;
    msg.u_st_sck_server_msg.op_write.size = size;

    ret = sck_server_write_operation(server_aux->params.server, & msg);
    if (ret < 0) 
    {
      printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] ERROR: sck_server_write_operation fails\n", serv->id);
      return -1;
    }

    diff = size;
    cont = 0;
    
    int buffer_size = size;

    // Max buffer size
    if (buffer_size > MAX_BUFFER_SIZE) 
    {
      buffer_size = MAX_BUFFER_SIZE;
    }

    // writes n times: number of bytes + write data (n bytes)
    do 
    {
      if (diff > buffer_size)
      {
        ret = sck_client_write_data(server_aux->params.server, (char *)buffer + cont, buffer_size, msg.id);
        if (ret < 0) {
          printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] ERROR: sck_client_write_data fails\n", serv->id);
        }
      }
      else
      {
        ret = sck_client_write_data(server_aux->params.server, (char *)buffer + cont, diff, msg.id);
        if (ret < 0) {
          printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] ERROR: sck_client_write_data fails\n", serv->id);
        }
      }

      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] sck_client_write_data=%d.\n",serv->id, ret);

      cont = cont + ret; //Send bytes
      diff = size - cont;

    } while ((diff > 0) && (ret != 0));

    ret = sck_client_read_data(server_aux->params.server, (char *)& req, sizeof(struct st_sck_server_write_req), msg.id);
    if (ret < 0) 
    {
      printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] ERROR: sck_client_read_data fails\n", serv->id);
      return -1;
    }

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] sck_client_read_data=%d.\n",serv->id, ret);

    if (req.size < 0) 
    {
      printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] ERROR: nfi_sck_server_write writes zero bytes from '%d' in server %s\n", serv->id, fh_aux->fd, serv->server);
      sck_server_err(SCK_SERVER_ERR_WRITE);
      return -1;
    }

    ret = cont;
  }
  /*****************************************/

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] nfi_sck_server_write(%d, %ld, %ld)=%d\n", serv->id, fh_aux->fd, offset, size, ret);
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_write] >> End\n", serv->id);

  return ret;
}

int nfi_sck_server_close ( struct nfi_server * serv, struct nfi_fhandle * fh )
{
  int ret = -1;
  struct nfi_sck_server_fhandle * fh_aux;
  struct nfi_sck_server_server * server_aux;
  struct st_sck_server_msg msg;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_close] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(fh, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_close] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // without session -> just return ok
  if (server_aux->params.xpn_session == 0)
  {
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_close] >> End\n", serv->id);
    return 1;
  }

  // if fh->priv_fh is NULL -> return -1
  if (NULL == fh->priv_fh)
  {
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_close] >> End\n", serv->id);
    return -1;
  }

  // private_info file handle
  fh_aux = (struct nfi_sck_server_fhandle *)fh->priv_fh;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_close] nfi_sck_server_close(%d)\n", serv->id, fh_aux->fd);

  /************** LOCAL *****************/
  if (server_aux->params.locality)
  {
    if (fh_aux != NULL)
    {
      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_close] real_posix_close(%d)\n", serv->id, fh_aux->fd);

      //if(server_aux->params.sem_server != 0) sem_wait(server_aux->params.sem_server);
      ret = real_posix_close(fh_aux->fd);
      //if(server_aux->params.sem_server != 0) sem_post(server_aux->params.sem_server);

      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_close] real_posix_close(%d)=%d\n", serv->id, fh_aux->fd, ret);
    }
  }
  /************** REMOTE ****************/
  else
  {
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_close] xpn_close(%d)\n", serv->id, fh_aux->fd);

    //bzero(&msg, sizeof(struct st_sck_server_msg));

    msg.type = SCK_SERVER_CLOSE_FILE_WS;
    memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
    msg.u_st_sck_server_msg.op_close.fd = fh_aux->fd;

    nfi_sck_server_do_request(server_aux, & msg, (char *)& (ret), sizeof(int));

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_close] xpn_close(%d)=%d\n", serv->id, fh_aux->fd, ret);
  }
  /*****************************************/

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_close] nfi_sck_server_close(%d)=%d\n", serv->id, fh_aux->fd, ret);

  // free memory
  FREE_AND_NULL(fh->priv_fh);
  fh->type = NFINULL;
  fh->server = NULL;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_close] >> End\n", serv->id);

  return ret;
}

int nfi_sck_server_remove ( struct nfi_server * serv, char * url )
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX];
  struct nfi_sck_server_server * server_aux;
  struct st_sck_server_msg msg;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_remove] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(url, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_remove] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // from url->server + dir
  ret = ParseURL(url, NULL, NULL, NULL, server, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_remove] ERROR: incorrect url '%s'.\n", serv->id, url);
    sck_server_err(SCK_SERVER_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_remove] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_remove] nfi_sck_server_remove(%s)\n", serv->id, dir);

  /************** LOCAL *****************/
  if (server_aux->params.locality)
  {
    char path [PATH_MAX];

    strcpy(path, server_aux->params.dirbase);
    strcat(path, "/");
    strcat(path, dir);

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_remove] real_posix_unlink(%s)\n", serv->id, path);

    ret = real_posix_unlink(path);
    if (ret < 0)
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_remove] ERROR: real_posix_unlink fails to unlink '%s' in server %s.\n", serv->id, path, serv->server);
      return -1;
    }

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_remove] real_posix_unlink(%s)=%d\n", serv->id, path, ret);
  }
  /************** REMOTE ****************/
  else
  {
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_remove] xpn_unlink(%s)\n", serv->id, dir);

    //bzero(&msg, sizeof(struct st_sck_server_msg));
    memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
    memccpy(msg.u_st_sck_server_msg.op_rm.path, dir, 0, PATH_MAX - 1);

    if ((serv -> wrk -> arg.is_master_node) == 1)
    {
      msg.type = SCK_SERVER_RM_FILE;
      nfi_sck_server_do_request(server_aux, &msg, (char *)&(ret), sizeof(int));
    }
    else
    {
      msg.type = SCK_SERVER_RM_FILE_ASYNC;

      // send request...
      ret = sck_server_write_operation(server_aux->params.server, &msg);
      if (ret >= 0) {
        return 0;
      }
    }

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_remove] xpn_unlink(%s)=%d\n", serv->id, dir, ret);
  }
  /*****************************************/

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_remove] nfi_sck_server_remove(%s)=%d\n", serv->id, dir, ret);
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_remove] >> End\n", serv->id);

  return ret;
}

int nfi_sck_server_rename ( struct nfi_server * serv, char * old_url, char * new_url )
{
  int ret;
  char server[PATH_MAX], old_path[PATH_MAX], new_path[PATH_MAX];
  struct nfi_sck_server_server * server_aux;
  struct st_sck_server_msg msg;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(old_url, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(new_url, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(old_url, NULL, NULL, NULL, server, NULL, old_path);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] ERROR: incorrect url '%s'.\n", serv->id, old_url);
    sck_server_err(SCK_SERVER_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] ParseURL(%s)= %s; %s\n", serv->id, old_url, server, old_path);

  // from url -> server + dir
  ret = ParseURL(new_url, NULL, NULL, NULL, server, NULL, new_path);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] ERROR: incorrect url '%s'.\n", serv->id, new_url);
    sck_server_err(SCK_SERVER_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] ParseURL(%s)= %s; %s\n", serv->id, new_path, server, new_path);

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] nfi_sck_server_rename(%s,%s)\n", serv->id, old_path, new_path);

  /************** LOCAL *****************/
  if (server_aux->params.locality)
  {
    char old_name [PATH_MAX];
    char new_name [PATH_MAX];

    strcpy(old_name, server_aux->params.dirbase);
    strcat(old_name, "/");
    strcat(old_name, old_path);

    strcpy(new_name, server_aux->params.dirbase);
    strcat(new_name, "/");
    strcat(new_name, new_path);

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] real_posix_rename(%s, %s)\n", serv->id, old_name, new_name);

    ret = real_posix_rename(old_name, new_name);
    if (ret < 0)
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] ERROR: real_posix_rename fails to rename '%s' in server %s.\n", serv->id, old_name, serv->server);
      return -1;
    }

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] real_posix_rename(%s, %s)=%d\n", serv->id, old_name, new_name, ret);
  }
  /************** REMOTE ****************/
  else
  {
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] xpn_rename(%s,%s)\n", serv->id, old_path, new_path);

    //bzero(&msg, sizeof(struct st_sck_server_msg));

    msg.type = SCK_SERVER_RENAME_FILE;
    memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
    memccpy(msg.u_st_sck_server_msg.op_rename.old_url, old_path, 0, PATH_MAX - 1);
    memccpy(msg.u_st_sck_server_msg.op_rename.new_url, new_path, 0, PATH_MAX - 1);

    nfi_sck_server_do_request(server_aux, &msg, (char *)&(ret), sizeof(int));

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] xpn_rename(%s, %s)=%d\n", serv->id, old_path, new_path, ret);
  }
  /*****************************************/

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rename] nfi_sck_server_rename(%s,%s)=%d\n", serv->id, old_path, new_path, ret);
  debug_info("[NFI_SCK] [nfi_sck_server_rename] >> End\n");

  return ret;
}

int nfi_sck_server_getattr ( struct nfi_server * serv, struct nfi_fhandle * fh, struct nfi_attr * attr )
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX];
  struct nfi_sck_server_server * server_aux;
  //struct nfi_sck_server_fhandle *fh_aux; // TODO: fstat
  struct st_sck_server_msg msg;
  struct st_sck_server_attr_req req;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_getattr] >> Begin\n", serv->id);

  // check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(fh, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(attr, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_getattr] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(fh->url, NULL, NULL, NULL, server, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_getattr] ERROR: incorrect url '%s'.\n", serv->id, fh->url);
    sck_server_err(SCK_SERVER_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_getattr] ParseURL(%s)= %s; %s\n", serv->id, fh->url, server, dir);
  
  // copy private information...
  //fh_aux     = (struct nfi_sck_server_fhandle *) fh->priv_fh; //TODO: fstat

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_getattr] nfi_sck_server_getattr(%s)\n", serv->id, dir);

  /************** LOCAL *****************/
  if (server_aux->params.locality)
  {
    char path [PATH_MAX];

    strcpy(path, server_aux->params.dirbase);
    strcat(path, "/");
    strcat(path, dir);

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_getattr] real_posix_stat(%s)\n", serv->id, path);

    req.status = real_posix_stat(path, & (req.attr));
    if (((int) req.status) < 0) 
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_getattr] ERROR: real_posix_stat fails to stat '%s' in server %s.\n", serv->id, path, serv->server);
      return req.status;
    }

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_getattr] real_posix_stat(%s)=%d\n", serv->id, path, req.status);
  }
  /************** REMOTE ****************/
  else
  {
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_getattr] xpn_stat(%s)\n", serv->id, dir);

    //bzero(&msg, sizeof(struct st_sck_server_msg));

    msg.type = SCK_SERVER_GETATTR_FILE;
    memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
    memccpy(msg.u_st_sck_server_msg.op_getattr.path, dir, 0, PATH_MAX - 1);

    nfi_sck_server_do_request(server_aux, & msg, (char *)& req, sizeof(struct st_sck_server_attr_req));

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_getattr] xpn_stat(%s)=%d\n", serv->id, dir, req.status);
  }
  /*****************************************/

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_getattr] nfi_sck_server_getattr(%s)=%d\n", serv->id, dir, req.status);

  sck_server_2_nfi_attr(attr, & req.attr);

  debug_info("[NFI_SCK] [nfi_sck_server_getattr] >> End\n");

  return req.status;
}

int nfi_sck_server_setattr ( struct nfi_server * serv, struct nfi_fhandle * fh, struct nfi_attr * attr ) 
{
  struct nfi_sck_server_server * server_aux;
  struct nfi_sck_server_fhandle * fh_aux;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_setattr] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(fh, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(attr, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(fh->priv_fh, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_setattr] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // private_info file handle
  fh_aux = (struct nfi_sck_server_fhandle *) fh->priv_fh;

  // TODO: setattr
  server_aux = server_aux;
  fh_aux     = fh_aux;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_setattr] >> End\n", serv->id);

  return 0;
}

// Directories API
int nfi_sck_server_mkdir ( struct nfi_server * serv, char * url, struct nfi_attr * attr, struct nfi_fhandle * fh )
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX];
  struct nfi_sck_server_server * server_aux;
  struct nfi_sck_server_fhandle * fh_aux;
  struct st_sck_server_msg msg;
  struct st_sck_server_attr_req req;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(attr, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, server, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] ERROR: incorrect url '%s'.\n", serv->id, url);
    sck_server_err(SCK_SERVER_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  // private_info file handle
  fh_aux = (struct nfi_sck_server_fhandle *)malloc(sizeof(struct nfi_sck_server_fhandle));
  NULL_RET_ERR(fh_aux, SCK_SERVER_ERR_MEMORY);
  bzero(fh_aux, sizeof(struct nfi_sck_server_fhandle));

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] nfi_sck_server_mkdir(%s)\n", serv->id, dir);

  /************** LOCAL *****************/
  if (server_aux->params.locality)
  {
    char path [PATH_MAX];

    strcpy(path, server_aux->params.dirbase);
    strcat(path, "/");
    strcat(path, dir);

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] real_posix_mkdir(%s)\n", serv->id, path);

    ret = real_posix_mkdir(path, /*attr->at_mode*/ 0777);
    if ((ret < 0) && (errno != EEXIST))
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] ERROR: real_posix_mkdir fails to mkdir '%s' in server %s.\n", serv->id, path, serv->server);
      FREE_AND_NULL(fh_aux);
      return -1;
    }
    fh_aux->fd = ret; //Cuidado

    //Get stat
    ret = real_posix_stat(dir, & (req.attr));
    if (ret < 0)
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] ERROR: real_posix_stat fails to stat '%s' in server %s.\n", serv->id, path, serv->server);
      return ret;
    }

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] real_posix_mkdir(%s)=%d\n", serv->id, path, ret);
  }
  /************** SERVER ****************/
  else
  {
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] xpn_mkdir(%s)\n", serv->id, dir);

    //bzero(&msg, sizeof(struct st_sck_server_msg));

    msg.type = SCK_SERVER_MKDIR_DIR;
    memccpy(msg.u_st_sck_server_msg.op_mkdir.path, dir, 0, PATH_MAX - 1);

    nfi_sck_server_do_request(server_aux, & msg, (char *)& (fh_aux->fd), sizeof(int));

    memccpy(fh_aux->path, dir, 0, PATH_MAX - 1);
    if ((fh_aux->fd < 0) && (errno != EEXIST))
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] ERROR: xpn_mkdir fails to mkdir '%s' in server %s.\n", serv->id, dir, serv->server);
      sck_server_err(SCK_SERVER_ERR_MKDIR);
      FREE_AND_NULL(fh_aux);
      return -1;
    }

    //Get stat
    msg.type = SCK_SERVER_GETATTR_FILE;
    memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
    memccpy(msg.u_st_sck_server_msg.op_getattr.path, dir, 0, PATH_MAX - 1);

    nfi_sck_server_do_request(server_aux, &msg, (char *)&req, sizeof(struct st_sck_server_attr_req));

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] xpn_mkdir(%s)=%d\n", serv->id, dir, ret);
  }
  /*****************************************/

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] nfi_sck_server_mkdir(%s)=%d\n", serv->id, dir, ret);

  fh->type = NFIDIR;
  fh->priv_fh = (void *)fh_aux;

  fh->url = STRING_MISC_StrDup(url);
  if (fh->url == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_MEMORY);
    FREE_AND_NULL(fh_aux);
    return -1;
  }

  sck_server_2_nfi_attr(attr, & req.attr);

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_mkdir] >> End\n", serv->id);

  return ret;
}

int nfi_sck_server_opendir ( struct nfi_server * serv, char * url, struct nfi_fhandle * fho )
{
  int ret;
  char dir[PATH_MAX], server[PATH_MAX];
  struct nfi_sck_server_server * server_aux;
  struct nfi_sck_server_fhandle * fh_aux;
  struct st_sck_server_msg msg;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_opendir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(url, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(fho, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_opendir] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *) serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, server, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_opendir] ERROR: incorrect url '%s'.\n", serv->id, url);
    sck_server_err(SCK_SERVER_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_opendir] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  // Copy url
  fho->url = strdup(url);
  NULL_RET_ERR(fho->url, SCK_SERVER_ERR_MEMORY);

  // private_info file handle
  fh_aux = (struct nfi_sck_server_fhandle *)malloc(sizeof(struct nfi_sck_server_fhandle));
  NULL_RET_ERR(fh_aux, SCK_SERVER_ERR_MEMORY);
  bzero(fh_aux, sizeof(struct nfi_sck_server_fhandle));

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_opendir] nfi_sck_server_opendir(%s)\n", serv->id, dir);

  /************** LOCAL *****************/
  if (server_aux->params.locality)
  {
    char path [PATH_MAX];

    strcpy(path, server_aux->params.dirbase);
    strcat(path, "/");
    strcat(path, dir);

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_opendir] real_posix_opendir(%s)\n", serv->id, path);

    fh_aux->dir = real_posix_opendir(path);
    if (fh_aux->dir == NULL)
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_opendir] ERROR: real_posix_opendir fails to opendir '%s' in server %s.\n", serv->id, path, serv->server);
      FREE_AND_NULL(fh_aux);
      FREE_AND_NULL(fho->url);
      return -1;
    }

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_opendir] real_posix_opendir(%s)=%p\n", serv->id, path, fh_aux->dir);
  }
  /************** SERVER ****************/
  else
  {
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_opendir] xpn_opendir(%s)\n", serv->id, dir);

    //bzero(&msg, sizeof(struct st_sck_server_msg));

    msg.type = SCK_SERVER_OPENDIR_DIR;
    memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
    memccpy(msg.u_st_sck_server_msg.op_opendir.path, dir, 0, PATH_MAX - 1);

    unsigned long long aux;
    nfi_sck_server_do_request(server_aux, & msg, (char *)& (aux), sizeof(DIR * ));

    fh_aux->dir = (DIR *)aux;

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_opendir] xpn_opendir(%s)=%p\n", serv->id, dir, fh_aux->dir);
  }
  /*****************************************/

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_opendir] nfi_sck_server_opendir(%s)=%p\n", serv->id, dir, fh_aux->dir);

  strcpy(fh_aux->path, dir);
  fho->type = NFIDIR;
  fho->server = serv;
  fho->priv_fh = (void *)fh_aux;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_opendir] >> End\n", serv->id);

  return 0;
}

int nfi_sck_server_readdir ( struct nfi_server * serv, struct nfi_fhandle * fh, struct dirent * entry )
{
  struct nfi_sck_server_server * server_aux;
  struct nfi_sck_server_fhandle * fh_aux;
  struct st_sck_server_msg msg;
  struct st_sck_server_direntry ret_entry;
  struct dirent * ent;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_readdir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(fh, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(fh->priv_fh, SCK_SERVER_ERR_PARAM);
  if (fh->type != NFIDIR) 
  {
    sck_server_err(SCK_SERVER_ERR_NOTDIR);
    return -1;
  }
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_readdir] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // private_info file handle
  fh_aux = (struct nfi_sck_server_fhandle *)fh->priv_fh;

  // clean all entry content
  memset(entry, 0, sizeof(struct dirent));

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_readdir] nfi_sck_server_readdir(%p)\n", serv->id, fh_aux->dir);

  /************** LOCAL *****************/
  if (server_aux->params.locality)
  {
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_readdir] real_posix_readdir(%p)\n", serv->id, fh_aux->dir);

    ent = real_posix_readdir(fh_aux->dir);
    if (ent == NULL)
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_readdir] ERROR: real_posix_readdir fails to open '%p' in server %s.\n", serv->id, fh_aux->dir, serv->server);
      return -1;
    }

    memcpy(entry, ent, sizeof(struct dirent));

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_readdir] real_posix_readdir(%p)=%p\n", serv->id, fh_aux->dir, entry);
  }
  /************** SERVER ****************/
  else
  {
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_readdir] xpn_readdir(%p)\n", serv->id, fh_aux->dir);

    //bzero(&msg, sizeof(struct st_sck_server_msg));

    msg.type = SCK_SERVER_READDIR_DIR;
    memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
    msg.u_st_sck_server_msg.op_readdir.dir = fh_aux->dir;

    nfi_sck_server_do_request(server_aux, & msg, (char *)& (ret_entry), sizeof(struct st_sck_server_direntry)); //NEW

    if (ret_entry.end == 0) {
      return -1;
    }

    memcpy(entry, & (ret_entry.ret), sizeof(struct dirent));

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_readdir] xpn_readdir(%p)=%p\n", serv->id, fh_aux->dir, entry);
  }
  /*****************************************/

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_readdir] nfi_sck_server_readdir(%p)=%p\n", serv->id, fh_aux->dir, entry);
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_readdir] >> End\n", serv->id);

  return 0;
}

int nfi_sck_server_closedir ( struct nfi_server * serv, struct nfi_fhandle * fh )
{
  int ret;
  struct st_sck_server_msg msg;
  struct nfi_sck_server_server * server_aux;
  struct nfi_sck_server_fhandle * fh_aux;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_closedir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(fh, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // do closedir...
  if (fh->priv_fh != NULL)
  {
    // private_info...
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_readdir] Get server private info\n", serv->id);

    server_aux = (struct nfi_sck_server_server *)serv->private_info;
    if (server_aux == NULL)
    {
      sck_server_err(SCK_SERVER_ERR_PARAM);
      return -1;
    }

    // private_info file handle
    fh_aux = (struct nfi_sck_server_fhandle *)fh->priv_fh;

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_closedir] nfi_sck_server_closedir(%p)\n", serv->id, fh_aux->dir);

    /************** LOCAL *****************/
    if (server_aux->params.locality)
    {
      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_closedir] real_posix_closedir(%p)\n", serv->id, fh_aux->dir);

      real_posix_closedir(fh_aux->dir);

      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_closedir] real_posix_closedir(%p)=%d\n", serv->id, fh_aux->dir, 0);
    }
    /************** SERVER ****************/
    else
    {
      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_closedir] xpn_closedir(%p)\n", serv->id, fh_aux->dir);

      //bzero(&msg, sizeof(struct st_sck_server_msg));

      msg.type = SCK_SERVER_CLOSEDIR_DIR;
      memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
      msg.u_st_sck_server_msg.op_closedir.dir = fh_aux->dir;

      nfi_sck_server_do_request(server_aux, & msg, (char *)& (ret), sizeof(int));

      debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_closedir] xpn_closedir(%p)=%d\n", serv->id, fh_aux->dir, 0);
    }
    /*****************************************/

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_closedir] nfi_sck_server_closedir(%p)=%d\n", serv->id, fh_aux->dir, 0);

    // free memory
    FREE_AND_NULL(fh->priv_fh);
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_closedir] >> End\n", serv->id);

  return 0;
}

int nfi_sck_server_rmdir ( struct nfi_server * serv, char * url )
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX];
  struct nfi_sck_server_server * server_aux;
  struct st_sck_server_msg msg;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(url, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *)serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, server, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] ERROR: incorrect url '%s'.\n", serv->id, url);
    sck_server_err(SCK_SERVER_ERR_URL);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] nfi_sck_server_rmdir(%s)\n", serv->id, dir);

  /************** LOCAL *****************/
  if (server_aux->params.locality)
  {
    char path [PATH_MAX];

    strcpy(path, server_aux->params.dirbase);
    strcat(path, "/");
    strcat(path, dir);

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] real_posix_rmdir(%s)\n", serv->id, path);

    ret = real_posix_rmdir(path);
    if (ret < 0)
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] ERROR: real_posix_rmdir fails to rm '%s' in server %s.\n", serv->id, path, serv->server);
      return -1;
    }

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] real_posix_rmdir(%s)=%d\n", serv->id, path, ret);
  }
  /************** SERVER ****************/
  else
  {
    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] xpn_rmdir(%s)\n", serv->id, dir);

    //bzero(&msg, sizeof(struct st_sck_server_msg));

    msg.type = SCK_SERVER_RMDIR_DIR;
    memccpy(msg.u_st_sck_server_msg.op_rmdir.path, dir, 0, PATH_MAX - 1);

    nfi_sck_server_do_request(server_aux, & msg, (char *)& (ret), sizeof(int));

    if (ret < 0)
    {
      debug_error("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] ERROR: xpn_rm fails to rm '%s' in server %s.\n", serv->id, dir, serv->server);
      sck_server_err(SCK_SERVER_ERR_REMOVE);
      return -1;
    }

    debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] xpn_rmdir(%s)=%d\n", serv->id, dir, ret);
  }
  /*****************************************/

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] nfi_sck_server_rmdir(%s)=%d\n", serv->id, dir, ret);
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_rmdir] >> End\n", serv->id);

  return 0;
}

int nfi_sck_server_statfs ( __attribute__((__unused__)) struct nfi_server * serv, __attribute__((__unused__)) struct nfi_info * inf)
{
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_statfs] >> Begin\n", serv->id);

  //TODO
  /*
  struct sck_server_info sck_serverinf;
  int ret;
  struct nfi_sck_server_server *server_aux;

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM) ;
  NULL_RET_ERR(inf,  SCK_SERVER_ERR_PARAM) ;
  nfi_sck_server_keepConnected(serv) ;
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM) ;

  // private_info...
  server_aux = (struct nfi_sck_server_server *)serv->private_info;

  ret = sck_server_statfs(server_aux->fh, &sck_serverinf, server_aux->cl) ;
  if (ret <0).{
    sck_server_err(SCK_SERVER_ERR_STATFS) ;
    return -1;
  }

  SCKtoNFIInfo(inf, &sck_serverinf) ;
  */

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_statfs] >> End\n", serv->id);

  return 0;
}

int nfi_sck_server_preload ( struct nfi_server * serv, char * url, char * virtual_path, char * storage_path, int opt )
{
  //char dir[PATH_MAX];
  int ret;
  struct nfi_sck_server_server * server_aux;
  struct st_sck_server_msg msg;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_preload] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(url, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(virtual_path, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(storage_path, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);
  if (url[strlen(url)-1] == '/')
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_preload] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *) serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_preload] nfi_sck_server_preload(%s,%s)\n", serv->id, virtual_path, storage_path);

  //bzero(&msg, sizeof(struct st_sck_server_msg));

  msg.type = SCK_SERVER_PRELOAD_FILE;
  memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
  memccpy(msg.u_st_sck_server_msg.op_preload.virtual_path, virtual_path, 0, PATH_MAX - 1);
  memccpy(msg.u_st_sck_server_msg.op_preload.storage_path, storage_path, 0, PATH_MAX - 1);
  msg.u_st_sck_server_msg.op_preload.block_size = serv->block_size;
  msg.u_st_sck_server_msg.op_preload.opt = opt;

  nfi_sck_server_do_request(server_aux, & msg, (char *)& ret, sizeof(int));

  if (ret < 0) {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_preload] ERROR: preload fails\n", serv->id);
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_preload] nfi_sck_server_preload(%s,%s)=%d\n", serv->id, virtual_path, storage_path, ret);
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_preload] >> End\n", serv->id);

  return ret;
}

int nfi_sck_server_flush ( struct nfi_server * serv, char * url, char * virtual_path, char * storage_path, int opt )
{
  //char dir[PATH_MAX];
  int ret;
  struct nfi_sck_server_server * server_aux;
  struct st_sck_server_msg msg;

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_flush] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(url, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(virtual_path, SCK_SERVER_ERR_PARAM);
  NULL_RET_ERR(storage_path, SCK_SERVER_ERR_PARAM);
  nfi_sck_server_keepConnected(serv);
  NULL_RET_ERR(serv->private_info, SCK_SERVER_ERR_PARAM);
  if (url[strlen(url)-1] == '/')
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_flush] Get server private info\n", serv->id);

  server_aux = (struct nfi_sck_server_server *) serv->private_info;
  if (server_aux == NULL)
  {
    sck_server_err(SCK_SERVER_ERR_PARAM);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_flush] nfi_sck_server_flush(%s,%s)\n", serv->id, virtual_path, storage_path);

  //bzero(&msg, sizeof(struct st_sck_server_msg));

  msg.type = SCK_SERVER_FLUSH_FILE;
  memccpy(msg.id, server_aux->id, 0, SCK_SERVER_ID - 1);
  memccpy(msg.u_st_sck_server_msg.op_flush.virtual_path, virtual_path, 0, PATH_MAX - 1);
  memccpy(msg.u_st_sck_server_msg.op_flush.storage_path, storage_path, 0, PATH_MAX - 1);
  msg.u_st_sck_server_msg.op_flush.block_size = serv->block_size;
  msg.u_st_sck_server_msg.op_flush.opt = opt;

  nfi_sck_server_do_request(server_aux, & msg, (char *)& ret, sizeof(int));

  if (ret < 0) {
    printf("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_flush] ERROR: preload fails\n", serv->id);
  }

  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_flush] nfi_sck_server_flush(%s,%s)=%d\n", serv->id, virtual_path, storage_path, ret);
  debug_info("[SERV_ID=%d] [NFI_SCK] [nfi_sck_server_flush] >> End\n", serv->id);

  return 0;
}


/* ................................................................... */
