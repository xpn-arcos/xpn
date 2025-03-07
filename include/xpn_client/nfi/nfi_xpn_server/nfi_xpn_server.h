
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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


#ifndef _NFI_XPN_SERVER_H_
#define _NFI_XPN_SERVER_H_

  #ifdef  __cplusplus
    extern "C" {
  #endif

  /* ... Include / Inclusion ........................................... */

  #include "all_system.h"
  #include "base/utils.h"
  #include "base/path_misc.h"
  #include "base/urlstr.h"
  #include "base/workers.h"
  #include "base/ns.h"
  #include "base/service_socket.h"
  #include "nfi.h"
  #include "nfi_local.h"
  #include "nfi_worker.h"
  #include "xpn_server/xpn_server_conf.h"
  #include "xpn_server/xpn_server_ops.h"


  /* ... Data structures / Estructuras de datos ........................ */

  struct nfi_xpn_server
  {
     // client identification
    int  size;
    int  rank;

    // client execution configuration
    int xpn_thread;
    int xpn_locality;
    int locality;

    // MQTT usage
    int xpn_mosquitto_mode;
    int xpn_mosquitto_qos;

    #ifdef HAVE_MOSQUITTO_H
    struct mosquitto * mqtt;
    #endif

    int keep_connected;

    // server comm
    int server_type;  // it can be XPN_SERVER_TYPE_MPI, XPN_SERVER_TYPE_SCK
    #ifdef ENABLE_MPI_SERVER
    MPI_Comm server_comm; // For mpi_server
    #endif
    #ifdef ENABLE_SCK_SERVER
    int server_socket; // For sck_server
    #endif
    // server port
    char port_name [MAX_PORT_NAME_LENGTH];
    char  srv_name [MAX_PORT_NAME_LENGTH];

    // server arguments
    int    argc;
    char **argv;
  };

  struct nfi_xpn_server_fhandle
  {
    char path[PATH_MAX];
    long telldir;
    DIR *dir;
    int fd;
  };


  /* ... Functions / Funciones ......................................... */

  int     nfi_xpn_server_init       ( char *url, struct nfi_server *serv, int server_type );
  int     nfi_xpn_server_destroy    ( struct nfi_server *server );

  int     nfi_xpn_server_connect    ( struct nfi_server *server, char *url, char* prt, char* serv, char* dir );
  int     nfi_xpn_server_reconnect  ( struct nfi_server *server );
  int     nfi_xpn_server_disconnect ( struct nfi_server *server );

  int     nfi_xpn_server_create     ( struct nfi_server *server, char *url, mode_t mode, struct nfi_attr *attr, struct nfi_fhandle  *fh );
  int     nfi_xpn_server_open       ( struct nfi_server *server, char *url, int flags, mode_t mode, struct nfi_fhandle *fho );
  ssize_t nfi_xpn_server_read       ( struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size );
  ssize_t nfi_xpn_server_write      ( struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size );
  int     nfi_xpn_server_close      ( struct nfi_server *server, struct nfi_fhandle *fh );
  int     nfi_xpn_server_remove     ( struct nfi_server *server, char *url );
  int     nfi_xpn_server_rename     ( struct nfi_server *server, char *old_url, char *new_url );

  int     nfi_xpn_server_getattr    ( struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr );
  int     nfi_xpn_server_setattr    ( struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr );

  int     nfi_xpn_server_mkdir      ( struct nfi_server *server, char *url, mode_t mode, struct nfi_attr *attr, struct nfi_fhandle *fh );
  int     nfi_xpn_server_opendir    ( struct nfi_server *server, char *url, struct nfi_fhandle *fho );
  int     nfi_xpn_server_readdir    ( struct nfi_server *server, struct nfi_fhandle *fhd, struct dirent *entry );
  int     nfi_xpn_server_closedir   ( struct nfi_server *server, struct nfi_fhandle *fhd );
  int     nfi_xpn_server_rmdir      ( struct nfi_server *server, char *url );

  int     nfi_xpn_server_statfs     ( struct nfi_server *server, struct nfi_info *inf );

  int     nfi_xpn_server_read_mdata      ( struct nfi_server *serv, char *url, struct xpn_metadata *mdata );
  int     nfi_xpn_server_write_mdata     ( struct nfi_server *serv, char *url, struct xpn_metadata *mdata, int only_file_size );

  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
