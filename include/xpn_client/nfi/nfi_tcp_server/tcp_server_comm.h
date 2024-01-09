#ifndef _TCP_SERVER_COMM_H_
#define _TCP_SERVER_COMM_H_

      #include "all_system.h"
      #include "tcp_server_params.h"
      #include "base/utils.h"
      #include "base/ns.h"
      #include "base/ns_tcp.h"
      #include "tcp_server_ops.h"

      /*
       *  API
       */

      int     tcpClient_comm_init       ( tcpClient_param_st *params ) ;
      int     tcpClient_comm_destroy    ( tcpClient_param_st *params ) ;
      int     tcpClient_comm_connect    ( tcpClient_param_st *params ) ;
      int     tcpClient_comm_disconnect ( tcpClient_param_st *params ) ;
      int     tcpClient_comm_locality   ( tcpClient_param_st *params ) ;

      ssize_t tcpClient_write_operation ( int fd, char *data, ssize_t size, char *msg_id ) ;
      ssize_t tcpClient_write_data      ( int fd, char *data, ssize_t size, char *msg_id ) ;
      ssize_t tcpClient_read_data       ( int fd, char *data, ssize_t size, char *msg_id ) ;

#endif

