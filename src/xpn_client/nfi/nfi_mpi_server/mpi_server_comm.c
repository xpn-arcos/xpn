
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Functions / Funciones ......................................... */

int mpi_client_comm_init ( mpi_client_param_st *params )
{
  int ret, provided, claimed;
  int flag = 0;

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_init] >> Begin\n");

  // MPI_Init
  MPI_Initialized(&flag);

  if (!flag)
  {
    // TODO: server->argc, server->argv from upper layers?

    // Threads disable
    if (!params->xpn_thread)
    { 
      debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_init] MPI Init without threads\n");

      ret = MPI_Init(NULL, NULL);
      if (MPI_SUCCESS != ret)
      {
        printf("[MPI_CLIENT_COMM] [mpi_client_comm_init] ERROR: MPI_Init fails\n");
        return -1;
      }
    }
    // Threads enable
    else
    {
      debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_init] MPI Init with threads\n");

      ret = MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided);
      if (MPI_SUCCESS != ret)
      {
        printf("[MPI_CLIENT_COMM] [mpi_client_comm_init] ERROR: MPI_Init_thread fails\n");
        return -1;
      }

      MPI_Query_thread(&claimed);
      if (claimed != MPI_THREAD_MULTIPLE) {
        printf("[MPI_CLIENT_COMM] [mpi_client_comm_init] INFO: your MPI implementation seem not supporting thereads\n");
      }
    }
  }

  // params->rank = comm_rank()
  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_init] Get MPI rank\n");

  ret = MPI_Comm_rank(MPI_COMM_WORLD, &(params->rank));
  if (MPI_SUCCESS != ret)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_comm_init] ERROR: MPI_Comm_rank fails\n");
    return -1;
  }

  // params->size = comm_size()
  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_init] Get MPI size\n");

  ret = MPI_Comm_size(MPI_COMM_WORLD, &(params->size));
  if (MPI_SUCCESS != ret)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_comm_init] ERROR: MPI_Comm_size fails\n");
    return -1;
  }

  // set is_mpi_server as the used protocol
  setenv("XPN_IS_MPI_SERVER", "1", 1);

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_init] server %d available at %s\n", params->rank, params->port_name);
  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_init] >> End\n");

  // Return OK
  return 1;
}

int mpi_client_comm_destroy ( mpi_client_param_st *params )
{
  int ret;

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_destroy] >> Begin\n");

  if (params->size < 0)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_comm_destroy] ERROR: number of process is negative\n");
    return -1;
  }

  MPI_Barrier(MPI_COMM_WORLD);

  // Finalize
  int flag = 0;
  MPI_Initialized(&flag);

  if (!flag)
  {
    debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_destroy] MPI Finalize\n");

    ret = PMPI_Finalize();
    if (MPI_SUCCESS != ret)
    {
      printf("[MPI_CLIENT_COMM] [mpi_client_comm_destroy] ERROR: PMPI_Finalize fails\n");
      return -1;
    }
  }

  //Indicates mpi_server are the used protocolo 
  unsetenv("XPN_IS_MPI_SERVER");

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_destroy] << End\n");

  // Return OK
  return 1;
}

int mpi_socket_send(char * srv_name, int code)
{
  int client_fd;
  struct sockaddr_in serv_addr;
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  struct hostent * hp;
  hp = gethostbyname(srv_name);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(MPI_SOCKET_PORT);
  memcpy( & (serv_addr.sin_addr), hp->h_addr, hp->h_length);
  int status;
  if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
      < 0) {
      printf("\nConnection Failed \n");
      return -1;
  }
  send(client_fd, &code, sizeof(int), 0);
  close(client_fd);
  return 0;
}

int mpi_client_comm_connect ( mpi_client_param_st *params )
{
  int ret;

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_connect] >> Begin\n");

  int version_len;
  char version[MPI_MAX_LIBRARY_VERSION_STRING];
  MPI_Get_library_version(version, &version_len);

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_connect] MPI Version: %s\n", version);

  if(strncasecmp(version,"Open MPI", strlen("Open MPI")) != 0)
  {
    // Lookup port name
    int lookup_retries = 0;
    char aux_srv_ip[1024];

    do
    {
      // Lookup port name on nameserver
      debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_connect] Lookup server %s\n", params->srv_name);

      ret = ns_lookup("mpi_server", params->srv_name, aux_srv_ip, params->port_name);
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
      printf("[MPI_CLIENT_COMM] [mpi_client_comm_connect] ERROR: DNS Lookup %s Port %s\n", params->srv_name, params->port_name);
      return -1;
    }
  }
  else
  {
    // Lookup port name on nameserver
    debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_connect] Lookup server %s\n", params->port_name);

    ret = MPI_Lookup_name(params->srv_name, MPI_INFO_NULL, params->port_name);
    if (MPI_SUCCESS != ret)
    {
      printf("[MPI_CLIENT_COMM] [mpi_client_comm_connect] ERROR: DNS Lookup %s Port %s\n", params->port_name, params->port_name);
      return -1;
    }
  }

  // Send connect intention
  if (params->rank == 0)
    mpi_socket_send(params->srv_name, MPI_SOCKET_ACCEPT);

  // Connect...
  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_connect] Connect port %s\n", params->port_name);

  int connect_retries = 0;
  do
  {
    ret = MPI_Comm_connect(params->port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &(params->server));
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
  
  if (MPI_SUCCESS != ret)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_comm_connect] ERROR: MPI_Comm_connect fails\n");
    return -1;
  }

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_connect] << End\n");

  // Return OK
  return 1;
}

int mpi_client_comm_disconnect ( mpi_client_param_st *params )
{
  int ret;

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_disconnect] >> Begin\n");

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &(rank));
  if (rank == 0)
  {
    debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_disconnect] Send disconnect message\n");
    mpi_client_write_operation(params->server, MPI_SERVER_DISCONNECT);
  }
  
  MPI_Barrier(MPI_COMM_WORLD);
  
  // Disconnect
  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_disconnect] Disconnect\n");

  ret = MPI_Comm_disconnect(&(params->server));
  if (MPI_SUCCESS != ret)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_comm_disconnect] ERROR: MPI_Comm_disconnect fails\n");
    return -1;
  }

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_disconnect] << End\n");

  // Return OK
  return 1;
}

int mpi_client_comm_locality ( mpi_client_param_st *params )
{
  int ret;
  char cli_name  [HOST_NAME_MAX];
  char serv_name [HOST_NAME_MAX];
  //MPI_Status status;

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_locality] >> Begin\n");

  // Locality disable
  if (!params->xpn_locality)
  { 
    debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_locality] Locality disable\n");

    params->locality = 0;
    return 1;
  }

  // Get client host name
  gethostname(cli_name, HOST_NAME_MAX);

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_locality] Send gethostname operation\n");

  ret = mpi_client_write_operation ( params->server, MPI_SERVER_GETNODENAME );
  if (ret < 0)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_comm_locality] ERROR: mpi_client_write_operation fails\n");
    return -1;
  }

  ret = mpi_client_read_data ( params->server, serv_name, HOST_NAME_MAX );
  if (ret != HOST_NAME_MAX)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_comm_locality] ERROR: mpi_client_write_data fails\n");
    return -1;
  }

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_locality] Receive host name: %s\n", serv_name);

  // Dirbase
  ret = mpi_client_read_data ( params->server, params->dirbase, PATH_MAX );
  if (ret != PATH_MAX)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_comm_locality] ERROR: MPI_Recv fails\n");
    return -1;
  }

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_locality] Receive dirbase: %s\n", params->dirbase);

  // Semaphore
  /*
  printf("[MPI_CLIENT_COMM] [mpi_client_comm_locality] Receive semaphore\n");

  ret = MPI_Recv(params->sem_name_server, PATH_MAX, MPI_CHAR, 0, 1, params->server, &status);
  if (MPI_SUCCESS != ret) {
    printf("[MPI_CLIENT_COMM] [mpi_client_comm_locality] ERROR: MPI_Recv fails\n");
    return -1;
  }
  */

  params->locality = 0;
  if (strcmp(cli_name, serv_name) == 0)
  {
    params->locality = 1;
    //params->sem_server = sem_open(params->sem_name_server, 0);
  }

  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_locality] Locality: %d\n", params->locality);
  debug_info("[MPI_CLIENT_COMM] [mpi_client_comm_locality] << End\n");
    
  // Return OK
  return 1;
}

ssize_t mpi_client_write_operation ( MPI_Comm fd, int op )
{
  int ret;
  int msg[2];

  debug_info("[MPI_CLIENT_COMM] [mpi_client_write_operation] >> Begin\n");

  //Message generation
  msg[0] = (int) (pthread_self() % 32450) + 1;
  msg[1] = (int) op;

  // Send message
  debug_info("[MPI_CLIENT_COMM] [mpi_client_write_operation] Write operation\n");

  ret = MPI_Send(msg, 2, MPI_INT, 0, 0, fd);
  if (MPI_SUCCESS != ret)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_write_operation] ERROR: MPI_Send < 0\n");
    return -1;
  }

  debug_info("[MPI_CLIENT_COMM] [mpi_client_write_operation] << End\n");

  // Return OK
  return 0;
}

ssize_t mpi_client_write_data ( MPI_Comm fd, char *data, ssize_t size )
{
  int ret;

  debug_info("[MPI_CLIENT_COMM] [mpi_client_write_data] >> Begin\n");

  // Check params
  if (size == 0) {
    return 0;
  }
  if (size < 0)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_write_data] ERROR: size < 0\n");
    return -1;
  }

  int tag = (int) (pthread_self() % 32450) + 1;

  // Send message
  debug_info("[MPI_CLIENT_COMM] [mpi_client_write_data] Write data\n");

  ret = MPI_Send(data, size, MPI_CHAR, 0, tag, fd);
  if (MPI_SUCCESS != ret)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_write_data] ERROR: MPI_Send fails\n");
    size = 0;
  }

  debug_info("[MPI_CLIENT_COMM] [mpi_client_write_data] << End\n");

  // Return bytes written
  return size;
}

ssize_t mpi_client_read_data ( MPI_Comm fd, char *data, ssize_t size )
{
  int ret;
  MPI_Status status;

  debug_info("[MPI_CLIENT_COMM] [mpi_client_read_data] >> Begin\n");

  // Check params
  if (size == 0) {
    return  0;
  }
  if (size < 0)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_read_data] ERROR: size < 0\n");
    return  -1;
  }

  int tag = (int) (pthread_self() % 32450) + 1;

  // Get message
  debug_info("[MPI_CLIENT_COMM] [mpi_client_read_data] Read data\n");

  ret = MPI_Recv(data, size, MPI_CHAR, 0, tag, fd, &status);
  if (MPI_SUCCESS != ret)
  {
    printf("[MPI_CLIENT_COMM] [mpi_client_read_data] ERROR: MPI_Recv fails\n");
    size = 0;
  }

  debug_info("[MPI_CLIENT_COMM] [mpi_client_read_data] << End\n");

  // Return bytes read
  return size;
}


/* ................................................................... */
