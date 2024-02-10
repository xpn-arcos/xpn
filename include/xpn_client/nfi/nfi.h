
/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


#ifndef _NFI_H
#define _NFI_H

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "debug_msg.h"
  #include "workers.h"


  /* ... Const / Const ................................................. */

  // PROTOCOLS
  /*
  #define LOCAL    1
  #define NFS    2
  #define NFS3     3
  #define TCP_SERVER   9
  #define MPI_SERVER  10
  */

  // TYPE FILES 
  #define NFIFILE    0
  #define NFIDIR     1
  #define NFINULL   -1


  /* ... Data structures / Estructuras de datos ........................ */

  // info of the servers
  struct nfi_ops;
  struct nfi_worker;

  struct nfi_server 
  {
    int id;                 // id of the server 
  //int  protocol;          // protocol          // It is never used
    char *server;           // server address   
    char *url;              // URL of this server -> protocol   
                            // + server         
                            // + path + more info (port, ...) 
    int    block_size;
    void  *private_info;    // info private       
    struct nfi_ops    *ops; // operations       
    struct nfi_worker *wrk; // this struct has the thread   

    // Execution configuration
    int xpn_thread;
  };

  struct nfi_attr_server
  {
    int type;
    int size;
    void *private_info;
    struct nfi_attr_server *next;
  };

  struct nfi_info
  {
    u_long at_size;
    u_long at_bsize;
    u_long at_blocks;
    u_long at_bfree;
    u_long at_bavail;
  };

  struct nfi_attr
  {
    dev_t   st_dev;     // ID of device containing file 
    ino_t   st_ino;     // inode number 

    int     at_type;    // FILE or DIR        
    mode_t  at_mode;    // protection     
    nlink_t at_nlink;   // number of hard links   
    uid_t   at_uid;     // user ID of owner   
    gid_t   at_gid;     // group ID of owner    
    off_t   at_size;    // total size, in bytes   
    u_long  at_blksize; // blocksize for filesystem I/O 
    u_long  at_blocks;  // number of blocks allocated 
    time_t  at_atime;   // time of last access    
    time_t  at_mtime;   // time of last modification  
    time_t  at_ctime;   // time of last status change 
    void    *private_info;  
  };

  struct nfi_fhandle 
  {
    int type;                   // file or directory    
    char *url;                  // url of DIR or FILE     
    struct nfi_server *server;  // server       
    void *priv_fh;              // pointer to private filehandle
  };

  struct nfi_ops 
  {
    int     (*nfi_reconnect) (struct nfi_server *serv);
    int     (*nfi_disconnect)(struct nfi_server *serv);
    //int   (*nfi_destroy)(struct nfi_server *serv);
    int     (*nfi_getattr)  (struct nfi_server *serv, struct nfi_fhandle *fh, struct nfi_attr *attr);
    int     (*nfi_setattr)  (struct nfi_server *serv, struct nfi_fhandle *fh, struct nfi_attr *attr);
    int     (*nfi_open)     (struct nfi_server *serv, char *url, struct nfi_fhandle *fho); 
    int     (*nfi_create)   (struct nfi_server *serv, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);
    int     (*nfi_close)    (struct nfi_server *serv, struct nfi_fhandle *fh);
    int     (*nfi_remove)   (struct nfi_server *serv, char *url);
    int     (*nfi_rename)   (struct nfi_server *serv, char *old_url, char *new_url);
    ssize_t (*nfi_read)     (struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);
    ssize_t (*nfi_write)    (struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);
    int     (*nfi_mkdir)    (struct nfi_server *serv, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);
    int     (*nfi_rmdir)    (struct nfi_server *serv, char *url);
    int     (*nfi_opendir)  (struct nfi_server *serv, char *url, struct nfi_fhandle *fho);
    int     (*nfi_readdir)  (struct nfi_server *serv, struct nfi_fhandle *fhd, struct dirent *entry);
    int     (*nfi_closedir) (struct nfi_server *serv, struct nfi_fhandle *fh);
    int     (*nfi_statfs)   (struct nfi_server *serv, struct nfi_info *inf);

    int     (*nfi_preload)  (struct nfi_server *serv, char *url, char *virtual_path, char* storage_path, int opt); 
    int     (*nfi_flush)    (struct nfi_server *serv, char *url, char *virtual_path, char* storage_path, int opt); 
  };


  /* ... Functions / Funciones ......................................... */


  /* ................................................................... */

 #ifdef  __cplusplus
    }
 #endif

#endif
