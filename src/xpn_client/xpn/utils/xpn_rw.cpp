
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

namespace XPN
{
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