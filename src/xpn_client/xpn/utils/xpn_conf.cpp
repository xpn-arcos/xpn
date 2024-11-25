
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

#include "xpn/xpn_conf.hpp"
#include "base_cpp/xpn_env.hpp"
#include "base_cpp/debug.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <csignal>

namespace XPN
{
    inline void trim(std::string& str)
    {
        str.erase(str.find_last_not_of(" \t\n\r")+1);
        str.erase(0, str.find_first_not_of(" \t\n\r"));
    }

    inline int getSizeFactor(const std::string &name)
    {
        constexpr int KB = 1024;
        constexpr int MB = 1024 * KB;
        constexpr int GB = 1024 * MB;
        switch(name.back())
	    {
            case 'K':
            case 'k':
                return atoi(name.c_str())*KB;
            case 'M':
            case 'm':
                return atoi(name.c_str())*MB;
            case 'G':
            case 'g':
                return atoi(name.c_str())*GB;
            case 'B':
            case 'b':
                switch(name[name.size()-2]){
                    case 'K':
                    case 'k':
                        return atoi(name.c_str())*KB;
                    case 'M':
                    case 'm':
                        return atoi(name.c_str())*MB;
                    case 'G':
                    case 'g':
                        return atoi(name.c_str())*GB;
                    default:
                        return 1;
                }
            default:
                return 1;
        }
      }

    xpn_conf::xpn_conf()
    {   
        XPN_DEBUG_BEGIN;
        const char * cfile_path = xpn_env::get_instance().xpn_conf;
        if (cfile_path == nullptr)
        {
            cfile_path = XPN_CONF::DEFAULT_PATH;
        }
        std::ifstream file(cfile_path);
        
        if (!file.is_open()) {
            std::cerr << "Error: while openning the XPN_CONF file: " << cfile_path << std::endl;
            std::raise(SIGTERM);
        }
        std::string line;
        int actual_index = -1;
        while (std::getline(file, line)) {
            trim(line);
            // First check if have TAG_PARTITION and each create a new partition
            if (line == XPN_CONF::TAG_PARTITION){
                partitions.emplace_back();
                actual_index++;
            }

            if (actual_index == -1){
                std::cerr << "Error: while parsing the XPN_CONF file: " << cfile_path << " not found " << XPN_CONF::TAG_PARTITION << std::endl;
                std::cerr << "Error: line '" << line << "'" << std::endl;
                std::raise(SIGTERM);
            }

            // In each partition read each line to get keys and values
            std::stringstream ss(line);
            std::string key, value;

            if (std::getline(ss, key, '=') && std::getline(ss, value)) {
                trim(key);
                trim(value);

                if (key == XPN_CONF::TAG_PARTITION_NAME){
                    partitions[actual_index].partition_name = value;
                }else
                if (key == XPN_CONF::TAG_BLOCKSIZE){
                    partitions[actual_index].bsize = getSizeFactor(value);
                }else
                if (key == XPN_CONF::TAG_REPLICATION_LEVEL){
                    partitions[actual_index].replication_level = atoi(value.c_str());
                }else
                if (key == XPN_CONF::TAG_SERVER_URL){
                    partitions[actual_index].server_urls.emplace_back(value);
                }else{
                    std::cerr << "Error: key '" << key << "' is not expected" << std::endl;
                    std::raise(SIGTERM);
                }
            }
        }

        XPN_DEBUG(to_string());
        int res = 0;
        XPN_DEBUG_END;
    }
}