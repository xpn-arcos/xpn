#ifndef _TCPSERVER_PARAMS_H_
#define _TCPSERVER_PARAMS_H_

   #include <stdlib.h>
   #include <stdio.h>
   #include <string.h>
   #include "tcpServer_conf.h"
   #include "tcpServer_utils.h"


   /*
    * Datatype
    */

   struct tcpServer_param_st
   {
        int port;
        int IOsize;
        char name[255];
        char file[255];
        char dirbase[255];
   };


   /*
    * API
    */

   void params_show_values ( struct tcpServer_param_st *params ) ;
   void params_show_usage  ( void ) ;
   int  params_get         ( int argc, char *argv[], struct tcpServer_param_st *params ) ;


#endif

