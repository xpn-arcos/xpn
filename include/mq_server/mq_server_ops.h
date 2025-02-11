
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


#ifndef _MQ_SERVER_OPS_H_
#define _MQ_SERVER_OPS_H_

    #include <libgen.h>
    #include "all_system.h"
    #include "base/filesystem.h"
    #include "base/urlstr.h"
    #include "base/utils.h"
    #include "base/workers.h"
    #include "mq_server_ops.h"
    #include "mq_server_comm.h"
    #include "mq_server_d2xpn.h"
    #include "mq_server_params.h"


  /*
   *  Constants
   */

   #ifndef MQ_SERVER_ID
   #define MQ_SERVER_ID 32
   #endif

  /* Operations */

  // File operations
  #define MQ_SERVER_OPEN_FILE_WS   0
  #define MQ_SERVER_CREAT_FILE_WS  1
  #define MQ_SERVER_READ_FILE_WS   2
  #define MQ_SERVER_WRITE_FILE_WS  3
  #define MQ_SERVER_CLOSE_FILE_WS  4
  #define MQ_SERVER_RM_FILE        5
  #define MQ_SERVER_RM_FILE_ASYNC  6
  #define MQ_SERVER_RENAME_FILE    7
  #define MQ_SERVER_GETATTR_FILE   8
  #define MQ_SERVER_SETATTR_FILE   9

  // File operations without session
  #define MQ_SERVER_OPEN_FILE_WOS      100
  #define MQ_SERVER_CREAT_FILE_WOS     101
  #define MQ_SERVER_READ_FILE_WOS      102
  #define MQ_SERVER_WRITE_FILE_WOS     103

  // Directory operations
  #define MQ_SERVER_MKDIR_DIR        20
  #define MQ_SERVER_RMDIR_DIR        21
  #define MQ_SERVER_RMDIR_DIR_ASYNC  22
  #define MQ_SERVER_OPENDIR_DIR      23
  #define MQ_SERVER_READDIR_DIR      24
  #define MQ_SERVER_CLOSEDIR_DIR     25

  // Import / Export operations
  #define MQ_SERVER_FLUSH_FILE     40
  #define MQ_SERVER_PRELOAD_FILE   41

  // FS Operations
  #define MQ_SERVER_STATFS_DIR     60
  #define MQ_SERVER_GETNODENAME    61
  #define MQ_SERVER_GETID          62

  // Metadata
  #define MQ_SERVER_READ_MDATA      70
  #define MQ_SERVER_WRITE_MDATA     71
  #define MQ_SERVER_WRITE_MDATA_FILE_SIZE     72

  // Connection operatons
  #define MQ_SERVER_FINALIZE       80
  #define MQ_SERVER_DISCONNECT     81
  #define MQ_SERVER_END            -1


  /*
   *  Message struct
   */

  struct st_mq_server_open
  {
    char path[PATH_MAX];            //TO-DO: Insert FLAGS - O_RDWR etc
  };

  struct st_mq_server_open_req
  {
   int fd;
  };

  struct st_mq_server_creat
  {
    char path[PATH_MAX];
  };

  struct st_mq_server_read
  {
    int fd;
    char path[PATH_MAX];
    offset_t offset;
    size_t size;
  };

  struct st_mq_server_read_req
  {
    ssize_t size;
    char last;
  };

  struct st_mq_server_write
  {
    int fd;
    char path[PATH_MAX];
    offset_t offset;
    size_t size;
  };

  struct st_mq_server_write_req
  {
    ssize_t size;
  };

  struct st_mq_server_close
  {
    int fd;
    char path[PATH_MAX];
  };

  struct st_mq_server_rename
  {
    char old_url[PATH_MAX];
    char new_url[PATH_MAX];
  };

  struct st_mq_server_rm
  {
    char path[PATH_MAX];
  };

  struct st_mq_server_getattr
  { 
    char path[PATH_MAX];
  };

  struct st_mq_server_setattr
  { 
    char path[PATH_MAX];
    struct stat attr;
  };

  struct st_mq_server_attr_req
  {
    char status;  
    struct stat attr;
  };
  

  
  struct st_mq_server_mkdir
  {  
    char path[PATH_MAX];
  };

  struct st_mq_server_opendir
  {  //NEW
    char path[PATH_MAX];
  };

  struct st_mq_server_readdir
  {  //NEW
    DIR * dir;
  };

  struct st_mq_server_direntry
  {  //NEW
    int end; //If end = 1 exist entry; 0 not exist
    struct dirent ret;
  };

  struct st_mq_server_closedir
  {  //NEW
    DIR * dir;
  };

  struct st_mq_server_rmdir
  {
    char path[PATH_MAX];
  };

  //TODO: define MQ_SERVER_OPENDIR_DIR, MQ_SERVER_READDIR_DIR, MQ_SERVER_CLOSEDIR_DIR

  struct st_mq_server_flush
  {
    char storage_path[PATH_MAX];
    char virtual_path[PATH_MAX];
    int block_size;
    char opt;
  };

  struct st_mq_server_preload
  {
    char storage_path[PATH_MAX];
    char virtual_path[PATH_MAX];
    int block_size;
    char opt;
  };



  struct st_mq_server_end
  {
    char status;
  };


  struct st_mq_server_read_mdata_req
  { 
    struct xpn_metadata mdata;
    struct st_mq_server_status status;
  };

  struct st_mq_server_write_mdata
  { 
    char path[PATH_MAX];
    struct xpn_metadata mdata;
  };

  struct st_mq_server_write_mdata_file_size
  { 
    char path[PATH_MAX];
    ssize_t size;
  };



  struct st_mq_server_msg
  {
    int type ;
    char id[MQ_SERVER_ID] ;
    union {
      struct st_mq_server_open     op_open;
      struct st_mq_server_creat    op_creat;
      struct st_mq_server_close    op_close;
      struct st_mq_server_read     op_read;
      struct st_mq_server_write    op_write;
      struct st_mq_server_rm       op_rm;
      struct st_mq_server_rename   op_rename;
      struct st_mq_server_mkdir    op_mkdir;
      struct st_mq_server_opendir  op_opendir;
      struct st_mq_server_readdir  op_readdir;
      struct st_mq_server_closedir op_closedir;
      struct st_mq_server_rmdir    op_rmdir;
      struct st_mq_server_getattr  op_getattr;
      struct st_mq_server_setattr  op_setattr;

  //    struct st_mq_server_flush    op_flush;
  //    struct st_mq_server_preload  op_preload;
      struct st_mq_server_end      op_end;

      struct st_mq_server_path           op_read_mdata;
      struct st_mq_server_write_mdata    op_write_mdata;
      struct st_mq_server_write_mdata_file_size             op_write_mdata_file_size;

    } u_st_mq_server_msg ;
  };

  


  /*
   *  API
   */

  char *mq_server_op2string    ( int op_code ) ;
  int   mq_server_do_operation ( struct st_th *th, int * the_end );

#endif

