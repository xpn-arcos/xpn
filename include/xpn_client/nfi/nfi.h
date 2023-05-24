/*
 *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

/**
 * @file nfi.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _NFI_H
#define _NFI_H

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************
 *  ... Includes
 ***********************************************/
#include "all_system.h"
#include "debug_msg.h"
#include "workers.h"

/* PROTOCOLS */
/*
#define LOCAL    1
#define NFS    2
#define NFS3     3
#define TCP_SERVER   9
#define MPI_SERVER  10
*/

/* TYPE FILES */
#define NFIFILE 0
#define NFIDIR 1
#define NFINULL -1

  /* info of the servers */
  struct nfi_ops;
  struct nfi_worker;

  /** @struct nfi_server
   *  This is a struct
   *
   *  @var nfi_server::id
   *    Id of the server.
   *  @var nfi_server::server
   *    Server address.
   *  @var nfi_server::url
   *    A 'TODO'.
   *  @var nfi_server::block_size
   *    A 'TODO'.
   *  @var nfi_server::private_info
   *    Info private.
   *  @var nfi_server::ops
   *    Operations.
   *  @var nfi_server::wrk
   *    This struct has the thread.
   *  @var nfi_server::xpn_thread
   *    A 'TODO'.
   */
  struct nfi_server
  {
    int id;
    // int  protocol;          /* protocol         */ // It is never used
    char *server;
    char *url; /* URL of this server -> protocol   */
               /* + server         */
               /* + path + more info (port, ...) */
    int block_size;
    void *private_info;
    struct nfi_ops *ops;
    struct nfi_worker *wrk;

    // Execution configuration
    int xpn_thread;
  };

  /** @struct nfi_attr_server
   *  This is a struct
   *
   *  @var nfi_attr_server::type
   *    A 'TODO'.
   *  @var nfi_attr_server::size
   *    A 'TODO'.
   *  @var nfi_attr_server::private_info
   *    A 'TODO'.
   *  @var nfi_attr_server::next
   *    A 'TODO'.
   */
  struct nfi_attr_server
  {
    int type;
    int size;
    void *private_info;
    struct nfi_attr_server *next;
  };

  /** @struct nfi_info
   *  This is a struct
   *
   *  @var nfi_info::at_size
   *    A 'TODO'.
   *  @var nfi_info::at_bsize
   *    A 'TODO'.
   *  @var nfi_info::at_blocks
   *    A 'TODO'.
   *  @var nfi_info::at_bfree
   *    A 'TODO'.
   *  @var nfi_info::at_bavail
   *    A 'TODO'.
   */
  struct nfi_info
  {
    u_long at_size;
    u_long at_bsize;
    u_long at_blocks;
    u_long at_bfree;
    u_long at_bavail;
  };

  /** @struct nfi_attr
   *  This is a struct
   *
   *  @var nfi_attr::st_dev
   *    ID of device containing file.
   *  @var nfi_attr::st_ino
   *    Inode number.
   *  @var nfi_attr::at_type
   *    FILE or DIR.
   *  @var nfi_attr::at_mode
   *    Protection.
   *  @var nfi_attr::at_nlink
   *    Number of hard links.
   *  @var nfi_attr::at_uid
   *    User ID of owner.
   *  @var nfi_attr::at_gid
   *    Group ID of owner.
   *  @var nfi_attr::at_size
   *    Total size, in bytes.
   *  @var nfi_attr::at_blksize
   *    Blocksize for filesystem I/O.
   *  @var nfi_attr::at_blocks
   *    Number of blocks allocated.
   *  @var nfi_attr::at_atime
   *    Time of last access.
   *  @var nfi_attr::at_mtime
   *    Time of last modification.
   *  @var nfi_attr::at_ctime
   *    Time of last status change.
   *  @var nfi_attr::private_info
   *    A 'TODO'.
   */
  struct nfi_attr
  {
    dev_t st_dev;
    ino_t st_ino;

    int at_type;
    mode_t at_mode;
    nlink_t at_nlink;
    uid_t at_uid;
    gid_t at_gid;
    off_t at_size;
    u_long at_blksize;
    u_long at_blocks;
    time_t at_atime;
    time_t at_mtime;
    time_t at_ctime;
    void *private_info;
  };

  /** @struct nfi_fhandle
   *  This is a struct
   *
   *  @var nfi_fhandle::type
   *    File or directory.
   *  @var nfi_fhandle::url
   *    Url of DIR or FILE.
   *  @var nfi_fhandle::server
   *    Server.
   *  @var nfi_fhandle::priv_fh
   *    Pointer to private filehandle.
   */
  struct nfi_fhandle
  {
    int type;
    char *url;
    struct nfi_server *server;
    void *priv_fh;
  };

  struct nfi_ops
  {
    int (*nfi_reconnect)(struct nfi_server *serv);
    int (*nfi_disconnect)(struct nfi_server *serv);
    // int   (*nfi_destroy)(struct nfi_server *serv);
    int (*nfi_getattr)(struct nfi_server *serv, struct nfi_fhandle *fh, struct nfi_attr *attr);
    int (*nfi_setattr)(struct nfi_server *serv, struct nfi_fhandle *fh, struct nfi_attr *attr);
    int (*nfi_open)(struct nfi_server *serv, char *url, struct nfi_fhandle *fho);
    int (*nfi_create)(struct nfi_server *serv, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);
    int (*nfi_close)(struct nfi_server *serv, struct nfi_fhandle *fh);
    int (*nfi_remove)(struct nfi_server *serv, char *url);
    int (*nfi_rename)(struct nfi_server *serv, char *old_url, char *new_url);
    ssize_t (*nfi_read)(struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);
    ssize_t (*nfi_write)(struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);
    int (*nfi_mkdir)(struct nfi_server *serv, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);
    int (*nfi_rmdir)(struct nfi_server *serv, char *url);
    int (*nfi_opendir)(struct nfi_server *serv, char *url, struct nfi_fhandle *fho);
    int (*nfi_readdir)(struct nfi_server *serv, struct nfi_fhandle *fhd, struct dirent *entry);
    int (*nfi_closedir)(struct nfi_server *serv, struct nfi_fhandle *fh);
    int (*nfi_statfs)(struct nfi_server *serv, struct nfi_info *inf);

    int (*nfi_preload)(struct nfi_server *serv, char *url, char *virtual_path, char *storage_path, int opt);
    int (*nfi_flush)(struct nfi_server *serv, char *url, char *virtual_path, char *storage_path, int opt);
  };

#ifdef __cplusplus
}
#endif

#endif
