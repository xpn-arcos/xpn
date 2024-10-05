
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

#include "xpn/xpn_file.hpp"
#include "xpn/xpn_api.hpp"

#include "base_cpp/debug.hpp"

namespace XPN
{
    // Check if serv is affected by op, in other words if it has data and have to do operations in that serv
    // For ops like open remove rename
    bool xpn_file::exist_in_serv(int serv)
    {
        int current_serv, i;
        // if not have mdata is dir so all servers affected
        if (!m_mdata.m_data.is_valid()){
            XPN_DEBUG("Serv "<<serv<<": is dir");
            return true;
        }
        int master_dir = m_mdata.master_dir();
        int n_serv = static_cast<int>(m_part.m_data_serv.size());
        // Files created for readdir in master_dir with replications
        for (i = 0; i < m_part.m_replication_level+1; i++) 
        {
            current_serv = (i + master_dir) % n_serv;
            if (current_serv == serv){
                XPN_DEBUG("Serv "<<serv<<": file in master_dir");
                return true;
            }
        }

        int master_file = m_mdata.master_file();
        // Files with metadata with replications
        for (i = 0; i < m_part.m_replication_level+1; i++) 
        {
            current_serv = (i + master_file) % n_serv;
            if (current_serv == serv){
                XPN_DEBUG("Serv "<<serv<<": file has metadata");
                return true;
            }
        }

        // For simplicity if has malleability all servers
        if (m_mdata.m_data.data_nserv[1] != 0){
            XPN_DEBUG("Serv "<<serv<<": file have malleability");
            return true;
        }

        // Calculate if has data in that server, the file must exist
        int64_t local_offset;
        int aux_serv;
        for (i = 0; i < n_serv; i++)
        {
            int64_t offset = m_mdata.m_data.block_size * i;
            if (offset > static_cast<int64_t>(m_mdata.m_data.file_size)){
                break;
            }
            for (int j = 0; j < m_mdata.m_data.replication_level+1; j++)
            {
                map_offset_mdata(offset, j, local_offset, aux_serv);
                if (aux_serv == serv){
                    XPN_DEBUG("Serv "<<serv<<": file have data in serv");
                    return true;
                }
            }
        }
        return false;
    }

    void xpn_file::map_offset(int block_size, int replication_level, int nserv, int64_t offset, int replication, int first_node, int64_t &local_offset, int &serv)
    {
        int64_t block = offset / block_size;
        int64_t block_replication = block * (replication_level + 1) + replication;
        int64_t block_line = block_replication / nserv;
        
        // Calculate the server	
        serv = (block_replication + first_node) % nserv;
        
        // Calculate the offset in the server
        local_offset = block_line * block_size + (offset % block_size);
    }

    void xpn_file::inverted_map_offset(int block_size, int replication_level, int nserv, int serv, int64_t local_offset, int first_node, int64_t &offset, int &replication)
    {
        int64_t block_line = local_offset / block_size;
        int64_t block_replication = block_line * nserv + (((serv - first_node) % nserv + nserv) % nserv);
        // round down
        int64_t block = block_replication / (replication_level + 1);
        // Calculate the offset
        offset = block * block_size + (local_offset % block_size);
        // Calculate the actual replication block
        replication = block_replication % (replication_level + 1);
    }

    void xpn_file::map_offset_mdata(int64_t offset, int replication, int64_t &local_offset, int &serv)
    {
        // Without expand or shrink
        if (m_mdata.m_data.data_nserv[1] == 0){
            map_offset(m_mdata.m_data.block_size, m_mdata.m_data.replication_level, m_mdata.m_data.data_nserv[0], offset, replication, m_mdata.m_data.first_node, local_offset, serv);
            return;
        }

        int actual_index = -1;
        int64_t block_num = offset / m_mdata.m_data.block_size;

        int is_shrink = 0;
        int is_expand = 0;
        int aux_is_shrink = 0;
        int aux_is_expand = 0;
        int have_greatter_shrink = 0;

        int64_t acum_local_offset = 0;
        int prev_data_nserv = 0;
        int64_t prev_block_num = 0;
        int aux_serv = 0;
        int64_t aux_local_offset = 0;
        int64_t array_local_offset[xpn_metadata::MAX_RECONSTURCTIONS] = {0};

        for (int i = 0; i < xpn_metadata::MAX_RECONSTURCTIONS; i++)
        {	
            // stop when there are no more reconfigurations
            if (m_mdata.m_data.data_nserv[i] == 0) break;
            // negative servers is calculated in the positive next serv
            if (m_mdata.m_data.data_nserv[i] < 0) continue;

            // calculate actual conf and prev
            is_shrink = 0;
            is_expand = 0;
            if (i > 0 && m_mdata.m_data.data_nserv[i-1] < 0){
                is_shrink = 1;
            }else{
                is_expand = 1;
            }

            // calculate if the block is in the actual data_nserv
            if (m_mdata.m_data.data_nserv[i+1] > 0){
                if (actual_index == -1 && (block_num > m_mdata.m_data.offsets[i] || block_num == 0) && (block_num <= m_mdata.m_data.offsets[i+1] || m_mdata.m_data.offsets[i+1] == 0)){
                    actual_index = i;
                }
            }else{
                if (actual_index == -1 && (block_num > m_mdata.m_data.offsets[i] || block_num == 0) && (block_num <= m_mdata.m_data.offsets[i+2] || m_mdata.m_data.offsets[i+2] == 0)){
                    actual_index = i;
                }
            }

            // check offset for expansions to add or substract
            prev_data_nserv = -1;
            if (is_expand){
                if (i-1 >= 0){
                    prev_data_nserv = m_mdata.m_data.data_nserv[i-1];
                    if (i-1 == 0){
                        prev_block_num = m_mdata.m_data.offsets[i];
                    }else{
                        prev_block_num = m_mdata.m_data.offsets[i] - m_mdata.m_data.offsets[i-1] - 1;
                    }
                    // if prev shrink count that blocks
                    if (i-2 >= 0 && m_mdata.m_data.data_nserv[i-2] < 0){
                        prev_block_num += m_mdata.m_data.offsets[i-2];
                    }
                }
            }else{
                if (i-2 >= 0){
                    prev_data_nserv = m_mdata.m_data.data_nserv[i-2];
                    if (i-2 == 0){
                        prev_block_num = m_mdata.m_data.offsets[i];
                    }else{
                        prev_block_num = m_mdata.m_data.offsets[i] - m_mdata.m_data.offsets[i-2] - 1;
                    }
                    // if prev shrink count that blocks
                    if (i-3 >= 0 && m_mdata.m_data.data_nserv[i-3] < 0){
                        prev_block_num += m_mdata.m_data.offsets[i-3];
                    }
                }
            }

            // Calculate offset of segment
            if (prev_data_nserv != -1){
                map_offset(m_mdata.m_data.block_size, m_mdata.m_data.replication_level, prev_data_nserv, prev_block_num*m_mdata.m_data.block_size, m_mdata.m_data.replication_level, m_mdata.m_data.first_node, aux_local_offset, aux_serv);
                acum_local_offset += aux_local_offset + m_mdata.m_data.block_size; 
                array_local_offset[i] = acum_local_offset;
            }

            

            // Calculate local_offset and serv
            if (actual_index != -1 && i == actual_index){
                // change offset to new segment of blocks
                if (i != 0){
                    offset-= m_mdata.m_data.block_size*(m_mdata.m_data.offsets[i] + 1);
                    if (is_shrink){
                        offset+= m_mdata.m_data.block_size*(m_mdata.m_data.offsets[i-1]);
                    }
                }
                map_offset(m_mdata.m_data.block_size, m_mdata.m_data.replication_level, m_mdata.m_data.data_nserv[i], offset, replication, m_mdata.m_data.first_node, local_offset, serv);
                // Add offset
                local_offset += array_local_offset[i];
                // Substract offset if necesary
                for (int j = i; j >= 0; j--)
                {

                    if (m_mdata.m_data.data_nserv[j] == 0) break;
                    aux_is_shrink = 0;
                    aux_is_expand = 0;
                    if (j > 0 && m_mdata.m_data.data_nserv[j-1] < 0){
                        aux_is_shrink = 1;
                    }else{
                        aux_is_expand = 1;
                    }
                    
                    if (aux_is_expand == 1){
                        if (j-1 >= 0 && (serv) > (m_mdata.m_data.data_nserv[j-1] - 1) && (serv) <= (m_mdata.m_data.data_nserv[j] - 1)){
                            local_offset -= array_local_offset[j];
                            break;
                        }
                    }
                    else if (aux_is_shrink == 1){
                        if (j-3 >= 0 && m_mdata.m_data.data_nserv[j-3] > 0)
                        if (j-2 >= 0 && (serv) == (m_mdata.m_data.data_nserv[j] - 1)){
                            local_offset -= array_local_offset[j-2];
                            break;
                        }
                    }
                }


            }else{
                if (is_shrink){
                    // Recalculate to reconfigure shrink
                    if ((serv) == (abs(m_mdata.m_data.data_nserv[i-1]) - 1)){
                        map_offset(m_mdata.m_data.block_size, m_mdata.m_data.replication_level, m_mdata.m_data.data_nserv[i], local_offset, replication, m_mdata.m_data.first_node, local_offset, serv);
                        
                        // Add or substract offset
                        local_offset += array_local_offset[i];
                        aux_local_offset = local_offset;

                        // Reduce offset
                        if (i-3 >= 0 && m_mdata.m_data.data_nserv[i-3] > 0)
                        if (i-2 >= 0 && (serv) == (m_mdata.m_data.data_nserv[i] - 1)){
                            local_offset -= array_local_offset[i-2];
                        }
                        
                    }else if ((serv) > (abs(m_mdata.m_data.data_nserv[i-1]) - 1)){
                        (serv)--;
                    }
                }
            }
            

            // if shrink greater than the actual config change block
            have_greatter_shrink = 0;
            for (int j = i; j < xpn_metadata::MAX_RECONSTURCTIONS; j++)
            {
                if (m_mdata.m_data.data_nserv[j] == 0) break;
                if (m_mdata.m_data.data_nserv[j] < 0) have_greatter_shrink = 1;
            }

            // if not greatter shrink and block calculated break
            if (i == actual_index && have_greatter_shrink == 0){
                break;
            }
        }
    }

    int xpn_file::initialize_vfh(int index)
    {
        int res = 0;
        if (m_data_vfh[index].is_initialized()){
            return res;
        }
        XPN_DEBUG_BEGIN_CUSTOM(index<<", "<<m_path);
        if (index < 0 || index >= static_cast<int>(m_data_vfh.size())){
            res = -1;
            XPN_DEBUG_END_CUSTOM(index<<", "<<m_path);
            return res;
        }
        auto& api = xpn_api::get_instance();
        api.m_worker->launch([&res, this, index](){
            res = this->m_part.m_data_serv[index]->nfi_open(this->m_path, O_RDWR | O_CREAT, S_IRWXU, this->m_data_vfh[index]);
        });

        api.m_worker->wait();
        
        XPN_DEBUG_END_CUSTOM(index<<", "<<m_path);
        return res;
    }

    int xpn_file::initialize_vfh_dir(int index)
    {
        int res = 0;
        XPN_DEBUG_BEGIN_CUSTOM(index<<", "<<m_path);
        if (m_data_vfh[index].is_initialized()){
            XPN_DEBUG_END_CUSTOM(index<<", "<<m_path);
            return res;
        }
        if (index < 0 || index >= static_cast<int>(m_data_vfh.size())){
            res = -1;
            XPN_DEBUG_END_CUSTOM(index<<", "<<m_path);
            return res;
        }
        auto& api = xpn_api::get_instance();
        api.m_worker->launch([&res, this, index](){
            res = this->m_part.m_data_serv[index]->nfi_opendir(this->m_path, this->m_data_vfh[index]);
        });

        api.m_worker->wait();
        
        XPN_DEBUG_END_CUSTOM(index<<", "<<m_path);
        return res;
    }
} // namespace XPN