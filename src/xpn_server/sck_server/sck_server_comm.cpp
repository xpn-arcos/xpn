
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

#include "sck_server_comm.hpp"
#include "base_cpp/debug.hpp"
#include "base_cpp/timer.hpp"
#include "base_cpp/ns.hpp"
#include "base_cpp/socket.hpp"
#include "base_c/filesystem.h"
#include <csignal>

namespace XPN
{
sck_server_control_comm::sck_server_control_comm ()
{
  int ret, val;
  struct sockaddr_in server_addr;

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] >> Begin");

  // Get timestap
  timer timer;

  // Socket init
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] Scoket init");

  m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_socket < 0)
  {
    print("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: socket fails");
    std::raise(SIGTERM);
  }

  // tcp_nodalay
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] TCP nodelay");

  val = 1;
  ret = ::setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));
  if (ret < 0)
  {
    print("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: setsockopt fails");
    std::raise(SIGTERM);
  }

  // sock_reuseaddr
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] Socket reuseaddr");

  val = 1;
  ret = ::setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&val, sizeof(int));
  if (ret < 0)
  {
    print("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: setsockopt fails");
    std::raise(SIGTERM);
  }

  // bind
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] Socket bind");

  bzero((char * )&server_addr, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port        = htons(0);


  ret = ::bind(m_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (ret < 0)
  {
    print("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: bind fails");
    std::raise(SIGTERM);
  }

  // listen
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] Socket listen");

  ret = ::listen(m_socket, 20);
  if (ret < 0)
  {
    print("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] ERROR: listen fails");
    std::raise(SIGTERM);
  }
  socklen_t len = sizeof(server_addr);
  ::getsockname(m_socket, (struct sockaddr *)&server_addr, &len);
  m_port_name = std::to_string(ntohs(server_addr.sin_port));

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] available at "<<m_port_name);
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] accepting...");

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_init] >> End");
}

sck_server_control_comm::~sck_server_control_comm()
{
  socket::close(m_socket);
}

xpn_server_comm* sck_server_control_comm::accept ( )
{
  int    ret, sc, flag;
  struct sockaddr_in client_addr;
  socklen_t size = sizeof(struct sockaddr_in);

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_accept] >> Begin");

  // Accept
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_accept] Accept");

  sc = ::accept(m_socket, (struct sockaddr *)&client_addr, &size);
  if (sc < 0)
  {
    print("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_destroy] ERROR: accept fails");
    return nullptr;
  }

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_destroy] desp. accept conection from "<<sc);
  // tcp_nodelay
  flag = 1;
  ret = ::setsockopt(sc, IPPROTO_TCP, TCP_NODELAY, & flag, sizeof(flag));
  if (ret < 0)
  {
    perror("setsockopt: ");
    return nullptr;
  }

  //NEW
  int val = MAX_BUFFER_SIZE; //1 MB
  ret = ::setsockopt(sc, SOL_SOCKET, SO_SNDBUF, (char *)&val, sizeof(int));
  if (ret < 0)
  {
    perror("setsockopt: ");
    return nullptr;
  }

  val = MAX_BUFFER_SIZE; //1 MB
  ret = ::setsockopt(sc, SOL_SOCKET, SO_RCVBUF, (char *)&val, sizeof(int));
  if (ret < 0)
  {
    perror("setsockopt: ");
    return nullptr;
  }

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_accept] << End");

  return new (std::nothrow) sck_server_comm(sc);
}


void sck_server_control_comm::disconnect ( xpn_server_comm* comm )
{
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_disconnect] >> Begin");
  
  sck_server_comm *in_comm = static_cast<sck_server_comm*>(comm);
  socket::close(in_comm->m_socket);

  delete comm;

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_disconnect] << End");
}


int64_t sck_server_comm::read_operation ( int &op, int &rank_client_id, int &tag_client_id )
{
  int ret;
  int msg[2];

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_read_operation] >> Begin");

  // Get message
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_read_operation] Read operation");

  ret = filesystem_read(m_socket, msg, sizeof(msg));
  if (MPI_SUCCESS != ret) {
    debug_warning("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_read_operation] ERROR: read fails");
  }

  rank_client_id = 0;
  tag_client_id  = msg[0];
  op             = msg[1];

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_read_operation] read (SOURCE "<<m_socket<<", MPI_TAG "<<tag_client_id<<", MPI_ERROR "<<ret<<")");
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_read_operation] << End");

  // Return OK
  return 0;
}

int64_t sck_server_comm::read_data ( void *data, int64_t size, [[maybe_unused]] int rank_client_id, [[maybe_unused]] int tag_client_id )
{
  int ret;

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_read_data] >> Begin");

  if (size == 0) {
    return  0;
  }
  if (size < 0)
  {
    print("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_read_data] ERROR: size < 0");
    return  -1;
  }

  // Get message
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_read_data] Read data tag "<< tag_client_id);

  ret = filesystem_read(m_socket, data, size);
  if (MPI_SUCCESS != ret) {
    debug_warning("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_read_data] ERROR: read fails");
  }

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_read_data] read (SOURCE "<<m_socket<<", MPI_TAG "<<tag_client_id<<", MPI_ERROR "<<ret<<")");
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_read_data] << End");

  // Return bytes read
  return size;
}

int64_t sck_server_comm::write_data ( const void *data, int64_t size, [[maybe_unused]] int rank_client_id, [[maybe_unused]] int tag_client_id )
{
  int ret;

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_write_data] >> Begin");

  if (size == 0) {
      return 0;
  }
  if (size < 0)
  {
    print("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_write_data] ERROR: size < 0");
    return -1;
  }

  // Send message
  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_write_data] Write data tag "<< tag_client_id);

  ret = filesystem_write(m_socket, data, size);
  if (MPI_SUCCESS != ret) {
    debug_warning("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_write_data] ERROR: MPI_Send fails");
  }

  debug_info("[Server="<<ns::get_host_name()<<"] [SCK_SERVER_COMM] [sck_server_comm_write_data] << End");

  // Return bytes written
  return size;
}

} // namespace XPN