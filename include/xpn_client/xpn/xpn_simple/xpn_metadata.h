
/*
 *  Copyright 2000-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Dario Muñoz Muñoz
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

#ifndef _XPN_METADATA_H_
#define _XPN_METADATA_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "nfi.h"

  /* ... Const / Const ................................................. */

  #define XPN_HEADER_SIZE 8192
  
  #define XPN_MAGIC_NUMBER "XPN"
  #define XPN_METADATA_VERSION 1
  #define XPN_METADATA_MAX_RECONSTURCTIONS 40
  #define XPN_METADATA_DISTRIBUTION_ROUND_ROBIN 1

  #define XPN_CHECK_MAGIC_NUMBER(mdata) \
         (((mdata)->magic_number[0] == XPN_MAGIC_NUMBER[0]) && \
          ((mdata)->magic_number[1] == XPN_MAGIC_NUMBER[1]) && \
          ((mdata)->magic_number[2] == XPN_MAGIC_NUMBER[2])) 

  // metadata of xpn file
  struct xpn_metadata
  {
    char    magic_number[3];
    int     version;                                      // Version number
    int     type;                                         // Type of file: file or directory
    ssize_t block_size;                                   // Size of block used
    ssize_t file_size;                                    // Size of the file
    int     replication_level;                            // Replication level of files: 0, 1, 2, ...
    int     first_node;                                   // Server which has the first block
    int     distribution_policy;                          // Distribution policy of blocks, default: round-robin
    int     data_nserv[XPN_METADATA_MAX_RECONSTURCTIONS]; // Array of number of servers to reconstruct
    int     offsets[XPN_METADATA_MAX_RECONSTURCTIONS];    // Array indicating the block where new server configuration starts
  };

  // Forward declaration
  struct nfi_server;

  /* ... Functions / Funciones ......................................... */

  int XpnGetMetadataPos(struct xpn_metadata *mdata, int pos);

  void XpnPrintMetadata(struct xpn_metadata *mdata);
  
  int XpnCreateMetadata(struct xpn_metadata *mdata, int pd, const char *path);
  int XpnCreateMetadataExtern(struct xpn_metadata *mdata, const char *path, int nserv, int block_size, int replication_level);

  int XpnReadMetadata(struct xpn_metadata *mdata, int nserv, struct nfi_server *servers, const char *path, int replication_level);

  int XpnUpdateMetadata(struct xpn_metadata *mdata, int nserv, struct nfi_server *servers, const char *path, int replication_level, int only_file_size);

  int xpn_simple_get_block_locality(char *path, off_t offset, int *url_c, char **url_v[]);
  int xpn_simple_free_block_locality(int *url_c, char **url_v[]);

  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
