#ifndef _MPISERVER_COMM_H_
#define _MPISERVER_COMM_H_

      #include "all_system.h"
      #include "nfi_mpiServer.h"
      #include "mpiServer_utils.h"
      #include "mpiServer_ops.h"


      /*
       *  API
       */

      int     mpiClient_comm_init       ( struct nfi_mpiServer_server *params ) ;
      int     mpiClient_comm_destroy    ( struct nfi_mpiServer_server *params ) ;
      int     mpiClient_comm_connect    ( struct nfi_mpiServer_server *params ) ;
      int     mpiClient_comm_disconnect ( struct nfi_mpiServer_server *params ) ;
      ssize_t mpiClient_write_data      ( int fd, char *data, ssize_t size, char *msg_id ) ;
      ssize_t mpiClient_read_data       ( int fd, char *data, ssize_t size, char *msg_id ) ;

#endif

