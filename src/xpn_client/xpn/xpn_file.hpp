
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
#include <dirent.h>

#include <xpn/xpn_partition.hpp>
#include <xpn/xpn_metadata.hpp>

namespace XPN
{
    struct xpn_fh
    {
        std::string path;           // url in the server   
        long telldir = 0;           // telldir of directory in the server when XPN_SESSION_DIR is not set
        DIR *dir = nullptr;         // pointer to directory in the server when XPN_SESSION_DIR set
        int fd = -1;                // file_descriptor in the server when XPN_SESSION_FILE set

        bool is_initialized() {return !path.empty();}
    };

    enum class file_type
    {
        null = 0,
        file = 1,
        dir = 2
    };

    class xpn_file
    {
    public:
        xpn_file(std::string &path, xpn_partition &part) : m_path(path), m_part(part), m_mdata(*this) 
        {
            m_data_vfh.resize(m_part.m_data_serv.size());
        }
        xpn_file(const xpn_file& file) : 
            m_path(file.m_path), 
            m_type(file.m_type),
            m_links(file.m_links),
            m_flags(file.m_flags),
            m_mode(file.m_mode),
            m_part(file.m_part), 
            m_mdata(*this, file.m_mdata.m_data),
            m_offset(file.m_offset),
            m_data_vfh(file.m_data_vfh)
        {}
        // Delete default constructors
        xpn_file() = delete;
        // Delete copy assignment operator
        xpn_file& operator=(const xpn_file&) = delete;
        // Delete move constructor
        xpn_file(xpn_file&&) = delete;
        // Delete move assignment operator
        xpn_file& operator=(xpn_file&&) = delete;
    public:
        bool exist_in_serv(int serv);
        void static map_offset(int block_size, int replication_level, int nserv, int64_t offset, int replication, int first_node, int64_t &local_offset, int &serv);
        void static inverted_map_offset(int block_size, int replication_level, int nserv, int serv, int64_t local_offset, int first_node, int64_t &offset, int &replication);
        void map_offset_mdata(int64_t offset, int replication, int64_t &local_offset, int &serv);
        int  initialize_vfh(int index);
        int  initialize_vfh_dir(int index);

    public:
        std::string m_path;                 // absolute path
        file_type m_type = file_type::null; // indicate FILE or DIR
        int m_links = 0;                    // number of links that this file has
        int m_flags = 0;                    // O_RDONLY, O_WRONLY,....    
        mode_t m_mode = 0;                  // S_IRUSR , S_IWUSR ,....  
        xpn_partition &m_part;              // partition
        xpn_metadata m_mdata;               // metadata
        off_t m_offset = 0;                 // offset of the open file
        std::vector<xpn_fh> m_data_vfh;     // virtual FH

    };
} // namespace XPN