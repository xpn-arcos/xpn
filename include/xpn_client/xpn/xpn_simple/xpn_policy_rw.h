
/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Dario Muñoz Muñoz
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

#ifndef _XPN_POLICY_RW_H
#define _XPN_POLICY_RW_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "xpn_file.h"
  #include "xpn_policy_open.h"


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  void XpnCalculateBlock(int block_size, int replication_level, int nserv, off_t offset, int replication, off_t *local_offset, int *serv);
  void XpnCalculateBlockInvert(int block_size, int replication_level, int nserv, int serv, off_t local_offset, off_t *offset, int *replication);

  int XpnReadGetBlock(int fd, off_t offset, int serv_client, off_t *local_offset, int *serv);
  int XpnWriteGetBlock(int fd, off_t offset, int replication, off_t *local_offset, int *serv);

  void *XpnReadBlocks      (int fd, const void *buffer, size_t size, off_t offset, int serv_client, struct nfi_worker_io ***io_out, int **ion_out, int num_servers);
  void XpnReadBlocksFinish(int fd, void *buffer, size_t size, off_t offset, int serv_client, struct nfi_worker_io ***io_out, int **ion_out, int num_servers, const void *new_buffer);

  void *XpnWriteBlocks      (int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers);

  ssize_t XpnReadGetTotalBytes (ssize_t *res_v, int num_servers);
  ssize_t XpnWriteGetTotalBytes (ssize_t *res_v, int num_servers, struct nfi_worker_io ***io, int *ion, struct nfi_server *servers);

  ssize_t XpnGetRealFileSize(struct xpn_partition *part, struct nfi_attr *attr, int n_serv);
 
  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif