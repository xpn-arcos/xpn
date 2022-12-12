#ifndef _MPI_SERVER_NS_H_
#define _MPI_SERVER_NS_H_


      #include "all_system.h"
      #include "base/utils.h"


      /*
       *  API
       */

      int mpi_server_dns_publish   ( char * param_srv_name, char * dns_file, char * port_name ) ;
      int mpi_server_dns_unpublish ( char * dns_file ) ;
      int mpi_server_dns_lookup    ( char * param_srv_name, char * port_name ) ;


#endif

