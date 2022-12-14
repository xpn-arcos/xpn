
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

  #include "mpi_server_comm.h"


  /* ... Functions / Funciones ......................................... */

  int mpi_server_comm_init ( mpi_server_param_st *params )
  {
    int ret, provided ;

    // Print server info
    char serv_name  [HOST_NAME_MAX];
    gethostname(serv_name, HOST_NAME_MAX);
    printf("--------------------------------\n");
    printf("Starting XPN MPI server %s\n", serv_name);
    printf("--------------------------------\n\n");

    //Get timestap
    struct timeval t0;
    TIME_MISC_Timer(&t0);

    DEBUG_BEGIN() ;

    // MPI_Init
    ret = MPI_Init_thread(&(params->argc), &(params->argv), MPI_THREAD_MULTIPLE, &provided) ;
    if (MPI_SUCCESS != ret) {
      debug_error("Server[%d]: MPI_Init fails :-(", -1) ;
      return -1 ;
    }

    // params->rank = comm_rank()
    ret = MPI_Comm_rank(MPI_COMM_WORLD, &(params->rank)) ;
    if (MPI_SUCCESS != ret) {
      debug_error("Server[%d]: MPI_Comm_rank fails :-(", params->rank) ;
      return -1 ;
    }

    // params->size = comm_size()
    ret = MPI_Comm_size(MPI_COMM_WORLD, &(params->size)) ;
    if (MPI_SUCCESS != ret) {
      debug_error("Server[%d]: MPI_Comm_size fails :-(", params->rank) ;
      return -1 ;
    }

    // Open server port...
    ret = MPI_Open_port(MPI_INFO_NULL, params->port_name) ;
    if (MPI_SUCCESS != ret) {
      debug_error("Server[%d]: MPI_Open_port fails :-(", params->rank) ;
      return -1 ;
    }

    // Generate DNS file
    ret = mpi_server_dns_publish(params->srv_name, params->dns_file, params->port_name);
    if (ret < 0)
    {
      return -1;
    }

    // Print server init information
    MPI_Barrier(MPI_COMM_WORLD);

    struct timeval t1;
    struct timeval tf;
    float time;
    TIME_MISC_Timer(&t1);
    TIME_MISC_DiffTime(&t0, &t1, &tf);
    time = TIME_MISC_TimevaltoFloat(&tf);

    if (params->rank == 0)
    {
      mpi_server_params_show(params) ;
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
  
  int mpi_server_comm_destroy ( mpi_server_param_st *params )
  {
    int ret ;

    DEBUG_BEGIN() ;

    // Close port
    MPI_Close_port(params->port_name) ;

    for (int i = 0; i < params->size; ++i)
    {
      if(params->rank == i){
        // Unpublish port name
        ret = mpi_server_dns_unpublish (params->dns_file);
        if (ret < 0) {
          debug_error("Server[%d]: mpi_server_dns_unpublish fails :-(", params->rank) ;
          return -1 ;
        }
      }
      MPI_Barrier(MPI_COMM_WORLD);
    }

    // Finalize
    ret = MPI_Finalize() ;
    if (MPI_SUCCESS != ret) {
      debug_error("Server[%d]: MPI_Finalize fails :-(", params->rank) ;
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
  
  MPI_Comm mpi_server_comm_accept ( mpi_server_param_st *params )
  {
    int ret ;

    DEBUG_BEGIN() ;

    // Accept
    ret = MPI_Comm_accept(params->port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &(params->client)) ;
    if (MPI_SUCCESS != ret) {
      debug_error("Server[%d]: MPI_Comm_accept fails :-(", params->rank) ;
      return MPI_COMM_NULL ;
    }

    DEBUG_END() ;

    // Return client MPI_Comm
    return params->client ;
  }
  
  int mpiClient_comm_close ( MPI_Comm fd )
  {
    int ret ;

    if (fd == MPI_COMM_NULL)
    {
      return 1;
    }

    // Disconnect
    ret = MPI_Comm_disconnect(&fd) ;
    if (MPI_SUCCESS != ret) {
      debug_error("Server[%d]: MPI_Comm_disconnect fails :-(", params->rank) ;
      return -1 ;
    }

    // Return OK
    return 1 ;
  }
  
  ssize_t mpi_server_comm_read_operation ( mpi_server_param_st *params, MPI_Comm fd, char *data, ssize_t size, int *rank_client_id )
  {
    int ret ;
    MPI_Status status ;

    DEBUG_BEGIN() ;

    // Check params
    if (size == 0) {
      return  0;
    }
    if (size < 0){
      debug_warning("Server[%d]: size < 0", params->rank) ;
      return  -1;
    }
    if (NULL == params) {
      debug_warning("Server[%d]: NULL params", params->rank) ;
      return -1;
    }

    // Get message
    ret = MPI_Recv(data, size, MPI_INT, MPI_ANY_SOURCE, 0, fd, &status) ;
    if (MPI_SUCCESS != ret) {
      debug_warning("Server[%d]: MPI_Recv fails :-(", params->rank) ;
    }

    *rank_client_id = status.MPI_SOURCE;

    debug_info("MPI SOURCE %d, MPI_TAG %d, MPI_ERROR %d\n", status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);

    DEBUG_END() ;

    // Return bytes read
    return size;
  }

  ssize_t mpi_server_comm_write_data ( mpi_server_param_st *params, MPI_Comm fd, char *data, ssize_t size, int rank_client_id )
  {
    int ret ;

    DEBUG_BEGIN() ;

    // Check params
    if (size == 0){
        return 0;
    }
    if (size < 0){
        debug_warning("Server[%d]: size < 0", params->rank) ;
        return -1;
    }
    if (NULL == params) {
        debug_warning("Server[%d]: NULL params", params->rank) ;
        return -1;
    }

    // Send message
    ret = MPI_Send(data, size, MPI_CHAR, rank_client_id, 1, fd) ;
    if (MPI_SUCCESS != ret) {
      debug_warning("Server[%d]: MPI_Recv fails :-(", params->rank) ;
    }

    DEBUG_END() ;

    // Return bytes written
    return size;
  }

  ssize_t mpi_server_comm_read_data ( mpi_server_param_st *params, MPI_Comm fd, char *data, ssize_t size, int rank_client_id )
  {
    int ret ;
    MPI_Status status ;

    DEBUG_BEGIN() ;

    // Check params
    if (size == 0) {
      return  0;
    }
    if (size < 0){
      debug_warning("Server[%d]: size < 0", params->rank) ;
      return  -1;
    }
    if (NULL == params) {
      debug_warning("Server[%d]: NULL params", params->rank) ;
      return -1;
    }

    // Get message
    ret = MPI_Recv(data, size, MPI_CHAR, rank_client_id, 1, fd, &status);
    if (MPI_SUCCESS != ret) {
      debug_warning("Server[%d]: MPI_Recv fails :-(", params->rank) ;
    }

    debug_info("MPI SOURCE %d, MPI_TAG %d, MPI_ERROR %d\n", status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);

    DEBUG_END() ;

    // Return bytes read
    return size;
  }
