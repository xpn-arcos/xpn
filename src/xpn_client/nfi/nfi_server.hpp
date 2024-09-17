
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
#include <memory>

#include "nfi_xpn_server_comm.hpp"

namespace XPN
{
    class nfi_server 
    {
    public:
        std::string m_url;      // URL of this server -> protocol   
                                // + server         
                                // + path + more info (port, ...) 
        std::string m_server;   // server address   
        std::string m_path;     // path of the server 

        int m_error;            // For fault tolerance
        
        std::unique_ptr<nfi_xpn_server_control_comm> m_control_comm;
    };
} // namespace XPN