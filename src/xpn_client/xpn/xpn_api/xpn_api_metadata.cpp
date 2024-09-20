
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

#include "xpn/xpn_api.hpp"

#include <vector>

namespace XPN
{
    int xpn_api::read_metadata(xpn_metadata &mdata)
    {
        XPN_DEBUG_BEGIN_CUSTOM(mdata.m_file.m_path);
        int res = 0;
        
        m_worker->launch([&res, &mdata](){
            res = mdata.m_file.m_part.m_data_serv[mdata.master_file()]->nfi_read_mdata(mdata.m_file.m_path, mdata);
        });

        m_worker->wait();

        XPN_DEBUG(mdata.to_string());

        XPN_DEBUG_END_CUSTOM(mdata.m_file.m_path);
        return res;
    }

    int xpn_api::write_metadata(xpn_metadata &mdata, bool only_file_size)
    {
        XPN_DEBUG_BEGIN_CUSTOM(mdata.m_file.m_path<<", "<<only_file_size);
        int res = 0;
        if (only_file_size){
            XPN_DEBUG(mdata.m_data.file_size);
        }else{
            XPN_DEBUG(mdata.to_string());
        }

        int server = xpn_path::hash(mdata.m_file.m_path, mdata.m_file.m_part.m_data_serv.size(), true);
        std::vector<int> v_res(mdata.m_file.m_part.m_replication_level+1);
        for (int i = 0; i < mdata.m_file.m_part.m_replication_level+1; i++)
        {
            server = (server+i) % mdata.m_file.m_part.m_data_serv.size();
            if (mdata.m_file.m_part.m_data_serv[server]->m_error != -1){
                m_worker->launch([i, &v_res, &mdata, only_file_size](){
                    v_res[i] = mdata.m_file.m_part.m_data_serv[mdata.master_file()]->nfi_write_mdata(mdata.m_file.m_path, mdata, only_file_size);
                });
            }
        }

        m_worker->wait();

        for (auto &aux_res : v_res)
        {
            if (aux_res < 0){
                res = aux_res;
            }
        }

        XPN_DEBUG_END_CUSTOM(mdata.m_file.m_path<<", "<<only_file_size);
        return res;
    }
} // namespace XPN
