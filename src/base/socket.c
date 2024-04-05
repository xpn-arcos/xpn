
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


/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */


/* ... Functions / Funciones ......................................... */

int socket_get_port()
{
  char *sck_port = getenv("XPN_SCK_PORT");
  int port = MPI_SOCKET_PORT;
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

int socket_send_data ( int socket, void * data, int size )
{
  int status = send(socket, data, size, 0);
  if (status < 0 || status != size) 
  {
    printf("[SOCKET] [socket_send_data] ERROR: socket send data size %d Failed\n", size);
    return -1;
  }
  return status;
}

int socket_read_data ( int socket, void * data, int size )
{
  int status = read(socket, data, size);
  if (status < 0 || status != size) 
  {
    printf("[SOCKET] [socket_read_data] ERROR: socket read data size %d Failed\n", size);
    return -1;
  }
  return status;
}

int socket_accept_send ( int socket, void * data, int size )
{
  struct sockaddr_in client_addr;
  socklen_t sock_size = sizeof(struct sockaddr_in);
  int new_socket = accept(socket, (struct sockaddr*)&client_addr, &sock_size);
  if (new_socket < 0) {
    printf("[SOCKET] [socket_accept_send] ERROR: socket accept\n");
    close(new_socket);
    return -1;
  }
  int valsend = socket_send_data(new_socket, data, size);
  if (valsend < 0) {
    printf("[SOCKET] [socket_accept_send] ERROR: socket send %d buffer size %d\n", valsend, size);
    close(new_socket);
    return -1;
  }
  close(new_socket);
  return 0;
}

// Return read int from socket
int socket_accept_read ( int socket )
{
  struct sockaddr_in client_addr;
  socklen_t size = sizeof(struct sockaddr_in);
  int new_socket = accept(socket, (struct sockaddr*)&client_addr, &size);
  if (new_socket < 0) {
    printf("[SOCKET] [socket_accept_read] ERROR: socket accept\n");
    close(new_socket);
    return -1;
  }
  int buffer;
  int valread = socket_read_data(new_socket, &buffer, sizeof(int));
  if (valread < 0) {
    printf("[SOCKET] [socket_accept_read] ERROR: socket read %d buffer size %d\n", valread, buffer);
    close(new_socket);
    return -1;
  }
  close(new_socket);
  return buffer;
}

int socket_send ( char * srv_name, int code )
{
  int client_fd;
  struct sockaddr_in serv_addr;
  client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd < 0) 
  {
    printf("[SOCKET] [socket_send] ERROR: socket creation error\n");
    return -1;
  }
  struct hostent * hp;
  hp = gethostbyname(srv_name);
  if (hp == NULL) 
  {
    printf("[SOCKET] [socket_send] ERROR: gethostbyname srv_name: %s\n",srv_name);
    close(client_fd);
    return -1;
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(socket_get_port());
  memcpy( & (serv_addr.sin_addr), hp->h_addr, hp->h_length);
  int status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if (status < 0) 
  {
    printf("[SOCKET] [socket_send] ERROR: socket connection Failed\n");
    close(client_fd);
    return -1;
  }
  status = socket_send_data(client_fd, &code, sizeof(int));
  if (status < 0) 
  {
    printf("[SOCKET] [socket_send] ERROR: socket send int %d Failed\n",code);
    close(client_fd);
    return -1;
  }

  close(client_fd);
  return 0;
}


int socket_read ( char * srv_name, void * data, int size )
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
    printf("[SOCKET] [socket_read] ERROR: gethostbyname srv_name: %s\n",srv_name);
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
  status = socket_read_data(client_fd, data, size);
  if (status < 0) 
  {
    printf("[SOCKET] [socket_read] ERROR: socket read data of size %d Failed\n", size);
    close(client_fd);
    return -1;
  }

  close(client_fd);
  return 0;
}

// Return the code
int socket_create ( int *out_socket )
{
  int ret = 0;
  int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server_socket < 0)
  {
    printf("[SOCKET] [socket_create] ERROR: socket fails\n");
    return -1;
  }

  struct sockaddr_in server_addr;

  int val = 1;
  ret = setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));
  if (ret < 0)
  {
    printf("[SOCKET] [socket_create] ERROR: setsockopt fails\n");
    return -1;
  }

  debug_info("[SOCKET] [socket_create] Socket reuseaddr\n");

  val = 1;
  ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&val, sizeof(int));
  if (ret < 0)
  {
    printf("[SOCKET] [socket_create] ERROR: setsockopt fails\n");
    return -1;
  }

  // bind
  debug_info("[SOCKET] [socket_create] Socket bind\n");

  bzero((char * )&server_addr, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port        = htons(socket_get_port());


  ret = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (ret < 0)
  {
    printf("[SOCKET] [socket_create] ERROR: bind fails\n");
    return -1;
  }

  // listen
  debug_info("[SOCKET] [socket_create] Socket listen\n");

  ret = listen(server_socket, SOMAXCONN);
  if (ret < 0)
  {
    printf("[SOCKET] [socket_create] ERROR: listen fails\n");
    return -1;
  }
  *out_socket = server_socket;
  return 0;
}