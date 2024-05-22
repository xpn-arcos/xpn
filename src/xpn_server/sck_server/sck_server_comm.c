
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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
#include "socket.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Functions / Funciones ......................................... */

int sck_server_comm_init ( int *new_socket, char *port_name )
{
  int ret, val;
  struct sockaddr_in server_addr;

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] >> Begin\n", 0);

  // Get timestap
  struct timeval t0;
  TIME_MISC_Timer(&t0);

  // Socket init
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] Scoket init\n", 0);

  *new_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (*new_socket < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: socket fails\n", 0);
    return -1;
  }

  // tcp_nodalay
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] TCP nodelay\n", 0);

  val = 1;
  ret = setsockopt(*new_socket, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));
  if (ret < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: setsockopt fails\n", 0);
    return -1;
  }

  // sock_reuseaddr
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] Socket reuseaddr\n", 0);

  val = 1;
  ret = setsockopt(*new_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&val, sizeof(int));
  if (ret < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: setsockopt fails\n", 0);
    return -1;
  }

  // bind
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] Socket bind\n", 0);

  bzero((char * )&server_addr, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port        = htons(0);


  ret = bind(*new_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (ret < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: bind fails\n", 0);
    return -1;
  }

  // listen
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] Socket listen\n", 0);

  ret = listen(*new_socket, 20);
  if (ret < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: listen fails\n", 0);
    return -1;
  }
  socklen_t len = sizeof(server_addr);
  getsockname(*new_socket, (struct sockaddr *)&server_addr, &len);
  sprintf(port_name, "%d", ntohs(server_addr.sin_port));

  // Print server init information
  struct timeval t1;
  struct timeval tf;
  float time;
  TIME_MISC_Timer(&t1);
  TIME_MISC_DiffTime(&t0, &t1, &tf);
  time = TIME_MISC_TimevaltoFloat(&tf);

  printf("\n\n");
  printf("Time to inizialize all servers: %f s\n", time);
  printf("\n");
  printf("---------------------------\n");
  printf("All XPN SCK servers running\n");
  printf("---------------------------\n");
  printf("\n\n");

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] server %d available at %s\n", 0, 0, port_name);
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] server %d accepting...\n",    0, 0);

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_init] >> End\n", 0);

  return 1;
}

int sck_server_comm_accept ( int socket, int **new_socket )
{
  int    ret, sc, flag;
  struct sockaddr_in client_addr;
  socklen_t size = sizeof(struct sockaddr_in);

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_accept] >> Begin\n", 0);

  // Accept
  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_accept] Accept\n", 0);

  sc = accept(socket, (struct sockaddr *)&client_addr, &size);
  if (sc < 0)
  {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_destroy] ERROR: accept fails\n", 0);
    return -1;
  }

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_destroy] desp. accept conection .... %d\n", 0, sc);
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

  debug_info("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_accept] << End\n", 0);

  *new_socket = malloc(sizeof(int));
  if (*new_socket == NULL) {
    printf("[Server=%d] [SCK_SERVER_COMM] [sck_server_comm_accept] ERROR: Memory allocation\n", 0);
    return -1;
  }

  **new_socket = sc;
  return 0;
}


int sck_server_comm_disconnect ( int *socket )
{
  socket_close(*socket);
  free(socket);
  return 0;
}


/* ................................................................... */
