
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

#include "base/profiler.h"
#include "xpn/xpn_api.hpp"
#include "xpn/xpn_env.hpp"
#include "xpn/xpn_conf.hpp"

#include <iostream>
#include <cstdio>

namespace XPN
{
    int xpn_api::init()
    {
        xpn_env &env = xpn_env::get_instance();

        XPN_PROFILER_BEGIN(env.xpn_profiler);
        XPN_DEBUG_BEGIN;

        std::setbuf(stdout,NULL);
        std::setbuf(stderr,NULL);

        std::unique_lock<std::mutex> lock(m_init_mutex);
        if (m_initialized){
            XPN_DEBUG("Already initialized");
            return 0;
        }

        xpn_conf conf;

        for (const auto &part : conf.partitions)
        {
            auto& xpn_part = m_partitions.emplace_back(part.partition_name, part.replication_level, part.bsize);

            for (const auto &srv_url : part.server_urls)
            {
                xpn_part.init_server(srv_url);
            }
        }
        // TODO: finish
    }
} // namespace XPN
