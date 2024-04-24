
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


#ifndef _XPN_SERVER_OPS_H_
#define _XPN_SERVER_OPS_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

  #include <libgen.h>
  #include "all_system.h"
  #include "base/filesystem.h"
  #include "base/urlstr.h"
  #include "base/utils.h"
  #include "base/workers.h"


  /* ... Const / Const ................................................. */

  #define XPN_HEADER_SIZE 8192

  /* Operations */

  // File operations
  #define XPN_SERVER_OPEN_FILE      0
  #define XPN_SERVER_CREAT_FILE     1
  #define XPN_SERVER_READ_FILE      2
  #define XPN_SERVER_WRITE_FILE     3
  #define XPN_SERVER_CLOSE_FILE     4
  #define XPN_SERVER_RM_FILE        5
  #define XPN_SERVER_RM_FILE_ASYNC  6
  #define XPN_SERVER_RENAME_FILE    7
  #define XPN_SERVER_GETATTR_FILE   8
  #define XPN_SERVER_SETATTR_FILE   9

  // Directory operations
  #define XPN_SERVER_MKDIR_DIR        20
  #define XPN_SERVER_RMDIR_DIR        21
  #define XPN_SERVER_RMDIR_DIR_ASYNC  22
  #define XPN_SERVER_OPENDIR_DIR      23
  #define XPN_SERVER_READDIR_DIR      24
  #define XPN_SERVER_CLOSEDIR_DIR     25

  // FS Operations
  #define XPN_SERVER_STATFS_DIR     60

  // Connection operatons
  #define XPN_SERVER_FINALIZE       80
  #define XPN_SERVER_DISCONNECT     81
  #define XPN_SERVER_END            -1


  /* ... Data structures / Estructuras de datos ........................ */

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
  };

  struct st_xpn_server_path
  {
    char path[PATH_MAX];
  };

  struct st_xpn_server_rw
  {
    char path[PATH_MAX];
    offset_t offset;
    size_t size;
  };

  struct st_xpn_server_rw_req
  {
    ssize_t size;
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
    struct stat attr;
  };

  struct st_xpn_server_attr_req
  {
    char status;  
    struct stat attr;
    struct st_xpn_server_status status_req;
  };

  struct st_xpn_server_readdir
  {
    char path[PATH_MAX];
    long telldir;
  };

  struct st_xpn_server_readdir_req
  {
    int end; //If end = 1 exist entry; 0 not exist
    struct dirent ret;
    long telldir;
    struct st_xpn_server_status status;
  };

  struct st_xpn_server_end
  {
    char status;
  };

  struct st_xpn_server_msg
  {
    int type;
    union {
      struct st_xpn_server_path_flags     op_open;
      struct st_xpn_server_path_flags     op_creat;
      struct st_xpn_server_path           op_close;
      struct st_xpn_server_rw             op_read;
      struct st_xpn_server_rw             op_write;
      struct st_xpn_server_path           op_rm;
      struct st_xpn_server_rename         op_rename;
      struct st_xpn_server_path           op_getattr;
      struct st_xpn_server_setattr        op_setattr;

      struct st_xpn_server_path_flags     op_mkdir;
      struct st_xpn_server_path           op_opendir;
      struct st_xpn_server_readdir        op_readdir;
      struct st_xpn_server_path           op_closedir;
      struct st_xpn_server_path           op_rmdir;

      struct st_xpn_server_end            op_end;
    } u_st_xpn_server_msg;
  };

  
  /* ... Functions / Funciones ......................................... */

  char *xpn_server_op2string    ( int op_code );
  int   xpn_server_do_operation ( struct st_th *th, int * the_end );


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
