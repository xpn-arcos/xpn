
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
#include <unistd.h>
#include <limits.h>
#include <netdb.h>
#include <arpa/inet.h>

namespace XPN
{
    class ns
	{
    public:
        static std::string get_host_name()
        {

            char hostname[HOST_NAME_MAX];
            if (gethostname(hostname, HOST_NAME_MAX) == 0){
                return std::string(hostname);
            }
            return {};
        }

        static std::string get_host_ip()
        {
            hostent* hostname = gethostbyname(get_host_name().c_str());
            if(hostname)
                return std::string(inet_ntoa(**(in_addr**)hostname->h_addr_list));
            return {};
        }
	};
} // namespace XPN
