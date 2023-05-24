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
 * @file nfi_local.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _NFI_LOCAL_H
#define _NFI_LOCAL_H

#ifdef __cplusplus
extern "C"
{
#endif

  /************************************************
   *  ... Includes
   ***********************************************/
#define ASYNC_CLOSE 1
#define FILESYSTEM_DLSYM 1
#include "all_system.h"
#include "base/filesystem.h"
#include "base/path_misc.h"
#include "base/urlstr.h"
#include "nfi/nfi.h"
#include "nfi/nfi_local/nfi_local_err.h"
#include "nfi_worker.h"

  /************************************************
   *  ... Data structures
   ***********************************************/

  /** @struct nfi_local_server
   *  This is a struct
   *
   *  @var nfi_local_server::path
   *    A 'TODO'.
   */
  struct nfi_local_server
  {
    char path[PATH_MAX];
  };

  /** @struct nfi_local_fhandle
   *  This is a struct
   *
   *  @var nfi_local_fhandle::path
   *    A 'TODO'.
   *  @var nfi_local_fhandle::fd
   *    A 'TODO'.
   *  @var nfi_local_fhandle::dir
   *    A 'TODO'.
   */
  struct nfi_local_fhandle
  {
    char path[PATH_MAX];
    int fd;
    DIR *dir;
  };

  /************************************************
   *  ... Functions
   ***********************************************/

  /**
   * @brief Init Local.
   *
   * 'TODO'
   *
   * @param url 'TODO'.
   * @param serv 'TODO'.
   * @param attr 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);

  /**
   * @brief Connect to the server.
   *
   * 'TODO'.
   *
   * @param serv 'TODO'.
   * @param url 'TODO'.
   * @param prt 'TODO'.
   * @param server 'TODO'.
   * @param dir 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_connect(struct nfi_server *serv, char *url, char *prt, char *server, char *dir);

  /**
   * @brief Reconnect to the LOCAL server.
   *
   * 'TODO'
   *
   * @param server 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_reconnect(struct nfi_server *server);

  /**
   * @brief Disconnect to the server.
   *
   * 'TODO'
   *
   * @param server 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_disconnect(struct nfi_server *server);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param url 'TODO'.
   * @param attr 'TODO'.
   * @param fh 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_create(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

  /************************************************************
   * Generic Functions                         *
   ************************************************************/

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param url 'TODO'.
   * @param fho 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param fh 'TODO'.
   * @param buffer 'TODO'.
   * @param offset 'TODO'.
   * @param size 'TODO'.
   * @return 'TODO'.
   */
  ssize_t nfi_local_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param fh 'TODO'.
   * @param buffer 'TODO'.
   * @param offset 'TODO'.
   * @param size 'TODO'.
   * @return 'TODO'.
   */
  ssize_t nfi_local_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param fh 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_close(struct nfi_server *server, struct nfi_fhandle *fh);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param url 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_remove(struct nfi_server *server, char *url);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param old_url 'TODO'.
   * @param new_url 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_rename(struct nfi_server *server, char *old_url, char *new_url);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param fh 'TODO'.
   * @param attr 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param fh 'TODO'.
   * @param attr 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param url 'TODO'.
   * @param attr 'TODO'.
   * @param fh 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param url 'TODO'.
   * @param fho 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param fhd 'TODO'.
   * @param entry 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, struct dirent *entry);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param fh 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_closedir(struct nfi_server *server, struct nfi_fhandle *fh);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param url 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_rmdir(struct nfi_server *server, char *url);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param inf 'TODO'.
   * @return 'TODO'.
   */
  int nfi_local_statfs(struct nfi_server *server, struct nfi_info *inf);
  // int   nfi_local_preload    ( struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt ) ;
  // int   nfi_local_flush      ( struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt ) ;

  /* ................................................................... */

#ifdef __cplusplus
}
#endif

#endif
