#ifndef _NFI_MPISERVER_H
#define _NFI_MPISERVER_H

#ifdef  __cplusplus
    extern "C" {
#endif


   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"
      #include "mpiServer_conf.h"
      #include "mpiServer_ops.h"
      #include "mpiServer_comm.h"
      #include "mpiServer_params.h"
      #include "base/utils.h"
      #include "base/string_misc.h"
      #include "nfi_mpiServer_err.h"
      #include "nfi.h"


   /* ... Data structures / Estructuras de datos ........................ */

      struct nfi_mpiServer_server
      {
         // old stuff of XPN
         char id[NFIMAXPATHLEN];
         char path[NFIMAXPATHLEN];
         //struct nfi_mpiServer_connector sd;

         // client stub
         mpiClient_param_st params ;
      };

      struct nfi_mpiServer_fhandle
      {
         char path[NFIMAXPATHLEN];
         int  fd;
         DIR *dir;
      };


   /* ... Functions / Funciones ......................................... */

      int     nfi_mpiServer_init      ( char *url, struct nfi_server *serv, struct nfi_attr_server *attr ) ;
      int     nfi_mpiServer_destroy   ( struct nfi_server *server ) ;

      int     nfi_mpiServer_connect   ( struct nfi_server *server, char *url, char* prt, char* serv, char* dir ) ;
      int     nfi_mpiServer_reconnect ( struct nfi_server *server ) ;
      int     nfi_mpiServer_disconnect( struct nfi_server *server ) ;

      int     nfi_mpiServer_preload   ( struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt ) ;
      int     nfi_mpiServer_flush     ( struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt ) ;

      int     nfi_mpiServer_getattr   ( struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr ) ;
      int     nfi_mpiServer_setattr   ( struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr ) ;

      int     nfi_mpiServer_open      ( struct nfi_server *server, char *url, struct nfi_fhandle *fho ) ;
      int     nfi_mpiServer_close     ( struct nfi_server *server, struct nfi_fhandle *fh ) ;
      ssize_t nfi_mpiServer_read      ( struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size ) ;
      ssize_t nfi_mpiServer_write     ( struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size ) ;
      int     nfi_mpiServer_create    ( struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh ) ;

      int     nfi_mpiServer_remove    ( struct nfi_server *server, char *url ) ;
      int     nfi_mpiServer_rename    ( struct nfi_server *server, char *old_url, char *new_url ) ;
      int     nfi_mpiServer_mkdir     ( struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh ) ;
      int     nfi_mpiServer_rmdir     ( struct nfi_server *server, char *url ) ;

      int     nfi_mpiServer_opendir   ( struct nfi_server *server, char *url, struct nfi_fhandle *fho ) ;
      int     nfi_mpiServer_readdir   ( struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type ) ;
      int     nfi_mpiServer_closedir  ( struct nfi_server *server, struct nfi_fhandle *fh ) ;

      int     nfi_mpiServer_statfs    ( struct nfi_server *server, struct nfi_info *inf ) ;


   /* ................................................................... */

 #ifdef  __cplusplus
     }
 #endif


#endif