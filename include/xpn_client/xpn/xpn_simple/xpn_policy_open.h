
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

#ifndef _XPN_POLICY_OPEN_H
#define _XPN_POLICY_OPEN_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "xpn_file.h"


  /* ... Const / Const ................................................. */

  #define XPN_META_SIZE (4*KB)

  /* ... Data structures / Estructuras de datos ........................ */

  enum xpn_work{
    no_xpn_op       = -1,
    op_xpn_getattr  =  0,
    op_xpn_setattr  =  1,
    op_xpn_open     =  2,
    op_xpn_creat    =  3,
    op_xpn_close    =  4,
    op_xpn_remove   =  5,
    op_xpn_rename   =  6,
    op_xpn_read     =  7,
    op_xpn_write    =  8,
    op_xpn_mkdir    =  9,
    op_xpn_rmdir    = 10,
    op_xpn_opendir  = 11,
    op_xpn_readdir  = 12,
    op_xpn_closedir = 13,
    op_xpn_statfs   = 14,
    op_xpn_preload  = 15,
    op_xpn_flush    = 16,
  };


  /* ... Functions / Funciones ......................................... */

  ssize_t XpnGetSizeThreads(struct xpn_partition *p);

  void XpnGetURLServer( struct nfi_server *serv, char *abs_path, char *url_serv);

  int XpnGetServers(int op, int pd, char *abs_path, int fd, struct nfi_server ***servers, int type);

  int XpnGetMetadataPos(struct xpn_metadata *mdata, int pos);
  int XpnCreateMetadata(struct xpn_metadata *mdata, int pd, char *path);

  int XpnReadMetadata(struct xpn_metadata *mdata, int nserv, struct nfi_server **servers , struct xpn_fh *fh, char *path, int pd);

  int XpnUpdateMetadata(struct xpn_metadata *mdata, int nserv, struct nfi_server **servers , struct xpn_fh *fh, char *path);

  int XpnGetFh(struct xpn_metadata *mdata, struct nfi_fhandle **fh,  struct nfi_server *servers,  char *path);

  int XpnGetAtribFd   (int fd,      struct stat *st);
  int XpnGetAtribPath (char * path, struct stat *st);


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
