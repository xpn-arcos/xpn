
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

#include "xpn/xpn_partition.hpp"
#include "base_cpp/ns.hpp"
#include "base_cpp/debug.hpp"

namespace XPN
{
        xpn_partition::xpn_partition(const std::string &name, int replication_level, uint64_t block_size) :
            m_name(name), m_replication_level(replication_level), m_block_size(block_size)
        {
        }

        int xpn_partition::init_server(const std::string &url)
        {
            XPN_DEBUG_BEGIN;
            int res = 0;
            int index = m_data_serv.size();

            auto& server = m_data_serv.emplace_back(url);

            if (server.m_server.compare(ns::get_host_name()) == 0 ||
                server.m_server.compare(ns::get_host_ip()) == 0)
            {
                m_local_serv = index;
            }

            res = server.init_comm();

            XPN_DEBUG_END;
            return res;
        }
} // namespace XPN