
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


#ifndef _MPI_SERVER_OPS_H_
#define _MPI_SERVER_OPS_H_

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "mpi_server_params.h"
  #include "base/utils.h"
  #include "base/filesystem.h"


  /* ... Const / Const ................................................. */

  #define ASYNC_CLOSE 1
  #define FILESYSTEM_DLSYM 1

  #ifndef MPI_SERVER_ID
    #define MPI_SERVER_ID 32
  #endif

  /* Operations */

  // File operations
  #define MPI_SERVER_OPEN_FILE_WS   0
  #define MPI_SERVER_CREAT_FILE_WS  1
  #define MPI_SERVER_READ_FILE_WS   2
  #define MPI_SERVER_WRITE_FILE_WS  3
  #define MPI_SERVER_CLOSE_FILE_WS  4
  #define MPI_SERVER_RM_FILE        5
  #define MPI_SERVER_RM_FILE_ASYNC  6
  #define MPI_SERVER_RENAME_FILE    7
  #define MPI_SERVER_GETATTR_FILE   8
  #define MPI_SERVER_SETATTR_FILE   9

  // File operations without session
  #define MPI_SERVER_OPEN_FILE_WOS   100
  #define MPI_SERVER_CREAT_FILE_WOS  101
  #define MPI_SERVER_READ_FILE_WOS   102
  #define MPI_SERVER_WRITE_FILE_WOS  103

  // Directory operations
  #define MPI_SERVER_MKDIR_DIR        20
  #define MPI_SERVER_RMDIR_DIR        21
  #define MPI_SERVER_RMDIR_DIR_ASYNC  22
  #define MPI_SERVER_OPENDIR_DIR      23
  #define MPI_SERVER_READDIR_DIR      24
  #define MPI_SERVER_CLOSEDIR_DIR     25

  // Import / Export operations
  #define MPI_SERVER_FLUSH_FILE    40
  #define MPI_SERVER_PRELOAD_FILE  41

  // FS Operations
  #define MPI_SERVER_STATFS_DIR   60
  #define MPI_SERVER_GETNODENAME  61
  #define MPI_SERVER_GETID        62

  // Connection operatons
  #define MPI_SERVER_FINALIZE    80
  #define MPI_SERVER_DISCONNECT  81
  #define MPI_SERVER_END         -1


  /* ... Data structures / Estructuras de datos ........................ */

  /*
   *  Message struct
   */

  struct st_mpi_server_open
  {
    char path[PATH_MAX]; //TO-DO: Insert FLAGS - O_RDWR etc
  };

  struct st_mpi_server_open_req
  {
   int fd;
  };

  struct st_mpi_server_creat
  {
    char path[PATH_MAX];
  };

  struct st_mpi_server_read
  {
    int fd;
    char path[PATH_MAX];
    offset_t offset;
    size_t size;
  };

  struct st_mpi_server_read_req
  {
    ssize_t size;
    char last;
  };

  struct st_mpi_server_write
  {
    int fd;
    char path[PATH_MAX];
    offset_t offset;
    size_t size;
  };

  struct st_mpi_server_write_req
  {
    ssize_t size;
  };

  struct st_mpi_server_close
  {
    int fd;
  };

  struct st_mpi_server_rename
  {
    char old_url[PATH_MAX];
    char new_url[PATH_MAX];
  };

  struct st_mpi_server_rm
  {
    char path[PATH_MAX];
  };

  struct st_mpi_server_getattr
  { 
    char path[PATH_MAX];
  };

  struct st_mpi_server_setattr
  { 
    char path[PATH_MAX];
    struct stat attr;
  };

  struct st_mpi_server_attr_req
  {
    char status;  
    struct stat attr;
  };
  

  
  struct st_mpi_server_mkdir
  {  
    char path[PATH_MAX];
  };

  struct st_mpi_server_opendir
  {
    char path[PATH_MAX];
  };

  struct st_mpi_server_readdir
  {
    DIR * dir;
  };

  struct st_mpi_server_direntry
  {
    int end; //If end = 1 exist entry; 0 not exist
    struct dirent ret;
  };

  struct st_mpi_server_closedir
  {
    DIR * dir;
  };

  struct st_mpi_server_rmdir
  {
    char path[PATH_MAX];
  };

  //TODO: define MPI_SERVER_OPENDIR_DIR, MPI_SERVER_READDIR_DIR, MPI_SERVER_CLOSEDIR_DIR


  struct st_mpi_server_flush
  {
    char storage_path[PATH_MAX];
    char virtual_path[PATH_MAX];
    int block_size;
    char opt;
  };

  struct st_mpi_server_preload
  {
    char storage_path[PATH_MAX];
    char virtual_path[PATH_MAX];
    int block_size;
    char opt;
  };


  struct st_mpi_server_end
  {
    char status;
  };


  struct st_mpi_server_msg
  {
    int type;
    char id[MPI_SERVER_ID];
    union {
      struct st_mpi_server_open     op_open;
      struct st_mpi_server_creat    op_creat;
      struct st_mpi_server_close    op_close;
      struct st_mpi_server_read     op_read;
      struct st_mpi_server_write    op_write;
      struct st_mpi_server_rm       op_rm;
      struct st_mpi_server_rename   op_rename;
      struct st_mpi_server_getattr  op_getattr;
      struct st_mpi_server_setattr  op_setattr;

      struct st_mpi_server_mkdir    op_mkdir;
      struct st_mpi_server_opendir  op_opendir;
      struct st_mpi_server_readdir  op_readdir;
      struct st_mpi_server_closedir op_closedir;
      struct st_mpi_server_rmdir    op_rmdir;

      struct st_mpi_server_flush    op_flush;
      struct st_mpi_server_preload  op_preload;

      struct st_mpi_server_end      op_end;
    } u_st_mpi_server_msg;
  };


  /* ... Functions / Funciones ......................................... */


  /* ................................................................... */

#endif
