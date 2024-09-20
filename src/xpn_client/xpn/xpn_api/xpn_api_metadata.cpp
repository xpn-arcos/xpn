
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

namespace XPN
{
    int xpn_api::read_metadata(xpn_metadata &mdata)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        
        m_worker->launch([&res, &mdata](){
            res = mdata.m_part.m_data_serv[mdata.master_file()]->nfi_read_mdata(mdata.m_path, mdata);
        });

        m_worker->wait();

        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::write_metadata(xpn_metadata &mdata, bool only_file_size)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;

        m_worker->launch([&res, &mdata, only_file_size](){
            res = mdata.m_part.m_data_serv[mdata.master_file()]->nfi_write_mdata(mdata.m_path, mdata, only_file_size);
        });

        m_worker->wait();

        XPN_DEBUG_END;
        return res;
    }
} // namespace XPN
