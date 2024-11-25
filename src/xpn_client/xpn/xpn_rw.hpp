
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

#include "xpn/xpn_file.hpp"

#include <vector>

namespace XPN
{
    class xpn_rw_buffer
    {
    public:
        xpn_rw_buffer(xpn_file &file, int64_t offset, void *buffer, uint64_t size);
        void calculate_reads();
        void join_ops();
        void fix_ops_reads();
        void calculate_writes();

        size_t num_ops();
        size_t size();

        std::string to_string();
        
        struct rw_buffer
        {
            char *buffer;
            uint64_t size;
            int64_t offset_serv;
            int64_t offset_buff;
            std::vector<char> v_buffer;
            std::vector<char*> origin_buffer;
            std::vector<uint64_t> origin_buffer_size;

            bool was_move() { return !v_buffer.empty(); }
            char * get_buffer() { return v_buffer.empty() ? buffer : v_buffer.data(); }
            int64_t get_size() { return size; }

            std::string to_string();
        };
        std::vector<std::vector<rw_buffer>> m_ops;

    private:
        xpn_file &m_file;
        int64_t m_offset;
        char *m_buffer;
    public:
        uint64_t m_size;
    };
    class xpn_rw
    {
    public:
        static int read_get_block (xpn_file &file, int64_t offset,                  int64_t &local_offset, int &serv);
        static int write_get_block(xpn_file &file, int64_t offset, int replication, int64_t &local_offset, int &serv);
    };
} // namespace XPN