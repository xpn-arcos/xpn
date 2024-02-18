
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

// init, destroy
int mpi_server_comm_init ( mpi_server_param_st *params )
{
  int  ret, provided, claimed;
  char serv_name [HOST_NAME_MAX];

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] >> Begin\n", params->rank);

  //Get timestap
  struct timeval t0;
  TIME_MISC_Timer(&t0);

  // MPI init
  // Threads disable
  if (!params->thread_mode)
  {
    debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] MPI Init without threads\n", params->rank);

    ret = MPI_Init(&(params->argc), &(params->argv));
    if (MPI_SUCCESS != ret)
    {
      printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Init fails\n", params->rank);
      return -1;
    }
  }
  // Threads enable
  else
  {
    debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] MPI Init with threads\n", params->rank);

    ret = MPI_Init_thread(&(params->argc), &(params->argv), MPI_THREAD_MULTIPLE, &provided);
    if (MPI_SUCCESS != ret)
    {
      printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Init_thread fails\n", params->rank);
      return -1;
    }

    MPI_Query_thread(&claimed);
    if (claimed == MPI_THREAD_SINGLE) {
      printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] INFO: your MPI implementation seem not supporting thereads\n", params->rank);
    }
  }

  // params->rank = comm_rank()
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] Get MPI rank\n", params->rank);

  ret = MPI_Comm_rank(MPI_COMM_WORLD, &(params->rank));
  if (MPI_SUCCESS != ret)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Comm_rank fails\n", params->rank);
    return -1;
  }

  // params->size = comm_size()
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] Get MPI size\n", params->rank);

  ret = MPI_Comm_size(MPI_COMM_WORLD, &(params->size));
  if (MPI_SUCCESS != ret)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Comm_size fails\n", params->rank);
    return -1;
  }

  // Open server port...
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] Open port\n", params->rank);

  ret = MPI_Open_port(MPI_INFO_NULL, params->port_name);
  if (MPI_SUCCESS != ret) {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Open_port fails\n", params->rank);
    return -1;
  }

  // Generate DNS file
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] Generate DNS file\n", params->rank);

  int version_len;
  char version[MPI_MAX_LIBRARY_VERSION_STRING];
  MPI_Get_library_version(version, &version_len);

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] MPI Version: %s\n", params->rank, version);

  if(strncasecmp(version,"Open MPI", strlen("Open MPI")) != 0)
  {
    for (int j=0; j < params->size; j++)
    {
      if (j == params->rank)
      {
        char *ip;

        // get hostname and IP
        debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] Get host name\n", params->rank);

        ns_get_hostname(params->srv_name);

        debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] Get host ip\n", params->rank);

        ip = ns_get_host_ip();
        if (ip == NULL)
        {
          printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: ns_get_host_ip fails\n", params->rank);
          return -1;
        }

        // Publish hostname
        debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] Publish host\n", params->rank);

        ret = ns_publish(params->dns_file, "mpi_server", params->srv_name, ip, params->port_name);
        if (ret < 0)
        {
          printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: ns_publish fails\n", params->rank);
          return -1;
        }
      }

      MPI_Barrier(MPI_COMM_WORLD);
    }
  }
  else
  {
    // Publish port name
    MPI_Info info;
    MPI_Info_create(&info);
    MPI_Info_set(info, "ompi_global_scope", "true");

    // get hostname
    debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] Get host name\n", params->rank);

    gethostname(serv_name, HOST_NAME_MAX);
    sprintf(params->srv_name, "%s", serv_name);

    // Publish hostname
    debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] Publish host\n", params->rank);

    ret = MPI_Publish_name(params->srv_name, info, params->port_name);
    if (MPI_SUCCESS != ret) {
      printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] ERROR: MPI_Publish_name fails\n", params->rank);
      return -1;
    }
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
    printf("\n\n");
    printf("Time to inizialize all servers: %f s\n", time);
    printf("\n");
    printf("---------------------------\n");
    printf("All XPN MPI servers running\n");
    printf("---------------------------\n");
    printf("\n\n");
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] server %d available at %s\n", params->rank, params->rank, params->port_name);
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] server %d accepting...\n",    params->rank, params->rank);

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_init] >> End\n", params->rank);

  return 1;
}

int mpi_server_comm_destroy ( mpi_server_param_st *params )
{
  int ret;

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] >> Begin\n", params->rank);

  // Close port
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] Close port\n", params->rank);

  MPI_Close_port(params->port_name);

  // Update DNS file
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] Update DNS file\n", params->rank);

  for (int i = 0; i < params->size; ++i)
  {
    if (params->rank == i)
    {
      int version_len;
      char version[MPI_MAX_LIBRARY_VERSION_STRING];
      MPI_Get_library_version(version, &version_len);

      debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] MPI Version: %s\n", params->rank, version);

      if (strncasecmp(version, "Open MPI", strlen("Open MPI")) != 0)
      {
        char hostname[1024];

        // get hostname and IP
        debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] Get host name\n", params->rank);

        ns_get_hostname(hostname);

        // Unpublish port name
        debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] Unpublish host\n", params->rank);

        ret = ns_unpublish(params->dns_file, "mpi_server", hostname);
        if (ret < 0)
        {
          printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] ERROR: ns_unpublish fails\n", params->rank);
          return -1;
        }
      }
      else
      {
        // Unpublish port name
        debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] Unpublish host\n", params->rank);

        ret = MPI_Unpublish_name(params->srv_name, MPI_INFO_NULL, params->port_name);
        if (MPI_SUCCESS != ret)
        {
          printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] ERROR: MPI_Unpublish_name fails\n", params->rank);
          return -1;
        }
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);
  }

  // Finalize
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] MPI Finalize\n", params->rank);

  ret = MPI_Finalize();
  if (MPI_SUCCESS != ret)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] ERROR: MPI_Finalize fails\n", params->rank);
    return -1;
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] << End\n", params->rank);

  // Return OK
  return 1;
}

int mpi_server_socket_send(char * srv_name, int code)
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

int mpi_server_socket_accept ( int socket )
{
  struct sockaddr_in client_addr;
  socklen_t size = sizeof(struct sockaddr_in);
  // printf("socket: %d\n",socket);
  int new_socket = accept(socket, (struct sockaddr*)&client_addr, &size);
  if (new_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
  int buffer;
  read(new_socket, &buffer, sizeof(int));
  // printf("buffer: %d\n",buffer);
  close(new_socket);
  return buffer;
}

// accept, disconnect
MPI_Comm mpi_server_comm_accept ( mpi_server_param_st *params )
{
  int ret;
  MPI_Comm sd = MPI_COMM_NULL;

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_accept] >> Begin\n", params->rank);

  // Accept
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_accept] Accept\n", params->rank);

  ret = MPI_Comm_accept(params->port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &(sd));
  if (MPI_SUCCESS != ret)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_destroy] ERROR: MPI_Comm_accept fails\n", params->rank);
    return MPI_COMM_NULL;
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_accept] << End\n", params->rank);

  // Return client MPI_Comm
  return sd;
}

int mpi_server_comm_disconnect ( MPI_Comm fd )
{
  int ret;

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_disconnect] >> Begin\n", -1);

  if (fd == MPI_COMM_NULL)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_disconnect] ERROR: The MPI_Comm is NULL\n", -1);
    return 1;
  }

  // Disconnect
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_disconnect] Disconnect\n", -1);

  ret = MPI_Comm_disconnect(&fd);
  if (MPI_SUCCESS != ret)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_disconnect] ERROR: MPI_Comm_disconnect fails\n", -1);
    return -1;
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_disconnect] << End\n", -1);

  // Return OK
  return 1;
}

// write_operation for shutdown, read_operation
ssize_t mpi_server_comm_write_operation_finalize ( MPI_Comm fd, int op )
{
  int ret;
  int msg[2];

  debug_info("[MPI_SERVER_COMM] [mpi_server_comm_write_operation_finalize] >> Begin\n");

  //Message generation
  msg[0] = (int) (pthread_self() % 32499) + 1;
  msg[1] = (int) op;

  // Send message
  debug_info("[MPI_SERVER_COMM] [mpi_server_comm_write_operation_finalize] Write operation\n");

  ret = MPI_Send(msg, 2, MPI_INT, 0, 0, fd);
  if (MPI_SUCCESS != ret)
  {
    printf("[MPI_SERVER_COMM] [mpi_server_comm_write_operation_finalize] ERROR: MPI_Send < 0\n");
    return -1;
  }

  debug_info("[MPI_SERVER_COMM] [mpi_server_comm_write_operation_finalize] << End\n");

  // Return OK
  return 0;
}

ssize_t mpi_server_comm_read_operation ( mpi_server_param_st *params, MPI_Comm fd, int *op, int *rank_client_id, int *tag_client_id )
{
  int ret;
  MPI_Status status;
  int msg[2];

  // Check params
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] ERROR: NULL arguments\n", -1);
    return -1;
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] >> Begin\n", params->rank);

  // Get message
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_disconnect] Read operation\n", params->rank);

  ret = MPI_Recv(msg, 2, MPI_INT, MPI_ANY_SOURCE, 0, fd, &status);
  if (MPI_SUCCESS != ret) {
    debug_warning("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] ERROR: MPI_Recv fails\n", params->rank);
  }

  *rank_client_id = status.MPI_SOURCE;
  *tag_client_id  = msg[0];
  *op             = msg[1];

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] MPI_Recv (MPI SOURCE %d, MPI_TAG %d, OP %d, MPI_ERROR %d)\n", params->rank, *rank_client_id, *rank_client_id, *op, status.MPI_ERROR);
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_operation] << End\n", params->rank);

  // Return OK
  return 0;
}


ssize_t mpi_server_comm_write_data ( mpi_server_param_st *params, MPI_Comm fd, char *data, ssize_t size, int rank_client_id, int tag_client_id )
{
  int ret;

  // Check params
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_write_data] ERROR: NULL arguments\n", -1);
    return -1;
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_write_data] >> Begin\n", params->rank);

  if (size == 0) {
      return 0;
  }
  if (size < 0)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_write_data] ERROR: size < 0\n", params->rank);
    return -1;
  }

  // Send message
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_write_data] Write data\n", params->rank);

  ret = MPI_Send(data, size, MPI_CHAR, rank_client_id, tag_client_id, fd);
  if (MPI_SUCCESS != ret) {
    debug_warning("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_write_data] ERROR: MPI_Send fails\n", params->rank);
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_write_data] << End\n", params->rank);

  // Return bytes written
  return size;
}

ssize_t mpi_server_comm_read_data ( mpi_server_param_st *params, MPI_Comm fd, char *data, ssize_t size, int rank_client_id, int tag_client_id )
{
  int ret;
  MPI_Status status;

  // Check params
  if (NULL == params)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] ERROR: NULL arguments\n", -1);
    return -1;
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] >> Begin\n", params->rank);

  if (size == 0) {
    return  0;
  }
  if (size < 0)
  {
    printf("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] ERROR: size < 0\n", params->rank);
    return  -1;
  }

  // Get message
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] Read data\n", params->rank);

  ret = MPI_Recv(data, size, MPI_CHAR, rank_client_id, tag_client_id, fd, &status);
  if (MPI_SUCCESS != ret) {
    debug_warning("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] ERROR: MPI_Recv fails\n", params->rank);
  }

  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] MPI_Recv (MPI SOURCE %d, MPI_TAG %d, MPI_ERROR %d)\n", params->rank, status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);
  debug_info("[Server=%d] [MPI_SERVER_COMM] [mpi_server_comm_read_data] << End\n", params->rank);

  // Return bytes read
  return size;
}


/* ................................................................... */

