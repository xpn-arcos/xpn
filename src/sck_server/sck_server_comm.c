
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

#include "sck_server/sck_server_comm.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Functions / Funciones ......................................... */

int sck_server_comm_init ( sck_server_param_st * params )
{
  int ret, val;
  struct sockaddr_in server_addr;

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] >> Begin\n", params->rank);

  // Get timestap
  struct timeval t0;
  TIME_MISC_Timer(&t0);

  // Socket init
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] Scoket init\n", params->rank);

  params->global_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (params->global_sock < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: socket fails\n", params->rank);
    return -1;
  }

  // tcp_nodalay
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] TCP nodelay\n", params->rank);

  val = 1;
  ret = setsockopt(params->global_sock, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));
  if (ret < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: setsockopt fails\n", params->rank);
    return -1;
  }

  // sock_reuseaddr
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] Socket reuseaddr\n", params->rank);

  val = 1;
  ret = setsockopt(params->global_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&val, sizeof(int));
  if (ret < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: setsockopt fails\n", params->rank);
    return -1;
  }

  // bind
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] Socket bind\n", params->rank);

  bzero((char * )&server_addr, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port        = htons(atoi(params->port));


  ret = bind(params->global_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (ret < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: bind fails\n", params->rank);
    return -1;
  }

  // listen
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] Socket listen\n", params->rank);

  listen(params->global_sock, 20);


  // Generate DNS file

  // get IP
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] Get host ip\n", params->rank);

  char *ip = ns_get_host_ip();
  if (ip == NULL)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: ns_get_host_ip fails\n", params->rank);
    return -1;
  }

  // Publish hostname
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] Publish host\n", params->rank);

  ret = ns_publish(params->dns_file, "sck_server", params->name, ip, params->port);
  if (ret < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: ns_publish fails\n", params->rank);
    return -1;
  }

  // Print server init information
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
    printf("All XPN SCK servers running\n");
    printf("---------------------------\n");
    printf("\n\n");
  }

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] server %d available at %s\n", params->rank, params->rank, params->port_name);
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] server %d accepting...\n",    params->rank, params->rank);

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] >> End\n", params->rank);

  return 1;
}

int sck_server_comm_destroy ( sck_server_param_st * params )
{
  int ret;

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] >> Begin\n", params->rank);

  // Update DNS file
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] Update DNS file\n", params->rank);

  for (int i = 0; i < params->size; ++i)
  {
    if (params->rank == i)
    {
      // Unpublish port name
      debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] Unpublish host\n", params->rank);

      ret = ns_unpublish(params->dns_file, "sck_server", params->srv_name);
      if (ret < 0)
      {
        printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: ns_unpublish fails\n", params->rank);
        return -1;
      }
    }
  }
  
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] << End\n", params->rank);

  return 1;
}

int sck_server_comm_accept ( sck_server_param_st * params )
{
  int    ret, sc, flag;
  struct sockaddr_in client_addr;
  socklen_t size = sizeof(struct sockaddr_in);

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_accept] >> Begin\n", params->rank);

  // Accept
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_accept] Accept\n", params->rank);

  sc = accept(params->global_sock, (struct sockaddr *)&client_addr, & size);
  if (sc < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_destroy] ERROR: accept fails\n", params->rank);
    return -1;
  }


  //TODO
  debug_info("[SRV_SCK_COMM] desp. accept conection .... %d\n", sc);
  // tcp_nodelay
  flag = 1;
  ret = setsockopt(sc, IPPROTO_TCP, TCP_NODELAY, & flag, sizeof(flag));
  if (ret < 0)
  {
    perror("setsockopt: ");
    return -1;
  }

  //NEW
  int val = 1024 * 1024; //1 MB

  ret = setsockopt(sc, SOL_SOCKET, SO_SNDBUF, (char *)&val, sizeof(int));
  if (ret < 0)
  {
    perror("setsockopt: ");
    return -1;
  }

  val = 1024 * 1024; //1 MB
  ret = setsockopt(sc, SOL_SOCKET, SO_RCVBUF, (char *)&val, sizeof(int));
  if (ret < 0)
  {
    perror("setsockopt: ");
    return -1;
  }
  // TODO: end


  params->client = sc;

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_accept] << End\n", params->rank);

  // Return client int
  return params->client;
}

//TODO
int sck_server_comm_connect ( sck_server_param_st * params, char *server_name, int port_number )
{
  struct hostent * hp;
  struct sockaddr_in server_addr;
  int ret, sd, flag, val;


  DEBUG_BEGIN();

  debug_info("[SRV_SCK_COMM] begin sck_server_comm_connect(...)\n");

  // Socket
  debug_info("[SRV_SCK_COMM]----SERVER = %s NEWSERVER = %s PORT = %d\n", params->srv_name, server_name, port_number);
  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sd < 0) {
    perror("socket:");
    return -1;
  }
  debug_info("[SRV_SCK_COMM]----SERVER = %s NEWSERVER = %s PORT = %d ==> %d\n", params->srv_name, server_name, port_number, sd);

  // sock-options
  flag = 1;
  ret = setsockopt(sd, IPPROTO_TCP, TCP_NODELAY, & flag, sizeof(flag));
  if (ret < 0) {
    perror("setsockopt: ");
    return -1;
  }

  val = params->IOsize; //1 MB
  ret = setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char *)&val, sizeof(int));
  if (ret < 0) {
    perror("setsockopt: ");
    return -1;
  }

  val = params->IOsize; //1 MB
  ret = setsockopt(sd, SOL_SOCKET, SO_RCVBUF, (char *)&val, sizeof(int));
  if (ret < 0) {
    perror("setsockopt: ");
    return -1;
  }

  // server_name to ip address...
  hp = gethostbyname(server_name);
  if (NULL == hp) {
    //sck_server_err(SCK_SERVERERR_MEMORY);
    fprintf(stderr, "sck_server_comm_connect: error gethostbyname (%s,%d)\n", server_name, port_number);
    return -1;
  }

  // Connect...
  debug_info("[SRV_SCK_COMM] server = %s-%d\n", server_name, port_number);

  bzero((char *)&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port   = htons(port_number);
  memcpy( & (server_addr.sin_addr), hp->h_addr, hp->h_length);

  //se establece la conexion
  ret = connect(sd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (ret < 0) {
    //sck_server_err(SCK_SERVERERR_MEMORY);
    fprintf(stderr, "nfi_sck_server_init: error in connect (%s,%d)\n", server_name, port_number);
    perror("nfi_sck_server_init:");
    return -1;
  }

  DEBUG_END();

  return sd;
}

int sck_server_comm_close ( int fd )
{
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_disconnect] >> Begin\n", -1);

  // Close
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_disconnect] close\n", -1);
  close(fd);

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_disconnect] << End\n", -1);

  // Return OK
  return 1;
}

ssize_t sck_server_comm_read_operation(sck_server_param_st * params, int fd, char * data, ssize_t size, int * rank_client_id)
{
  int ret;

  // Check arguments
  if (NULL == params)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_operation] ERROR: NULL arguments\n", -1);
    return -1;
  }

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_operation] >> Begin\n", params->rank);

  if (size == 0) {
    return 0;
  }
  if (size < 0) {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_operation] ERROR: size < 0\n", params->rank);
    return -1;
  }

  // Get message
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_disconnect] Read operation\n", params->rank);

  ret = sck_server_comm_read_data(params, fd, data, size * sizeof(int), *rank_client_id);
  if (ret < 0)
  {
    debug_warning("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_operation] ERROR: sck_server_comm_read_op fails\n", params->rank);
    return ret;
  }

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_operation] << End\n", params->rank);

  // Return int readed
  return ret / sizeof(int);
}

ssize_t sck_server_comm_write_data(sck_server_param_st * params, int fd, char * data, ssize_t size, __attribute__((__unused__)) int rank_client_id) //TODO rank client
{
  int ret;

  // Check params
  if (NULL == params)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_write_data] ERROR: NULL arguments\n", -1);
    return -1;
  }

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_write_data] >> Begin\n", params->rank);

  if (size == 0) {
    return 0;
  }
  if (size < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_write_data] ERROR: size < 0\n", params->rank);
    return -1;
  }

  int cont = 0;
  do
  {
    ret = 0;
    debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_write_data] Write data(%d, %p, %ld)\n", params->rank, fd, data + cont, size - cont);

    ret = write(fd, data + cont, size - cont);
    if (ret < 0)
    {
      printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_write_data] ERROR: write fails\n", params->rank);
      return -1;
    }

    debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_write_data] Write data(%d, %p, %ld)=%d\n", params->rank, fd, data + cont, size - cont, ret);
    
    cont += ret;

  } while ((ret > 0) && (cont != size));

  if (ret < 0) {
    debug_warning("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_write_data] ERROR: write fails\n", params->rank);
    return ret;
  }

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_write_data] << End\n", params->rank);

  // Return bytes written
  return cont;
}

ssize_t sck_server_comm_read_data(sck_server_param_st * params, int fd, char * data, ssize_t size, __attribute__((__unused__)) int rank_client_id) //TODO rank client
{
  int ret;

  // Check params
  if (NULL == params) {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_data] ERROR: NULL arguments\n", -1);
    return -1;
  }
  
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_data] >> Begin\n", params->rank);

  if (size == 0) {
    return 0;
  }
  if (size < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_data] ERROR: size < 0\n", params->rank);
    return -1;
  }

  int cont = 0;
  do
  {
    ret = 0;
    debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_data] Read data(%d, %p, %ld)\n", params->rank, fd, data + cont, size - cont);

    ret = read(fd, data + cont, size - cont);
    if (ret < 0) {
      printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_data] ERROR: read fails\n", params->rank);
      return -1;
    }

    debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_data] Read data(%d, %p, %ld)=%d\n", params->rank, fd, data + cont, size - cont, ret);
    
    cont += ret;

  } while ((ret > 0) && (cont != size));

  if (ret < 0)
  {
    debug_warning("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_data] ERROR: write fails\n", params->rank);
    return ret;
  }

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_read_data] << End\n", params->rank);

  // Return bytes read
  return cont;
}


/* ................................................................... */
