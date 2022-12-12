#ifndef _NFI_MPI_SERVER_H
#define _NFI_MPI_SERVER_H

#ifdef  __cplusplus
    extern "C" {
#endif


   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"
      #include "mpi_server_conf.h"
      #include "mpi_server_ops.h"
      #include "mpi_server_comm.h"
      #include "mpi_server_params.h"
      #include "base/utils.h"
      #include "base/string_misc.h"
      #include "nfi_mpi_server_err.h"
      #include "nfi.h"


   /* ... Data structures / Estructuras de datos ........................ */

      struct nfi_mpi_server_server
      {
         // old stuff of XPN
         char id[NFIMAXPATHLEN];
         char path[NFIMAXPATHLEN];
         //struct nfi_mpi_server_connector sd;

         // client stub
         mpiClient_param_st params ;
      };

      struct nfi_mpi_server_fhandle
      {
         char path[NFIMAXPATHLEN];
         int  fd;
         DIR *dir;
      };


   /* ... Functions / Funciones ......................................... */

      int     nfi_mpi_server_init      ( char *url, struct nfi_server *serv, struct nfi_attr_server *attr ) ;

      int     nfi_mpi_server_connect   ( struct nfi_server *server, char *url, char* prt, char* serv, char* dir ) ;
      int     nfi_mpi_server_reconnect ( struct nfi_server *server ) ;
      int     nfi_mpi_server_disconnect( struct nfi_server *server ) ;

      int     nfi_mpi_server_preload   ( struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt ) ;
      int     nfi_mpi_server_flush     ( struct nfi_server *server, char *url, char *virtual_path, char *storage_path, int opt ) ;

      int     nfi_mpi_server_getattr   ( struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr ) ;
      int     nfi_mpi_server_setattr   ( struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr ) ;

      int     nfi_mpi_server_open      ( struct nfi_server *server, char *url, struct nfi_fhandle *fho ) ;
      int     nfi_mpi_server_close     ( struct nfi_server *server, struct nfi_fhandle *fh ) ;
      ssize_t nfi_mpi_server_read      ( struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size ) ;
      ssize_t nfi_mpi_server_write     ( struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size ) ;
      int     nfi_mpi_server_create    ( struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh ) ;

      int     nfi_mpi_server_remove    ( struct nfi_server *server, char *url ) ;
      int     nfi_mpi_server_rename    ( struct nfi_server *server, char *old_url, char *new_url ) ;
      int     nfi_mpi_server_mkdir     ( struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh ) ;
      int     nfi_mpi_server_rmdir     ( struct nfi_server *server, char *url ) ;

      int     nfi_mpi_server_opendir   ( struct nfi_server *server, char *url, struct nfi_fhandle *fho ) ;
      int     nfi_mpi_server_readdir   ( struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type ) ;
      int     nfi_mpi_server_closedir  ( struct nfi_server *server, struct nfi_fhandle *fh ) ;

      int     nfi_mpi_server_statfs    ( struct nfi_server *server, struct nfi_info *inf ) ;


   /* ................................................................... */

 #ifdef  __cplusplus
     }
 #endif


#endif


