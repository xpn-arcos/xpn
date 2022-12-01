#ifndef _MPISERVER_NS_H_
#define _MPISERVER_NS_H_


      #include "all_system.h"
      #include "base/utils.h"


      /*
       *  API
       */

      int mpiServer_dns_publish   ( char * param_srv_name, char * dns_file, char * port_name ) ;
      int mpiServer_dns_unpublish ( char * dns_file ) ;
      int mpiServer_dns_lookup    ( char * param_srv_name, char * port_name ) ;


#endif

