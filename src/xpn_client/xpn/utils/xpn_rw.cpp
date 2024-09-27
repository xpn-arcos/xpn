
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

#include "xpn/xpn_rw.hpp"
#include "base_cpp/debug.hpp"
#include <algorithm>
#include <sstream>
namespace XPN
{

    xpn_rw_buffer::xpn_rw_buffer(xpn_file &file, int64_t offset, void *buffer, uint64_t size) :
        m_file(file), m_offset(offset), m_buffer(static_cast<char*>(buffer)), m_size(size)
    {
        m_ops.resize(file.m_part.m_data_serv.size());
    }

    void xpn_rw_buffer::calculate_reads()
    {
        off_t new_offset, l_offset;
        int l_serv;
        size_t l_size, count;

        new_offset = m_offset;
        count = 0;

        while(m_size>count)
        {
            xpn_rw::read_get_block(m_file, new_offset, l_offset, l_serv);

            // l_size is the remaining bytes from new_offset until the end of the block
            l_size = m_file.m_part.m_block_size -
                (new_offset%m_file.m_part.m_block_size);

            // If l_size > the remaining bytes to read/write, then adjust l_size
            if ((m_size - count) < l_size)
                l_size = m_size - count;

            xpn_rw_buffer::rw_buffer buff;

            buff.offset_serv = l_offset;
            buff.size = l_size;
            buff.buffer = m_buffer + count;
            buff.offset_buff = count;

            m_ops[l_serv].push_back(buff);

            count = l_size + count;
            new_offset = m_offset + count;
        }
        
        debug_info(to_string());

        join_ops();
    }

    void xpn_rw_buffer::fix_ops_reads()
    {
        for (auto &srv_ops : m_ops)
        {
            for (auto &op : srv_ops)
            {
                if (!op.was_move()) continue;
                uint64_t offset = 0;
                for (size_t i = 0; i < op.origin_buffer.size(); i++)
                {
                    std::copy(op.v_buffer.begin()+offset, op.v_buffer.begin()+offset+op.origin_buffer_size[i], op.origin_buffer[i]);
                    offset += op.origin_buffer_size[i];
                }
            }
        }
    }

    void xpn_rw_buffer::calculate_writes()
    {
        off_t new_offset, l_offset;
        int l_serv;
        size_t l_size = 0, count;

        new_offset = m_offset;
        count = 0;

        while(m_size>count)
        {
            for (int j = 0; j < m_file.m_part.m_replication_level + 1; j++)
            {
                if (xpn_rw::write_get_block(m_file, new_offset, j, l_offset, l_serv) == 0){
                    // l_size is the remaining bytes from new_offset until the end of the block
                    l_size = m_file.m_part.m_block_size -
                        (new_offset%m_file.m_part.m_block_size);

                    // If l_size > the remaining bytes to read/write, then adjust l_size
                    if ((m_size - count) < l_size)
                        l_size = m_size - count;

                    xpn_rw_buffer::rw_buffer buff;

                    buff.offset_serv = l_offset;
                    buff.size = l_size;
                    buff.buffer = m_buffer + count;
                    buff.offset_buff = count;

                    m_ops[l_serv].push_back(buff);
                }
            }
            count = l_size + count;
            new_offset = m_offset + count;
        }

        debug_info(to_string());

        join_ops();
    }
    
    void xpn_rw_buffer::join_ops()
    {
        for (auto &serv_ops : m_ops)
        {   
            if (serv_ops.size() < 2) continue;
            std::sort(serv_ops.begin(), serv_ops.end(), [](rw_buffer &a, rw_buffer &b){return a.offset_serv<b.offset_serv;});
            for (size_t i = 0; i < serv_ops.size()-1;)
            {   
                rw_buffer &actual = serv_ops[i];
                rw_buffer &next = serv_ops[i+1];
                // Check if it can be unions and not be more than MAX_BUFFER_SIZE 
                if (actual.offset_serv + static_cast<int64_t>(actual.size) == next.offset_serv && actual.size < MAX_BUFFER_SIZE){
                    if (actual.v_buffer.empty()){
                        actual.v_buffer.insert(actual.v_buffer.begin(), actual.buffer, actual.buffer+actual.size);
                        actual.origin_buffer.push_back(actual.buffer);
                        actual.origin_buffer_size.push_back(actual.size);
                    }
                    actual.size += next.size;
                    actual.v_buffer.insert(actual.v_buffer.end(), next.buffer, next.buffer+next.size);
                    actual.origin_buffer.push_back(next.buffer);
                    actual.origin_buffer_size.push_back(next.size);
                    serv_ops.erase(serv_ops.begin() + i + 1);
                }else{
                    i++;
                }
            }
        }
        
        debug_info(to_string());
    }

    size_t xpn_rw_buffer::num_ops()
    {
        size_t count = 0;
        for (auto &srv_ops : m_ops)
        {
            count += srv_ops.size();
        }
        return count;
    }

    size_t xpn_rw_buffer::size()
    {
        int count = 0;
        for (auto &srv_ops : m_ops)
        {
            for (auto &op : srv_ops)
            {
                if (op.v_buffer.empty()){
                    count += op.size;
                }else{
                    count += op.v_buffer.size();
                }
            }
        }
        return count;
    }

    std::string xpn_rw_buffer::to_string()
    {
        std::stringstream out;
        out << "xpn_rw_buffer " << m_file.m_path;
        out << " buf " << (void *)m_buffer;
        out << " size " << m_size;
        out << " off " << m_offset << std::endl;

        for (size_t i = 0; i < m_ops.size(); i++)
        {
            out << "Ops in serv " << i << ":" << std::endl;
            for (size_t j = 0; j < m_ops[i].size(); j++)
            {
                out << "    " << m_ops[i][j].to_string() << std::endl;
            }
        }

        return out.str();
    }

    std::string xpn_rw_buffer::rw_buffer::to_string()
    {
        std::stringstream out;
        out << "buf " << (void *)get_buffer();
        out << " size " << get_size();
        out << " off_serv " << offset_serv;
        out << " was_move " << (was_move() ? "true" : "false");
        return out.str();
    }

    /**
     * Calculates the server and the offset (in server) for reads of the given offset (origin file) of a file with replication.
     *
     * @param fd[in] A file descriptor.
     * @param offset[in] The original offset.
     * @param serv_client[in] To optimize: the server where the client is.
     * @param replication[in] The replication of actual offset.
     * @param local_offset[out] The offset in the server.
     * @param serv[out] The server in which is located the given offset.
     *
     * @return Returns 0 on success or -1 on error.
     */
    int xpn_rw::read_get_block(xpn_file &file, int64_t offset, int64_t &local_offset, int &serv)
    {	
        int retries = 0;
        int replication = 0;
        int replication_level = file.m_part.m_replication_level;
        if (file.m_part.m_local_serv != -1){
            do{
                file.map_offset_mdata(offset, replication, local_offset, serv);
                if (serv == file.m_part.m_local_serv && file.m_part.m_data_serv[serv]->m_error != -1 ){
                    return 0;
                }
                replication++;
            }while(replication <= replication_level);
        }
        
        replication = 0;
        if (replication_level != 0)
            replication = rand() % (replication_level + 1);

        do{
            file.map_offset_mdata(offset, replication, local_offset, serv);
            if (replication_level != 0)
                replication = (replication + 1) % (replication_level + 1);
            retries++;
        }while(file.m_part.m_data_serv[serv]->m_error == -1 && retries <= replication_level);
        
        return 0;
    }

    /**
     * Calculates the server and the offset (in server) for writes of the given offset (origin file) of a file with replication.
     *
     * @param fd[in] A file descriptor.
     * @param offset[in] The original offset.
     * @param replication[in] The replication of actual offset.
     * @param local_offset[out] The offset in the server.
     * @param serv[out] The server in which is located the given offset.
     *
     * @return Returns 0 on success or -1 on error.
     */
    int xpn_rw::write_get_block(xpn_file &file, int64_t offset, int replication, int64_t &local_offset, int &serv)
    {
        file.map_offset_mdata(offset, replication, local_offset, serv);
        return file.m_part.m_data_serv[serv]->m_error;
    }
} // namespace XPN