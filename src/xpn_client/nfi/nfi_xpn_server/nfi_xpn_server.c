
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

#include "nfi_xpn_server.h"
#include "nfi_xpn_server_comm.h"
#include "xpn_server/xpn_server_ops.h"


/* ... Const / Const ................................................. */

#define FILESYSTEM_DLSYM 1


/* ... Global variables / Variables globales ........................ */


/* ... Auxiliar Functions / Funciones Auxiliares ..................... */

//Communication
int nfi_write_operation ( struct nfi_xpn_server *params, struct st_xpn_server_msg *head )
{
  int ret;

  debug_info("[NFI_XPN] [nfi_write_operation] >> Begin\n");

  debug_info("[NFI_XPN] [nfi_write_operation] Send operation\n");

  ret = nfi_xpn_server_comm_write_operation(params, head->type);
  if (ret < 0)
  {
    printf("[NFI_XPN] [nfi_write_operation] ERROR: nfi_write_operation fails\n");
    return -1;
  }

  debug_info("[NFI_XPN] [nfi_write_operation] Execute operation: %d -> \n", head->type);

  switch (head->type)
  {
    //File API
    case XPN_SERVER_OPEN_FILE:
      debug_info("[NFI_XPN] [nfi_write_operation] OPEN operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_open), sizeof(struct st_xpn_server_path_flags));
      break;
    case XPN_SERVER_CREAT_FILE:
      debug_info("[NFI_XPN] [nfi_write_operation] CREAT operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_creat), sizeof(struct st_xpn_server_path_flags));
      break;
    case XPN_SERVER_READ_FILE:
      debug_info("[NFI_XPN] [nfi_write_operation] READ operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_read), sizeof(struct st_xpn_server_rw));
      break;
    case XPN_SERVER_WRITE_FILE:
      debug_info("[NFI_XPN] [nfi_write_operation] WRITE operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_write), sizeof(struct st_xpn_server_rw));
      break;
    case XPN_SERVER_CLOSE_FILE:
      debug_info("[NFI_XPN] [nfi_write_operation] CLOSE operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_close), sizeof(struct st_xpn_server_path));
      break;
    case XPN_SERVER_RM_FILE:
      debug_info("[NFI_XPN] [nfi_write_operation] RM operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_rm), sizeof(struct st_xpn_server_path));
      break;
    case XPN_SERVER_RM_FILE_ASYNC:
      debug_info("[NFI_XPN] [nfi_write_operation] RM_ASYNC operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_rm), sizeof(struct st_xpn_server_path));
      break;
    case XPN_SERVER_RENAME_FILE:
      debug_info("[NFI_XPN] [nfi_write_operation] RENAME operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_rename), sizeof(struct st_xpn_server_rename));
      break;
    case XPN_SERVER_GETATTR_FILE:
      debug_info("[NFI_XPN] [nfi_write_operation] GETATTR operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_getattr), sizeof(struct st_xpn_server_path));
      break;

    //Directory API
    case XPN_SERVER_MKDIR_DIR:
      debug_info("[NFI_XPN] [nfi_write_operation] MDKIR operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_mkdir), sizeof(struct st_xpn_server_path_flags));
      break;
    case XPN_SERVER_OPENDIR_DIR:
      debug_info("[NFI_XPN] [nfi_write_operation] OPENDIR operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_opendir), sizeof(struct st_xpn_server_path));
      break;
    case XPN_SERVER_READDIR_DIR:
      debug_info("[NFI_XPN] [nfi_write_operation] READDIR operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_readdir), sizeof(struct st_xpn_server_readdir));
      break;
    case XPN_SERVER_CLOSEDIR_DIR:
      debug_info("[NFI_XPN] [nfi_write_operation] CLOSEDIR operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_closedir), sizeof(struct st_xpn_server_path));
      break;
    case XPN_SERVER_RMDIR_DIR:
      debug_info("[NFI_XPN] [nfi_write_operation] RMDIR operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_rmdir), sizeof(struct st_xpn_server_path));
      break;
    case XPN_SERVER_RMDIR_DIR_ASYNC:
      debug_info("[NFI_XPN] [nfi_write_operation] RMDIR_ASYNC operation\n");
      ret = nfi_xpn_server_comm_write_data(params, (char *)&(head->u_st_xpn_server_msg.op_rmdir), sizeof(struct st_xpn_server_path));
      break;
  }

  debug_info("[NFI_XPN] [nfi_write_operation] >> End\n");

  return ret;
}

int nfi_xpn_server_do_request ( struct nfi_xpn_server *server_aux, struct st_xpn_server_msg *msg, char *req, int req_size )
{
  ssize_t ret;
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_do_request] >> Begin\n", 0);

  // send request...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_do_request] Send operation: %d\n", 0, msg->type);

  ret = nfi_write_operation(server_aux, msg);
  if (ret < 0) {
    return -1;
  }

  // read response...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_do_request] Response operation: %d\n", 0, msg->type);

  ret = nfi_xpn_server_comm_read_data(server_aux, req, req_size);
  if (ret < 0) {
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_do_request] >> End\n", 0);

  return 0;
}

int nfi_xpn_server_keep_connected ( struct nfi_server *serv )
{
  // check params...
  if (serv == NULL)
  {
    printf("[SERV_ID=%d] [NFI_n] [nfi_xpn_server_keep_connected] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_keep_connected] >> Begin\n", serv->id);

#ifdef NFI_DYNAMIC
  if (serv->private_info == NULL)
  {
    debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_keep_connected] Server reconnect\n", 0);

    int ret = nfi_xpn_server_reconnect(serv);
    if (ret < 0)
    {
      serv->private_info = NULL;
      return -1;
    }
  }
#endif

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_keep_connected] >> End\n", serv->id);

  return (serv->private_info != NULL);
}

void nfi_2_xpn_attr ( struct stat *att, struct nfi_attr *nfi_att )
{
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_2_xpn_attr] >> Begin\n", -1);

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

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_2_xpn_attr] >> End\n", -1);
}

void xpn_2_nfi_attr ( struct nfi_attr *nfi_att, struct stat *att )
{
  debug_info("[SERV_ID=%d] [NFI_XPN] [xpn_2_nfi_attr] >> Begin\n", -1);

  nfi_att->st_dev = att->st_dev;
  nfi_att->st_ino = att->st_ino;

  if (S_ISREG(att->st_mode)) {
    nfi_att->at_type = NFIFILE;
  }
  if (S_ISDIR(att->st_mode)) {
    nfi_att->at_type = NFIDIR;
  }

  nfi_att->at_mode    = att->st_mode&(S_IRWXU|S_IRWXG|S_IRWXO); // protection
  nfi_att->at_nlink   = att->st_nlink;                          // number of hard links
  nfi_att->at_uid     = att->st_uid;                            // user ID of owner
  nfi_att->at_gid     = att->st_gid;                            // group ID of owner
  nfi_att->at_size    = att->st_size;                           // total size, in bytes
  nfi_att->at_blksize = att->st_blksize;                        // blocksize for filesystem I/O
  nfi_att->at_blocks  = att->st_blocks;                         // number of blocks allocated
  nfi_att->at_atime   = att->st_atime;                          // time of last access
  nfi_att->at_mtime   = att->st_mtime;                          // time of last modification
  nfi_att->at_ctime   = att->st_ctime;                          // time of last change

  debug_info("[SERV_ID=%d] [NFI_XPN] [xpn_2_nfi_attr] >> End\n", -1);
}

void xpn_2_nfi_info( __attribute__((__unused__)) struct nfi_info *nfi_inf, __attribute__((__unused__)) struct nfi_info *xpn_inf )
{
  debug_info("[SERV_ID=%d] [NFI_XPN] [xpn_2_nfi_info] >> Begin\n", -1);

  //TODO

  debug_info("[SERV_ID=%d] [NFI_XPN] [xpn_2_nfi_info] >> End\n", -1);
}


/* ... Functions / Funciones ......................................... */

int nfi_xpn_server_init ( char *url, struct nfi_server *serv, int server_type )
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX], prt[PATH_MAX];
  struct nfi_xpn_server *server_aux;
  char hostname[HOST_NAME_MAX];
  char *hostip;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_n] [nfi_xpn_server_init] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] >> Begin\n", serv->id);

  // new nfi_ops with xpn functions...
  serv->ops = (struct nfi_ops *)malloc(sizeof(struct nfi_ops));
  NULL_RET_ERR(serv->ops, ENOMEM);

  // Fill serv->ops...
  bzero(serv->ops, sizeof(struct nfi_ops));
  serv->ops->nfi_reconnect  = nfi_xpn_server_reconnect;
  serv->ops->nfi_disconnect = nfi_xpn_server_disconnect;

  serv->ops->nfi_open       = nfi_xpn_server_open;
  serv->ops->nfi_create     = nfi_xpn_server_create;
  serv->ops->nfi_read       = nfi_xpn_server_read;
  serv->ops->nfi_write      = nfi_xpn_server_write;
  serv->ops->nfi_close      = nfi_xpn_server_close;
  serv->ops->nfi_remove     = nfi_xpn_server_remove;
  serv->ops->nfi_rename     = nfi_xpn_server_rename;
  serv->ops->nfi_getattr    = nfi_xpn_server_getattr;
  serv->ops->nfi_setattr    = nfi_xpn_server_setattr;

  serv->ops->nfi_opendir    = nfi_xpn_server_opendir;
  serv->ops->nfi_mkdir      = nfi_xpn_server_mkdir;
  serv->ops->nfi_readdir    = nfi_xpn_server_readdir;
  serv->ops->nfi_closedir   = nfi_xpn_server_closedir;
  serv->ops->nfi_rmdir      = nfi_xpn_server_rmdir;

  serv->ops->nfi_statfs     = nfi_xpn_server_statfs;

  // parse url...
  ret = ParseURL(url, prt, NULL, NULL, server, NULL, dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] ERROR: incorrect url '%s'.\n", serv->id, url);
    FREE_AND_NULL(serv->ops);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  // new nfi_XPNserver...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] Copy nfi_xpn_server structure\n", serv->id);

  server_aux = (struct nfi_xpn_server *)malloc(sizeof(struct nfi_xpn_server));
  if (server_aux == NULL)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] ERROR: out of memory.\n", serv->id);
    FREE_AND_NULL(serv->ops);
    return -1;
  }

  // private_info...
  serv->private_info = (void *)server_aux;

  // Initialize params
  memset(server_aux, 0, sizeof(struct nfi_xpn_server));

  server_aux->server_type = server_type;

  // thread mode
  server_aux->xpn_thread = TH_NOT;
  serv->xpn_thread = TH_NOT;
  char * env_thread = getenv("XPN_THREAD");
  if (env_thread != NULL)
  {
    server_aux->xpn_thread = atoi(env_thread);
    serv->xpn_thread = atoi(env_thread);
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] Thread mode: %d\n", serv->id, serv->xpn_thread);

  // Locality mode
  server_aux->xpn_locality = 1;
  char * env_locality = getenv("XPN_LOCALITY");
  if (env_locality != NULL) {
    server_aux->xpn_locality = atoi(env_locality);
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] Locality enable: %d\n", serv->id, server_aux->xpn_locality);

  // Initialize MPI Client communication side...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] Initialize MPI Client communication side\n", serv->id);

  ret = nfi_xpn_server_comm_init(server_aux);
  if (ret < 0)
  {
    FREE_AND_NULL(serv->ops);
    FREE_AND_NULL(server_aux);
    return -1;
  }
  
  // copy 'url' string...
  serv->url = strdup(url);
  NULL_RET_ERR(serv->url, ENOMEM);

  // new server wrk...
  serv->wrk = (struct nfi_worker *)malloc(sizeof(struct nfi_worker));
  memset(serv->wrk, 0, sizeof(struct nfi_worker));
  serv->wrk->server = serv;

  // Server conection
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] Server conection\n", serv->id);

  ret = nfi_xpn_server_connect(serv, url, prt, server, dir);
  if (ret < 0)
  {
    FREE_AND_NULL(serv->ops);
    FREE_AND_NULL(server_aux);
    return -1;
  }

  // Check locality
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] Data locality\n", serv->id);

  if (server_aux->xpn_locality == 1)
  {
    hostip = ns_get_host_ip();
    ns_get_hostname(hostname);
    if (strstr(server, hostip) != NULL || strstr(server, hostname) != NULL)
    {
      XPN_DEBUG("Locality in serv_url: %s client: %s hostname: %s", server, hostip, hostname);

      // free private_info, 'url' string and 'server' string...
      FREE_AND_NULL(serv->ops);
      void * aux_private_info = serv->private_info;
      ret = nfi_local_init( url, serv, NULL );
      struct nfi_local_server * priv = (struct nfi_local_server *)serv->private_info;
      priv->private_info_server = aux_private_info;
      return ret;
    }
  }

  // Initialize workers
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] Initialize workers\n", serv->id);

  ret = nfiworker_init(serv);

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] << End\n", serv->id);

  return 0;
}

int nfi_xpn_server_destroy ( struct nfi_server *serv )
{
  int ret;
  struct nfi_xpn_server *server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_n] [nfi_xpn_server_destroy] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_destroy] >> Begin\n", serv->id);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_destroy] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *)serv->private_info;
  if (server_aux == NULL) {
    return 0;
  }

  // Workers destroy...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_destroy] Destroy workers\n", serv->id);

  nfiworker_destroy(serv);

  // MPI Finalize...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_destroy] Destroy MPI Client communication\n", serv->id);

  ret = nfi_xpn_server_comm_destroy( server_aux );
  if (ret < 0) {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_init] ERROR: nfi_xpn_server_comm_destroy fails\n", serv->id);
  }

  // free private_info, 'url' string and 'server' string...
  FREE_AND_NULL(serv->ops);
  FREE_AND_NULL(serv->private_info);
  FREE_AND_NULL(serv->url);
  FREE_AND_NULL(serv->server);

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_destroy] << End\n", serv->id);

  return 0;
}

int nfi_xpn_server_connect ( struct nfi_server *serv, __attribute__((__unused__)) char *url, __attribute__((__unused__)) char* prt, char* server, __attribute__((__unused__)) char* dir )
{
  int ret;
  struct nfi_xpn_server *server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_n] [nfi_xpn_server_connect] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_connect] >> Begin\n", serv->id);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_connect] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *) (serv->private_info);
  if (server_aux == NULL) {
    return -1;
  }

  // Copy server name
  strcpy(server_aux->srv_name, server);

  // MPI Connection...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_connect] XPN server conection\n", serv->id);

  ret = nfi_xpn_server_comm_connect(server_aux);
  if (ret < 0) {
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_connect] << End\n", serv->id);

  return 0;
}

int nfi_xpn_server_disconnect ( struct nfi_server *serv )
{
  int ret;
  struct nfi_xpn_server *server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_n] [nfi_xpn_server_disconnect] ERROR: serv argument is NULL\n", -1);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_disconnect] >> Begin\n", serv->id);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_disconnect] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *)serv->private_info;
  if (server_aux == NULL) {
    return 0;
  }

  // MPI Disconnect...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_disconnect] XPN server disconnect\n", serv->id);

  ret = nfi_xpn_server_comm_disconnect( server_aux );
  if (ret < 0) {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_disconnect] ERROR: nfi_xpn_server_comm_disconnect fails\n", serv->id);
  }

  FREE_AND_NULL(serv->private_info);

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_disconnect] << End\n", serv->id);

  return 0;
}

int nfi_xpn_server_reconnect(struct nfi_server *serv)
{
  int    ret;
  char   server[PATH_MAX], dir[PATH_MAX];
  struct nfi_xpn_server *server_aux;

  // check params...
  if (serv == NULL) {
    printf("[SERV_ID=%d] [NFI_n] [nfi_xpn_server_reconnect] ERROR: serv argument is NULL\n", -1);
    errno = EINVAL;
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_reconnect] >> Begin\n", serv->id);

  // parse url...
  ret = ParseURL(serv->url,  NULL, NULL, NULL, server,  NULL,  dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_reconnect] ERROR: url %s incorrect.\n", serv->id, serv->url);
    errno = EINVAL;
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_reconnect] ParseURL(%s)= %s; %s\n", serv->id, serv->url, server, dir);

  // new nfi_XPNserver...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_reconnect] Copy nfi_xpn_server structure\n", serv->id);

  server_aux = (struct nfi_xpn_server *)malloc(sizeof(struct nfi_xpn_server));
  NULL_RET_ERR(server_aux, ENOMEM);

  serv->private_info = (void *)server_aux;

  // reconnect...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_reconnect] MPI server conection\n", serv->id);

  ret = nfi_xpn_server_comm_connect(server_aux);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_reconnect] ERROR: nfi_xpn_server_comm_connect fails\n", serv->id);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_reconnect] << End\n", serv->id);

  return 0;
}

// File API
int nfi_xpn_server_open ( struct nfi_server *serv,  char *url, int flags, mode_t mode, struct nfi_fhandle *fho )
{
  int    ret;
  char   dir[PATH_MAX], server[PATH_MAX];
  struct nfi_xpn_server *server_aux;
  struct nfi_xpn_server_fhandle *fh_aux;
  struct st_xpn_server_msg msg;
  struct st_xpn_server_status status;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_open] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, EINVAL);
  NULL_RET_ERR(fho,  EINVAL);
  nfi_xpn_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_open] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *) serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_open] ERROR: incorrect url '%s'.\n", serv->id, url);
    errno = EINVAL;
    return -1;
  }
  
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_open] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  //Copy url
  fho->url = strdup(url);
  NULL_RET_ERR(fho->url, ENOMEM);

  fh_aux = (struct nfi_xpn_server_fhandle *)malloc(sizeof(struct nfi_xpn_server_fhandle));
  NULL_RET_ERR(fh_aux, ENOMEM);
  bzero(fh_aux, sizeof(struct nfi_xpn_server_fhandle));

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_open] nfi_xpn_server_open(%s)\n", serv->id, dir);

  msg.type = XPN_SERVER_OPEN_FILE;

  memccpy(msg.u_st_xpn_server_msg.op_open.path, dir,            0, PATH_MAX-1);
  msg.u_st_xpn_server_msg.op_open.flags = flags;
  msg.u_st_xpn_server_msg.op_open.mode = mode;

  nfi_xpn_server_do_request(server_aux, &msg, (char *)&(status), sizeof(struct st_xpn_server_status));
  if (status.ret < 0)
  { 
    errno = status.server_errno;
    debug_error("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_open] ERROR: remote open fails to open '%s' in server %s.\n", serv->id, dir, serv->server);
    FREE_AND_NULL(fh_aux);
    FREE_AND_NULL(fho->url);
    return -1;
  }

  memccpy(fh_aux->path, dir, 0, PATH_MAX-1);

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_open] nfi_xpn_server_open(%s)\n", serv->id, dir);

  fho->type    = NFIFILE;
  fho->priv_fh = NULL;
  fho->server  = serv;
  fho->priv_fh = (void *) fh_aux;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_open] >> End\n", serv->id);

  return 0;
}

int nfi_xpn_server_create (struct nfi_server *serv,  char *url, mode_t mode, __attribute__((__unused__)) struct nfi_attr *attr, struct nfi_fhandle  *fh)
{
  //NOTE: actualy creat is not in use, it use like POSIX open(path, O_WRONLY|O_CREAT|O_TRUNC, mode);
  return nfi_xpn_server_open(serv, url, O_WRONLY|O_CREAT|O_TRUNC, mode, fh);
}

ssize_t nfi_xpn_server_read ( struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size )
{
  int    ret, cont, diff;
  struct nfi_xpn_server *server_aux;
  struct nfi_xpn_server_fhandle *fh_aux;
  struct st_xpn_server_msg msg;
  struct st_xpn_server_rw_req req;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, EINVAL);
  NULL_RET_ERR(fh,   EINVAL);
  nfi_xpn_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *) serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }

  // private_info file handle
  fh_aux = (struct nfi_xpn_server_fhandle *) fh->priv_fh;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] nfi_xpn_server_read(%s, %ld, %ld)\n", serv->id, fh_aux->path, offset, size);

  //Debug
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] xpn_read(%s, %ld, %ld)\n", serv->id, msg.u_st_xpn_server_msg.op_read.path, offset, size);

  msg.type = XPN_SERVER_READ_FILE;
  memccpy(msg.u_st_xpn_server_msg.op_read.path, fh_aux->path, 0, PATH_MAX);
  
  msg.u_st_xpn_server_msg.op_read.offset   = offset;
  msg.u_st_xpn_server_msg.op_read.size     = size;

  ret = nfi_write_operation(server_aux, &msg);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] ERROR: nfi_write_operation fails\n", serv->id);
    return -1;
  }

  // read n times: number of bytes + read data (n bytes)
  cont = 0;
  do
  {
    ret = nfi_xpn_server_comm_read_data(server_aux, (char *)&req, sizeof(struct st_xpn_server_rw_req));
    if (ret < 0)
    {
      printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] ERROR: nfi_xpn_server_comm_read_data fails\n", serv->id);
      return -1;
    }

    if (req.status.ret < 0){
      errno = req.status.server_errno;
      return -1;
    }
    
    debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] nfi_xpn_server_comm_read_data=%d.\n",serv->id, ret);

    if (req.size > 0)
    {
      debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] nfi_xpn_server_comm_read_data(%ld)\n",serv->id, req.size);

      ret = nfi_xpn_server_comm_read_data(server_aux, (char *)buffer+cont, req.size);
      if (ret < 0) {
        printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] ERROR: nfi_xpn_server_comm_read_data fails\n", serv->id);
      }

      debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] nfi_xpn_server_comm_read_data(%ld)=%d\n",serv->id, req.size, ret);
    }
    cont = cont + req.size;
    diff = msg.u_st_xpn_server_msg.op_read.size - cont;

  } while ((diff > 0) && (req.size != 0));

  if (req.size < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] ERROR: nfi_xpn_server_read reads zero bytes from '%s' in server %s\n", serv->id, fh_aux->path, serv->server);
    if (req.status.ret < 0)
      errno = req.status.server_errno;
    return -1;
  }

  if (req.status.ret < 0)
    errno = req.status.server_errno;

  ret = cont;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] nfi_xpn_server_read(%s, %ld, %ld)=%d\n", serv->id, fh_aux->path, offset, size, ret);
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_read] >> End\n", serv->id);

  return ret;
}

ssize_t nfi_xpn_server_write ( struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size )
{
  int ret, diff, cont;
  struct nfi_xpn_server *server_aux;
  struct nfi_xpn_server_fhandle *fh_aux;
  struct st_xpn_server_msg msg;
  struct st_xpn_server_rw_req req;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] >> Begin\n", serv->id);

  // Check arguments...
  if (size == 0) {
    return 0;
  }

  NULL_RET_ERR(serv, EINVAL);
  NULL_RET_ERR(fh,   EINVAL);
  nfi_xpn_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *) serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }
  
  // private_info file handle
  fh_aux     = (struct nfi_xpn_server_fhandle *) fh->priv_fh;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] nfi_xpn_server_write(%s, %ld, %ld)\n", serv->id, fh_aux->path, offset, size);

  //Debug
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] xpn_write(%s, %ld, %ld)\n", serv->id, msg.u_st_xpn_server_msg.op_read.path, offset, size);

  msg.type = XPN_SERVER_WRITE_FILE;
  memccpy(msg.u_st_xpn_server_msg.op_write.path, fh_aux->path, 0, PATH_MAX-1);
  msg.u_st_xpn_server_msg.op_write.offset = offset;
  msg.u_st_xpn_server_msg.op_write.size   = size;

  ret = nfi_write_operation(server_aux, &msg);
  if(ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] ERROR: nfi_write_operation fails\n", serv->id);
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
      ret = nfi_xpn_server_comm_write_data(server_aux, (char *)buffer + cont, buffer_size);
      if (ret < 0) {
        printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] ERROR: nfi_xpn_server_comm_write_data fails\n", serv->id);
      }
    }
    else
    {
      ret = nfi_xpn_server_comm_write_data(server_aux, (char *)buffer + cont, diff);
      if (ret < 0) {
        printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] ERROR: nfi_xpn_server_comm_write_data fails\n", serv->id);
      }
    }

    debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] nfi_xpn_server_comm_write_data=%d.\n",serv->id, ret);

    cont = cont + ret; //Send bytes
    diff = size - cont;

  } while ((diff > 0) && (ret != 0));

  ret = nfi_xpn_server_comm_read_data(server_aux, (char *)&req, sizeof(struct st_xpn_server_rw_req));
  if (ret < 0) 
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] ERROR: nfi_xpn_server_comm_read_data fails\n", serv->id);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] nfi_xpn_server_comm_read_data=%d.\n",serv->id, ret);

  if (req.size < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] ERROR: nfi_xpn_server_write writes zero bytes from '%s' in server %s\n", serv->id, fh_aux->path, serv->server);
    if (req.status.ret < 0)
      errno = req.status.server_errno;
    return -1;
  }

  if (req.status.ret < 0)
    errno = req.status.server_errno;

  ret = cont;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] nfi_xpn_server_write(%s, %ld, %ld)=%d\n", serv->id, fh_aux->path, offset, size, ret);
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_write] >> End\n", serv->id);

  return ret;
}

int nfi_xpn_server_close ( __attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_fhandle *fh )
{
  // Without sesion close do nothing
  return 0;
}

int nfi_xpn_server_remove ( struct nfi_server *serv,  char *url )
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX];
  struct nfi_xpn_server *server_aux;
  struct st_xpn_server_msg msg;
  struct st_xpn_server_status status;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_remove] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, EINVAL);
  NULL_RET_ERR(url,  EINVAL);
  nfi_xpn_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_remove] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *) serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
  if(ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_remove] ERROR: incorrect url '%s'.\n", serv->id, url);
    errno = EINVAL;
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_remove] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_remove] nfi_xpn_server_remove(%s)\n", serv->id, dir);

  memccpy(msg.u_st_xpn_server_msg.op_rm.path, dir, 0, PATH_MAX-1);

  if ((serv -> wrk -> arg.is_master_node) == 1)
  {
    msg.type = XPN_SERVER_RM_FILE;
    nfi_xpn_server_do_request(server_aux, &msg, (char *)&(status), sizeof(struct st_xpn_server_status));
    if (status.ret < 0)
      errno = status.server_errno;
  }
  else
  {
    msg.type = XPN_SERVER_RM_FILE_ASYNC;

    // send request...
    ret = nfi_write_operation(server_aux, &msg);
    if (ret >= 0) {
      return 0;
    }
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_remove] nfi_xpn_server_remove(%s)=%d\n", serv->id, dir, ret);
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_remove] >> End\n", serv->id);

  return ret;
}

int nfi_xpn_server_rename ( struct nfi_server *serv,  char *old_url, char *new_url )
{
  int ret;
  char server[PATH_MAX], old_path[PATH_MAX], new_path[PATH_MAX];
  struct nfi_xpn_server *server_aux;
  struct st_xpn_server_msg msg;
  struct st_xpn_server_status status;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rename] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv,     EINVAL);
  NULL_RET_ERR(old_url,  EINVAL);
  NULL_RET_ERR(new_url,  EINVAL);
  nfi_xpn_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rename] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *) serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(old_url, NULL, NULL, NULL, server,  NULL,  old_path);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rename] ERROR: incorrect url '%s'.\n", serv->id, old_url);
    errno = EINVAL;
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rename] ParseURL(%s)= %s; %s\n", serv->id, old_url, server, old_path);

  // from url -> server + dir
  ret = ParseURL(new_url, NULL, NULL, NULL, server,  NULL,  new_path);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rename] ERROR: incorrect url '%s'.\n", serv->id, new_url);
    errno = EINVAL;
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rename] ParseURL(%s)= %s; %s\n", serv->id, new_path, server, new_path);

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rename] nfi_xpn_server_rename(%s,%s)\n", serv->id, old_path, new_path);

  msg.type = XPN_SERVER_RENAME_FILE;
  memccpy(msg.u_st_xpn_server_msg.op_rename.old_url, old_path,       0, PATH_MAX-1);
  memccpy(msg.u_st_xpn_server_msg.op_rename.new_url, new_path,       0, PATH_MAX-1);

  nfi_xpn_server_do_request(server_aux, &msg, (char *)&(status), sizeof(struct st_xpn_server_status));
  if (status.ret < 0)
    errno = status.server_errno;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rename] xpn_rename(%s, %s)=%d\n", serv->id, old_path, new_path, ret);

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rename] nfi_xpn_server_rename(%s,%s)=%d\n", serv->id, old_path, new_path, ret);
  debug_info("[NFI_XPN] [nfi_xpn_server_rename] >> End\n");

  return ret;
}

int nfi_xpn_server_getattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX];
  struct nfi_xpn_server *server_aux;
  struct st_xpn_server_msg msg;
  struct st_xpn_server_attr_req req;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_getattr] >> Begin\n", serv->id);

  // check arguments...
  NULL_RET_ERR(serv,               EINVAL);
  NULL_RET_ERR(fh,                 EINVAL);
  NULL_RET_ERR(attr,               EINVAL);
  nfi_xpn_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_getattr] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server  *) serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(fh->url, NULL, NULL, NULL, server,  NULL,  dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_getattr] ERROR: incorrect url '%s'.\n", serv->id, fh->url);
    errno = EINVAL;
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_getattr] ParseURL(%s)= %s; %s\n", serv->id, fh->url, server, dir);
  
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_getattr] nfi_xpn_server_getattr(%s)\n", serv->id, dir);


  msg.type = XPN_SERVER_GETATTR_FILE;
  memccpy(msg.u_st_xpn_server_msg.op_getattr.path, dir,            0, PATH_MAX-1);

  nfi_xpn_server_do_request(server_aux, &msg, (char *)&req, sizeof(struct st_xpn_server_attr_req));

  if (req.status_req.ret < 0)
      errno = req.status_req.server_errno;
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_getattr] nfi_xpn_server_getattr(%s)=%d\n", serv->id, dir, req.status);

  xpn_2_nfi_attr(attr, &req.attr);

  debug_info("[NFI_XPN] [nfi_xpn_server_getattr] >> End\n");

  return req.status;
}

int nfi_xpn_server_setattr ( struct nfi_server *serv,  struct nfi_fhandle *fh, struct nfi_attr *attr )
{
  struct nfi_xpn_server *server_aux;
  struct nfi_xpn_server_fhandle *fh_aux;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_setattr] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, EINVAL);
  NULL_RET_ERR(fh,   EINVAL);
  NULL_RET_ERR(attr, EINVAL);
  NULL_RET_ERR(fh->priv_fh, EINVAL);
  nfi_xpn_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_setattr] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *) serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }
  
  // private_info file handle
  fh_aux = (struct nfi_xpn_server_fhandle *) fh->priv_fh;

  // TODO: setattr
  server_aux = server_aux;
  fh_aux     = fh_aux;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_setattr] >> End\n", serv->id);

  return 0;
}

// Directories API
int nfi_xpn_server_mkdir(struct nfi_server *serv,  char *url, mode_t mode, __attribute__((__unused__)) struct nfi_attr *attr, __attribute__((__unused__)) struct nfi_fhandle *fh)
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX];
  struct nfi_xpn_server *server_aux;
  struct st_xpn_server_msg msg;
  struct st_xpn_server_status status;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_mkdir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, EINVAL);
  nfi_xpn_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_mkdir] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *)serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_mkdir] ERROR: incorrect url '%s'.\n", serv->id, url);
    errno = EINVAL;
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_mkdir] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_mkdir] nfi_xpn_server_mkdir(%s)\n", serv->id, dir);

  msg.type = XPN_SERVER_MKDIR_DIR;
  msg.u_st_xpn_server_msg.op_mkdir.mode = mode;
  memccpy(msg.u_st_xpn_server_msg.op_mkdir.path, dir, 0, PATH_MAX-1);

  nfi_xpn_server_do_request(server_aux, &msg, (char *)&(status), sizeof(struct st_xpn_server_status));

  if (status.ret <0)
    errno = status.server_errno;

  if ((status.ret < 0)&&(errno != EEXIST))
  {
    debug_error("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_mkdir] ERROR: xpn_mkdir fails to mkdir '%s' in server %s.\n", serv->id, dir, serv->server);
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_mkdir] nfi_xpn_server_mkdir(%s)=%d\n", serv->id, dir, ret);

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_mkdir] >> End\n", serv->id);

  return ret;
}

int nfi_xpn_server_opendir(struct nfi_server *serv,  char *url, struct nfi_fhandle *fho)
{
  int ret;
  char dir[PATH_MAX], server[PATH_MAX];
  struct nfi_xpn_server *server_aux;
  struct nfi_xpn_server_fhandle *fh_aux;
  struct st_xpn_server_msg msg;
  struct st_xpn_server_status status;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_opendir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, EINVAL);
  NULL_RET_ERR(url,  EINVAL);
  NULL_RET_ERR(fho,  EINVAL);
  nfi_xpn_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_opendir] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *) serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(url, NULL, NULL, NULL, server,  NULL,  dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_opendir] ERROR: incorrect url '%s'.\n", serv->id, url);
    errno = EINVAL;
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_opendir] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  // Copy url
  fho->url = strdup(url);
  NULL_RET_ERR(fho->url, ENOMEM);

  // private_info file handle
  fh_aux = (struct nfi_xpn_server_fhandle *)malloc(sizeof(struct nfi_xpn_server_fhandle));
  NULL_RET_ERR(fh_aux, ENOMEM);
  bzero(fh_aux, sizeof(struct nfi_xpn_server_fhandle));

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_opendir] nfi_xpn_server_opendir(%s)\n", serv->id, dir);
  
  msg.type = XPN_SERVER_OPENDIR_DIR;
  memccpy(msg.u_st_xpn_server_msg.op_opendir.path, dir,            0, PATH_MAX-1);

  nfi_xpn_server_do_request(server_aux, &msg, (char *)&(status), sizeof(struct st_xpn_server_status));
  if (status.ret < 0)
  {
    errno = status.server_errno;
    return -1;
  }

  fh_aux->telldir = status.ret;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_opendir] nfi_xpn_server_opendir(%s)\n", serv->id, dir);

  strcpy(fh_aux->path, dir);
  fho->type = NFIDIR;
  fho->server = serv;
  fho->priv_fh = (void *) fh_aux;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_opendir] >> End\n", serv->id);

  return 0;
}

int nfi_xpn_server_readdir(struct nfi_server *serv,  struct nfi_fhandle *fh, struct dirent *entry )
{
  struct nfi_xpn_server *server_aux;
  struct nfi_xpn_server_fhandle *fh_aux;
  struct st_xpn_server_msg msg;
  struct st_xpn_server_readdir_req ret_entry;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_readdir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv,        EINVAL);
  NULL_RET_ERR(fh,          EINVAL);
  NULL_RET_ERR(fh->priv_fh, EINVAL);
  if (fh->type != NFIDIR)
  {
    errno = ENOTDIR;
    return -1;
  }
  nfi_xpn_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_readdir] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *)serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }

  // private_info file handle
  fh_aux = (struct nfi_xpn_server_fhandle *)fh->priv_fh;

  // clean all entry content
  memset(entry, 0, sizeof(struct dirent));

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_readdir] nfi_xpn_server_readdir(%s)\n", serv->id, fh_aux->path);

  msg.type = XPN_SERVER_READDIR_DIR;
  memccpy(msg.u_st_xpn_server_msg.op_readdir.path, fh_aux->path,            0, PATH_MAX-1);
  msg.u_st_xpn_server_msg.op_readdir.telldir = fh_aux->telldir;

  nfi_xpn_server_do_request(server_aux, &msg, (char *)&(ret_entry), sizeof(struct st_xpn_server_readdir_req));
  
  if (ret_entry.status.ret < 0)
    errno = ret_entry.status.server_errno;
  else
    fh_aux->telldir = ret_entry.telldir;
  
  if (ret_entry.end == 0) {
    return -1;
  }

  memcpy(entry, &(ret_entry.ret), sizeof(struct dirent));

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_readdir] nfi_xpn_server_readdir(%s)=%p\n", serv->id, fh_aux->path, entry);
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_readdir] >> End\n", serv->id);

  return 0;
}

int nfi_xpn_server_closedir ( __attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__)) struct nfi_fhandle *fh )
{
  // Without sesion close do nothing
  return 0;
}

int nfi_xpn_server_rmdir(struct nfi_server *serv,  char *url)
{
  int ret;
  char server[PATH_MAX], dir[PATH_MAX];
  struct nfi_xpn_server *server_aux;
  struct st_xpn_server_msg msg;
  struct st_xpn_server_status status;

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rmdir] >> Begin\n", serv->id);

  // Check arguments...
  NULL_RET_ERR(serv, EINVAL);
  NULL_RET_ERR(url,  EINVAL);
  nfi_xpn_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rmdir] Get server private info\n", serv->id);

  server_aux = (struct nfi_xpn_server *)serv->private_info;
  if (server_aux == NULL)
  {
    errno = EINVAL;
    return -1;
  }

  // from url -> server + dir
  ret = ParseURL(url,  NULL, NULL, NULL, server,  NULL,  dir);
  if (ret < 0)
  {
    printf("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rmdir] ERROR: incorrect url '%s'.\n", serv->id, url);
    errno = EINVAL;
    return -1;
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rmdir] ParseURL(%s)= %s; %s\n", serv->id, url, server, dir);

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rmdir] nfi_xpn_server_rmdir(%s)\n", serv->id, dir);

  memccpy(msg.u_st_xpn_server_msg.op_rmdir.path, dir, 0, PATH_MAX-1);

  if ((serv -> wrk -> arg.is_master_node) == 1)
  {
    msg.type = XPN_SERVER_RMDIR_DIR;
    nfi_xpn_server_do_request(server_aux, &msg, (char *)&(status), sizeof(struct st_xpn_server_status));
    if (status.ret < 0){
      errno = status.server_errno;
      return status.ret;
    }
  }
  else
  {
    msg.type = XPN_SERVER_RMDIR_DIR_ASYNC;

    // send request...
    ret = nfi_write_operation(server_aux, &msg);
    if (ret >= 0) {
      return 0;
    }
  }

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rmdir] nfi_xpn_server_rmdir(%s)=%d\n", serv->id, dir, ret);
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_rmdir] >> End\n", serv->id);

  return 0;
}

int nfi_xpn_server_statfs(__attribute__((__unused__)) struct nfi_server *serv, __attribute__((__unused__))  struct nfi_info *inf)
{
  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_statfs] >> Begin\n", serv->id);

  //TODO
  /*
  struct xpn_info xpninf;
  int ret;
  struct nfi_xpn_server *server_aux;

  // Check arguments...
  NULL_RET_ERR(serv, EINVAL);
  NULL_RET_ERR(inf,  EINVAL);
  nfi_xpn_server_keep_connected(serv);
  NULL_RET_ERR(serv->private_info, EINVAL);

  // private_info...
  server_aux = (struct nfi_xpn_server *)serv->private_info;

  ret = xpn_statfs(server_aux->fh, &xpninf, server_aux->cl);
  if (ret <0).{
    xpn_err(xpn_ERR_STATFS);
    return -1;
  }

  MPItoNFIInfo(inf, &xpninf);
  */

  debug_info("[SERV_ID=%d] [NFI_XPN] [nfi_xpn_server_statfs] >> End\n", serv->id);

  return 0;
}

/* ................................................................... */
