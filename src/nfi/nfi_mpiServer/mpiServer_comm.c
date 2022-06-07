
  /*
   *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
   *
   *  This file is part of mpiServer.
   *
   *  mpiServer is free software: you can redistribute it and/or modify
   *  it under the terms of the GNU Lesser General Public License as published by
   *  the Free Software Foundation, either version 3 of the License, or
   *  (at your option) any later version.
   *
   *  mpiServer is distributed in the hope that it will be useful,
   *  but WITHOUT ANY WARRANTY; without even the implied warranty of
   *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   *  GNU Lesser General Public License for more details.
   *
   *  You should have received a copy of the GNU Lesser General Public License
   *  along with mpiServer.  If not, see <http://www.gnu.org/licenses/>.
   *
   */



  /* ... Include / Inclusion ........................................... */

  #include "mpiServer_comm.h"



  /* ... Functions / Funciones ......................................... */

  int mpiClient_comm_init ( mpiClient_param_st *params )
  {
    int ret, provided, claimed ;
    int flag = 0 ;
    // char srv_name[1024] ;


    memset(params, 0, sizeof(mpiClient_param_st)); // Initialize params 

    debug_info("[COMM] begin mpiClient_comm_init(...)\n") ;

    // MPI_Init
    MPI_Initialized(&flag);

    if (!flag)
    {
      ret = MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided) ; // TODO: server->argc, server->argv from upper layers?
      if (MPI_SUCCESS != ret) {
        debug_error("Server[%d]: MPI_Init fails :-(", -1) ;
        return -1 ;
      }
    }

    MPI_Query_thread(&claimed) ;
    if (claimed != MPI_THREAD_MULTIPLE) {
      printf("MPI_Init: your MPI implementation seem not supporting thereads\n") ;
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
      debug_error("Server[%d]: MPI_Comm_size fails :-(", params->size) ;
      return -1 ;
    }

    debug_info("[COMM] server %d available at %s\n", params->rank, params->port_name) ;
    debug_info("[COMM] server %d accepting...\n",    params->rank) ;
    debug_info("[COMM] end mpiClient_comm_init(...)\n") ;

    // Return OK
    return 1 ;
  }

  //TODO: revisar
  int mpiClient_comm_destroy ( mpiClient_param_st *params )
  {
    int ret ;
    int data[512];
    int size;

    debug_info("[COMM] begin mpiClient_comm_destroy(...)\n") ;

    //MPI_Barrier(MPI_COMM_WORLD);

    //MPI_Comm_size(params->server, &size);
    debug_info("[COMM] mpiClient_comm_disconnect nservers: %d\n", size) ;
    /*if (params->rank==0) {  
      for (int i = 0; i < size; i++) { 
        data[0] = MPISERVER_DISCONNECT;
        MPI_Send( data, 1, MPI_INT, i, 0, params->server );
      }
    }*/

    data[0] = MPISERVER_DISCONNECT;
    MPI_Send( data, 1, MPI_INT, 0, 0, params->server );

    // Disconnect
    ret = MPI_Comm_disconnect(&(params->server)) ;
    if (MPI_SUCCESS != ret) {
      debug_error("Server[%d]: MPI_Comm_disconnect fails :-(", params->rank) ;
      return -1 ;
    }

    ret = MPI_Finalize();
    if (MPI_SUCCESS != ret) {
      debug_error("Server[%d]: MPI_Finalize fails :-(", params->rank) ;
      return -1 ;
    }

    debug_info("[COMM] end mpiClient_comm_destroy(...)\n") ;

    // Return OK
    return 1 ;
  }

  int mpiClient_comm_connect ( mpiClient_param_st *params )
  {
    int ret ;
    char srv_name[1024] ;

    // Lookup port name
    //sprintf(srv_name, "mpiServer.%d", params->rank) ;
    sprintf(srv_name, "mpiServer.%s", params->srv_name) ;

    ret = MPI_Lookup_name(srv_name, MPI_INFO_NULL, params->port_name) ;
    if (MPI_SUCCESS != ret) {
      debug_error("Server[%d]: MPI_Lookup_name fails :-(", params->rank) ;
      return -1 ;
    }

    // Connect...
    ret = MPI_Comm_connect(params->port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &(params->server)) ;
    if (MPI_SUCCESS != ret) {
      debug_error("Client[%d]: MPI_Comm_connect fails :-(", params->rank) ;
      return -1 ;
    }

    // To identify client type
    int data = 0;
    ret = MPI_Send( &data, 1, MPI_INT, 0, 0, params->server );
    if (MPI_SUCCESS != ret) {
      debug_warning("Server[?]: MPI_Recv fails :-(") ;
    }

    // Return OK
    return 1 ;
  }

  int mpiClient_comm_disconnect ( mpiClient_param_st *params )
  {
    int ret ;
    int data;
    int size;

    debug_info("[COMM] mpiClient_comm_disconnect nservers\n") ;

    data = MPISERVER_DISCONNECT;
    MPI_Send( &data, 1, MPI_INT, 0, 0, params->server );
    
    // Disconnect
    ret = MPI_Comm_disconnect(&(params->server)) ;
    if (MPI_SUCCESS != ret) {
      debug_error("Server[%d]: MPI_Comm_disconnect fails :-(", params->rank) ;
      return -1 ;
    }

    int flag = 0;
    MPI_Initialized(&flag);

    if (!flag)
    {
      ret = PMPI_Finalize();
      if (MPI_SUCCESS != ret) {
        debug_error("Server[%d]: MPI_Finalize fails :-(", params->rank) ;
        return -1 ;
      }
    }

    // Return OK
    return 1 ;
  }

  int mpiClient_comm_locality ( mpiClient_param_st *params )
  {
    int ret;
    int data;
    char cli_name  [HOST_NAME_MAX];
    char serv_name [HOST_NAME_MAX];
    MPI_Status status ;

    // Get client host name
    gethostname(cli_name, HOST_NAME_MAX);

    data = MPISERVER_GETNODENAME;
    ret = MPI_Send( &data, 1, MPI_INT, 0, 0, params->server );
    if (MPI_SUCCESS != ret) {
      debug_warning("Server[?]: MPI_Send fails :-(") ;
      return -1;
    }

    ret = MPI_Recv(serv_name, HOST_NAME_MAX, MPI_CHAR, 0, 1, params->server, &status);
    if (MPI_SUCCESS != ret) {
      debug_warning("Server[?]: MPI_Recv fails :-(") ;
      return -1;
    }

    if (strcmp(cli_name, serv_name) == 0)
    {
      params->locality = 0; //CUIDADO
    }
    else
    {
      params->locality = 0;
    }
      
    // Return OK
    return 1;
  }

  ssize_t mpiClient_write_operation ( MPI_Comm fd, char *data, ssize_t size, char *msg_id )
  {
    int ret ;

    debug_info("[COMM] server: begin comm_write_data(...)\n") ;

    // Check params
    if (size == 0) {
        return 0;
    }
    if (size < 0) {
        debug_warning("Server[?]: size < 0") ;
        return -1;
    }
    msg_id = msg_id ; // TODO: msg_id is used?

    // Send message
    ret = MPI_Send(data, size, MPI_INT, 0, 0, fd) ;
    if (MPI_SUCCESS != ret) {
        debug_warning("Server[?]: MPI_Recv fails :-(") ;
        size = 0 ;
    }

    debug_info("[COMM] server: end comm_write_data(...)\n") ;

    // Return bytes written
    return size;
  }
  
  ssize_t mpiClient_write_data ( MPI_Comm fd, char *data, ssize_t size, char *msg_id )
  {
    int ret ;
    int rank;

    debug_info("[COMM] server: begin comm_write_data(...)\n") ;

    // Check params
    if (size == 0) {
        return 0;
    }
    if (size < 0) {
        debug_warning("Server[?]: size < 0") ;
        return -1;
    }
    msg_id = msg_id ; // TODO: msg_id is used?

    // Send message
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ret = MPI_Send(data, size, MPI_CHAR, 0, 1, fd) ;
    if (MPI_SUCCESS != ret) {
        debug_warning("Server[?]: MPI_Recv fails :-(") ;
        size = 0 ;
    }

    debug_info("[COMM] server: end comm_write_data(...)\n") ;

    // Return bytes written
    return size;
  }

  ssize_t mpiClient_read_data ( MPI_Comm fd, char *data, ssize_t size, char *msg_id )
  {
    int ret ;
    MPI_Status status ;

    debug_info("[COMM] server: begin comm_read_data(...)\n") ;

    // Check params
    if (size == 0) {
        return  0;
    }
    if (size < 0){
        debug_warning("Server[?]: size < 0") ;
        return  -1;
    }
    msg_id = msg_id ; // TODO: msg_id is used?

    // Get message
    ret = MPI_Recv(data, size, MPI_CHAR, 0, 1, fd, &status);
    if (MPI_SUCCESS != ret) {
        debug_warning("Server[?]: MPI_Recv fails :-(") ;
        size = 0 ;
    }

    debug_info("[COMM] server: end comm_read_data(...)\n") ;

    // Return bytes read
    return size;
  }
