
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
#include "xpn/xpn_partition.hpp"

#include "base_cpp/xpn_path.hpp"
namespace XPN
{
    
    // Fordward declaration
    class xpn_partition;

    class xpn_metadata
    {
    public:
        static constexpr const int HEADER_SIZE = 8192;
        static constexpr const char * MAGIC_NUMBER = "XPN";
        static constexpr const int VERSION = 1;
        static constexpr const int MAX_RECONSTURCTIONS = 40;
        static constexpr const int DISTRIBUTION_ROUND_ROBIN = 1;
    public:
        xpn_metadata(const std::string &path, const xpn_partition& part) : m_path(path), m_part(part) {}
        int read();
        int write();
        int update_file_size();
    public:
        struct data{
            std::array<int, 3> magic_number = {0};                     // Magic number to identify if is correct the metadata
            int      version = 0;                                      // Version number
            int      type = 0;                                         // Type of file: file or directory
            uint64_t block_size = 0;                                   // Size of block used
            uint64_t file_size = 0;                                    // Size of the file
            int      replication_level = 0;                            // Replication level of files: 0, 1, 2, ...
            int      first_node = 0;                                   // Server which has the first block
            int      distribution_policy = 0;                          // Distribution policy of blocks, default: round-robin
            std::array<int, MAX_RECONSTURCTIONS> data_nserv = {0}; // Array of number of servers to reconstruct
            std::array<int, MAX_RECONSTURCTIONS> offsets = {0};    // Array indicating the block where new server configuration starts
            
            bool in_valid() 
            { 
                return magic_number[0] == MAGIC_NUMBER[0] && 
                       magic_number[1] == MAGIC_NUMBER[1] && 
                       magic_number[2] == MAGIC_NUMBER[2];
            } 
        };
    public:
        const std::string &m_path;
        const xpn_partition &m_part;
        data m_data;

    public:
    private:
        int calculate_master(bool is_file)
        {
            int master = xpn_path::hash(m_path, m_part.m_data_serv.size(), is_file);
            for (int i = 0; i < m_part.m_replication_level; i++)
            {
                master = (master+1)%m_part.m_data_serv.size();
                if (m_part.m_data_serv[master].m_error != -1){
                    break;
                }
            }
            return master;
        }
    public:
        int master_file() {return calculate_master(true);}
        int master_dir() {return calculate_master(false);}
    };
} // namespace XPN