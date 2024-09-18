
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

#include "base_cpp/debug.hpp"
#include "nfi_server.hpp"
#include "xpn/xpn_api.hpp"

#include <iostream>
#include <csignal>

namespace XPN
{
    nfi_server::nfi_server(const std::string &url) : m_url(url)
    {
        // Find the position of "://"
        size_t protocol_pos = m_url.find("://");

        if (protocol_pos == std::string::npos) {
            std::cerr << "Invalid format of server_url: '://' not found '" << m_url << "'" << std::endl;
            std::raise(SIGTERM);
        }

        // Extract the first part (before "://")
        m_protocol = m_url.substr(0, protocol_pos);

        // Extract the second part (after "://")
        std::string remainder = m_url.substr(protocol_pos + 3);

        // Find the position of the first '/'
        size_t ip_pos = remainder.find('/');

        if (ip_pos == std::string::npos) {
            std::cerr << "Invalid format: '/' not found after IP '" << m_url << "'" << std::endl;
            std::raise(SIGTERM);
        }

        // Extract the IP address
        m_server = remainder.substr(0, ip_pos);

        // Extract the path (after the first '/')
        m_path = remainder.substr(ip_pos);

        XPN_DEBUG("Parse '"<<m_url<<"' to protocol '"
        << m_protocol <<"' server '"
        << m_server << "' path '"
        << m_path << "'");
    }

    int nfi_server::init_comm()
    {
        // Init the comunication
        m_control_comm = nfi_xpn_server_control_comm::Create(m_protocol);

        // Connect to the server
        m_comm = m_control_comm->connect(m_server);

        if (m_comm == nullptr){
            m_error = -1;
            return -1;
        }
        return 0;
    }

    int nfi_server::destroy_comm()
    {
        m_control_comm->disconnect(m_comm);

        m_control_comm.reset();

        return 0;
    }
} // namespace XPN