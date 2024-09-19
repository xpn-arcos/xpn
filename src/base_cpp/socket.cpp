
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

#include "socket.hpp"
#include "xpn_env.hpp"
#include "base/filesystem.h"

#include <string>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <unistd.h>

namespace XPN
{
    int socket::get_xpn_port()
    {
        const char *sck_port = xpn_env::get_instance().xpn_sck_port;
        int port = DEFAULT_XPN_SCK_PORT;
        if (sck_port != NULL)
        {
            int aux_port = atoi(sck_port);
            if (aux_port != 0){
                port = aux_port;
            }
            else{
                std::cerr<<"Error: env XPN_SCK_PORT '"<<sck_port<<"' is not a number, using default '"<<DEFAULT_XPN_SCK_PORT<<"'"<<std::endl;
            }
        }
        return port;
    }

    int64_t socket::send ( int socket, void * buffer, int64_t size )
    {
        int64_t ret;

        ret = filesystem_write(socket, buffer, size);
        if (ret < 0)
            printf("[SOCKET] [socket::recv] ERROR: socket read buffer size %ld Failed\n", size);

        return size;
    }

    int64_t socket::recv ( int socket, void * buffer, int64_t size )
    {
        int64_t ret;

        ret = filesystem_read(socket, buffer, size);
        if (ret < 0)
            printf("[SOCKET] [socket::recv] ERROR: socket read buffer size %ld Failed\n", size);

        return size;
    }

    int socket::server_create ( int &out_socket )
    {
        int ret = 0;
        int server_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (server_socket < 0)
        {
            printf("[SOCKET] [socket::server_create] ERROR: socket fails\n");
            return -1;
        }


        int val = 1;
        ret = setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val));
        if (ret < 0)
        {
            printf("[SOCKET] [socket::server_create] ERROR: setsockopt fails\n");
            return -1;
        }

        debug_info("[SOCKET] [socket::server_create] Socket reuseaddr\n");

        val = 1;
        ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&val, sizeof(int));
        if (ret < 0)
        {
            printf("[SOCKET] [socket::server_create] ERROR: setsockopt fails\n");
            return -1;
        }

        // bind
        debug_info("[SOCKET] [socket::server_create] Socket bind\n");

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family      = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port        = htons(get_xpn_port());


        ret = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0)
        {
            printf("[SOCKET] [socket::server_create] ERROR: bind fails\n");
            return -1;
        }

        // listen
        debug_info("[SOCKET] [socket::server_create] Socket listen\n");

        ret = listen(server_socket, SOMAXCONN);
        if (ret < 0)
        {
            printf("[SOCKET] [socket::server_create] ERROR: listen fails\n");
            return -1;
        }
        out_socket = server_socket;
        return 0;
    }

    int socket::server_accept ( int socket, int &out_conection_socket )
    {
        struct sockaddr_in client_addr;
        socklen_t sock_size = sizeof(sockaddr_in);
        int new_socket = accept(socket, (struct sockaddr*)&client_addr, &sock_size);
        if (new_socket < 0) {
            printf("[SOCKET] [socket::accept_send] ERROR: socket accept\n");
            return -1;
        }
        out_conection_socket = new_socket;
        return 0;
    }

    int socket::client_connect ( const std::string &srv_name, int &out_socket )
    {
        int client_fd;
        struct sockaddr_in serv_addr;
        client_fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (client_fd < 0) 
        {
            printf("[SOCKET] [socket::read] ERROR: socket creation error\n");
            return -1;
        }
        struct hostent * hp;
        hp = gethostbyname(srv_name.c_str());
        if (hp == NULL) 
        {
            printf("[SOCKET] [socket::read] ERROR: gethostbyname srv_name: %s\n", srv_name.c_str());
            close(client_fd);
            return -1;
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(get_xpn_port());
        memcpy( & (serv_addr.sin_addr), hp->h_addr, hp->h_length);
        int status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        if (status < 0) 
        {
            printf("[SOCKET] [socket::read] ERROR: socket connection failed to %s in port %d %s\n", srv_name.c_str(), get_xpn_port(), strerror(errno));
            close(client_fd);
            return -1;
        }

        out_socket = client_fd;
        return 0;
    }

    int socket::close ( int socket )
    {
        int ret;

        ret = ::close(socket);
        if (ret < 0) {
            printf("[SOCKET] [socket::socket_close] ERROR: socket close Failed\n");
            return -1;
        }

        return ret;
    }
} // namespace XPN
