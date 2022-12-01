#ifndef _MPISERVER_OPS_H_
#define _MPISERVER_OPS_H_

  #include "all_system.h"
  #include "mpiServer_params.h"
  #include "base/filesystem.h"
  #include "base/utils.h"


  /*
   *  Constants
   */

#ifndef MPISERVER_MAXPATHLEN
  #define MPISERVER_MAXPATHLEN 128
#endif
#ifndef MPISERVER_ID
  #define MPISERVER_ID 32
#endif

  /* Operations */

  // File operations
  #define MPISERVER_OPEN_FILE_WS   0
  #define MPISERVER_CREAT_FILE_WS  1
  #define MPISERVER_READ_FILE_WS   2
  #define MPISERVER_WRITE_FILE_WS  3
  #define MPISERVER_CLOSE_FILE_WS  4
  #define MPISERVER_RM_FILE        5
  #define MPISERVER_RENAME_FILE    6
  #define MPISERVER_GETATTR_FILE   7
  #define MPISERVER_SETATTR_FILE   8

  // File operations without session
  #define MPISERVER_OPEN_FILE_WOS      100
  #define MPISERVER_CREAT_FILE_WOS     101
  #define MPISERVER_READ_FILE_WOS      102
  #define MPISERVER_WRITE_FILE_WOS     103

  // Directory operations
  #define MPISERVER_MKDIR_DIR      20
  #define MPISERVER_RMDIR_DIR      21
  #define MPISERVER_OPENDIR_DIR    22
  #define MPISERVER_READDIR_DIR    23
  #define MPISERVER_CLOSEDIR_DIR   24

  // Import / Export operations
  #define MPISERVER_FLUSH_FILE     40
  #define MPISERVER_PRELOAD_FILE   41

  // FS Operations
  #define MPISERVER_STATFS_DIR     60
  #define MPISERVER_GETNODENAME    61
  #define MPISERVER_GETID          62

  // Connection operatons
  #define MPISERVER_FINALIZE       80
  #define MPISERVER_DISCONNECT     81
  #define MPISERVER_END            -1


  /*
   *  Message struct
   */

  struct st_mpiServer_open{
    char path[MPISERVER_MAXPATHLEN];
  };

  struct st_mpiServer_open_req{
   int fd;
  };

  struct st_mpiServer_creat{
    char path[MPISERVER_MAXPATHLEN];
  };

  struct st_mpiServer_read{
    int fd;
    char path[MPISERVER_MAXPATHLEN];
    offset_t offset;
    size_t size;
  };

  struct st_mpiServer_read_req{
    ssize_t size;
    char last;
  };

  struct st_mpiServer_write{
    int fd;
    char path[MPISERVER_MAXPATHLEN];
    offset_t offset;
    size_t size;
  };

  struct st_mpiServer_write_req{
    ssize_t size;
  };

  struct st_mpiServer_close{
    int fd;
  };

  struct st_mpiServer_rm{
    char path[MPISERVER_MAXPATHLEN];
  };

  struct st_mpiServer_rename{
    char old_url[MPISERVER_MAXPATHLEN];
    char new_url[MPISERVER_MAXPATHLEN];
  };

  struct st_mpiServer_getattr{ 
    char path[MPISERVER_MAXPATHLEN];
  };

  struct st_mpiServer_setattr{ 
    char path[MPISERVER_MAXPATHLEN];
    struct stat attr;
  };

  struct st_mpiServer_attr_req{
    char status;  
    struct stat attr;
  };
  

  
  struct st_mpiServer_mkdir{  
    char path[MPISERVER_MAXPATHLEN];
  };

  struct st_mpiServer_opendir{  //NEW
    char path[MPISERVER_MAXPATHLEN];
  };

  struct st_mpiServer_readdir{  //NEW
    DIR * dir;
  };

  struct st_mpiServer_direntry{  //NEW
    int end; //If end = 1 exist entry; 0 not exist
    struct dirent ret;
  };

  struct st_mpiServer_closedir{  //NEW
    DIR * dir;
  };

  struct st_mpiServer_rmdir{
    char path[MPISERVER_MAXPATHLEN];
  };

  //TODO: define MPISERVER_OPENDIR_DIR, MPISERVER_READDIR_DIR, MPISERVER_CLOSEDIR_DIR

  struct st_mpiServer_flush{
    char storage_path[MPISERVER_MAXPATHLEN];
    char virtual_path[MPISERVER_MAXPATHLEN];
    int  block_size;
    char opt;
  };

  struct st_mpiServer_preload{
    char storage_path[MPISERVER_MAXPATHLEN];
    char virtual_path[MPISERVER_MAXPATHLEN];
    int  block_size;
    char opt;
  };



  struct st_mpiServer_end{
    char status;
  };
  


  struct st_mpiServer_msg
  {
    int type ;
    char id[MPISERVER_ID] ;
    union {
      struct st_mpiServer_open     op_open;
      struct st_mpiServer_creat    op_creat;
      struct st_mpiServer_close    op_close;
      struct st_mpiServer_read     op_read;
      struct st_mpiServer_write    op_write;
      struct st_mpiServer_rm       op_rm;    
      struct st_mpiServer_mkdir    op_mkdir;
      struct st_mpiServer_opendir  op_opendir;
      struct st_mpiServer_readdir  op_readdir;
      struct st_mpiServer_closedir op_closedir;
      struct st_mpiServer_rename   op_rename;
      struct st_mpiServer_rmdir    op_rmdir;
      struct st_mpiServer_getattr  op_getattr;
      struct st_mpiServer_setattr  op_setattr;

      struct st_mpiServer_flush    op_flush;
      struct st_mpiServer_preload  op_preload;
      struct st_mpiServer_end      op_end;
    } u_st_mpiServer_msg ;
  };


#endif

