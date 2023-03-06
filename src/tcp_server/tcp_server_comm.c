
  /*
   *  Copyright 2020-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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


  /* ... Include / Inclusion ........................................... */

  #include "tcp_server_comm.h"


  /* ... Functions / Funciones ......................................... */

  int tcp_server_comm_init ( tcp_server_param_st *params )
  {
    int  ret, provided ;
    char serv_name [HOST_NAME_MAX];

    // Print server info
    gethostname(serv_name, HOST_NAME_MAX);
    printf("--------------------------------\n");
    printf("Starting XPN MPI server %s\n", serv_name);
    printf("--------------------------------\n\n");

    //Get timestap
    struct timeval t0;
    TIME_MISC_Timer(&t0);

    DEBUG_BEGIN() ;

    // TCP_Init
    ret = TCP_Init_thread(&(params->argc), &(params->argv), TCP_THREAD_MULTIPLE, &provided) ;
    if (ret != 0) {
      debug_error("Server[%d]: TCP_Init fails :-(", -1) ;
      return -1 ;
    }

    // params->rank = comm_rank()
    ret = int_rank(TCP_COMM_WORLD, &(params->rank)) ;
    if (ret != 0) {
      debug_error("Server[%d]: int_rank fails :-(", params->rank) ;
      return -1 ;
    }

    // params->size = comm_size()
    ret = int_size(TCP_COMM_WORLD, &(params->size)) ;
    if (ret != 0) {
      debug_error("Server[%d]: int_size fails :-(", params->rank) ;
      return -1 ;
    }

    // Open server port...
    ret = TCP_Open_port(TCP_INFO_NULL, params->port_name) ;
    if (ret != 0) {
      debug_error("Server[%d]: TCP_Open_port fails :-(", params->rank) ;
      return -1 ;
    }

    // Generate DNS file
#ifndef TCP_SERVICE_NAME
    ret = ns_publish(params->dns_file, params->srv_name, params->port_name);
    if (ret < 0) {
      debug_error("Server[%d]: NS_PUBLISH fails :-(", params->rank) ;
      return -1;
    }
#else
    // Publish port name
    TCP_Info info ;
    TCP_Info_create(&info) ;
    TCP_Info_set(info, "otcp_global_scope", "true") ;

    struct hostent *serv_entry;
    gethostname(serv_name, HOST_NAME_MAX); // get hostname
    serv_entry = gethostbyname(serv_name); // find host information
    sprintf(params->srv_name, "%s", serv_name) ;

    ret = TCP_Publish_name(params->srv_name, info, params->port_name) ;
    if (ret != 0) {
      debug_error("Server[%d]: TCP_Publish_name fails :-(", params->rank) ;
      return -1 ;
    }
#endif

    // Print server init information
    TCP_Barrier(TCP_COMM_WORLD);

    struct timeval t1;
    struct timeval tf;
    float time;
    TIME_MISC_Timer(&t1);
    TIME_MISC_DiffTime(&t0, &t1, &tf);
    time = TIME_MISC_TimevaltoFloat(&tf);

    if (params->rank == 0)
    {
      tcp_server_params_show(params) ;
      printf("\n\n");
      printf("Time to inizialize all servers: %f s\n", time);
      printf("\n");
      printf("---------------------------\n");
      printf("All XPN MPI servers running\n");
      printf("---------------------------\n");
      printf("\n\n");
    }

    debug_info("[SERV-COMM] server %d available at %s\n", params->rank, params->port_name) ;
    debug_info("[SERV-COMM] server %d accepting...\n",    params->rank) ;

    DEBUG_END() ;

    // Return OK
    return 1 ;
  }
  

  int tcp_server_comm_destroy ( tcp_server_param_st *params )
  {
    int ret ;

    DEBUG_BEGIN() ;

    // Close port
    TCP_Close_port(params->port_name) ;

    for (int i = 0; i < params->size; ++i)
    {
      if (params->rank == i)
      {
#ifndef TCP_SERVICE_NAME
        // Unpublish port name
        ret = ns_unpublish(params->dns_file) ;
        if (ret < 0) {
          debug_error("Server[%d]: ns_unpublish fails :-(", params->rank) ;
          return -1 ;
        }
#else
        // Unpublish port name
        ret = TCP_Unpublish_name(params->srv_name, TCP_INFO_NULL, params->port_name) ;
        if (ret != 0) {
            debug_error("Server[%d]: port unregistration fails :-(\n", params->rank) ;
            return -1 ;
        }
#endif
      }

      TCP_Barrier(TCP_COMM_WORLD);
    }

    // Finalize
    ret = TCP_Finalize() ;
    if (ret != 0) {
      debug_error("Server[%d]: TCP_Finalize fails :-(", params->rank) ;
      return -1 ;
    }

    // Print server info
    char serv_name  [HOST_NAME_MAX];
    gethostname(serv_name, HOST_NAME_MAX);
    printf("--------------------------------\n");
    printf("XPN MPI server %s stopped\n", serv_name);
    printf("--------------------------------\n\n");

    DEBUG_END() ;

    // Return OK
    return 1 ;
  }
  

  int tcp_server_comm_accept ( tcp_server_param_st *params )
  {
    int ret ;

    DEBUG_BEGIN() ;

    // Accept
    ret = int_accept(params->port_name, TCP_INFO_NULL, 0, TCP_COMM_SELF, &(params->client)) ;
    if (ret != 0) {
      debug_error("Server[%d]: int_accept fails :-(", params->rank) ;
      return TCP_COMM_NULL ;
    }

    DEBUG_END() ;

    // Return client int
    return params->client ;
  }

  
  int tcpClient_comm_close ( int fd )
  {
    int ret ;

    if (fd == TCP_COMM_NULL) {
      debug_error("Server[]: ERROR: TCP_COMM_NULL as communication descriptor :-(") ;
      return 1;
    }

    // Disconnect
    ret = int_disconnect(&fd) ;
    if (ret != 0) {
      debug_error("Server[]: ERROR: int_disconnect fails :-(") ;
      return -1 ;
    }

    // Return OK
    return 1 ;
  }
  

  ssize_t tcp_server_comm_read_operation ( tcp_server_param_st *params, int fd, char *data, ssize_t size, int *rank_client_id )
  {
    int ret ;
    TCP_Status status ;

    DEBUG_BEGIN() ;

    // Check params
    if (NULL == params) {
      debug_warning("Server[%d]: ERROR: NULL arguments", -1) ;
      return -1;
    }
    if (size == 0) {
      return  0;
    }
    if (size < 0){
      debug_warning("Server[%d]: ERROR: size < 0", params->rank) ;
      return  -1;
    }

    // Get message
    ret = TCP_Recv(data, size, TCP_INT, TCP_ANY_SOURCE, 0, fd, &status) ;
    if (ret != 0) {
      debug_warning("Server[%d]: TCP_Recv fails :-(", params->rank) ;
    }

    *rank_client_id = status.TCP_SOURCE;

    debug_info("[SERV-COMM] MPI SOURCE %d, TCP_TAG %d, TCP_ERROR %d\n", status.TCP_SOURCE, status.TCP_TAG, status.TCP_ERROR);

    DEBUG_END() ;

    // Return bytes read
    return size;
  }


  ssize_t tcp_server_comm_write_data ( tcp_server_param_st *params, int fd, char *data, ssize_t size, int rank_client_id )
  {
    int ret ;

    DEBUG_BEGIN() ;

    // Check params
    if (NULL == params) {
        debug_warning("Server[%d]: ERROR: NULL params", -1) ;
        return -1;
    }
    if (size == 0){
        return 0;
    }
    if (size < 0){
        debug_warning("Server[%d]: ERROR: size < 0", params->rank) ;
        return -1;
    }

    // Send message
    ret = TCP_Send(data, size, TCP_CHAR, rank_client_id, 1, fd) ;
    if (ret != 0) {
      debug_warning("Server[%d]: ERROR: TCP_Recv fails :-(", params->rank) ;
    }

    DEBUG_END() ;

    // Return bytes written
    return size;
  }


  ssize_t tcp_server_comm_read_data ( tcp_server_param_st *params, int fd, char *data, ssize_t size, int rank_client_id )
  {
    int ret ;
    TCP_Status status ;

    DEBUG_BEGIN() ;

    // Check params
    if (NULL == params) {
      debug_warning("Server[-1]: ERROR: NULL params", -1) ;
      return -1;
    }
    if (size == 0) {
      return  0;
    }
    if (size < 0){
      debug_warning("Server[%d]: ERROR: size < 0", params->rank) ;
      return  -1;
    }

    // Get message
    ret = TCP_Recv(data, size, TCP_CHAR, rank_client_id, 1, fd, &status);
    if (ret != 0) {
      debug_warning("Server[%d]: ERROR: TCP_Recv fails :-(", params->rank) ;
    }

    debug_info("[SERV-COMM] MPI SOURCE %d, TCP_TAG %d, TCP_ERROR %d\n", status.TCP_SOURCE, status.TCP_TAG, status.TCP_ERROR);

    DEBUG_END() ;

    // Return bytes read
    return size;
  }


 /* ................................................................... */

