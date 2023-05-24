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
 * @file nfi_ops.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _NFI_OPS_H_
#define _NFI_OPS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef XPN_HEADER_SIZE
#define XPN_HEADER_SIZE 8192
#endif

/************************************************
 *  ... Includes
 ***********************************************/
#include "nfi_worker_task.h"
#include "nfi_worker.h"

  enum nfi_work
  {
    noop = -1,
    op_open = 0,
    op_create = 1,
    op_read = 2,
    op_write = 3,
    op_close = 4,
    op_remove = 5,
    op_rename = 6,
    op_getattr = 7,
    op_setattr = 8,

    op_mkdir = 20,
    op_rmdir = 21,
    op_opendir = 22,
    op_readdir = 23,
    op_closedir = 24,

    op_statfs = 60,
    op_preload = 61,
    op_flush = 62,
  };

  /************************************************
   *  ... Functions
   ***********************************************/

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param th_arg 'TODO'.
   * @par Returns
   *    Nothing.
   */
  void nfi_do_operation(struct st_th th_arg);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param url 'TODO'.
   * @param fho 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_open(struct nfi_worker *wrk, char *url, struct nfi_fhandle *fho);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param url 'TODO'.
   * @param attr 'TODO'.
   * @param fh 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_create(struct nfi_worker *wrk, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param fh 'TODO'.
   * @param io 'TODO'.
   * @param n 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_read(struct nfi_worker *wrk, struct nfi_fhandle *fh, struct nfi_worker_io *io, int n);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param fh 'TODO'.
   * @param io 'TODO'.
   * @param n 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_write(struct nfi_worker *wrk, struct nfi_fhandle *fh, struct nfi_worker_io *io, int n);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param fh 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_close(struct nfi_worker *wrk, struct nfi_fhandle *fh);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param url 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_remove(struct nfi_worker *wrk, char *url);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param old_url 'TODO'.
   * @param new_url 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_rename(struct nfi_worker *wrk, char *old_url, char *new_url);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param fh 'TODO'.
   * @param attr 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_getattr(struct nfi_worker *wrk, struct nfi_fhandle *fh, struct nfi_attr *attr);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param fh 'TODO'.
   * @param attr 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_setattr(struct nfi_worker *wrk, struct nfi_fhandle *fh, struct nfi_attr *attr);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param url 'TODO'.
   * @param attr 'TODO'.
   * @param fh 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_mkdir(struct nfi_worker *wrk, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param url 'TODO'.
   * @param fho 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_opendir(struct nfi_worker *wrk, char *url, struct nfi_fhandle *fho);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param fhd 'TODO'.
   * @param entry 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_readdir(struct nfi_worker *wrk, struct nfi_fhandle *fhd, struct dirent *entry);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param fh 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_closedir(struct nfi_worker *wrk, struct nfi_fhandle *fh);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param url 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_rmdir(struct nfi_worker *wrk, char *url);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param inf 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_statfs(struct nfi_worker *wrk, struct nfi_info *inf);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param url 'TODO'.
   * @param virtual_path 'TODO'.
   * @param storage_path 'TODO'.
   * @param opt 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_preload(struct nfi_worker *wrk, char *url, char *virtual_path, char *storage_path, int opt);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param wrk 'TODO'.
   * @param url 'TODO'.
   * @param virtual_path 'TODO'.
   * @param storage_path 'TODO'.
   * @param opt 'TODO'.
   * @return 'TODO'.
   */
  int nfi_worker_do_flush(struct nfi_worker *wrk, char *url, char *virtual_path, char *storage_path, int opt);

#ifdef __cplusplus
}
#endif

#endif
