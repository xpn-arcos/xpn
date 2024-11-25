
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

#pragma once

#include <string>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

namespace XPN
{
    class socket
	{
    public:
        constexpr static const int DEFAULT_XPN_SCK_PORT = 3456;
        constexpr static const int ACCEPT_CODE          = 123;
        constexpr static const int FINISH_CODE          = 666;
        constexpr static const int FINISH_CODE_AWAIT    = 667;
        constexpr static const int STATS_CODE           = 444;
        constexpr static const int STATS_wINDOW_CODE    = 445;
    public:
        static int get_xpn_port();
        static int64_t send ( int socket, const void * buffer, int64_t size );
        static int64_t recv ( int socket, void * buffer, int64_t size );
        static int server_create ( int port, int &out_socket );
        static int server_accept ( int socket, int &out_conection_socket );
        static int client_connect ( const std::string &srv_name, int port, int &out_socket );
        static int close ( int socket );
	};
} // namespace XPN
