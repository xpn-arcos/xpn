
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

  int mpiClient_comm_init ( mpiClient_param_st *params )
  {
    int ret, provided, claimed ;
    int flag = 0 ;

    debug_info("[CLI-COMM] begin mpiClient_comm_init(...)\n") ;

    // MPI_Init
    MPI_Initialized(&flag);

    if (!flag)
    {
      // TODO: server->argc, server->argv from upper layers?

      // Threads disable
      if (!params->xpn_thread)
      { 
        ret = MPI_Init(NULL, NULL);
        if (MPI_SUCCESS != ret)
        {
          debug_error("Server[%d]: MPI_Init fails :-(", -1) ;
          return -1 ;
        }
      }
      else
      {
        ret = MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided);
        if (MPI_SUCCESS != ret)
        {
          debug_error("Server[%d]: MPI_Init_thread fails :-(", -1) ;
          return -1 ;
        }

        MPI_Query_thread(&claimed);
        if (claimed != MPI_THREAD_MULTIPLE) {
          printf("MPI_Init_thread: your MPI implementation seem not supporting thereads\n") ;
        }
      }
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

    // set is_mpi_server as the used protocol
    setenv("XPN_IS_MPI_SERVER", "1", 1);

    debug_info("[CLI-COMM] server %d available at %s\n", params->rank, params->port_name) ;

    debug_info("[CLI-COMM] end mpiClient_comm_init(...)\n") ;

    // Return OK
    return 1 ;
  }

  int mpiClient_comm_destroy ( mpiClient_param_st *params )
  {
    int ret ;

    debug_info("[CLI-COMM] begin mpiClient_comm_destroy(...)\n") ;

    if (params->size < 0)
    {
      debug_error("Server[%d]: number of process is negative :-(", params->rank) ;
      return -1 ;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int flag = 0;
    MPI_Initialized(&flag);

    if (!flag)
    {
      ret = PMPI_Finalize();
      if (MPI_SUCCESS != ret)
      {
        debug_error("Server[%d]: MPI_Finalize fails :-(", params->rank) ;
        return -1 ;
      }
    }

    //Indicates mpi_server are the used protocolo 
    unsetenv("XPN_IS_MPI_SERVER");

    debug_info("[CLI-COMM] end mpiClient_comm_destroy(...)\n") ;

    // Return OK
    return 1 ;
  }

  int mpiClient_comm_connect ( mpiClient_param_st *params )
  {
    int ret ;

    debug_info("[CLI-COMM] begin mpiClient_comm_connect(...)\n") ;

    int version_len;
    char version[MPI_MAX_LIBRARY_VERSION_STRING];
    MPI_Get_library_version(version, &version_len);

    if(strncasecmp(version,"Open MPI", strlen("Open MPI")) != 0)
    {
      // Lookup port name
      int lookup_retries = 0;
      char aux_srv_ip[1024] ;
      do {
        ret = ns_lookup("mpi_server", params->srv_name, aux_srv_ip, params->port_name) ;
        if (ret < 0)
        {
          if (lookup_retries == 0)
          { 
            char cli_name  [HOST_NAME_MAX];
            gethostname(cli_name, HOST_NAME_MAX);
            printf("----------------------------------------------------------------\n");
            printf("XPN Client %s : Waiting for servers being up and runing...\n", cli_name);
            printf("----------------------------------------------------------------\n\n");
          }
          lookup_retries++;
          sleep(2);
        }
      } while((ret < 0) && (lookup_retries < 150));

      if (ret < 0)
      {
        debug_error("ERROR: DNS Lookup %s Port %s\n", params->srv_name, params->port_name);
        return -1;
      }
    }
    else
    {
      // Lookup port name on nameserver
      ret = MPI_Lookup_name(params->srv_name, MPI_INFO_NULL, params->port_name) ;
      if (MPI_SUCCESS != ret) {
        debug_error("Server[%d]: MPI_Lookup_name fails :-(", params->rank) ;
        return -1 ;
      }
    }

    // Connect...
    int connect_retries = 0;
    do{
      ret = MPI_Comm_connect(params->port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &(params->server)) ;
      if (MPI_SUCCESS != ret)
      {
        if (connect_retries == 0)
        {
          char cli_name  [HOST_NAME_MAX];
          gethostname(cli_name, HOST_NAME_MAX);
          printf("----------------------------------------------------------------\n");
          printf("XPN Client %s : Waiting for servers being up and runing...\n", cli_name);
          printf("----------------------------------------------------------------\n\n");
        }
        connect_retries++;
        sleep(2);
      }
    } while(MPI_SUCCESS != ret && connect_retries < 150);
    
    if (MPI_SUCCESS != ret) {
      debug_error("Client[%d]: MPI_Comm_connect fails :-(", params->rank) ;
      return -1 ;
    }

    debug_info("[CLI-COMM] end mpiClient_comm_connect(...)\n") ;

    // Return OK
    return 1 ;
  }

  int mpiClient_comm_disconnect ( mpiClient_param_st *params )
  {
    int ret ;
    int data;

    debug_info("[CLI-COMM] begin mpiClient_comm_disconnect nservers\n") ;

    data = MPI_SERVER_DISCONNECT;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &(rank));
    if (rank == 0)
    {
      MPI_Send( &data, 1, MPI_INT, 0, 0, params->server );
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Disconnect
    ret = MPI_Comm_disconnect(&(params->server)) ;
    if (MPI_SUCCESS != ret)
    {
      debug_error("Server[%d]: MPI_Comm_disconnect fails :-(", params->rank) ;
      return -1 ;
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

    debug_info("[CLI-COMM] begin mpiClient_comm_locality\n") ;

    // Locality disable
    if (!params->xpn_locality)
    { 
      debug_info("[CLI-COMM] mpiClient_comm_locality disable\n") ;
      params->locality = 0;
      return 1;
    }

    // Get client host name
    gethostname(cli_name, HOST_NAME_MAX);

    data = MPI_SERVER_GETNODENAME;

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

    //Dirbase
    ret = MPI_Recv(params->dirbase, PATH_MAX, MPI_CHAR, 0, 1, params->server, &status);
    if (MPI_SUCCESS != ret) {
      debug_warning("Server[?]: MPI_Recv fails :-(") ;
      return -1;
    }

    //Semaphore
    /*ret = MPI_Recv(params->sem_name_server, PATH_MAX, MPI_CHAR, 0, 1, params->server, &status);
    if (MPI_SUCCESS != ret) {
      debug_warning("Server[?]: MPI_Recv fails :-(") ;
      return -1;
    }*/

    if (strcmp(cli_name, serv_name) == 0)
    {
      params->locality = 1;
      //params->sem_server = sem_open(params->sem_name_server, 0);
    }
    else
    {
      params->locality = 0;
    }

    debug_info("[CLI-COMM] end mpiClient_comm_locality\n") ;
      
    // Return OK
    return 1;
  }

  ssize_t mpiClient_write_operation ( MPI_Comm fd, char *data, ssize_t size, __attribute__((__unused__)) char *msg_id )
  {
    int ret ;

    debug_info("[CLI-COMM] begin mpiClient_write_operation(...)\n") ;

    // Check params
    if (size == 0) {
        return 0;
    }
    if (size < 0) {
        debug_warning("Server[?]: size < 0") ;
        return -1;
    }

    // TODO
    // msg_id = msg_id ; // TODO: msg_id is used?
    // TODO

    // Send message
    ret = MPI_Send(data, size, MPI_INT, 0, 0, fd) ;
    if (MPI_SUCCESS != ret) {
      debug_warning("Server[?]: MPI_Recv fails :-(") ;
      size = 0 ;
    }

    debug_info("[CLI-COMM] end mpiClient_write_operation(...)\n") ;

    // Return bytes written
    return size;
  }
  
  ssize_t mpiClient_write_data ( MPI_Comm fd, char *data, ssize_t size, char *msg_id )
  {
    int ret ;

    debug_info("[CLI-COMM] begin mpiClient_write_data(...)\n") ;

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
    ret = MPI_Send(data, size, MPI_CHAR, 0, 1, fd) ;
    if (MPI_SUCCESS != ret) {
      debug_warning("Server[?]: MPI_Recv fails :-(") ;
      size = 0 ;
    }

    debug_info("[CLI-COMM] end mpiClient_write_data(...)\n") ;

    // Return bytes written
    return size;
  }

  ssize_t mpiClient_read_data ( MPI_Comm fd, char *data, ssize_t size, char *msg_id )
  {
    int ret ;
    MPI_Status status ;

    debug_info("[CLI-COMM] begin mpiClient_read_data(...)\n") ;

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

    debug_info("[CLI-COMM] end mpiClient_read_data(...)\n") ;

    // Return bytes read
    return size;
  }

