
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

#include "nfi_xpn_server.hpp"
#include "xpn/xpn_file.hpp"
#include "base_cpp/debug.hpp"
#include "base_cpp/xpn_env.hpp"
#include "xpn_server/xpn_server_ops.hpp"

namespace XPN
{

// File API
int nfi_xpn_server::nfi_open (const std::string &path, int flags, mode_t mode, xpn_fh &fho)
{
  int ret;
  st_xpn_server_path_flags msg;
  st_xpn_server_status status;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_open] >> Begin");

  fho.path = m_path + "/" + path;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_open] nfi_xpn_server_open("<<fho.path<<", "<<flags<<", "<<mode<<")");

  std::size_t length = fho.path.copy(msg.path, PATH_MAX - 1);
  msg.path[length] = '\0';
  msg.flags = flags;
  msg.mode = mode;
  msg.xpn_session = xpn_env::get_instance().xpn_session_file;

  ret = nfi_do_request(XPN_SERVER_OPEN_FILE, msg, status);
  if (status.ret < 0 || ret < 0){ 
    errno = status.server_errno;
    debug_error("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_open] ERROR: remote open fails to open '"<<fho.path<<"'");
    return -1;
  }

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_open] nfi_xpn_server_open("<<fho.path<<")="<<status.ret);
  
  fho.fd = status.ret;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_open] >> End");

  return status.ret;
}

int nfi_xpn_server::nfi_create (const std::string &path, mode_t mode, xpn_fh &fho)
{
  //NOTE: actualy creat is not in use, it use like POSIX open(path, O_WRONLY|O_CREAT|O_TRUNC, mode);
  return nfi_open(path, O_WRONLY|O_CREAT|O_TRUNC, mode, fho);
}

int nfi_xpn_server::nfi_close (const xpn_fh &fh)
{
  if (xpn_env::get_instance().xpn_session_file == 1){
    st_xpn_server_close msg;
    st_xpn_server_status status;

    debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_close] >> Begin");

    debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_close] nfi_xpn_server_close("<<fh.fd<<")");

    msg.fd = fh.fd;

    nfi_do_request(XPN_SERVER_CLOSE_FILE, msg, status);

    if (status.ret < 0){
      errno = status.server_errno;
    }

    debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_close] nfi_xpn_server_close("<<fh.fd<<")="<<status.ret);
    debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_close] >> End");
    
    return status.ret;
  }else{
    // Without sesion close do nothing
    return 0;
  }
}

int64_t nfi_xpn_server::nfi_read (const xpn_fh &fh, char *buffer, int64_t offset, uint64_t size)
{
  int64_t ret, cont, diff;
  st_xpn_server_rw msg;
  st_xpn_server_rw_req req;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read] >> Begin");

  // Check arguments...
  if (size == 0) {
    return 0;
  }

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read] nfi_xpn_server_read("<<fh.path<<", "<<offset<<", "<<size<<")");

  if (!xpn_env::get_instance().xpn_session_connect && m_comm == nullptr){
    m_comm = m_control_comm->connect(m_server);
  }

  std::size_t length = fh.path.copy(msg.path, PATH_MAX - 1);
  msg.path[length] = '\0';
  msg.offset      = offset;
  msg.size        = size;
  msg.fd          = fh.fd;
  msg.xpn_session = xpn_env::get_instance().xpn_session_file;

  ret = nfi_write_operation(XPN_SERVER_READ_FILE, msg);
  if (ret < 0)
  {
    debug_error("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read] ERROR: nfi_write_operation fails");
    return -1;
  }

  // read n times: number of bytes + read data (n bytes)
  cont = 0;
  do
  {
    ret = m_comm->read_data(&req, sizeof(req));
    if (ret < 0)
    {
      debug_error("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read] ERROR: nfi_xpn_server_comm_read_data fails");
      return -1;
    }

    if (req.status.ret < 0){
      errno = req.status.server_errno;
      return -1;
    }
    
    debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read] nfi_xpn_server_comm_read_data="<<ret);

    if (req.size > 0)
    {
      debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read] nfi_xpn_server_comm_read_data("<<req.size<<")");

      ret = m_comm->read_data(buffer+cont, req.size);
      if (ret < 0) {
        debug_error("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read] ERROR: nfi_xpn_server_comm_read_data fails");
      }

      debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read] nfi_xpn_server_comm_read_data("<<req.size<<")="<< ret);
    }
    cont = cont + req.size;
    diff = msg.size - cont;

  } while ((diff > 0) && (req.size != 0));

  if (req.size < 0)
  {
    debug_error("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read] ERROR: nfi_xpn_server_read reads zero bytes from '"<<fh.path<<"'");
    if (req.status.ret < 0)
      errno = req.status.server_errno;
    return -1;
  }

  if (req.status.ret < 0)
    errno = req.status.server_errno;

  ret = cont;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read] nfi_xpn_server_read("<<fh.path<<", "<<offset<<", "<<size<<")="<<ret);
  
  if (!xpn_env::get_instance().xpn_session_connect){
    m_control_comm->disconnect(m_comm);
    m_comm = nullptr;
  }

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read] >> End");

  return ret;
}

ssize_t nfi_xpn_server::nfi_write (const xpn_fh &fh, const char *buffer, int64_t offset, uint64_t size)
{
  int ret, diff, cont;
  st_xpn_server_rw msg;
  st_xpn_server_rw_req req;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write] >> Begin");

  // Check arguments...
  if (size == 0) {
    return 0;
  }

  if (!xpn_env::get_instance().xpn_session_connect && m_comm == nullptr){
    m_comm = m_control_comm->connect(m_server);
  }

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write] nfi_xpn_server_write("<<fh.path<<", "<<offset<<", "<<size<<")");

  std::size_t length = fh.path.copy(msg.path, PATH_MAX - 1);
  msg.path[length] = '\0';
  msg.offset      = offset;
  msg.size        = size;
  msg.fd          = fh.fd;
  msg.xpn_session = xpn_env::get_instance().xpn_session_file;

  ret = nfi_write_operation(XPN_SERVER_WRITE_FILE, msg);
  if(ret < 0)
  {
    debug_error("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write] ERROR: nfi_write_operation fails");
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
      ret = m_comm->write_data(buffer + cont, buffer_size);
      if (ret < 0) {
        debug_error("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write] ERROR: nfi_xpn_server_comm_write_data fails");
      }
    }
    else
    {
      ret = m_comm->write_data(buffer + cont, diff);
      if (ret < 0) {
        debug_error("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write] ERROR: nfi_xpn_server_comm_write_data fails");
      }
    }

    debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write] nfi_xpn_server_comm_write_data="<< ret);

    cont = cont + ret; //Send bytes
    diff = size - cont;

  } while ((diff > 0) && (ret != 0));

  ret = m_comm->read_data(&req, sizeof(req));
  if (ret < 0) 
  {
    debug_error("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write] ERROR: nfi_xpn_server_comm_read_data fails");
    return -1;
  }

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write] nfi_xpn_server_comm_read_data="<< ret);

  if (req.size < 0)
  {
    debug_error("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write] ERROR: nfi_xpn_server_write writes zero bytes from '"<<fh.path<<"'");
    if (req.status.ret < 0)
      errno = req.status.server_errno;
    return -1;
  }

  if (req.status.ret < 0)
    errno = req.status.server_errno;

  ret = cont;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write] nfi_xpn_server_write("<<fh.path<<", "<<offset<<", "<<size<<")="<<ret);
  
  if (!xpn_env::get_instance().xpn_session_connect){
    m_control_comm->disconnect(m_comm);
    m_comm = nullptr;
  }

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write] >> End");

  return ret;
}

int nfi_xpn_server::nfi_remove (const std::string &path, bool is_async)
{
  int ret;
  st_xpn_server_path msg;
  st_xpn_server_status req;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_remove] >> Begin");

  std::string srv_path = m_path + "/" + path;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_remove] nfi_xpn_server_remove("<<srv_path<<", "<<is_async<<")");

  std::size_t length = srv_path.copy(msg.path, PATH_MAX - 1);
  msg.path[length] = '\0';
  if (is_async)
  {
    ret = nfi_write_operation(XPN_SERVER_RM_FILE_ASYNC, msg);
  }
  else
  {
    ret = nfi_do_request(XPN_SERVER_RM_FILE, msg, req);
    if (req.ret < 0)
      errno = req.server_errno;
    ret = req.ret;
  }

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_remove] nfi_xpn_server_remove("<<srv_path<<", "<<is_async<<")="<<ret);
  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_remove] >> End");

  return ret;
}

int nfi_xpn_server::nfi_rename (const std::string &path, const std::string &new_path)
{
  int ret;
  st_xpn_server_rename msg;
  st_xpn_server_status req;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_rename] >> Begin");

  std::string srv_path = m_path + "/" + path;
  std::string new_srv_path = m_path + "/" + new_path;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_rename] nfi_xpn_server_rename("<<srv_path<<", "<<new_srv_path<<")");

  std::size_t length = srv_path.copy(msg.old_url, PATH_MAX - 1);
  msg.old_url[length] = '\0';
  
  length = new_srv_path.copy(msg.new_url, PATH_MAX - 1);
  msg.new_url[length] = '\0';

  ret = nfi_do_request(XPN_SERVER_RENAME_FILE, msg, req);
  if (req.ret < 0){
    errno = req.server_errno;
    ret = req.ret;
  }

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_rename] nfi_xpn_server_rename("<<srv_path<<", "<<new_srv_path<<")="<<ret);
  debug_info("[NFI_XPN] [nfi_xpn_server_rename] >> End\n");

  return ret;
}

int nfi_xpn_server::nfi_getattr (const std::string &path, struct ::stat &st)
{
  int ret;
  st_xpn_server_path msg;
  st_xpn_server_attr_req req;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_getattr] >> Begin");

  std::string srv_path = m_path + "/" + path;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_getattr] nfi_xpn_server_getattr("<<srv_path<<")");

  std::size_t length = srv_path.copy(msg.path, PATH_MAX - 1);
  msg.path[length] = '\0';

  ret = nfi_do_request(XPN_SERVER_GETATTR_FILE, msg, req);

  memcpy(&st, &req.attr, sizeof(req.attr));

  if (req.status_req.ret < 0){
    errno = req.status_req.server_errno;
    ret = req.status_req.ret;
  }
  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_getattr] nfi_xpn_server_getattr("<<srv_path<<")="<<ret);

  debug_info("[NFI_XPN] [nfi_xpn_server_getattr] >> End\n");

  return ret;
}

int nfi_xpn_server::nfi_setattr ([[maybe_unused]] const std::string &path, [[maybe_unused]] struct ::stat &st)
{
  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_setattr] >> Begin");

  // TODO: setattr

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_setattr] >> End");

  return 0;
}

// Directories API
int nfi_xpn_server::nfi_mkdir(const std::string &path, mode_t mode)
{
  int ret;
  st_xpn_server_path_flags msg;
  st_xpn_server_status req;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_mkdir] >> Begin");

  std::string srv_path = m_path + "/" + path;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_mkdir] nfi_xpn_server_mkdir("<<srv_path<<")");

  std::size_t length = srv_path.copy(msg.path, PATH_MAX - 1);
  msg.path[length] = '\0';
  msg.mode = mode;

  ret = nfi_do_request(XPN_SERVER_MKDIR_DIR, msg, req);

  if (req.ret < 0){
    errno = req.server_errno;
  }

  if ((req.ret < 0)&&(errno != EEXIST))
  {
    debug_error("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_mkdir] ERROR: xpn_mkdir fails to mkdir '"<<srv_path<<"'");
    return -1;
  }

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_mkdir] nfi_xpn_server_mkdir("<<srv_path<<")="<<ret);

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_mkdir] >> End");

  return ret;
}

int nfi_xpn_server::nfi_opendir(const std::string &path, xpn_fh &fho)
{
  int ret;
  st_xpn_server_path_flags msg;
  st_xpn_server_opendir_req req;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_opendir] >> Begin");

  fho.path = m_path + "/" + path;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_opendir] nfi_xpn_server_opendir("<<fho.path<<")");
  
  std::size_t length = fho.path.copy(msg.path, PATH_MAX - 1);
  msg.path[length] = '\0';
  msg.xpn_session = xpn_env::get_instance().xpn_session_dir;

  ret = nfi_do_request(XPN_SERVER_OPENDIR_DIR, msg, req);
  if (req.status.ret < 0)
  {
    errno = req.status.server_errno;
    return req.status.ret;
  }

  fho.telldir = req.status.ret;
  fho.dir = req.dir;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_opendir] nfi_xpn_server_opendir("<<fho.path<<")="<<ret);

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_opendir] >> End");

  return ret;
}

int nfi_xpn_server::nfi_readdir(xpn_fh &fhd, struct ::dirent &entry)
{
  int ret;
  st_xpn_server_readdir msg;
  st_xpn_server_readdir_req req;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_readdir] >> Begin");

  // clean all entry content
  memset(&entry, 0, sizeof(struct ::dirent));

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_readdir] nfi_xpn_server_readdir("<<fhd.path<<")");

  std::size_t length = fhd.path.copy(msg.path, PATH_MAX - 1);
  msg.path[length] = '\0';
  msg.telldir =     fhd.telldir;
  msg.dir =         fhd.dir;
  msg.xpn_session = xpn_env::get_instance().xpn_session_dir;

  ret = nfi_do_request(XPN_SERVER_READDIR_DIR, msg, req);
  
  if (req.status.ret < 0){
    errno = req.status.server_errno;
    ret = req.status.ret;
  }else{
    fhd.telldir = req.telldir;
  }
  
  if (req.end == 0) {
    return -1;
  }

  memcpy(&entry, &(req.ret), sizeof(struct dirent));

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_readdir] nfi_xpn_server_readdir("<<fhd.path<<")="<<(void*)&entry);
  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_readdir] >> End");

  return ret;
}

int nfi_xpn_server::nfi_closedir (const xpn_fh &fhd)
{
  if (xpn_env::get_instance().xpn_session_dir == 1){
    int ret;
    struct st_xpn_server_close msg;
    struct st_xpn_server_status req;

    debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_closedir] >> Begin");

    debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_closedir] nfi_xpn_server_closedir("<<fhd.dir<<")");

    msg.dir = fhd.dir;

    ret = nfi_do_request(XPN_SERVER_CLOSEDIR_DIR, msg, req);

    if (req.ret < 0){
      errno = req.server_errno;
      ret = req.ret;
    }

    debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_closedir] nfi_xpn_server_closedir("<<fhd.dir<<")="<<ret);
    debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_closedir] >> End");

    return ret;
  }else{
    // Without sesion close do nothing
    return 0;
  }
}

int nfi_xpn_server::nfi_rmdir(const std::string &path, bool is_async)
{
  int ret;
  struct st_xpn_server_path msg;
  struct st_xpn_server_status req;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_rmdir] >> Begin");

  std::string srv_path = m_path + "/" + path;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_rmdir] nfi_xpn_server_rmdir("<<srv_path<<")");

  std::size_t length = srv_path.copy(msg.path, PATH_MAX - 1);
  msg.path[length] = '\0';

  if (is_async)
  {
    ret = nfi_write_operation(XPN_SERVER_RMDIR_DIR_ASYNC, msg);
  }
  else
  {
    ret = nfi_do_request(XPN_SERVER_RMDIR_DIR, msg, req);
    if (req.ret < 0){
      errno = req.server_errno;
      ret = req.ret;
    }
  }

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_rmdir] nfi_xpn_server_rmdir("<<srv_path<<")="<<ret);
  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_rmdir] >> End");

  return ret;
}

int nfi_xpn_server::nfi_statvfs(const std::string &path, struct ::statvfs &inf)
{
  int ret;
  struct st_xpn_server_path msg;
  struct st_xpn_server_statvfs_req req;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_statvfs] >> Begin");

  std::string srv_path = m_path + "/" + path;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_statvfs] nfi_xpn_server_statvfs("<<srv_path<<")");

  std::size_t length = srv_path.copy(msg.path, PATH_MAX - 1);
  msg.path[length] = '\0';

  ret = nfi_do_request(XPN_SERVER_STATVFS_DIR, msg, req);

  memcpy(&inf, &req.attr, sizeof(req.attr));

  if (req.status_req.ret < 0){
    errno = req.status_req.server_errno;
    ret = req.status_req.ret;
  }

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_statvfs] nfi_xpn_server_statvfs("<<srv_path<<")="<<ret);
  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_statvfs] >> End");

  return ret;
}

int nfi_xpn_server::nfi_read_mdata (const std::string &path, xpn_metadata &mdata)
{
  int ret;
  struct st_xpn_server_path msg;
  struct st_xpn_server_read_mdata_req req;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read_mdata] >> Begin");

  std::string srv_path = m_path + "/" + path;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read_mdata] nfi_xpn_server_read_mdata("<<srv_path<<")");

  std::size_t length = srv_path.copy(msg.path, PATH_MAX - 1);
  msg.path[length] = '\0';

  ret = nfi_do_request(XPN_SERVER_READ_MDATA, msg, req);

  if (req.status.ret < 0){
    errno = req.status.server_errno;
    ret = req.status.ret;
  }
  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_read_mdata] nfi_xpn_server_read_mdata("<<srv_path<<")="<<ret);

  memcpy(&mdata.m_data, &req.mdata, sizeof(req.mdata));

  debug_info("[NFI_XPN] [nfi_xpn_server_read_mdata] >> End\n");

  return ret;
}

int nfi_xpn_server::nfi_write_mdata (const std::string &path, const xpn_metadata &mdata, bool only_file_size)
{
  int ret;
  struct st_xpn_server_status req;

  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write_mdata] >> Begin");

  std::string srv_path = m_path + "/" + path;
  
  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write_mdata] nfi_xpn_server_write_mdata("<<srv_path<<")");

  if (only_file_size){
    struct st_xpn_server_write_mdata_file_size msg;
    std::size_t length = srv_path.copy(msg.path, PATH_MAX - 1);
    msg.path[length] = '\0';
    msg.size = mdata.m_data.file_size;
    ret = nfi_do_request(XPN_SERVER_WRITE_MDATA_FILE_SIZE, msg, req);
  }else{
    struct st_xpn_server_write_mdata msg;
    std::size_t length = srv_path.copy(msg.path, PATH_MAX - 1);
    msg.path[length] = '\0';
    memcpy(&msg.mdata, &mdata.m_data, sizeof(mdata.m_data));
    ret = nfi_do_request(XPN_SERVER_WRITE_MDATA, msg, req);
  }

  if (req.ret < 0){
    errno = req.server_errno;
    ret = req.ret;
  }
  debug_info("[SERV_ID="<<m_server<<"] [NFI_XPN] [nfi_xpn_server_write_mdata] nfi_xpn_server_write_mdata("<<srv_path<<")="<<ret);

  debug_info("[NFI_XPN] [nfi_xpn_server_write_mdata] >> End\n");

  return ret;
}

} // namespace XPN
