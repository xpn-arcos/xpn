
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

#include "xpn/xpn_metadata.hpp"
#include "xpn/xpn_file.hpp"

#include <iostream>
#include <string>
#include <iomanip>
#include <map>

namespace XPN
{
    
    void xpn_metadata::data::fill(const xpn_metadata& mdata){
        magic_number        = {MAGIC_NUMBER[0], MAGIC_NUMBER[1], MAGIC_NUMBER[2]};
        data_nserv[0]       = static_cast<int>(mdata.m_file.m_part.m_data_serv.size());
        version             = VERSION;
        type                = 0;
        block_size          = mdata.m_file.m_part.m_block_size;
        replication_level   = mdata.m_file.m_part.m_replication_level;
        first_node          = mdata.master_file();
        distribution_policy = DISTRIBUTION_ROUND_ROBIN;
    }
    
    int xpn_metadata::calculate_master(bool is_file) const
    {
        int master = xpn_path::hash(m_file.m_path, m_file.m_part.m_data_serv.size(), is_file);
        for (int i = 0; i < m_file.m_part.m_replication_level; i++)
        {
            master = (master+1)%m_file.m_part.m_data_serv.size();
            if (m_file.m_part.m_data_serv[master]->m_error != -1){
                break;
            }
        }
        return master;
    }

    std::string xpn_metadata::to_string(bool with_data){
        std::stringstream out;
        out << "Metadata of: " << m_file.m_path << std::endl;
        if (with_data){
            out << m_data.to_string();
        }
        return out.str();
    }

    std::string xpn_metadata::to_string_blocks(int blocks)
    {
        off_t offset, local_offset;
        int serv;
        int check_sum_1 = 0, check_sum_2 = 0;
        
        std::map<int, std::vector<int>> queues;
        
        for (int i = 0; i < blocks; i++)
        {
            offset = i * m_data.block_size;
            check_sum_1 += i;
            for (int j = 0; j < (m_data.replication_level+1); j++)
            {   
                m_file.map_offset_mdata(offset, j, local_offset, serv);
                local_offset /= m_data.block_size;
                auto &vec = queues[serv];
                // Resize in case it does not fit
                if (vec.size() <= static_cast<size_t>(local_offset)) vec.resize(local_offset+1, -1);
                vec[local_offset] = i;
            }
        }	

        std::stringstream out;
        out << "Header" << std::endl;
        // Header
        for (auto &[key, vec] : queues)
        {
            out << "Serv " << std::setw(2) << key;
            if (key != static_cast<int>(queues.size())-1) out << " | ";
        }
        out << std::endl;
        for (auto &[key, vec] : queues)
        {
            out << "-------";
            if (key !=  static_cast<int>(queues.size())-1) out << " | ";
        }
        out << std::endl;


        // Body
        size_t max_ops = 1000;
        size_t count = 0;
        bool finish = false;
        while(count < max_ops && !finish){
            finish = true;
            for (auto &[key, vec] : queues)
            {
                if (count < vec.size()){
                    out << std::setw(7) << vec[count];
                    check_sum_2+=vec[count];
                    finish = false;
                }else{
                    out << std::setw(7) << "";
                }
                if (key !=  static_cast<int>(queues.size())-1) out << " | ";
            }
            out << std::endl;
            count++;
        }

        if (check_sum_1 == check_sum_2){
            out << "Correct: All blocks are present" << std::endl;
        }else{
            out << "Error: not all blocks are present" << std::endl;
        }

        return out.str();
    }
} // namespace XPN
