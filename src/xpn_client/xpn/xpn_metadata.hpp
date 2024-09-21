
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

#include "base_cpp/xpn_path.hpp"
#include "base_cpp/debug.hpp"

namespace XPN
{
    
    // Fordward declaration
    class xpn_file;

    class xpn_metadata
    {
    public:
        static constexpr const int HEADER_SIZE = 8192;
        static constexpr const char * MAGIC_NUMBER = "XPN";
        static constexpr const int VERSION = 1;
        static constexpr const int MAX_RECONSTURCTIONS = 40;
        static constexpr const int DISTRIBUTION_ROUND_ROBIN = 1;
    public:
        struct data{
            void fill(const xpn_metadata& mdata);

            std::array<char, 3> magic_number = {'0', '0', '0'};                     // Magic number to identify if is correct the metadata
            int      version = 0;                                      // Version number
            int      type = 0;                                         // Type of file: file or directory
            uint64_t block_size = 0;                                   // Size of block used
            uint64_t file_size = 0;                                    // Size of the file
            int      replication_level = 0;                            // Replication level of files: 0, 1, 2, ...
            int      first_node = 0;                                   // Server which has the first block
            int      distribution_policy = 0;                          // Distribution policy of blocks, default: round-robin
            std::array<int, MAX_RECONSTURCTIONS> data_nserv = {0}; // Array of number of servers to reconstruct
            std::array<int, MAX_RECONSTURCTIONS> offsets = {0};    // Array indicating the block where new server configuration starts
            
            bool is_valid() { 
                return magic_number[0] == MAGIC_NUMBER[0] && 
                       magic_number[1] == MAGIC_NUMBER[1] && 
                       magic_number[2] == MAGIC_NUMBER[2];
            } 
            std::string to_string();
        };
    public:
        // xpn_metadata(std::string &path, xpn_partition& part) : m_path(path), m_part(part) {}
        xpn_metadata(xpn_file& file) : m_file(file) {}
        xpn_metadata(xpn_file& file, const xpn_metadata::data &data) : m_file(file), m_data(data) {}
        // Delete default constructors
        xpn_metadata() = delete;
        // Delete copy constructor
        xpn_metadata(const xpn_metadata&) = delete;
        // Delete copy assignment operator
        xpn_metadata& operator=(const xpn_metadata&) = delete;
        // Delete move constructor
        xpn_metadata(xpn_metadata&&) = delete;
        // Delete move assignment operator
        xpn_metadata& operator=(xpn_metadata&&) = delete;
    public:
        xpn_file &m_file;
        // std::string &m_path;
        // xpn_partition &m_part;
        data m_data;

    public:
    private:
        int calculate_master(bool is_file) const;
    public:
        int master_file() const {return calculate_master(true);}
        int master_dir() const {return calculate_master(false);}
        std::string to_string(bool with_data = true);
    };
} // namespace XPN