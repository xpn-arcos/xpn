
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

#include "mpi_server_ops.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Auxiliar Functions / Funciones Auxiliares ..................... */

char * mpi_server_op2string ( int op_code )
{
  char *ret = "Unknown";

  switch (op_code)
  {
    // File operations
    case MPI_SERVER_OPEN_FILE:
      ret = "OPEN";
      break;
    case MPI_SERVER_CREAT_FILE:
      ret = "CREAT";
      break;
    case MPI_SERVER_READ_FILE:
      ret = "READ";
      break;
    case MPI_SERVER_WRITE_FILE:
      ret = "WRITE";
      break;
    case MPI_SERVER_CLOSE_FILE:
      ret = "CLOSE";
      break;
    case MPI_SERVER_RM_FILE:
      ret = "RM";
      break;
    case MPI_SERVER_RM_FILE_ASYNC:
      ret = "RM_ASYNC";
      break;
    case MPI_SERVER_RENAME_FILE:
      ret = "RENAME";
      break;
    case MPI_SERVER_GETATTR_FILE:
      ret = "GETATTR";
      break;
    case MPI_SERVER_SETATTR_FILE:
      ret = "SETATTR";
      break;

    // Directory operations
    case MPI_SERVER_MKDIR_DIR:
      ret = "MKDIR";
      break;
    case MPI_SERVER_RMDIR_DIR:
      ret = "RMDIR";
      break;
    case MPI_SERVER_RMDIR_DIR_ASYNC:
      ret = "RMDIR_ASYNC";
      break;
    case MPI_SERVER_OPENDIR_DIR:
      ret = "OPENDIR";
      break;
    case MPI_SERVER_READDIR_DIR:
      ret = "READDIR";
      break;
    case MPI_SERVER_CLOSEDIR_DIR:
      ret = "CLOSEDIR";
      break;

    // Import / Export operations
    case MPI_SERVER_FLUSH_FILE:
      ret = "FLUSH";
      break;
    case MPI_SERVER_PRELOAD_FILE:
      ret = "PRELOAD";
      break;

    // FS Operations
    case MPI_SERVER_STATFS_DIR:
      ret = "STATFS";
      break;
    case MPI_SERVER_FINALIZE:
      ret = "FINALIZE";
      break;
    case MPI_SERVER_GETID:
      ret = "GETID";
      break;

    // Connection operatons
    case MPI_SERVER_DISCONNECT:
      ret = "DISCONNECT";
      break;
    case MPI_SERVER_GETNODENAME:
      ret = "GETNODENAME";
      break;
    case MPI_SERVER_END:
      ret = "END";
      break;
  }

  return ret;
}


/* ... Functions / Funciones ......................................... */

// File operations
void mpi_server_op_open        ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_creat       ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_read        ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_write       ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_close       ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_rm          ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_rm_async    ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_rename      ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_setattr     ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_getattr     ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );

// Directory operations
void mpi_server_op_mkdir       ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_opendir     ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_readdir     ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_closedir    ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_rmdir       ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_rmdir_async ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );

// Import / Export operations
void mpi_server_op_flush       ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_preload     ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );

// FS Operations
void mpi_server_op_getnodename ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id );
void mpi_server_op_fstat       ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id ); //TODO: implement
void mpi_server_op_getid       ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id ); //TODO: call in switch

//Read the operation to realize
int mpi_server_do_operation ( struct st_th *th, int * the_end )
{
  int ret;
  struct st_mpi_server_msg head;

  debug_info("[TH_ID=%d] [MPI_SERVER_OPS] [mpi_server_do_operation] >> Begin\n", th->id);
  debug_info("[TH_ID=%d] [MPI_SERVER_OPS] [mpi_server_do_operation] OP '%s'; OP_ID %d\n", th->id, mpi_server_op2string(th->type_op), th->type_op);

  switch (th->type_op)
  {
    //File API
    case MPI_SERVER_OPEN_FILE:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_open), sizeof(struct st_mpi_server_path_flags), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_open(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_CREAT_FILE:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_creat), sizeof(struct st_mpi_server_path_flags), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_creat(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_READ_FILE:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_read), sizeof(struct st_mpi_server_rw), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_read(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_WRITE_FILE:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_write), sizeof(struct st_mpi_server_rw), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_write(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_CLOSE_FILE:
      // ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_close), sizeof(struct st_mpi_server_close), th->rank_client_id, th->tag_client_id);
      // if (ret != -1) {
      //   mpi_server_op_close(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      // }
      break;
    case MPI_SERVER_RM_FILE:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_rm), sizeof(struct st_mpi_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_rm(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_RM_FILE_ASYNC:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_rm), sizeof(struct st_mpi_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_rm_async(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_RENAME_FILE:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_rename), sizeof(struct st_mpi_server_rename), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_rename(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_GETATTR_FILE:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_getattr), sizeof(struct st_mpi_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_getattr(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_SETATTR_FILE:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_setattr), sizeof(struct st_mpi_server_setattr), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_setattr(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;

    //Directory API
    case MPI_SERVER_MKDIR_DIR:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_mkdir), sizeof(struct st_mpi_server_path_flags), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_mkdir(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_OPENDIR_DIR:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_opendir), sizeof(struct st_mpi_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_opendir(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_READDIR_DIR:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_readdir), sizeof(struct st_mpi_server_readdir), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_readdir(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_CLOSEDIR_DIR:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_closedir), sizeof(struct st_mpi_server_closedir), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_closedir(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_RMDIR_DIR:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_rmdir), sizeof(struct st_mpi_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_rmdir(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_RMDIR_DIR_ASYNC:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_rmdir), sizeof(struct st_mpi_server_path), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_rmdir_async(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;

    // Import / Export API
    case MPI_SERVER_PRELOAD_FILE:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_preload), sizeof(struct st_mpi_server_preload), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_preload(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;
    case MPI_SERVER_FLUSH_FILE:
      ret = mpi_server_comm_read_data(th->params, (MPI_Comm) th->sd, (char *)&(head.u_st_mpi_server_msg.op_flush), sizeof(struct st_mpi_server_flush), th->rank_client_id, th->tag_client_id);
      if (ret != -1) {
        mpi_server_op_flush(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      }
      break;

    //FS API
    case MPI_SERVER_GETNODENAME:
      mpi_server_op_getnodename(th->params, (MPI_Comm) th->sd, &head, th->rank_client_id, th->tag_client_id);
      break;

    //Connection API
    case MPI_SERVER_DISCONNECT:
      break;
    case MPI_SERVER_FINALIZE:
      *the_end = 1;
      break;
  }

  debug_info("[TH_ID=%d] [MPI_SERVER_OPS] [mpi_server_do_operation] << End\n", th->id);

  return 0;
}


// File API
void mpi_server_op_open ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  char path [PATH_MAX];
  struct st_mpi_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_open] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_open] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_open] open(%s)\n", params->rank, head->u_st_mpi_server_msg.op_open.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_mpi_server_msg.op_open.path);

  // do open
  status.ret = filesystem_open(path, O_RDWR);
  status.server_errno = errno;
  // TODO: flags
  // fd = filesystem_open(path, head->u_st_mpi_server_msg.op_open.flags);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_open] open(%s)=%d\n", params->rank, head->u_st_mpi_server_msg.op_open.path, status.ret);
  if (status.ret < 0){
    mpi_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);
    return;
  }else{
    status.ret = filesystem_close(status.ret);
    status.server_errno = errno;

    mpi_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_open] << End\n", params->rank);
}

void mpi_server_op_creat ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  char path [PATH_MAX];
  struct st_mpi_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_creat] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_creat] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_creat] creat(%s)\n", params->rank, head->u_st_mpi_server_msg.op_creat.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_mpi_server_msg.op_creat.path);

  // do creat
  status.ret = filesystem_creat(path, 0770);
  status.server_errno = errno;
  // TODO: flags
  // fd = filesystem_creat(path, head->u_st_mpi_server_msg.op_open.flags);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_creat] creat(%s)=%d\n", params->rank, head->u_st_mpi_server_msg.op_creat.path, fd);
  if (status.ret < 0){
    mpi_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);
    return;
  }else{
    status.ret = filesystem_close(status.ret);
    status.server_errno = errno;

    mpi_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);
  }

  // show debug info
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_creat] << End\n", params->rank);
}

void mpi_server_op_read ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  struct st_mpi_server_rw_req req;
  struct st_mpi_server_status status;
  char * buffer;
  long   size, diff, to_read, cont;
  char   path [PATH_MAX];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_read] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_read] >> Begin\n",  params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_read] read(%s, %ld %ld)\n", params->rank, head->u_st_mpi_server_msg.op_read.path, head->u_st_mpi_server_msg.op_read.offset, head->u_st_mpi_server_msg.op_read.size);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_mpi_server_msg.op_read.path);

  // initialize counters
  cont = 0;
  size = head->u_st_mpi_server_msg.op_read.size;
  if (size > MAX_BUFFER_SIZE) {
    size = MAX_BUFFER_SIZE;
  }
  diff = head->u_st_mpi_server_msg.op_read.size - cont;

  //Open file
  int fd = filesystem_open(path, O_RDONLY);
  if (fd < 0)
  {
    req.size = -1;  // TODO: check in client that -1 is treated properly... :-9
    mpi_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpi_server_rw_req), rank_client_id, tag_client_id);
    status.ret = fd;
    status.server_errno = errno;
    mpi_server_comm_write_data(params, sd,(char *)&status,sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);
    return;
  }

  // malloc a buffer of size...
  buffer = (char *)malloc(size);
  if (NULL == buffer)
  {
    req.size = -1;  // TODO: check in client that -1 is treated properly... :-9
    mpi_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpi_server_rw_req), rank_client_id, tag_client_id);

    filesystem_close(fd);
    status.ret = -1;
    status.server_errno = errno;
    mpi_server_comm_write_data(params, sd,(char *)&status,sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);
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
    filesystem_lseek(fd, head->u_st_mpi_server_msg.op_read.offset + cont, SEEK_SET); //TODO: check error
    req.size = filesystem_read(fd, buffer, to_read);
    // if error then send as "how many bytes" -1
    if (req.size < 0)
    {
      req.size = -1;  // TODO: check in client that -1 is treated properly... :-)
      mpi_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpi_server_rw_req), rank_client_id, tag_client_id);

      filesystem_close(fd);

      FREE_AND_NULL(buffer);
      status.ret = -1;
      status.server_errno = errno;
      mpi_server_comm_write_data(params, sd,(char *)&status,sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);
      return;
    }
    // send (how many + data) to client...
    mpi_server_comm_write_data(params, sd, (char *)&req, sizeof(struct st_mpi_server_rw_req), rank_client_id, tag_client_id);
    debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_read] op_read: send size %ld\n", params->rank, req.size);

    // send data to client...
    if (req.size > 0)
    {
      mpi_server_comm_write_data(params, sd, buffer, req.size, rank_client_id, tag_client_id);
      debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_read] op_read: send data\n", params->rank);
    }
    cont = cont + req.size; //Send bytes
    diff = head->u_st_mpi_server_msg.op_read.size - cont;

  } while ((diff > 0) && (req.size != 0));

  filesystem_close(fd);

  status.ret = 0;
  status.server_errno = errno;
  mpi_server_comm_write_data(params, sd,(char *)&status,sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);

  // free buffer
  FREE_AND_NULL(buffer);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_read] read(%s, %ld %ld)=%ld\n", params->rank, head->u_st_mpi_server_msg.op_read.path, head->u_st_mpi_server_msg.op_read.offset, head->u_st_mpi_server_msg.op_read.size, cont);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_read] << End\n", params->rank);
}

void mpi_server_op_write ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  struct st_mpi_server_rw_req req;
  struct st_mpi_server_status status;
  char * buffer;
  int    size, diff, cont, to_write;
  char   path [PATH_MAX];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_write] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_write] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_write] write(%s, %ld %ld)\n", params->rank, head->u_st_mpi_server_msg.op_write.path, head->u_st_mpi_server_msg.op_write.offset, head->u_st_mpi_server_msg.op_write.size);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_mpi_server_msg.op_write.path);

  // initialize counters
  cont = 0;
  size = (head->u_st_mpi_server_msg.op_write.size);
  if (size > MAX_BUFFER_SIZE) {
    size = MAX_BUFFER_SIZE;
  }
  diff = head->u_st_mpi_server_msg.op_read.size - cont;

  //Open file
  int fd = filesystem_open(path, O_WRONLY);
  if (fd < 0)
  {
    req.size = -1;  // TODO: check in client that -1 is treated properly... :-)
    mpi_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpi_server_rw_req), rank_client_id, tag_client_id); 
    status.ret = fd;
    status.server_errno = errno;
    mpi_server_comm_write_data(params, sd,(char *)&status,sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);
    return;
  }

  // malloc a buffer of size...
  buffer = (char *)malloc(size);
  if (NULL == buffer)
  {
    req.size = -1;  // TODO: check in client that -1 is treated properly... :-)
    mpi_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpi_server_rw_req), rank_client_id, tag_client_id);

    filesystem_close(fd);
    status.ret = -1;
    status.server_errno = errno;
    mpi_server_comm_write_data(params, sd,(char *)&status,sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);
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
    mpi_server_comm_read_data(params, sd, buffer, to_write, rank_client_id, tag_client_id);
    filesystem_lseek(fd, head->u_st_mpi_server_msg.op_write.offset + cont, SEEK_SET); //TODO: check error
    //sem_wait(&disk_sem);
    req.size = filesystem_write(fd, buffer, to_write);
    //sem_post(&disk_sem);

    // update counters
    cont = cont + req.size; // Received bytes
    diff = head->u_st_mpi_server_msg.op_read.size - cont;

  } while ((diff > 0) && (req.size != 0));

  // write to the client the status of the write operation
  req.size = cont;
  mpi_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpi_server_rw_req), rank_client_id, tag_client_id);

  filesystem_close(fd);

  status.ret = 0;
  status.server_errno = errno;
  mpi_server_comm_write_data(params, sd,(char *)&status,sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);

  // free buffer
  FREE_AND_NULL(buffer);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_write] write(%s, %ld %ld)=%d\n", params->rank, head->u_st_mpi_server_msg.op_write.path, head->u_st_mpi_server_msg.op_write.offset, head->u_st_mpi_server_msg.op_write.size, cont);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_write] << End\n", params->rank);
}

void mpi_server_op_close ( mpi_server_param_st *params, __attribute__((__unused__)) MPI_Comm sd, __attribute__((__unused__)) struct st_mpi_server_msg *head, __attribute__((__unused__)) int rank_client_id, __attribute__((__unused__)) int tag_client_id )
{
  // TODO: think if is necesary a method for close
  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_close] ERROR: NULL arguments\n", -1);
    return;
  }

  // debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_close] >> Begin\n", params->rank);
  // debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_close] close(%d)\n", params->rank, head->u_st_mpi_server_msg.op_close.fd);

  // // do close
  // int ret = filesystem_close(head->u_st_mpi_server_msg.op_close.fd);

  // mpi_server_comm_write_data(params, sd, (char *)&ret, sizeof(int), rank_client_id, tag_client_id);

  // debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_close] close(%d)=%d\n", params->rank, head->u_st_mpi_server_msg.op_close.fd, ret);
  // debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_close] << End\n", params->rank);
}

void mpi_server_op_rm ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  char path [PATH_MAX];
  struct st_mpi_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rm] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rm] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rm] unlink(%s)\n", params->rank, head->u_st_mpi_server_msg.op_rm.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_mpi_server_msg.op_rm.path);

  // do rm
  status.ret = filesystem_unlink(path);
  status.server_errno = errno;
  mpi_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rm] unlink(%s)=%d\n", params->rank, head->u_st_mpi_server_msg.op_rm.path, ret);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rm] << End\n", params->rank);
}

void mpi_server_op_rm_async ( mpi_server_param_st *params, __attribute__((__unused__)) MPI_Comm sd, struct st_mpi_server_msg *head, __attribute__((__unused__)) int rank_client_id, __attribute__((__unused__)) int tag_client_id )
{
  char path [PATH_MAX];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rm_async] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rm_async] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rm_async] unlink(%s)\n", params->rank, head->u_st_mpi_server_msg.op_rm.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_mpi_server_msg.op_rm.path);

  // do rm
  filesystem_unlink(path);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rm_async] unlink(%s)=%d\n", params->rank, head->u_st_mpi_server_msg.op_rm.path, 0);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rm_async] << End\n", params->rank);
}

void mpi_server_op_rename ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  char old_path [PATH_MAX];
  char new_path [PATH_MAX];
  struct st_mpi_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rename] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rename] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rename] rename(%s, %s)\n", params->rank, head->u_st_mpi_server_msg.op_rename.old_url, head->u_st_mpi_server_msg.op_rename.new_url);

  strcpy(old_path, params->dirbase);
  strcat(old_path, head->u_st_mpi_server_msg.op_rename.old_url);

  strcpy(new_path, params->dirbase);
  strcat(new_path, head->u_st_mpi_server_msg.op_rename.new_url );

  // do rename
  status.ret = filesystem_rename(old_path, new_path);
  status.server_errno = errno;
  mpi_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rename] rename(%s, %s)=%d\n", params->rank, head->u_st_mpi_server_msg.op_rename.old_url, head->u_st_mpi_server_msg.op_rename.new_url, ret);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rename] << End\n", params->rank);
}

void mpi_server_op_getattr ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  struct st_mpi_server_attr_req req;
  char path [PATH_MAX];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getattr] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getattr] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getattr] stat(%s)\n", params->rank, head->u_st_mpi_server_msg.op_getattr.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_mpi_server_msg.op_getattr.path);

  // do getattr
  req.status = filesystem_stat(path, &req.attr);

  mpi_server_comm_write_data(params, sd,(char *)&req,sizeof(struct st_mpi_server_attr_req), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getattr] stat(%s)=%d\n", params->rank, head->u_st_mpi_server_msg.op_getattr.path, req.status);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getattr] << End\n", params->rank);
}

void mpi_server_op_setattr (mpi_server_param_st *params, __attribute__((__unused__)) MPI_Comm sd, struct st_mpi_server_msg *head, __attribute__((__unused__)) int rank_client_id, __attribute__((__unused__)) int tag_client_id)
{
  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_setattr] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_setattr] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_setattr] SETATTR(...)\n", params->rank);

  if (NULL == head) {
    return;
  }

  // do setattr
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_setattr] SETATTR operation to be implemented !!\n", params->rank);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_setattr] SETATTR(...)=(...)\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_setattr] << End\n", params->rank);
}

//Directory API
void mpi_server_op_mkdir ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  char path [PATH_MAX];
  struct st_mpi_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_mkdir] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_mkdir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_mkdir] mkdir(%s)\n", params->rank, head->u_st_mpi_server_msg.op_mkdir.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_mpi_server_msg.op_mkdir.path);

  // do mkdir
  status.ret = filesystem_mkdir(path, 0777); //TO-DO: 0777 received from the client
  // status.ret = filesystem_mkdir(path, head->u_st_mpi_server_msg.op_mkdir.flags); //TO-DO: 0777 received from the client
  status.server_errno = errno;
  mpi_server_comm_write_data(params, sd,(char *)&status,sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_mkdir] mkdir(%s)=%d\n", params->rank, head->u_st_mpi_server_msg.op_mkdir.path, ret);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_mkdir] << End\n", params->rank);
}

void mpi_server_op_opendir ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  DIR* ret;
  char path [PATH_MAX];
  struct st_mpi_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_opendir] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_opendir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_opendir] opendir(%s)\n", params->rank, head->u_st_mpi_server_msg.op_opendir.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_mpi_server_msg.op_opendir.path);

  // do mkdir
  ret = filesystem_opendir(path);

  unsigned long long aux;
  aux = (unsigned long long)ret;  // TODO: why not "(char *)&ret" ?

  mpi_server_comm_write_data(params, sd, (char *)&aux, (int)sizeof(DIR *), rank_client_id, tag_client_id);

  status.ret = ret == NULL ? -1 : 0;
  status.server_errno = errno;
  mpi_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_opendir] opendir(%s)=%p\n", params->rank, head->u_st_mpi_server_msg.op_opendir.path, ret);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_opendir] << End\n", params->rank);
}

void mpi_server_op_readdir ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  struct dirent * ret;
  struct st_mpi_server_direntry ret_entry;
  DIR* s;
  struct st_mpi_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_readdir] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_readdir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_readdir] readdir(%p)\n", params->rank, head->u_st_mpi_server_msg.op_readdir.dir);

  // do mkdir
  s = head->u_st_mpi_server_msg.op_readdir.dir;
  ret = filesystem_readdir(s);
  if (ret != NULL)
  {
    ret_entry.end = 1;
    ret_entry.ret = *ret;
  }
  else{
    ret_entry.end = 0;
  }

  mpi_server_comm_write_data(params, sd,(char *)&ret_entry, sizeof(struct st_mpi_server_direntry), rank_client_id, tag_client_id);
  
  status.ret = ret == NULL ? -1 : 0;
  status.server_errno = errno;
  mpi_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_readdir] readdir(%p)=%p\n", params->rank, s, ret);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_readdir] << End\n", params->rank);
}

void mpi_server_op_closedir ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  DIR* s;
  struct st_mpi_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_closedir] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_closedir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_closedir] closedir(%p)\n", params->rank, head->u_st_mpi_server_msg.op_closedir.dir);

  // do mkdir
  s = head->u_st_mpi_server_msg.op_closedir.dir;
  status.ret = filesystem_closedir(s);
  status.server_errno = errno;
  mpi_server_comm_write_data(params, sd,(char *)&status, sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_closedir] closedir(%p)=%d\n", params->rank, s, ret);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_closedir] << End\n", params->rank);
}

void mpi_server_op_rmdir ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  char path [PATH_MAX];
  struct st_mpi_server_status status;

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rmdir] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rmdir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rmdir] rmdir(%s)\n", params->rank, head->u_st_mpi_server_msg.op_rmdir.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_mpi_server_msg.op_rmdir.path);

  // do rmdir
  status.ret = filesystem_rmdir(path);
  status.server_errno = errno;  
  mpi_server_comm_write_data(params, sd, (char *)&status, sizeof(struct st_mpi_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rmdir] rmdir(%s)=%d\n", params->rank, head->u_st_mpi_server_msg.op_rmdir.path, ret);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rmdir] << End\n", params->rank);
}

void mpi_server_op_rmdir_async ( __attribute__((__unused__)) mpi_server_param_st *params, __attribute__((__unused__)) MPI_Comm sd, struct st_mpi_server_msg *head, __attribute__((__unused__)) int rank_client_id, int tag_client_id )
{
  int ret;
  char path [PATH_MAX];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rmdir_async] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rmdir_async] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rmdir_async] rmdir(%s)\n", params->rank, head->u_st_mpi_server_msg.op_rmdir.path);

  strcpy(path, params->dirbase);
  strcat(path, "/");
  strcat(path, head->u_st_mpi_server_msg.op_rmdir.path);

  // do rmdir
  ret = filesystem_rmdir(path);

  mpi_server_comm_write_data(params, sd, (char *)&ret, sizeof(int), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rmdir_async] rmdir(%s)=%d\n", params->rank, head->u_st_mpi_server_msg.op_rmdir.path, ret);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_rmdir_async] << End\n", params->rank);
}

//Import / Export API
void mpi_server_op_preload ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id )
{
  int   ret;
  int   fd_dest, fd_orig;
  char  protocol[1024];
  char  user[1024];
  char  pass[1024];
  char  machine[1024];
  char  port[1024];
  char  file[1024];

  int  BLOCKSIZE = head->u_st_mpi_server_msg.op_preload.block_size;
  char buffer [BLOCKSIZE];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_preload] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_preload] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_preload] preload(%s,%s)\n", params->rank, head->u_st_mpi_server_msg.op_preload.virtual_path, head->u_st_mpi_server_msg.op_preload.storage_path);

  // Open origin file
  fd_orig = filesystem_open(head->u_st_mpi_server_msg.op_preload.storage_path, O_RDONLY);
  if (fd_orig < 0)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_preload] ERROR: open operation on '%s' fails\n", params->rank, file);
    mpi_server_comm_write_data(params, sd, (char *)&fd_orig, sizeof(int), rank_client_id, tag_client_id); // TO-DO: Check error treatment client-side
    return;
  }

  ret = ParseURL(head->u_st_mpi_server_msg.op_preload.virtual_path, protocol, user, pass, machine, port, file);

  // Create new file
  fd_dest = filesystem_creat(file, 0777);
  if (fd_dest < 0)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_preload] ERROR: open operation on '%s' fails\n", params->rank, head->u_st_mpi_server_msg.op_flush.storage_path);
    mpi_server_comm_write_data(params, sd, (char *)&fd_orig, sizeof(int), rank_client_id, tag_client_id); // TO-DO: Check error treatment client-side
    filesystem_close(fd_orig);
    return;
  }

  off_t cont = BLOCKSIZE * params->rank;
  int read_bytes, write_bytes;

  do
  {
    off_t ret_2 = filesystem_lseek(fd_orig, cont, SEEK_SET);
    if (ret_2 == (off_t) -1)
    {
      filesystem_close(fd_orig);
      filesystem_close(fd_dest);
      mpi_server_comm_write_data(params, sd, (char *)&fd_orig, sizeof(int), rank_client_id, tag_client_id); // TO-DO: Check error treatment client-side
      return;
    }

    read_bytes = filesystem_read(fd_orig, &buffer, BLOCKSIZE);
    if (read_bytes < 0)
    {
      filesystem_close(fd_orig);
      filesystem_close(fd_dest);
      mpi_server_comm_write_data(params, sd, (char *)&fd_orig, sizeof(int), rank_client_id, tag_client_id); // TO-DO: Check error treatment client-side
      return;
    }

    if (read_bytes > 0)
    {
      write_bytes = filesystem_write(fd_dest, &buffer, read_bytes);
      if (write_bytes==-1)
      {
        filesystem_close(fd_orig);
        filesystem_close(fd_dest);
        mpi_server_comm_write_data(params, sd, (char *)&fd_orig, sizeof(int), rank_client_id, tag_client_id); // TO-DO: Check error treatment client-side
        return;
      }
    }

    cont = cont + (BLOCKSIZE * params->size);

  } while(read_bytes == BLOCKSIZE);

  filesystem_close(fd_orig);
  filesystem_close(fd_dest);

  mpi_server_comm_write_data(params, sd, (char *)&ret, sizeof(int), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_preload] preload(%s,%s)=%d\n", params->rank, head->u_st_mpi_server_msg.op_preload.virtual_path, head->u_st_mpi_server_msg.op_preload.storage_path,ret);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_preload] << END\n", params->rank);
}

void mpi_server_op_flush ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id)
{
  int   ret;
  int   fd_dest, fd_orig;
  char  protocol[1024];
  char  user[1024];
  char  pass[1024];
  char  machine[1024];
  char  port[1024];
  char  file[1024];

  int BLOCKSIZE = head->u_st_mpi_server_msg.op_flush.block_size;
  char buffer [BLOCKSIZE];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_flush] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_flush] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_flush] preload(%s,%s)\n", params->rank, head->u_st_mpi_server_msg.op_flush.virtual_path, head->u_st_mpi_server_msg.op_flush.storage_path);

  ret = ParseURL(head->u_st_mpi_server_msg.op_flush.virtual_path, protocol, user, pass, machine, port, file);

  // Open origin file
  fd_orig = filesystem_open(file, O_RDONLY);
  if (fd_orig < 0)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_flush] ERROR: open operation on '%s' fails\n", params->rank, file);
    mpi_server_comm_write_data(params, sd, (char * ) & ret, sizeof(int), rank_client_id, tag_client_id);
    return;
  }

  // Create new file
  fd_dest = filesystem_open(head->u_st_mpi_server_msg.op_flush.storage_path, O_WRONLY | O_CREAT);
  if (fd_dest < 0)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_flush] ERROR: open operation on '%s' fails\n", params->rank, head->u_st_mpi_server_msg.op_flush.storage_path);
    filesystem_close(fd_orig);
    mpi_server_comm_write_data(params, sd, (char *)&fd_dest, sizeof(int), rank_client_id, tag_client_id);
    return;
  }

  MPI_Barrier(MPI_COMM_WORLD);

  int cont = BLOCKSIZE * params->rank;
  int read_bytes, write_bytes;

  do
  {
    read_bytes = filesystem_read(fd_orig, &buffer, BLOCKSIZE);
    if (read_bytes < 0)
    {
      filesystem_close(fd_orig);
      filesystem_close(fd_dest);
      mpi_server_comm_write_data(params, sd, (char * ) & read_bytes, sizeof(int), rank_client_id, tag_client_id); // TO-DO: Check error treatment client-side
      return;
    }

    if (read_bytes > 0)
    {
      filesystem_lseek(fd_dest, cont, SEEK_SET); //TODO: check error

      write_bytes = filesystem_write(fd_dest, &buffer, read_bytes);
      if (write_bytes < 0)
      {
        filesystem_close(fd_orig);
        filesystem_close(fd_dest);
        mpi_server_comm_write_data(params, sd, (char * ) & write_bytes, sizeof(int), rank_client_id, tag_client_id); // TO-DO: Check error treatment client-side
        return;
      }
    }

    cont = cont + (BLOCKSIZE * params->size);

  } while(read_bytes == BLOCKSIZE);

  filesystem_close(fd_orig);
  filesystem_close(fd_dest);

  mpi_server_comm_write_data(params, sd, (char *)&ret, sizeof(int), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_flush] preload(%s,%s)=%d\n", params->rank, head->u_st_mpi_server_msg.op_flush.virtual_path, head->u_st_mpi_server_msg.op_flush.storage_path, ret);  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_flush] << END\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_flush] << END\n", params->rank);
}

//FS API
void  mpi_server_op_getnodename (mpi_server_param_st *params, MPI_Comm sd,  __attribute__((__unused__)) struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id)
{
  char serv_name[HOST_NAME_MAX];

  // check params...
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getnodename] ERROR: NULL arguments\n", -1);
    return;
  }

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getnodename] >> Begin\n", params->rank);

  // Get server host name
  gethostname(serv_name, HOST_NAME_MAX);

  mpi_server_comm_write_data(params, sd, (char *)(serv_name),       HOST_NAME_MAX, rank_client_id, tag_client_id); // Send one single message
  mpi_server_comm_write_data(params, sd, (char *)(params->dirbase), PATH_MAX,      rank_client_id, tag_client_id); // Send one single message
  //mpi_server_comm_write_data(params, sd, (char *)params->sem_name_server, PATH_MAX, rank_client_id, tag_client_id); // Send one single message

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getnodename] gethostname=%s\n", params->rank, serv_name);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getnodename] << END\n", params->rank);
}

void mpi_server_op_getid ( mpi_server_param_st *params, MPI_Comm sd, struct st_mpi_server_msg *head, int rank_client_id, int tag_client_id)
{
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getid] >> Begin\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getid] GETID(...)\n", params->rank);

  // do getid
  mpi_server_comm_write_data(params, sd, (char *)(head->id), MPI_SERVER_ID, rank_client_id, tag_client_id);

  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getid] GETID(...)=(...)\n", params->rank);
  debug_info("[Server=%d] [MPI_SERVER_OPS] [mpi_server_op_getid] << END\n", params->rank);
}


/* ................................................................... */

