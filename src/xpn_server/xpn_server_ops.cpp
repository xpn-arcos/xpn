
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

#include "xpn_server.hpp"
#include <stddef.h>

namespace XPN
{
  
template <typename OperationType, typename OperationFunction>
inline void handle_operation(xpn_server_comm *comm, int rank, int tag, struct st_xpn_server_msg &msg, OperationType& operation, OperationFunction op_function) {
    int ret = comm->read_data((char *)&operation, sizeof(operation), rank, tag);
    if (ret != -1) {
        op_function(*comm, msg, rank, tag);
    }
}

//Read the operation to realize
void xpn_server::do_operation ( xpn_server_comm *comm, int type_op, int rank, int tag )
{
  struct st_xpn_server_msg head;

  debug_info("[TH_ID=%d] [XPN_SERVER_OPS] [xpn_server_do_operation] >> Begin\n", th->id);
  debug_info("[TH_ID=%d] [XPN_SERVER_OPS] [xpn_server_do_operation] OP '%s'; OP_ID %d\n", th->id, xpn_server_op2string(type_op), type_op);

  switch (type_op)
  {
    //File API
    case XPN_SERVER_OPEN_FILE: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_open, xpn_server::op_open); break;
    case XPN_SERVER_CREAT_FILE: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_creat, xpn_server::op_creat); break;
    case XPN_SERVER_READ_FILE: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_read, xpn_server::op_read); break;
    case XPN_SERVER_WRITE_FILE: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_write, xpn_server::op_write); break;
    case XPN_SERVER_CLOSE_FILE: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_close, xpn_server::op_close); break;
    case XPN_SERVER_RM_FILE: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_rm, xpn_server::op_rm); break;
    case XPN_SERVER_RM_FILE_ASYNC: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_rm, xpn_server::op_rm_async); break;
    case XPN_SERVER_RENAME_FILE: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_rename, xpn_server::op_rename); break;
    case XPN_SERVER_GETATTR_FILE: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_getattr, xpn_server::op_getattr); break;
    case XPN_SERVER_SETATTR_FILE: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_setattr, xpn_server::op_setattr); break;

    //Directory API
    case XPN_SERVER_MKDIR_DIR: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_mkdir, xpn_server::op_mkdir); break;
    case XPN_SERVER_OPENDIR_DIR: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_opendir, xpn_server::op_opendir); break;
    case XPN_SERVER_READDIR_DIR: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_readdir, xpn_server::op_readdir); break;
    case XPN_SERVER_CLOSEDIR_DIR: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_closedir, xpn_server::op_closedir); break;
    case XPN_SERVER_RMDIR_DIR: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_rmdir, xpn_server::op_rmdir); break;
    case XPN_SERVER_RMDIR_DIR_ASYNC: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_rmdir, xpn_server::op_rmdir_async); break;

    //Metadata API
    case XPN_SERVER_READ_MDATA: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_read_mdata, xpn_server::op_read_mdata); break;
    case XPN_SERVER_WRITE_MDATA: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_write_mdata, xpn_server::op_write_mdata); break;
    case XPN_SERVER_WRITE_MDATA_FILE_SIZE: handle_operation(comm, rank, tag, head, head.u_st_xpn_server_msg.op_write_mdata_file_size, xpn_server::op_write_mdata_file_size); break;

    //Connection API
    case XPN_SERVER_DISCONNECT: break;
  }

  debug_info("[TH_ID=%d] [XPN_SERVER_OPS] [xpn_server_do_operation] << End\n", th->id);
}


// File API
void xpn_server::op_open ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_status status;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_open] >> Begin\n", params->rank);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_open] open(%s, %d, %d)\n", params->rank, head.u_st_xpn_server_msg.op_open.path, head.u_st_xpn_server_msg.op_open.flags, head.u_st_xpn_server_msg.op_open.mode);

  // do open
  status.ret = filesystem_open2(head.u_st_xpn_server_msg.op_open.path, head.u_st_xpn_server_msg.op_open.flags, head.u_st_xpn_server_msg.op_open.mode);
  status.server_errno = errno;
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_open] open(%s)=%d\n", params->rank, head.u_st_xpn_server_msg.op_open.path, status.ret);
  if (status.ret < 0){
    comm.write_data((char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);
  }else{
    if (head.u_st_xpn_server_msg.op_open.xpn_session == 0){
      status.ret = filesystem_close(status.ret);
    }
    status.server_errno = errno;

    comm.write_data((char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);
  }

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_open] << End\n", params->rank);
}

void xpn_server::op_creat ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_status status;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_creat] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_creat] creat(%s)\n", params->rank, head.u_st_xpn_server_msg.op_creat.path);

  // do creat
  status.ret = filesystem_creat(head.u_st_xpn_server_msg.op_creat.path, head.u_st_xpn_server_msg.op_creat.mode);
  status.server_errno = errno;
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_creat] creat(%s)=%d\n", params->rank, head.u_st_xpn_server_msg.op_creat.path, status.ret);
  if (status.ret < 0){
    comm.write_data((char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);
    return;
  }else{
    status.ret = filesystem_close(status.ret);
    status.server_errno = errno;

    comm.write_data((char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);
  }

  // show debug info
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_creat] << End\n", params->rank);
}

void xpn_server::op_read ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_rw_req req;
  char * buffer = NULL;
  long   size, diff, to_read, cont;
  off_t ret_lseek;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] >> Begin\n",  params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] read(%s, %ld %ld)\n", params->rank, head.u_st_xpn_server_msg.op_read.path, head.u_st_xpn_server_msg.op_read.offset, head.u_st_xpn_server_msg.op_read.size);

  // initialize counters
  cont = 0;
  size = head.u_st_xpn_server_msg.op_read.size;
  if (size > MAX_BUFFER_SIZE) {
    size = MAX_BUFFER_SIZE;
  }
  diff = head.u_st_xpn_server_msg.op_read.size - cont;

  //Open file
  int fd;
  if (head.u_st_xpn_server_msg.op_read.xpn_session == 1){
    fd = head.u_st_xpn_server_msg.op_read.fd;
  }else{
    fd = filesystem_open(head.u_st_xpn_server_msg.op_read.path, O_RDONLY);
  }
  if (fd < 0)
  {
    req.size = -1;
    req.status.ret = fd;
    req.status.server_errno = errno;
    comm.write_data((char *)&req,sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id);
    goto cleanup_xpn_server_op_read;
  }

  // malloc a buffer of size...
  buffer = (char *)malloc(size);
  if (NULL == buffer)
  {
    req.size = -1;
    req.status.ret = -1;
    req.status.server_errno = errno;
    comm.write_data((char *)&req,sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id);
    goto cleanup_xpn_server_op_read;
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
    ret_lseek = filesystem_lseek(fd, head.u_st_xpn_server_msg.op_read.offset + cont, SEEK_SET);
    if (ret_lseek == -1)
    {
      req.size = -1;
      req.status.ret = -1;
      req.status.server_errno = errno;
      comm.write_data((char *)&req,sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id);
      goto cleanup_xpn_server_op_read;
    }

    req.size = filesystem_read(fd, buffer, to_read);
    // if error then send as "how many bytes" -1
    if (req.size < 0 || req.status.ret == -1)
    {
      req.size = -1;
      req.status.ret = -1;
      req.status.server_errno = errno;
      comm.write_data((char *)&req,sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id);
      goto cleanup_xpn_server_op_read;
    }
    // send (how many + data) to client...
    req.status.ret = 0;
    req.status.server_errno = errno;
    comm.write_data((char *)&req, sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id);
    debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] op_read: send size %ld\n", params->rank, req.size);

    // send data to client...
    if (req.size > 0)
    {
      comm.write_data(buffer, req.size, rank_client_id, tag_client_id);
      debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] op_read: send data\n", params->rank);
    }
    cont = cont + req.size; //Send bytes
    diff = head.u_st_xpn_server_msg.op_read.size - cont;

  } while ((diff > 0) && (req.size != 0));
cleanup_xpn_server_op_read:
  if (head.u_st_xpn_server_msg.op_read.xpn_session == 0){
    filesystem_close(fd);
  }

  // free buffer
  FREE_AND_NULL(buffer);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] read(%s, %ld %ld)=%ld\n", params->rank, head.u_st_xpn_server_msg.op_read.path, head.u_st_xpn_server_msg.op_read.offset, head.u_st_xpn_server_msg.op_read.size, cont);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read] << End\n", params->rank);
}

void xpn_server::op_write ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_rw_req req;
  char * buffer = NULL;
  int    size, diff, cont, to_write;
  off_t ret_lseek;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write] write(%s, %ld %ld)\n", params->rank, head.u_st_xpn_server_msg.op_write.path, head.u_st_xpn_server_msg.op_write.offset, head.u_st_xpn_server_msg.op_write.size);

  // initialize counters
  cont = 0;
  size = (head.u_st_xpn_server_msg.op_write.size);
  if (size > MAX_BUFFER_SIZE) {
    size = MAX_BUFFER_SIZE;
  }
  diff = head.u_st_xpn_server_msg.op_read.size - cont;

  //Open file
  int fd;
  if (head.u_st_xpn_server_msg.op_write.xpn_session == 1){
    fd = head.u_st_xpn_server_msg.op_write.fd;
  }else{
    fd = filesystem_open(head.u_st_xpn_server_msg.op_write.path, O_WRONLY);
  }
  if (fd < 0)
  {
    req.size = -1;
    req.status.ret = -1;
    goto cleanup_xpn_server_op_write;
  }

  // malloc a buffer of size...
  buffer = (char *)malloc(size);
  if (NULL == buffer)
  {
    req.size = -1;
    req.status.ret = -1;
    goto cleanup_xpn_server_op_write;
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
    comm.read_data(buffer, to_write, rank_client_id, tag_client_id);
    ret_lseek = filesystem_lseek(fd, head.u_st_xpn_server_msg.op_write.offset + cont, SEEK_SET);
    if (ret_lseek < 0)
    {
      req.status.ret = -1;
      goto cleanup_xpn_server_op_write;
    }
    req.size = filesystem_write(fd, buffer, to_write);
    if (req.size < 0)
    {
      req.status.ret = -1;
      goto cleanup_xpn_server_op_write;
    }

    // update counters
    cont = cont + req.size; // Received bytes
    diff = head.u_st_xpn_server_msg.op_read.size - cont;

  } while ((diff > 0) && (req.size != 0));

  req.size = cont;
  req.status.ret = 0;
cleanup_xpn_server_op_write:
  // write to the client the status of the write operation
  req.status.server_errno = errno;
  comm.write_data((char *)&req,sizeof(struct st_xpn_server_rw_req), rank_client_id, tag_client_id);

  if (head.u_st_xpn_server_msg.op_write.xpn_session == 1){
    filesystem_fsync(fd);
  }else{
    filesystem_close(fd);
  }

  // free buffer
  FREE_AND_NULL(buffer);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write] write(%s, %ld %ld)=%d\n", params->rank, head.u_st_xpn_server_msg.op_write.path, head.u_st_xpn_server_msg.op_write.offset, head.u_st_xpn_server_msg.op_write.size, cont);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write] << End\n", params->rank);
}

void xpn_server::op_close ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_status status;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_close] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_close] close(%d)\n", params->rank, head.u_st_xpn_server_msg.op_close.fd);

  // do rm
  status.ret = filesystem_close(head.u_st_xpn_server_msg.op_close.fd);
  status.server_errno = errno;
  comm.write_data((char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_close] close(%d)=%d\n", params->rank, head.u_st_xpn_server_msg.op_close.fd, status.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_close] << End\n", params->rank);

}

void xpn_server::op_rm ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_status status;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm] unlink(%s)\n", params->rank, head.u_st_xpn_server_msg.op_rm.path);

  // do rm
  status.ret = filesystem_unlink(head.u_st_xpn_server_msg.op_rm.path);
  status.server_errno = errno;
  comm.write_data((char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm] unlink(%s)=%d\n", params->rank, head.u_st_xpn_server_msg.op_rm.path, status.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm] << End\n", params->rank);
}

void xpn_server::op_rm_async ( [[maybe_unused]] xpn_server_comm &comm, struct st_xpn_server_msg &head, [[maybe_unused]] int rank_client_id, [[maybe_unused]] int tag_client_id )
{
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm_async] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm_async] unlink(%s)\n", params->rank, head.u_st_xpn_server_msg.op_rm.path);

  // do rm
  filesystem_unlink(head.u_st_xpn_server_msg.op_rm.path);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm_async] unlink(%s)=%d\n", params->rank, head.u_st_xpn_server_msg.op_rm.path, 0);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rm_async] << End\n", params->rank);
}

void xpn_server::op_rename ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_status status;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rename] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rename] rename(%s, %s)\n", params->rank, head.u_st_xpn_server_msg.op_rename.old_url, head.u_st_xpn_server_msg.op_rename.new_url);

  // do rename
  status.ret = filesystem_rename(head.u_st_xpn_server_msg.op_rename.old_url, head.u_st_xpn_server_msg.op_rename.new_url);
  status.server_errno = errno;
  comm.write_data((char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rename] rename(%s, %s)=%d\n", params->rank, head.u_st_xpn_server_msg.op_rename.old_url, head.u_st_xpn_server_msg.op_rename.new_url, status.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rename] << End\n", params->rank);
}

void xpn_server::op_getattr ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_attr_req req;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getattr] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getattr] stat(%s)\n", params->rank, head.u_st_xpn_server_msg.op_getattr.path);

  // do getattr
  req.status = filesystem_stat(head.u_st_xpn_server_msg.op_getattr.path, &req.attr);
  req.status_req.ret = req.status;
  req.status_req.server_errno = errno;

  comm.write_data((char *)&req,sizeof(struct st_xpn_server_attr_req), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getattr] stat(%s)=%d\n", params->rank, head.u_st_xpn_server_msg.op_getattr.path, req.status);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_getattr] << End\n", params->rank);
}

void xpn_server::op_setattr ( [[maybe_unused]] xpn_server_comm &comm, [[maybe_unused]] struct st_xpn_server_msg &head, [[maybe_unused]] int rank_client_id, [[maybe_unused]] int tag_client_id)
{
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_setattr] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_setattr] SETATTR(...)\n", params->rank);

  // do setattr
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_setattr] SETATTR operation to be implemented !!\n", params->rank);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_setattr] SETATTR(...)=(...)\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_setattr] << End\n", params->rank);
}

//Directory API
void xpn_server::op_mkdir ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_status status;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_mkdir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_mkdir] mkdir(%s)\n", params->rank, head.u_st_xpn_server_msg.op_mkdir.path);

  // do mkdir
  status.ret = filesystem_mkdir(head.u_st_xpn_server_msg.op_mkdir.path, head.u_st_xpn_server_msg.op_mkdir.mode);
  status.server_errno = errno;
  comm.write_data((char *)&status,sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_mkdir] mkdir(%s)=%d\n", params->rank, head.u_st_xpn_server_msg.op_mkdir.path, status.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_mkdir] << End\n", params->rank);
}

void xpn_server::op_opendir ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  DIR* ret;
  struct st_xpn_server_opendir_req req;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_opendir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_opendir] opendir(%s)\n", params->rank, head.u_st_xpn_server_msg.op_opendir.path);

  ret = filesystem_opendir(head.u_st_xpn_server_msg.op_opendir.path);
  req.status.ret = ret == NULL ? -1 : 0;
  req.status.server_errno = errno;

  if (req.status.ret == 0){
    if (head.u_st_xpn_server_msg.op_opendir.xpn_session == 1){
      req.dir = ret;
    }else{
      req.status.ret = filesystem_telldir(ret);
    }
    req.status.server_errno = errno;
  }

  if (head.u_st_xpn_server_msg.op_opendir.xpn_session == 0){
    filesystem_closedir(ret);
  }

  comm.write_data((char *)&req, sizeof(struct st_xpn_server_opendir_req), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_opendir] opendir(%s)=%p\n", params->rank, head.u_st_xpn_server_msg.op_opendir.path, ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_opendir] << End\n", params->rank);
}

void xpn_server::op_readdir ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  struct dirent * ret;
  struct st_xpn_server_readdir_req ret_entry;
  DIR* s = NULL;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_readdir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_readdir] readdir(%s)\n", params->rank, head.u_st_xpn_server_msg.op_readdir.path);

  if (head.u_st_xpn_server_msg.op_readdir.xpn_session == 1){
    // Reset errno
    errno = 0;
    ret = filesystem_readdir(head.u_st_xpn_server_msg.op_readdir.dir);
  }else{

    s = filesystem_opendir(head.u_st_xpn_server_msg.op_readdir.path);
    ret_entry.status.ret = s == NULL ? -1 : 0;
    ret_entry.status.server_errno = errno;

    filesystem_seekdir(s, head.u_st_xpn_server_msg.op_readdir.telldir);

    // Reset errno
    errno = 0;
    ret = filesystem_readdir(s);
  }
  if (ret != NULL)
  {
    ret_entry.end = 1;
    ret_entry.ret = *ret;
  }
  else{
    ret_entry.end = 0;
  }

  ret_entry.status.ret = ret == NULL ? -1 : 0;

  if (head.u_st_xpn_server_msg.op_readdir.xpn_session == 0){
    ret_entry.telldir = filesystem_telldir(s);

    ret_entry.status.ret = filesystem_closedir(s);
  }
  ret_entry.status.server_errno = errno;

  comm.write_data((char *)&ret_entry, sizeof(struct st_xpn_server_readdir_req), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_readdir] readdir(%p)=%p\n", params->rank, s, ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_readdir] << End\n", params->rank);
}

void xpn_server::op_closedir ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_status status;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_closedir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_closedir] closedir(%ld)\n", params->rank, head.u_st_xpn_server_msg.op_closedir.dir);

  // do rm
  status.ret = filesystem_closedir(head.u_st_xpn_server_msg.op_closedir.dir);
  status.server_errno = errno;
  comm.write_data((char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_closedir] closedir(%d)=%d\n", params->rank, head.u_st_xpn_server_msg.op_closedir.dir, status.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_closedir] << End\n", params->rank);

}

void xpn_server::op_rmdir ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  struct st_xpn_server_status status;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir] rmdir(%s)\n", params->rank, head.u_st_xpn_server_msg.op_rmdir.path);

  // do rmdir
  status.ret = filesystem_rmdir(head.u_st_xpn_server_msg.op_rmdir.path);
  status.server_errno = errno;
  comm.write_data((char *)&status, sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir] rmdir(%s)=%d\n", params->rank, head.u_st_xpn_server_msg.op_rmdir.path, status.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir] << End\n", params->rank);
}

void xpn_server::op_rmdir_async ( [[maybe_unused]] xpn_server_comm &comm, struct st_xpn_server_msg &head, [[maybe_unused]] int rank_client_id, [[maybe_unused]] int tag_client_id )
{
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir_async] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir_async] rmdir(%s)\n", params->rank, head.u_st_xpn_server_msg.op_rmdir.path);

  // do rmdir
  filesystem_rmdir(head.u_st_xpn_server_msg.op_rmdir.path);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir_async] rmdir(%s)=%d\n", params->rank, head.u_st_xpn_server_msg.op_rmdir.path, 0);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_rmdir_async] << End\n", params->rank);
}

void xpn_server::op_read_mdata   ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  int ret, fd;
  struct st_xpn_server_read_mdata_req req = {0};

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read_mdata] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read_mdata] read_mdata(%s)\n", params->rank, head.u_st_xpn_server_msg.op_read_mdata.path);

  fd = filesystem_open(head.u_st_xpn_server_msg.op_read_mdata.path, O_RDWR);
  if (fd < 0){
    if (errno == EISDIR){
      // if is directory there are no metadata to read so return 0
      ret = 0;
	    memset(&req.mdata, 0, sizeof(struct xpn_metadata));
      goto cleanup_xpn_server_op_read_mdata;
    }
    ret = fd;
    goto cleanup_xpn_server_op_read_mdata;
  }

  ret = filesystem_read(fd, &req.mdata, sizeof(struct xpn_metadata));

  if (!XPN_CHECK_MAGIC_NUMBER(&req.mdata)){
	  memset(&req.mdata, 0, sizeof(struct xpn_metadata));
  }

  filesystem_close(fd); //TODO: think if necesary check error in close

cleanup_xpn_server_op_read_mdata:
  req.status.ret = ret;
  req.status.server_errno = errno;

  comm.write_data((char *)&req,sizeof(struct st_xpn_server_read_mdata_req), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read_mdata] read_mdata(%s)=%d\n", params->rank, head.u_st_xpn_server_msg.op_read_mdata.path, req.status.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_read_mdata] << End\n", params->rank);
}

void xpn_server::op_write_mdata ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  int ret, fd;
  struct st_xpn_server_status req;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write_mdata] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write_mdata] write_mdata(%s)\n", params->rank, head.u_st_xpn_server_msg.op_write_mdata.path);

  fd = filesystem_open2(head.u_st_xpn_server_msg.op_write_mdata.path, O_WRONLY | O_CREAT, S_IRWXU);
  if (fd < 0){
    if (errno == EISDIR){
      // if is directory there are no metadata to write so return 0
      ret = 0;
      goto cleanup_xpn_server_op_write_mdata;
    }
    ret = fd;
    goto cleanup_xpn_server_op_write_mdata;
  }

  ret = filesystem_write(fd, &head.u_st_xpn_server_msg.op_write_mdata.mdata, sizeof(struct xpn_metadata));

  filesystem_close(fd); //TODO: think if necesary check error in close

cleanup_xpn_server_op_write_mdata:
  req.ret = ret;
  req.server_errno = errno;

  comm.write_data((char *)&req,sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write_mdata] write_mdata(%s)=%d\n", params->rank, head.u_st_xpn_server_msg.op_write_mdata.path, req.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write_mdata] << End\n", params->rank);

}

pthread_mutex_t op_write_mdata_file_size_mutex = PTHREAD_MUTEX_INITIALIZER;

void xpn_server::op_write_mdata_file_size ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id )
{
  int ret, fd;
  ssize_t actual_file_size = 0;
  struct st_xpn_server_status req;

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write_mdata_file_size] >> Begin\n", params->rank);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write_mdata_file_size] write_mdata_file_size(%s, %ld)\n", params->rank, head.u_st_xpn_server_msg.op_write_mdata_file_size.path, head.u_st_xpn_server_msg.op_write_mdata_file_size.size);
  
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write_mdata_file_size] mutex lock\n", params->rank);
  pthread_mutex_lock(&op_write_mdata_file_size_mutex);

  fd = filesystem_open(head.u_st_xpn_server_msg.op_write_mdata_file_size.path, O_RDWR);
  if (fd < 0){
    if (errno == EISDIR){
      // if is directory there are no metadata to write so return 0
      ret = 0;
      goto cleanup_xpn_server_op_write_mdata_file_size;
    }
    ret = fd;
    goto cleanup_xpn_server_op_write_mdata_file_size;
  }

  filesystem_lseek(fd, offsetof(struct xpn_metadata, file_size), SEEK_SET);
  ret = filesystem_read(fd, &actual_file_size, sizeof(ssize_t));
  if (ret > 0 && actual_file_size < head.u_st_xpn_server_msg.op_write_mdata_file_size.size){
    filesystem_lseek(fd, offsetof(struct xpn_metadata, file_size), SEEK_SET);
    ret = filesystem_write(fd, &head.u_st_xpn_server_msg.op_write_mdata_file_size.size, sizeof(ssize_t));
  }

  filesystem_close(fd); //TODO: think if necesary check error in close

cleanup_xpn_server_op_write_mdata_file_size:

  pthread_mutex_unlock(&op_write_mdata_file_size_mutex);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write_mdata_file_size] mutex unlock\n", params->rank);

  req.ret = ret;
  req.server_errno = errno;

  comm.write_data((char *)&req,sizeof(struct st_xpn_server_status), rank_client_id, tag_client_id);

  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write_mdata_file_size] write_mdata_file_size(%s, %ld)=%d\n", params->rank, head.u_st_xpn_server_msg.op_write_mdata_file_size.path, head.u_st_xpn_server_msg.op_write_mdata_file_size.size, req.ret);
  debug_info("[Server=%d] [XPN_SERVER_OPS] [xpn_server_op_write_mdata_file_size] << End\n", params->rank);

}
/* ................................................................... */

} // namespace XPN
