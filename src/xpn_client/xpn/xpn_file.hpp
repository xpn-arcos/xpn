
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
    class xpn_fh
    {
        std::string url;            // url in the server   
        long telldir;               // telldir of directory in the server when XPN_SESSION_DIR is not set
        DIR *dir;                   // pointer to directory in the server when XPN_SESSION_DIR set
        int fd;                     // file_descriptor in the server when XPN_SESSION_FILE set
    };

    enum class file_type
    {
        file = 0,
        dir = 1
    };

    class xpn_file
    {
        const std::string m_path;       // absolute path
        file_type m_type;               // indicate FILE or DIR
        int m_links;                    // number of links that this file has
        const xpn_partition &m_part;    // partition
        xpn_metadata m_mdata;           // metadata
        off_t m_offset;                 // offset of the open file
        std::vector<xpn_fh> m_data_vfh; // virtual FH

        xpn_file(const xpn_partition &part) : m_part(part) {}
    };
} // namespace XPN