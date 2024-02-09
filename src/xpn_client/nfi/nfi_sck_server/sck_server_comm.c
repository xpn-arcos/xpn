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

#include "sck_server_comm.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Functions / Funciones ......................................... */

int sck_client_comm_init ( __attribute__((__unused__)) sck_client_param_st * params ) //TODO
{
  // int ret;

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_init] >> Begin\n");

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_init] >> End\n");

  // Return OK
  return 0;
}

int sck_client_comm_destroy ( __attribute__((__unused__)) sck_client_param_st * params ) //TODO
{
  // int ret;

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_destroy] >> Begin\n");

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_destroy] >> End\n");

  // Return OK
  return 0;
}

int sck_client_comm_connect ( sck_client_param_st * params )
{
  struct hostent * hp;
  struct sockaddr_in server_addr;
  int ret, sd, flag, val;
  int lookup_retries;

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_connect] >> Begin\n");

  // Lookup port name
  lookup_retries = 0;
  do
  {
    // Lookup port name on nameserver
    debug_info("[SCK_CLIENT_COMM] [sck_client_comm_connect] Lookup server %s\n", params->srv_name);

    ret = ns_lookup("sck_server", params->srv_name, params->server_name, params->port_number);
    if (ret < 0)
    {
      if (lookup_retries == 0)
      {
        char cli_name[HOST_NAME_MAX];
        gethostname(cli_name, HOST_NAME_MAX);
        printf("----------------------------------------------------------------\n");
        printf("XPN Client %s : Waiting for servers being up and running...\n", cli_name);
        printf("----------------------------------------------------------------\n\n");
      }
      lookup_retries++;
      sleep(2);
    }
  } while ((ret < 0) && (lookup_retries < 150));

  if (ret < 0) {
    printf("[SCK_CLIENT_COMM] [sck_client_comm_connect] ERROR: DNS Lookup %s Port %s\n", params->srv_name, params->port_number);
    return -1;
  }


  // TODO: init?
  debug_info("[SCK_CLIENT_COMM] ----SERVER = %s NEWSERVER = %s PORT = %s\n", params->srv_name, params->server_name, params->port_number);

  // Socket...
  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sd < 0) {
    perror("socket: ");
    return -1;
  }
  debug_info("[SCK_CLIENT_COMM] ----SERVER = %s NEWSERVER = %s PORT = %s ==> %d\n", params->srv_name, params->server_name, params->port_number, sd);

  // Set sockopt
  flag = 1;
  ret = setsockopt(sd, IPPROTO_TCP, TCP_NODELAY, & flag, sizeof(flag));
  if (ret < 0) {
    perror("setsockopt: ");
    return -1;
  }

  val = 1024 * 1024; //1 MB
  ret = setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char * ) & val, sizeof(int));
  if (ret < 0) {
    perror("setsockopt: ");
    return -1;
  }

  val = 1024 * 1024; //1 MB
  ret = setsockopt(sd, SOL_SOCKET, SO_RCVBUF, (char * ) & val, sizeof(int));
  if (ret < 0) {
    perror("setsockopt: ");
    return -1;
  }

  // gethost by name
  hp = gethostbyname(params->server_name);
  if (hp == NULL)
  {
    //sck_server_err(SCK_SERVER_ERR_MEMORY);
    fprintf(stderr, "nfi_sck_server_init: error gethostbyname %s (%s,%s)\n",
    params->srv_name, params->server_name, params->port_number);
    return -1;
  }
  // END TODO


  // Connect...
  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_connect] Connect port %s\n", params->port_number);

  bzero((char * ) & server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port   = htons(atoi(params->port_number));
  memcpy( & (server_addr.sin_addr), hp->h_addr, hp->h_length);

  int connect_retries = 0;
  do
  {
    ret = connect(sd, (struct sockaddr * ) & server_addr, sizeof(server_addr));
    if (ret < 0)
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
  } while(ret < 0 && connect_retries < 150);

  if (ret < 0)
  {
    printf("[SCK_CLIENT_COMM] [sck_client_comm_connect] ERROR: connect fails\n");
    return -1;
  }

  params->server = sd;

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_connect] << End\n");

  return ret;
}

int sck_client_comm_disconnect ( __attribute__((__unused__)) sck_client_param_st * params )
{
  // int ret;

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_disconnect] >> Begin\n");

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_disconnect] >> End\n");

  // Return OK
  return 0;
}

int sck_client_comm_locality ( sck_client_param_st * params )
{
  int ret;
  int data;
  char cli_name[HOST_NAME_MAX];
  char serv_name[HOST_NAME_MAX];

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_locality] >> Begin\n");

  // Locality disable
  if (!params->xpn_locality)
  {
    debug_info("[SCK_CLIENT_COMM] [sck_client_comm_locality] Locality disable\n");

    params->locality = 0;
    return 1;
  }

  // Get client host name
  gethostname(cli_name, HOST_NAME_MAX);

  data = SCK_SERVER_GETNODENAME;

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_locality] Send gethostname operation\n");

  ret = sck_client_write_data(params->server, (char *)&data, 1 * sizeof(int), "<unused msg_id>");
  if (ret < 0)
  {
    printf("[SCK_CLIENT_COMM] [mpi_client_comm_locality] ERROR: sck_client_write_data fails\n");
    return -1;
  }

  ret = sck_client_read_data( params->server, serv_name, HOST_NAME_MAX * sizeof(char), "<unused msg_id>");
  if (ret < 0)
  {
    printf("[SCK_CLIENT_COMM] [mpi_client_comm_locality] ERROR: sck_client_read_data fails\n");
    return -1;
  }

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_locality] Receive host name: %s\n", serv_name);

  //Dirbase
  ret = sck_client_read_data( params->server, params->dirbase, PATH_MAX * sizeof(char), "<unused msg_id>");
  if (ret < 0)
  {
    printf("[SCK_CLIENT_COMM] [mpi_client_comm_locality] ERROR: MPI_Recv fails\n");
    return -1;
  }

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_locality] Receive dirbase: %s\n", params->dirbase);

  //Semaphore
  /*
  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_locality] Receive semaphore\n");

  ret = sck_client_read_data( params->server, params->sem_name_server, PATH_MAX * sizeof(char), "<unused msg_id>");
  if (ret < 0)
  {
    printf("[SCK_CLIENT_COMM] [sck_client_comm_disconnect] ERROR: sck_client_read_data fails\n");
    return -1;
  }
  */

  params->locality = 0;
  if (strcmp(cli_name, serv_name) == 0)
  {
    params->locality = 1;
    //params->sem_server = sem_open(params->sem_name_server, 0);
  }

  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_locality] Locality: %d\n", params->locality);
  debug_info("[SCK_CLIENT_COMM] [sck_client_comm_locality] << End\n");

  // Return OK
  return 1;
}

ssize_t sck_client_write_operation ( int fd, char * data, ssize_t size, __attribute__((__unused__)) char * msg_id )
{
  int ret;

  debug_info("[SCK_CLIENT_COMM] [sck_client_write_operation] >> Begin\n");

  // Check params
  if (size == 0) {
    return 0;
  }
  if (size < 0)
  {
    printf("[SCK_CLIENT_COMM] [sck_client_write_operation] ERROR: size < 0\n");
    return -1;
  }

  // Send message
  debug_info("[SCK_CLIENT_COMM] [sck_client_write_operation] Write operation\n");

  ret = sck_client_write_data(fd, data, size * sizeof(int), msg_id);
  if (ret < 0)
  {
    printf("[SCK_CLIENT_COMM] [sck_client_write_operation] ERROR: sck_client_write_data fails\n");
    return -1;
  }

  debug_info("[SCK_CLIENT_COMM] [sck_client_write_operation] << End\n");

  // Return integers written
  return ret / sizeof(int);
}

ssize_t sck_client_write_data ( int fd, char * data, ssize_t size, __attribute__((__unused__)) char * msg_id )
{
  int ret;

  debug_info("[SCK_CLIENT_COMM] [sck_client_write_data] >> Begin\n");

  // Check params
  if (size == 0) {
    return 0;
  }
  if (size < 0) {
    printf("[SCK_CLIENT_COMM] [sck_client_write_data] ERROR: size < 0\n");
    return -1;
  }

  // Send message
  debug_info("[SCK_CLIENT_COMM] [sck_client_write_data] Write data\n");

  int cont = 0;
  do
  {
    ret = 0;
    debug_info("[SCK_CLIENT_COMM] [sck_client_write_data] Write data(%d, %p, %ld)\n", fd, data + cont, size - cont);

    ret = dlsym_write(fd, data + cont, size - cont);
    if (ret < 0)
    {
      printf("[SCK_CLIENT_COMM] [sck_client_write_data] ERROR: dlsym_write fails\n");
      return ret;
    }

    debug_info("[SCK_CLIENT_COMM] [sck_client_write_data] Write data(%d, %p, %ld)=%d\n", fd, data + cont, size - cont, ret);

    cont += ret;

  } while ((ret > 0) && (cont != size));

  if (ret < 0)
  {
    printf("[SCK_CLIENT_COMM] [sck_client_write_data] ERROR: dlsym_write fails\n");
    return ret;
  }

  debug_info("[SCK_CLIENT_COMM] [sck_client_write_data] << End\n");

  // Return bytes written
  return cont;
}

ssize_t sck_client_read_data ( int fd, char * data, ssize_t size, __attribute__((__unused__)) char * msg_id )
{
  int ret;

  debug_info("[SCK_CLIENT_COMM] [sck_client_read_data] >> Begin\n");

  // Check params
  if (size == 0) {
    return 0;
  }
  if (size < 0)
  {
    printf("[SCK_CLIENT_COMM] [sck_client_read_data] ERROR: size < 0\n");
    return -1;
  }

  int cont = 0;
  do
  {
    ret = 0;
    debug_info("[SCK_CLIENT_COMM] [sck_client_read_data] Read data(%d, %p, %ld)\n", fd, data + cont, size - cont);

    ret = dlsym_read(fd, data + cont, size - cont);
    if (ret < 0)
    {
      printf("[SCK_CLIENT_COMM] [sck_client_read_data] ERROR: read fails\n");
      return ret;
    }

    debug_info("[SCK_CLIENT_COMM] [sck_client_read_data] Read data(%d, %p, %ld)=%d\n", fd, data + cont, size - cont, ret);

    cont += ret;

  } while ((ret > 0) && (cont != size));

  if (ret < 0) {
    debug_warning("[SCK_CLIENT_COMM] [sck_client_read_data] ERROR: write fails\n");
    return ret;
  }

  debug_info("[SCK_CLIENT_COMM] [sck_client_read_data] << End\n");

  // Return bytes read
  return cont;
}


/* ................................................................... */
