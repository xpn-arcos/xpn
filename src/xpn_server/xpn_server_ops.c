
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

#include "xpn_server_ops.h"
#include "xpn_server_params.h"
#include "xpn_server_comm.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Auxiliar Functions / Funciones Auxiliares ..................... */

char * xpn_server_op2string ( int op_code )
{
  char *ret = "Unknown";

  switch (op_code)
  {
    // File operations
    case XPN_SERVER_OPEN_FILE:
      ret = "OPEN";
      break;
    case XPN_SERVER_CREAT_FILE:
      ret = "CREAT";
      break;
    case XPN_SERVER_READ_FILE:
      ret = "READ";
      break;
    case XPN_SERVER_WRITE_FILE:
      ret = "WRITE";
      break;
    case XPN_SERVER_CLOSE_FILE:
      ret = "CLOSE";
      break;
    case XPN_SERVER_RM_FILE:
      ret = "RM";
      break;
    case XPN_SERVER_RM_FILE_ASYNC:
      ret = "RM_ASYNC";
      break;
    case XPN_SERVER_RENAME_FILE:
      ret = "RENAME";
      break;
    case XPN_SERVER_GETATTR_FILE:
      ret = "GETATTR";
      break;
    case XPN_SERVER_SETATTR_FILE:
      ret = "SETATTR";
      break;

    // Directory operations
    case XPN_SERVER_MKDIR_DIR:
      ret = "MKDIR";
      break;
    case XPN_SERVER_RMDIR_DIR:
      ret = "RMDIR";
      break;
    case XPN_SERVER_RMDIR_DIR_ASYNC:
      ret = "RMDIR_ASYNC";
      break;
    case XPN_SERVER_OPENDIR_DIR:
      ret = "OPENDIR";
      break;
    case XPN_SERVER_READDIR_DIR:
      ret = "READDIR";
      break;
    case XPN_SERVER_CLOSEDIR_DIR:
      ret = "CLOSEDIR";
      break;

    // FS Operations
    case XPN_SERVER_STATFS_DIR:
      ret = "STATFS";
      break;
    case XPN_SERVER_FINALIZE:
      ret = "FINALIZE";
      break;
    case XPN_SERVER_GETID:
      ret = "GETID";
      break;

    // Connection operatons
    case XPN_SERVER_DISCONNECT:
      ret = "DISCONNECT";
      break;
    case XPN_SERVER_GETNODENAME:
      ret = "GETNODENAME";
      break;
    case XPN_SERVER_END:
      ret = "END";
      break;
  }

  return ret;
}


/* ... Functions / Funciones ......................................... */

// File operations
void xpn_server_op_open        ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_creat       ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_read        ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_write       ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_close       ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_rm          ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_rm_async    ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_rename      ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_setattr     ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_getattr     ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );

// Directory operations
void xpn_server_op_mkdir       ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_opendir     ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_readdir     ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_closedir    ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_rmdir       ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_rmdir_async ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );

// FS Operations
void xpn_server_op_getnodename ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id );
void xpn_server_op_fstat       ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id ); //TODO: implement

//Read the operation to realize
int xpn_server_do_operation ( struct st_th *th, int * the_end )
{
  int ret;
  struct st_xpn_server_msg head;

  debug_info("[TH_ID=%d] [XPN_SERVER_OPS] [xpn_server_do_operation] >> Begin\n", th->id);
  debug_info("[TH_ID=%d] [XPN_SERVER_OPS] [xpn_server_do_operation] OP '%s'; OP_ID %d\n", th->id, xpn_server_op2string(th->type_op), th->type_op);

  switch (th->type_op)
  {
    //File API
    case XPN_SERVER_OPEN_FILE:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_open), sizeof(struct st_xpn_server_path_flags), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_open(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_CREAT_FILE:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_creat), sizeof(struct st_xpn_server_path_flags), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_creat(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_READ_FILE:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_read), sizeof(struct st_xpn_server_rw), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_read(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_WRITE_FILE:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_write), sizeof(struct st_xpn_server_rw), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_write(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_CLOSE_FILE:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_close), sizeof(struct st_xpn_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_close(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_RM_FILE:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_rm), sizeof(struct st_xpn_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_rm(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_RM_FILE_ASYNC:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_rm), sizeof(struct st_xpn_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_rm_async(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_RENAME_FILE:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_rename), sizeof(struct st_xpn_server_rename), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_rename(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_GETATTR_FILE:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_getattr), sizeof(struct st_xpn_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_getattr(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_SETATTR_FILE:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_setattr), sizeof(struct st_xpn_server_setattr), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_setattr(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;

    //Directory API
    case XPN_SERVER_MKDIR_DIR:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_mkdir), sizeof(struct st_xpn_server_path_flags), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_mkdir(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_OPENDIR_DIR:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_opendir), sizeof(struct st_xpn_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_opendir(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_READDIR_DIR:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_readdir), sizeof(struct st_xpn_server_readdir), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_readdir(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_CLOSEDIR_DIR:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_closedir), sizeof(struct st_xpn_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_closedir(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_RMDIR_DIR:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_rmdir), sizeof(struct st_xpn_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_rmdir(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case XPN_SERVER_RMDIR_DIR_ASYNC:
      ret = xpn_server_comm_read_data(th->params, (int) th->sd, (char *)&(head.u_st_xpn_server_msg.op_rmdir), sizeof(struct st_xpn_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        xpn_server_op_rmdir_async(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;

    //FS API
    case XPN_SERVER_GETNODENAME:
      xpn_server_op_getnodename(th->params, (int) th->sd, &head, th->rank_client_id, th->tag_client_id);
      break;

    //Connection API
    case XPN_SERVER_DISCONNECT:
      break;
    case XPN_SERVER_FINALIZE:
      *the_end = 1;
      break;
  }

  debug_info("[TH_ID=%d] [XPN_SERVER_OPS] [xpn_server_do_operation] << End\n", th->id);

  return 0;
}


// File API
void xpn_server_op_open ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id )
{
  char path [PATH_MAX];
  struct st_xpn_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_open] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_open] >> Begin\n", params->rank);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_xpn_server_msg.op_open.path);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_open] open(%s, %d, %d)\n", params->rank, path, head->u_st_xpn_server_msg.op_open.flags, head->u_st_xpn_server_msg.op_open.mode);

  // do open
  status.ret = filesystem_open2(path, head->u_st_xpn_server_msg.op_open.flags, head->u_st_xpn_server_msg.op_open.mode);
  status.server_errno = errno;
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_open] open(%s)=%d\n", params->rank, head->u_st_xpn_server_msg.op_open.path, status.ret);
  if (status.ret < 0){
    xpn_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);
  }else{
    status.ret = filesystem_close(status.ret);
    status.server_errno = errno;

    xpn_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_open] << End\n", params->rank);
}

void xpn_server_op_creat ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id )
{
  char path [PATH_MAX];
  struct st_xpn_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_creat] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_creat] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_creat] creat(%s)\n", params->rank, head->u_st_xpn_server_msg.op_creat.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_xpn_server_msg.op_creat.path);

  // do creat
  status.ret = filesystem_creat(path, head->u_st_xpn_server_msg.op_creat.mode);
  status.server_errno = errno;
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_creat] creat(%s)=%d\n", params->rank, head->u_st_xpn_server_msg.op_creat.path, status.ret);
  if (status.ret < 0){
    xpn_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);
    return;
  }else{
    status.ret = filesystem_close(status.ret);
    status.server_errno = errno;

    xpn_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);
  }

  // show debug info
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_creat] << End\n", params->rank);
}

void xpn_server_op_read ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_rw_req req;
  char * buffer;
  long   size, diff, to_read, cont;
  char   path [PATH_MAX];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] >> Begin\n",  params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] read(%s, %ld %ld)\n", params->rank, head->u_st_xpn_server_msg.op_read.path, head->u_st_xpn_server_msg.op_read.offset, head->u_st_xpn_server_msg.op_read.size);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_xpn_server_msg.op_read.path);

  // initialize counters
  cont = 0;
  size = head->u_st_xpn_server_msg.op_read.size;
  if (size > MAX_BUFFER_SIZE) {
    size = MAX_BUFFER_SIZE;
  }
  diff = head->u_st_xpn_server_msg.op_read.size - cont;

  //Open file
  int fd = filesystem_open(path, O_RDONLY);
  if (fd < 0)
  {
    req.size = -1;  // TODO: check in client that -1 is treated properly... :-9
    req.status.ret = fd;
    req.status.server_errno = errno;
    xpn_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id);
    return;
  }

  // malloc a buffer of size...
  buffer = (char *)malloc(size);
  if (NULL == buffer)
  {
    req.size = -1;  // TODO: check in client that -1 is treated properly... :-9
    req.status.ret = -1;
    req.status.server_errno = errno;
    xpn_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id);

    filesystem_close(fd);
    return;
  }

  // loop...
  do
  {
    if (diff > size) {
      to_read = size;
    }
    else {
      to_read = diff;
    }

    // lseek and read data...
    filesystem_lseek(fd, head->u_st_xpn_server_msg.op_read.offset + cont, SEEK_SET); //TODO: check error
    req.size = filesystem_read(fd, buffer, to_read);
    // if error then send as "how many bytes" -1
    if (req.size < 0)
    {
      req.size = -1;  // TODO: check in client that -1 is treated properly... :-)
      req.status.ret = -1;
      req.status.server_errno = errno;
      xpn_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id);

      filesystem_close(fd);

      FREE_AND_NULL(buffer);
      return;
    }
    // send (how many + data) to client...
    req.status.ret = 0;
    req.status.server_errno = errno;
    xpn_server_comm_write_data(params, sd, (char *)&req, sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id);
    debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] op_read: send size %ld\n", params->rank, req.size);

    // send data to client...
    if (req.size > 0)
    {
      xpn_server_comm_write_data(params, sd, buffer, req.size, rank_client_id, tag_client_id);
      debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] op_read: send data\n", params->rank);
    }
    cont = cont + req.size; //Send bytes
    diff = head->u_st_xpn_server_msg.op_read.size - cont;

  } while ((diff > 0) && (req.size != 0));

  filesystem_close(fd);

  // free buffer
  FREE_AND_NULL(buffer);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] read(%s, %ld %ld)=%ld\n", params->rank, head->u_st_xpn_server_msg.op_read.path, head->u_st_xpn_server_msg.op_read.offset, head->u_st_xpn_server_msg.op_read.size, cont);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] << End\n", params->rank);
}

void xpn_server_op_write ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_rw_req req;
  char * buffer;
  int    size, diff, cont, to_write;
  char   path [PATH_MAX];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write] write(%s, %ld %ld)\n", params->rank, head->u_st_xpn_server_msg.op_write.path, head->u_st_xpn_server_msg.op_write.offset, head->u_st_xpn_server_msg.op_write.size);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_xpn_server_msg.op_write.path);

  // initialize counters
  cont = 0;
  size = (head->u_st_xpn_server_msg.op_write.size);
  if (size > MAX_BUFFER_SIZE) {
    size = MAX_BUFFER_SIZE;
  }
  diff = head->u_st_xpn_server_msg.op_read.size - cont;

  //Open file
  int fd = filesystem_open(path, O_WRONLY);
  if (fd < 0)
  {
    req.size = -1;  // TODO: check in client that -1 is treated properly... :-)
    req.status.ret = fd;
    req.status.server_errno = errno;
    xpn_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id); 
    return;
  }

  // malloc a buffer of size...
  buffer = (char *)malloc(size);
  if (NULL == buffer)
  {
    req.size = -1;  // TODO: check in client that -1 is treated properly... :-)
    req.status.ret = -1;
    req.status.server_errno = errno;
    xpn_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id);

    filesystem_close(fd);
    return;
  }

  // loop...
  do
  {
    if (diff > size){
      to_write = size;
    }
    else{
      to_write = diff;
    }

    // read data from MPI and write into the file
    xpn_server_comm_read_data(params, sd, buffer, to_write, rank_client_id, tag_client_id);
    filesystem_lseek(fd, head->u_st_xpn_server_msg.op_write.offset + cont, SEEK_SET); //TODO: check error
    //sem_wait(&disk_sem);
    req.size = filesystem_write(fd, buffer, to_write);
    //sem_post(&disk_sem);

    // update counters
    cont = cont + req.size; // Received bytes
    diff = head->u_st_xpn_server_msg.op_read.size - cont;

  } while ((diff > 0) && (req.size != 0));

  // write to the client the status of the write operation
  req.size = cont;
  req.status.ret = 0;
  req.status.server_errno = errno;
  xpn_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id);

  filesystem_close(fd);

  // free buffer
  FREE_AND_NULL(buffer);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write] write(%s, %ld %ld)=%d\n", params->rank, head->u_st_xpn_server_msg.op_write.path, head->u_st_xpn_server_msg.op_write.offset, head->u_st_xpn_server_msg.op_write.size, cont);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write] << End\n", params->rank);
}

void xpn_server_op_close ( __attribute__((__unused__)) xpn_server_param_st *params, __attribute__((__unused__)) int sd, __attribute__((__unused__)) struct st_xpn_server_msg *head, __attribute__((__unused__)) int rank_client_id, __attribute__((__unused__)) int tag_client_id )
{
  // Without sesion close do nothing
  return;
}

void xpn_server_op_rm ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id )
{
  char path [PATH_MAX];
  struct st_xpn_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm] unlink(%s)\n", params->rank, head->u_st_xpn_server_msg.op_rm.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_xpn_server_msg.op_rm.path);

  // do rm
  status.ret = filesystem_unlink(path);
  status.server_errno = errno;
  xpn_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm] unlink(%s)=%d\n", params->rank, head->u_st_xpn_server_msg.op_rm.path, status.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm] << End\n", params->rank);
}

void xpn_server_op_rm_async ( xpn_server_param_st *params, __attribute__((__unused__)) int sd, struct st_xpn_server_msg *head, __attribute__((__unused__)) int rank_client_id, __attribute__((__unused__)) int tag_client_id )
{
  char path [PATH_MAX];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm_async] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm_async] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm_async] unlink(%s)\n", params->rank, head->u_st_xpn_server_msg.op_rm.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_xpn_server_msg.op_rm.path);

  // do rm
  filesystem_unlink(path);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm_async] unlink(%s)=%d\n", params->rank, head->u_st_xpn_server_msg.op_rm.path, 0);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm_async] << End\n", params->rank);
}

void xpn_server_op_rename ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id )
{
  char old_path [PATH_MAX];
  char new_path [PATH_MAX];
  struct st_xpn_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rename] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rename] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rename] rename(%s, %s)\n", params->rank, head->u_st_xpn_server_msg.op_rename.old_url, head->u_st_xpn_server_msg.op_rename.new_url);

  strcpy(old_path, params->dirbase);
  strcat(old_path, head->u_st_xpn_server_msg.op_rename.old_url);

  strcpy(new_path, params->dirbase);
  strcat(new_path, head->u_st_xpn_server_msg.op_rename.new_url );

  // do rename
  status.ret = filesystem_rename(old_path, new_path);
  status.server_errno = errno;
  xpn_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rename] rename(%s, %s)=%d\n", params->rank, head->u_st_xpn_server_msg.op_rename.old_url, head->u_st_xpn_server_msg.op_rename.new_url, status.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rename] << End\n", params->rank);
}

void xpn_server_op_getattr ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_attr_req req;
  char path [PATH_MAX];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getattr] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getattr] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getattr] stat(%s)\n", params->rank, head->u_st_xpn_server_msg.op_getattr.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_xpn_server_msg.op_getattr.path);

  // do getattr
  req.status = filesystem_stat(path, &req.attr);
  req.status_req.ret = req.status;
  req.status_req.server_errno = errno;

  xpn_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_xpn_server_attr_req), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getattr] stat(%s)=%d\n", params->rank, head->u_st_xpn_server_msg.op_getattr.path, req.status);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getattr] << End\n", params->rank);
}

void xpn_server_op_setattr (xpn_server_param_st *params, __attribute__((__unused__)) int sd, struct st_xpn_server_msg *head, __attribute__((__unused__)) int rank_client_id, __attribute__((__unused__)) int tag_client_id)
{
  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_setattr] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_setattr] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_setattr] SETATTR(...)\n", params->rank);

  if (NULL == head) {
    return;
  }

  // do setattr
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_setattr] SETATTR operation to be implemented !!\n", params->rank);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_setattr] SETATTR(...)=(...)\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_setattr] << End\n", params->rank);
}

//Directory API
void xpn_server_op_mkdir ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id )
{
  char path [PATH_MAX];
  struct st_xpn_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_mkdir] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_mkdir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_mkdir] mkdir(%s)\n", params->rank, head->u_st_xpn_server_msg.op_mkdir.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_xpn_server_msg.op_mkdir.path);

  // do mkdir
  status.ret = filesystem_mkdir(path, head->u_st_xpn_server_msg.op_mkdir.mode);
  status.server_errno = errno;
  xpn_server_comm_write_data(params, sd,(char *)&status,sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_mkdir] mkdir(%s)=%d\n", params->rank, head->u_st_xpn_server_msg.op_mkdir.path, status.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_mkdir] << End\n", params->rank);
}

void xpn_server_op_opendir ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id )
{
  DIR* ret;
  char path [PATH_MAX];
  struct st_xpn_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_opendir] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_opendir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_opendir] opendir(%s)\n", params->rank, head->u_st_xpn_server_msg.op_opendir.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_xpn_server_msg.op_opendir.path);

  ret = filesystem_opendir(path);
  status.ret = ret == NULL ? -1 : 0;
  status.server_errno = errno;

  if (status.ret == 0){
    status.ret = filesystem_telldir(ret);
    status.server_errno = errno;
  }

  xpn_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_opendir] opendir(%s)=%p\n", params->rank, head->u_st_xpn_server_msg.op_opendir.path, ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_opendir] << End\n", params->rank);
}

void xpn_server_op_readdir ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id )
{
  struct dirent * ret;
  struct st_xpn_server_readdir_req ret_entry;
  char path [PATH_MAX];
  DIR* s;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_readdir] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_readdir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_readdir] readdir(%s)\n", params->rank, head->u_st_xpn_server_msg.op_readdir.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_xpn_server_msg.op_readdir.path);
  
  s = filesystem_opendir(path);
  ret_entry.status.ret = s == NULL ? -1 : 0;
  ret_entry.status.server_errno = errno;

  filesystem_seekdir(s, head->u_st_xpn_server_msg.op_readdir.telldir);

  // Reset errno
  errno = 0;
  ret = filesystem_readdir(s);
  if (ret != NULL)
  {
    ret_entry.end = 1;
    ret_entry.ret = *ret;
  }
  else{
    ret_entry.end = 0;
  }

  ret_entry.status.ret = ret == NULL ? -1 : 0;

  ret_entry.telldir = filesystem_telldir(s);

  ret_entry.status.ret = filesystem_closedir(s);
  ret_entry.status.server_errno = errno;

  xpn_server_comm_write_data(params, sd,(char *)&ret_entry, sizeof(struct st_xpn_server_readdir_req), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_readdir] readdir(%p)=%p\n", params->rank, s, ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_readdir] << End\n", params->rank);
}

void xpn_server_op_closedir ( __attribute__((__unused__)) xpn_server_param_st *params, __attribute__((__unused__)) int sd, __attribute__((__unused__)) struct st_xpn_server_msg *head, __attribute__((__unused__)) int rank_client_id, __attribute__((__unused__)) int tag_client_id )
{
  // Without sesion close do nothing
  return;
}

void xpn_server_op_rmdir ( xpn_server_param_st *params, int sd, struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id )
{
  char path [PATH_MAX];
  struct st_xpn_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir] rmdir(%s)\n", params->rank, head->u_st_xpn_server_msg.op_rmdir.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_xpn_server_msg.op_rmdir.path);

  // do rmdir
  status.ret = filesystem_rmdir(path);
  status.server_errno = errno;  
  xpn_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir] rmdir(%s)=%d\n", params->rank, head->u_st_xpn_server_msg.op_rmdir.path, status.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir] << End\n", params->rank);
}

void xpn_server_op_rmdir_async ( __attribute__((__unused__)) xpn_server_param_st *params, __attribute__((__unused__)) int sd, struct st_xpn_server_msg *head, __attribute__((__unused__)) int rank_client_id, int tag_client_id )
{
  int ret;
  char path [PATH_MAX];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir_async] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir_async] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir_async] rmdir(%s)\n", params->rank, head->u_st_xpn_server_msg.op_rmdir.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_xpn_server_msg.op_rmdir.path);

  // do rmdir
  ret = filesystem_rmdir(path);

  xpn_server_comm_write_data(params, sd, (char *)&ret, sizeof(int), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir_async] rmdir(%s)=%d\n", params->rank, head->u_st_xpn_server_msg.op_rmdir.path, ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir_async] << End\n", params->rank);
}

//FS API
void  xpn_server_op_getnodename (xpn_server_param_st *params, int sd,  __attribute__((__unused__)) struct st_xpn_server_msg *head, int rank_client_id, int tag_client_id)
{
  char serv_name[HOST_NAME_MAX];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getnodename] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getnodename] >> Begin\n", params->rank);

  // Get server host name
  gethostname(serv_name, HOST_NAME_MAX);

  xpn_server_comm_write_data(params, sd, (char *)(serv_name),       HOST_NAME_MAX, rank_client_id, tag_client_id); // Send one single message
  xpn_server_comm_write_data(params, sd, (char *)(params->dirbase), PATH_MAX,      rank_client_id, tag_client_id); // Send one single message
  //xpn_server_comm_write_data(params, sd, (char *)params->sem_name_server, PATH_MAX, rank_client_id, tag_client_id); // Send one single message

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getnodename] gethostname=%s\n", params->rank, serv_name);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getnodename] << END\n", params->rank);
}

/* ................................................................... */

