
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

#include "nfi_sck_server_comm.hpp"
#include "xpn_server/xpn_server_params.hpp"
#include "base_cpp/debug.hpp"
#include "base_cpp/socket.hpp"
#include "base_cpp/ns.hpp"
#include <csignal>
#include <xpn_server/xpn_server_ops.hpp>

namespace XPN {

nfi_xpn_server_comm* nfi_sck_server_control_comm::connect ( const std::string &srv_name )
{
  struct hostent * hp;
  struct sockaddr_in server_addr;
  int ret, sd, flag, val;
  int connection_socket;
  char port_name[MAX_PORT_NAME];

  debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_connect] >> Begin");

  // Lookup port name
  ret = socket::client_connect(srv_name, connection_socket);
  if (ret < 0)
  {
    debug_error("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_connect] ERROR: socket connect\n");
    return nullptr;
  }
  int buffer = socket::ACCEPT_CODE;
  ret = socket::send(connection_socket, &buffer, sizeof(buffer));
  if (ret < 0)
  {
    debug_error("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_connect] ERROR: socket send\n");
    socket::close(connection_socket);
    return nullptr;
  }
  ret = socket::recv(connection_socket, port_name, MAX_PORT_NAME);
  if (ret < 0)
  {
    debug_error("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_connect] ERROR: socket read\n");
    socket::close(connection_socket);
    return nullptr;
  }
  socket::close(connection_socket);

  if (ret < 0) {
    printf("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_connect] ERROR: Lookup %s Port %s\n", srv_name.c_str(), port_name);
    return nullptr;
  }

  debug_info("[NFI_SCK_SERVER_COMM] ----SERVER = "<<srv_name<<" PORT = "<<port_name);

  // Socket...
  sd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sd < 0) {
    perror("socket: ");
    return nullptr;
  }
  debug_info("[NFI_SCK_SERVER_COMM] ----SERVER = "<<srv_name<<" PORT = "<<port_name<<" ==> "<<sd);

  // Set sockopt
  flag = 1;
  ret = ::setsockopt(sd, IPPROTO_TCP, TCP_NODELAY, & flag, sizeof(flag));
  if (ret < 0) {
    perror("setsockopt: ");
    return nullptr;
  }

  val = MAX_BUFFER_SIZE; //1 MB
  ret = ::setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char * ) & val, sizeof(int));
  if (ret < 0) {
    perror("setsockopt: ");
    return nullptr;
  }

  val = MAX_BUFFER_SIZE; //1 MB
  ret = setsockopt(sd, SOL_SOCKET, SO_RCVBUF, (char * ) & val, sizeof(int));
  if (ret < 0) {
    perror("setsockopt: ");
    return nullptr;
  }

  // gethost by name
  hp = gethostbyname(srv_name.c_str());
  if (hp == NULL)
  {
    fprintf(stderr, "nfi_sck_server_init: error gethostbyname %s (%s,%s)\n",
    srv_name.c_str(), srv_name.c_str(), port_name);
    return nullptr;
  }

  // Connect...
  debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_connect] Connect port "<<port_name);

  bzero((char * ) & server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port   = htons(atoi(port_name));
  memcpy( & (server_addr.sin_addr), hp->h_addr, hp->h_length);

  int connect_retries = 0;
  do
  {
    ret = ::connect(sd, (struct sockaddr * ) & server_addr, sizeof(server_addr));
    if (ret < 0)
    {
      if (connect_retries == 0)
      {
        printf("----------------------------------------------------------------\n");
        print("XPN Client "<<ns::get_host_name()<<" : Waiting for servers being up and runing...");
        printf("----------------------------------------------------------------\n\n");
      }
      connect_retries++;
      sleep(2);
    }
  } while(ret < 0 && connect_retries < 1);

  if (ret < 0)
  {
    printf("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_connect] ERROR: connect fails\n");
    return nullptr;
  }

  debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_connect] << End\n");

  return new (std::nothrow) nfi_sck_server_comm(sd);
}

void nfi_sck_server_control_comm::disconnect(nfi_xpn_server_comm *comm) 
{
  int ret;
  int code = XPN_SERVER_DISCONNECT;
  nfi_sck_server_comm *in_comm = static_cast<nfi_sck_server_comm*>(comm);

  debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] >> Begin");

  debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] Send disconnect message");
  ret = in_comm->write_operation(code);
  if (ret < 0) {
    printf("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] ERROR: nfi_sck_server_comm_write_operation fails");
  }

  // Disconnect
  debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] Disconnect");

  ret = socket::close(in_comm->m_socket);
  if (ret < 0) {
    printf("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] ERROR: MPI_Comm_disconnect fails");
  }

  debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_disconnect] << End");
}

int64_t nfi_sck_server_comm::write_operation(int op) {
    int ret;
    int msg[2];

    debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_write_operation] >> Begin");

    // Message generation
    msg[0] = (int)(pthread_self() % 32450) + 1;
    msg[1] = (int)op;

    // Send message
    debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_write_operation] Write operation send tag "<< msg[0]);

    ret = socket::send(m_socket, msg, sizeof(msg));
    if (ret < 0) {
        debug_error("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_write_operation] ERROR: socket::send < 0 : "<< ret);
        return -1;
    }

    debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_write_operation] << End");

    // Return OK
    return 0;
}

int64_t nfi_sck_server_comm::write_data(const void *data, int64_t size) {
    int ret;

    debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_write_data] >> Begin");

    // Check params
    if (size == 0) {
        return 0;
    }
    if (size < 0) {
        printf("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_write_data] ERROR: size < 0");
        return -1;
    }

    // Send message
    debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_write_data] Write data");

    ret = socket::send(m_socket, data, size);
    if (ret < 0) {
        printf("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_write_data] ERROR: MPI_Send fails");
        size = 0;
    }

    debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_write_data] << End");

    // Return bytes written
    return size;
}

int64_t nfi_sck_server_comm::read_data(void *data, ssize_t size) {
    int ret;

    debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_read_data] >> Begin");

    // Check params
    if (size == 0) {
        return 0;
    }
    if (size < 0) {
        printf("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_read_data] ERROR: size < 0");
        return -1;
    }

    // Get message
    debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_read_data] Read data");

    ret = socket::recv(m_socket, data, size);
    if (ret < 0) {
        printf("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_read_data] ERROR: MPI_Recv fails");
        size = 0;
    }

    debug_info("[NFI_SCK_SERVER_COMM] [nfi_sck_server_comm_read_data] << End");

    // Return bytes read
    return size;
}

} //namespace XPN
