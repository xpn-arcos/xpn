/*
 *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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
 * @file nfi_tcp_server.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _NFI_TCP_SERVER_H
#define _NFI_TCP_SERVER_H

#ifdef __cplusplus
extern "C"
{
#endif

  /************************************************
   *  ... Includes
   ***********************************************/
#include "all_system.h"
#include "base/utils.h"
#include "base/path_misc.h"
#include "base/urlstr.h"
#include "base/workers.h"
#include "tcp_server_ops.h"
#include "tcp_server_comm.h"
#include "tcp_server_params.h"
#include "nfi_tcp_server_err.h"
#include "nfi.h"
#include "nfi_worker.h"

#ifdef HAVE_MOSQUITTO_H
#include <mosquitto.h>
#endif

  /************************************************
   *  ... Constants
   ***********************************************/

#ifndef MOSQ_OPT_TCP_NODELAY
#define MOSQ_OPT_TCP_NODELAY 0
#endif

  /************************************************
   *  ... Data structures
   ***********************************************/

  /** @struct nfi_tcp_server_server
   *  This is a struct
   *
   *  @var nfi_tcp_server_server::id
   *    A 'TODO'.
   *  @var nfi_tcp_server_server::path
   *    A 'TODO'.
   *  @var nfi_tcp_server_server::mqtt
   *    A 'TODO'.
   *  @var nfi_tcp_server_server::params
   *    A 'TODO'.
   */
  struct nfi_tcp_server_server
  {
    // old stuff of XPN
    char id[PATH_MAX];
    char path[PATH_MAX];
    // struct nfi_tcp_server_connector sd;
    struct mosquitto *mqtt;

    // client stub
    tcpClient_param_st params;
  };

  /** @struct nfi_tcp_server_fhandle
   *  This is a struct
   *
   *  @var nfi_tcp_server_fhandle::path
   *    A 'TODO'.
   *  @var nfi_tcp_server_fhandle::fd
   *    A 'TODO'.
   *  @var nfi_tcp_server_fhandle::dir
   *    A 'TODO'.
   */
  struct nfi_tcp_server_fhandle
  {
    char path[PATH_MAX];
    int fd;
    DIR *dir;
  };

  /************************************************
   *  ... Functions
   ***********************************************/

  /**
   * @brief Init tcp_server.
   *
   * 'TODO'.
   *
   * @param url 'TODO'.
   * @param serv 'TODO'.
   * @param attr 'TODO'.
   * @return 'TODO'.
   */
  int nfi_tcp_server_init(char *url, struct nfi_server *serv, struct nfi_attr_server *attr);

  /**
   * @brief Connect to the server.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param url 'TODO'.
   * @param prt 'TODO'.
   * @param serv 'TODO'.
   * @param dir 'TODO'.
   * @return 'TODO'.
   */
  int nfi_tcp_server_connect(struct nfi_server *server, char *url, char *prt, char *serv, char *dir);

  /**
   * @brief Reconnect to the TCP server.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @return 'TODO'.
   */
  int nfi_tcp_server_reconnect(struct nfi_server *server);

  /**
   * @brief Disconnect to the server.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @return 'TODO'.
   */
  int nfi_tcp_server_disconnect(struct nfi_server *server);

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
  int nfi_tcp_server_create(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

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
  int nfi_tcp_server_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

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
  ssize_t nfi_tcp_server_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

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
  ssize_t nfi_tcp_server_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param fh 'TODO'.
   * @return 'TODO'.
   */
  int nfi_tcp_server_close(struct nfi_server *server, struct nfi_fhandle *fh);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param url 'TODO'.
   * @return 'TODO'.
   */
  int nfi_tcp_server_remove(struct nfi_server *server, char *url);

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
  int nfi_tcp_server_rename(struct nfi_server *server, char *old_url, char *new_url);

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
  int nfi_tcp_server_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

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
  int nfi_tcp_server_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

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
  int nfi_tcp_server_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

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
  int nfi_tcp_server_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

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
  int nfi_tcp_server_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, struct dirent *entry);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param fhd 'TODO'.
   * @return 'TODO'.
   */
  int nfi_tcp_server_closedir(struct nfi_server *server, struct nfi_fhandle *fhd);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param url 'TODO'.
   * @return 'TODO'.
   */
  int nfi_tcp_server_rmdir(struct nfi_server *server, char *url);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param inf 'TODO'.
   * @return 'TODO'.
   */
  int nfi_tcp_server_statfs(struct nfi_server *server, struct nfi_info *inf);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param url 'TODO'.
   * @param virtual_path 'TODO'.
   * @param storage_path 'TODO'.
   * @param opt 'TODO'.
   * @return 'TODO'.
   */
  int nfi_tcp_server_preload(struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param server 'TODO'.
   * @param url 'TODO'.
   * @param virtual_path 'TODO'.
   * @param storage_path 'TODO'.
   * @param opt 'TODO'.
   * @return 'TODO'.
   */
  int nfi_tcp_server_flush(struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt);

  /* ................................................................... */

#ifdef __cplusplus
}
#endif

#endif
