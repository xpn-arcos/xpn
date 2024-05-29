
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Dario Muñoz Muñoz
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

#include "base/socket.h"
#include "filesystem.h"


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Functions / Funciones ......................................... */

int socket_get_port()
{
  char *sck_port = getenv("XPN_SCK_PORT");
  int port = DEFAULT_XPN_SCK_PORT;
  if (sck_port != NULL)
  {
    int aux_port = atoi(sck_port);
    if (aux_port != 0){
      port = aux_port;
    }
    else{
      printf("[SOCKET] [socket_get_port] ERROR: env XPN_SCK_PORT %s is not a number\n", sck_port);
    }
  }
  return port;
}

int socket_send ( int socket, void * buffer, int size )
{
  int r;
  int l = size;

  do
  {
    r = dlsym_write(socket, buffer, l);
    if (r < 0) {
      printf("[SOCKET] [socket_send] ERROR: socket send buffer size %d Failed\n", size);
      return -1;
    }
    l = l - r;
    buffer = buffer + r;

  } while ((l>0) && (r>=0));

  return size;
}

int socket_recv ( int socket, void * buffer, int size )
{
  int r;
  int l = size;

  do {
    r = dlsym_read(socket, buffer, l);
    if (r < 0) {
      printf("[SOCKET] [socket_recv] ERROR: socket read buffer size %d Failed\n", size);
      return -1;
    }
    l = l -r ;
    buffer = buffer + r;

  } while ((l>0) && (r>=0));

  return size;
}

int socket_server_create ( int *out_socket )
{
  int ret = 0;
  int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server_socket < 0)
  {
    printf("[SOCKET] [socket_server_create] ERROR: socket fails\n");
    return -1;
  }

  struct sockaddr_in server_addr;

  int val = 1;
  ret = setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));
  if (ret < 0)
  {
    printf("[SOCKET] [socket_server_create] ERROR: setsockopt fails\n");
    return -1;
  }

  debug_info("[SOCKET] [socket_server_create] Socket reuseaddr\n");

  val = 1;
  ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&val, sizeof(int));
  if (ret < 0)
  {
    printf("[SOCKET] [socket_server_create] ERROR: setsockopt fails\n");
    return -1;
  }

  // bind
  debug_info("[SOCKET] [socket_server_create] Socket bind\n");

  bzero((char * )&server_addr, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port        = htons(socket_get_port());


  ret = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (ret < 0)
  {
    printf("[SOCKET] [socket_server_create] ERROR: bind fails\n");
    return -1;
  }

  // listen
  debug_info("[SOCKET] [socket_server_create] Socket listen\n");

  ret = listen(server_socket, SOMAXCONN);
  if (ret < 0)
  {
    printf("[SOCKET] [socket_server_create] ERROR: listen fails\n");
    return -1;
  }
  *out_socket = server_socket;
  return 0;
}

int socket_server_accept ( int socket, int *out_conection_socket )
{
  struct sockaddr_in client_addr;
  socklen_t sock_size = sizeof(struct sockaddr_in);
  int new_socket = accept(socket, (struct sockaddr*)&client_addr, &sock_size);
  if (new_socket < 0) {
    printf("[SOCKET] [socket_accept_send] ERROR: socket accept\n");
    return -1;
  }
  *out_conection_socket = new_socket;
  return 0;
}

int socket_client_connect ( char * srv_name, int *out_socket )
{
  int client_fd;
  struct sockaddr_in serv_addr;
  client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd < 0) 
  {
    printf("[SOCKET] [socket_read] ERROR: socket creation error\n");
    return -1;
  }
  struct hostent * hp;
  hp = gethostbyname(srv_name);
  if (hp == NULL) 
  {
    printf("[SOCKET] [socket_read] ERROR: gethostbyname srv_name: %s\n", srv_name);
    close(client_fd);
    return -1;
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(socket_get_port());
  memcpy( & (serv_addr.sin_addr), hp->h_addr, hp->h_length);
  int status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if (status < 0) 
  {
    printf("[SOCKET] [socket_read] ERROR: socket connection Failed\n");
    close(client_fd);
    return -1;
  }

  *out_socket = client_fd;
  return 0;
}

int socket_close ( int socket )
{
  int ret;

  ret = close(socket);
  if (ret < 0) {
    printf("[SOCKET] [socket_close] ERROR: socket close Failed\n");
    return -1;
  }

  return ret;
}
