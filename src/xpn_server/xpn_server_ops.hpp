
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

#pragma once

#include "base_c/filesystem.h"
#include "xpn/xpn_metadata.hpp"

/* Operations */

namespace XPN
{
    
  enum class xpn_server_ops{
    // File operations
    OPEN_FILE,
    CREAT_FILE,
    READ_FILE,
    WRITE_FILE,
    CLOSE_FILE,
    RM_FILE,
    RM_FILE_ASYNC,
    RENAME_FILE,
    GETATTR_FILE,
    SETATTR_FILE,

    // Directory operations
    MKDIR_DIR,
    RMDIR_DIR,
    RMDIR_DIR_ASYNC,
    OPENDIR_DIR,
    READDIR_DIR,
    CLOSEDIR_DIR,

    // FS Operations
    STATVFS_DIR,

    // Metadata
    READ_MDATA,
    WRITE_MDATA,
    WRITE_MDATA_FILE_SIZE,

    // Connection operatons
    FINALIZE,
    DISCONNECT,
    END,

    // For enum count
    size,
  };

  static const std::array<std::string, static_cast<size_t>(xpn_server_ops::size)+1> xpn_server_ops_names ={
    // File operations
    "OPEN_FILE",
    "CREAT_FILE",
    "READ_FILE",
    "WRITE_FILE",
    "CLOSE_FILE",
    "RM_FILE",
    "RM_FILE_ASYNC",
    "RENAME_FILE",
    "GETATTR_FILE",
    "SETATTR_FILE",

    // Directory operations
    "MKDIR_DIR",
    "RMDIR_DIR",
    "RMDIR_DIR_ASYNC",
    "OPENDIR_DIR",
    "READDIR_DIR",
    "CLOSEDIR_DIR",

    // FS Operations
    "STATVFS_DIR",

    // Metadata
    "READ_MDATA",
    "WRITE_MDATA",
    "WRITE_MDATA_FILE_SIZE",

    // Connection operatons
    "FINALIZE",
    "DISCONNECT",
    "END",

    // For enum count
    "size",
  };

  static inline const std::string& xpn_server_ops_name(xpn_server_ops op) {
    return xpn_server_ops_names[static_cast<size_t>(op)];
  }

  /* Message struct */

  struct st_xpn_server_status
  {
    int ret;
    int server_errno;
  };

  struct st_xpn_server_path_flags
  {
    char path[PATH_MAX];
    int flags;
    mode_t mode;
    char xpn_session;
  };

  struct st_xpn_server_path
  {
    char path[PATH_MAX];
  };

  struct st_xpn_server_close
  {
    int fd;
    ::DIR *dir;
  };

  struct st_xpn_server_rw
  {
    char path[PATH_MAX];
    int64_t offset;
    uint64_t size;
    int fd;
    char xpn_session;
  };

  struct st_xpn_server_rw_req
  {
    int64_t size;
    char last;
    struct st_xpn_server_status status;
  };

  struct st_xpn_server_rename
  {
    char old_url[PATH_MAX];
    char new_url[PATH_MAX];
  };

  struct st_xpn_server_setattr
  { 
    char path[PATH_MAX];
    struct ::stat attr;
  };

  struct st_xpn_server_attr_req
  {
    char status;  
    struct ::stat attr;
    struct st_xpn_server_status status_req;
  };

  struct st_xpn_server_readdir
  {
    char path[PATH_MAX];
    long telldir;
    ::DIR *dir;
    char xpn_session;
  };

  struct st_xpn_server_opendir_req
  {
    ::DIR *dir;
    struct st_xpn_server_status status;
  };

  struct st_xpn_server_readdir_req
  {
    int end; //If end = 1 exist entry; 0 not exist
    struct ::dirent ret;
    long telldir;
    struct st_xpn_server_status status;
  };

  struct st_xpn_server_read_mdata_req
  { 
    xpn_metadata::data mdata;
    struct st_xpn_server_status status;
  };

  struct st_xpn_server_write_mdata
  { 
    char path[PATH_MAX];
    xpn_metadata::data mdata;
  };

  struct st_xpn_server_write_mdata_file_size
  { 
    char path[PATH_MAX];
    uint64_t size;
  };
  
  struct st_xpn_server_statvfs_req
  {
    struct ::statvfs attr;
    struct st_xpn_server_status status_req;
  };

  struct st_xpn_server_end
  {
    char status;
  };


} // namespace XPN