
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

#include <vector>
#include <string>
#include <sstream>

namespace XPN
{
    namespace XPN_CONF
    {
        constexpr const char * TAG_PARTITION = "[partition]";
        constexpr const char * TAG_PARTITION_NAME = "partition_name";
        constexpr const char * TAG_REPLICATION_LEVEL = "replication_level";
        constexpr const char * TAG_BLOCKSIZE = "bsize";
        constexpr const char * TAG_SERVER_URL = "server_url";
        constexpr const int DEFAULT_REPLICATION_LEVEL = 0;
        constexpr const int DEFAULT_BLOCKSIZE = 512 * 1024;

        constexpr const char * DEFAULT_PATH = "/etc/xpn/xpn.conf";
    }

    class xpn_conf
    {
        struct partition
        {
            std::string partition_name;
            int bsize = XPN_CONF::DEFAULT_BLOCKSIZE;
            int replication_level = XPN_CONF::DEFAULT_REPLICATION_LEVEL;
            std::vector<std::string> server_urls;

            partition() = default;
            partition(const partition&) = default;

            std::string to_string()
            {
                std::stringstream out;
                out << XPN_CONF::TAG_PARTITION << std::endl;
                out << XPN_CONF::TAG_PARTITION_NAME << " = " << partition_name << std::endl;
                out << XPN_CONF::TAG_BLOCKSIZE << " = " << bsize << std::endl;
                out << XPN_CONF::TAG_REPLICATION_LEVEL << " = " << replication_level << std::endl;
                for (auto &srv : server_urls)
                {
                    out << XPN_CONF::TAG_SERVER_URL << " = " << srv << std::endl;
                }
                return out.str();
            }
        };

    public:
        xpn_conf();

        std::string to_string()
        {
            std::stringstream out;
            for (auto &part : partitions)
            {
                out << part.to_string();
            }
            return out.str();
        }
    public:
        std::vector<partition> partitions;
    };
}