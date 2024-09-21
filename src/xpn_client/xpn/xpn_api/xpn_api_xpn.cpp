
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

#include "base_c/profiler.h"
#include "base_cpp/debug.hpp"
#include "xpn/xpn_api.hpp"
#include "base_cpp/xpn_env.hpp"
#include "xpn/xpn_conf.hpp"

#include <iostream>
#include <cstdio>
#include <csignal>

namespace XPN
{
    int xpn_api::init()
    {
        int res = 0;

        XPN_PROFILER_BEGIN(env.xpn_profiler);
        XPN_DEBUG_BEGIN;

        std::setbuf(stdout,NULL);
        std::setbuf(stderr,NULL);

        std::unique_lock<std::mutex> lock(m_init_mutex);
        if (m_initialized){
            XPN_DEBUG("Already initialized");
            XPN_DEBUG_END;
            return res;
        }
        m_initialized = true;
        
        xpn_conf conf;
        xpn_env::get_instance();

        for (const auto &part : conf.partitions)
        {
            // Emplace without creation of temp xpn_partition
            auto[key, inserted] = m_partitions.emplace(std::piecewise_construct,
                                                       std::forward_as_tuple(part.partition_name),
                                                       std::forward_as_tuple(part.partition_name, part.replication_level, part.bsize));
            if (!inserted){
                std::cerr << "Error: cannot create xpn_partition" << std::endl;
                std::raise(SIGTERM);
            }
            //  = {part.partition_name, part.replication_level, part.bsize};
            auto& xpn_part = m_partitions.at(part.partition_name);
            // m_partitions.emplace(std::make_pair(part.partition_name, xpn_partition{part.partition_name,part.replication_level, part.bsize}));
            int server_with_error = 0;
            for (const auto &srv_url : part.server_urls)
            {
                res = xpn_part.init_server(srv_url);
            }

            for (const auto &srv : xpn_part.m_data_serv)
            {
                if (srv->m_error < 0)
                {
                    server_with_error++;
                }
            }
            if (server_with_error > xpn_part.m_replication_level)
            {
                std::cerr << "Error: More servers with errors '" << server_with_error 
                << "' than replication level permit '" << xpn_part.m_replication_level 
                << "'" << std::endl;
                std::raise(SIGTERM);
            }
        }
        
        m_worker = workers::Create(static_cast<workers_mode>(xpn_env::get_instance().xpn_thread));

        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::destroy()
    {
        int res = 0;
        XPN_DEBUG_BEGIN;
        std::unique_lock<std::mutex> lock(m_init_mutex);
        if (!m_initialized){
            XPN_DEBUG("Not initialized");
            XPN_DEBUG_END;
            return res;
        }
        m_initialized = false;

        for (auto &[key, part] : m_partitions)
        {
            for (auto &serv : part.m_data_serv)
            {
                serv->destroy_comm();
            }
        }
        m_partitions.clear();
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::mark_error_server(int index)
    {
        int res = -1;
        XPN_DEBUG_BEGIN;
        for (auto &[key, part] : m_partitions)
        {
            if (index < static_cast<int>(part.m_data_serv.size())){
                part.m_data_serv[index]->m_error = -1;
                res = 0;
            }
        }
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::get_block_locality(char *path, off_t offset, int *url_c, char **url_v[])
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::free_block_locality(int *url_c, char **url_v[])
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }
} // namespace XPN
