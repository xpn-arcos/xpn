
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

  #include <libgen.h>
  #include "all_system.h"
  #include "base/filesystem.h"
  #include "base/urlstr.h"
  #include "base/utils.h"
  #include "base/workers.h"
  #include "mpi_server_ops.h"
  #include "mpi_server_comm.h"
  #include "mpi_server_d2xpn.h"
  #include "mpi_server_params.h"


  /* ... Const / Const ................................................. */

  #ifndef MPI_SERVER_ID
    #define MPI_SERVER_ID 32
  #endif

  #define XPN_HEADER_SIZE 8192

  /* Operations */

  // File operations
  #define MPI_SERVER_OPEN_FILE      0
  #define MPI_SERVER_CREAT_FILE     1
  #define MPI_SERVER_READ_FILE      2
  #define MPI_SERVER_WRITE_FILE     3
  #define MPI_SERVER_CLOSE_FILE     4
  #define MPI_SERVER_RM_FILE        5
  #define MPI_SERVER_RM_FILE_ASYNC  6
  #define MPI_SERVER_RENAME_FILE    7
  #define MPI_SERVER_GETATTR_FILE   8
  #define MPI_SERVER_SETATTR_FILE   9

  // Directory operations
  #define MPI_SERVER_MKDIR_DIR        20
  #define MPI_SERVER_RMDIR_DIR        21
  #define MPI_SERVER_RMDIR_DIR_ASYNC  22
  #define MPI_SERVER_OPENDIR_DIR      23
  #define MPI_SERVER_READDIR_DIR      24
  #define MPI_SERVER_CLOSEDIR_DIR     25

  // Import / Export operations
  #define MPI_SERVER_FLUSH_FILE     40
  #define MPI_SERVER_PRELOAD_FILE   41

  // FS Operations
  #define MPI_SERVER_STATFS_DIR     60
  #define MPI_SERVER_GETNODENAME    61
  #define MPI_SERVER_GETID          62

  // Connection operatons
  #define MPI_SERVER_FINALIZE       80
  #define MPI_SERVER_DISCONNECT     81
  #define MPI_SERVER_END            -1


  /* ... Data structures / Estructuras de datos ........................ */

  /* Message struct */

  struct st_mpi_server_path_flags
  {
    char path[PATH_MAX];
    int flags; //TODO: use flags
  };

  struct st_mpi_server_path
  {
    char path[PATH_MAX];
  };

  struct st_mpi_server_open_req
  {
   int fd;
  };

  struct st_mpi_server_rw
  {
    char path[PATH_MAX];
    offset_t offset;
    size_t size;
  };

  struct st_mpi_server_rw_req
  {
    ssize_t size;
    char last;
  };

  struct st_mpi_server_rename
  {
    char old_url[PATH_MAX];
    char new_url[PATH_MAX];
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
      struct st_mpi_server_path_flags     op_open;
      struct st_mpi_server_path_flags     op_creat;
      struct st_mpi_server_path           op_close;
      struct st_mpi_server_rw             op_read;
      struct st_mpi_server_rw             op_write;
      struct st_mpi_server_path           op_rm;
      struct st_mpi_server_rename         op_rename;
      struct st_mpi_server_path           op_getattr;
      struct st_mpi_server_setattr        op_setattr;

      struct st_mpi_server_path           op_mkdir;
      struct st_mpi_server_path           op_opendir;
      struct st_mpi_server_readdir        op_readdir;
      struct st_mpi_server_closedir       op_closedir;
      struct st_mpi_server_path           op_rmdir;

      struct st_mpi_server_flush          op_flush;
      struct st_mpi_server_preload        op_preload;

      struct st_mpi_server_end            op_end;
    } u_st_mpi_server_msg;
  };

  
  /* ... Functions / Funciones ......................................... */

  char *mpi_server_op2string    ( int op_code );
  int   mpi_server_do_operation ( struct st_th *th, int * the_end );


  /* ................................................................... */

#endif
