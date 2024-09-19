
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
#include <vector>

#include "nfi/nfi_server.hpp"
#include "xpn/xpn_conf.hpp"

namespace XPN
{
    // Fordward declaration
    class nfi_server;

    class xpn_partition
    {
    public:
        xpn_partition(const std::string &name, int replication_level, uint64_t block_size);
        int init_server(const std::string &url);
    public:
        std::string m_name;                                             // name of partition 
        int m_replication_level = XPN_CONF::DEFAULT_REPLICATION_LEVEL;  // replication_level of files :0, 1, 2,... 
        uint64_t m_block_size = XPN_CONF::DEFAULT_BLOCKSIZE;            // size of distribution used 

        std::vector<std::unique_ptr<nfi_server>> m_data_serv;                            // list of data servers in the partition 

        int m_local_serv = -1;                                          // server with locality
    };
} // namespace XPN