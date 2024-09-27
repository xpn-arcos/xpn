
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

    std::string xpn_metadata::to_string(bool with_data){
        std::stringstream out;
        out << "Metadata of: " << m_file.m_path << std::endl;
        if (with_data){
            out << m_data.to_string();
        }
        return out.str();
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
} // namespace XPN
