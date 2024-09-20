
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

#include "base_cpp/debug.hpp"
#include "xpn/xpn_api.hpp"

#include <iostream>

namespace XPN
{
    #ifndef O_WRONLY
    #define O_WRONLY             01
    #endif
    #ifndef O_CREAT
    #define O_CREAT            0100
    #endif
    #ifndef O_TRUNC
    #define O_TRUNC           01000
    #endif
    #ifndef O_DIRECTORY
    #define O_DIRECTORY    00200000
    #endif

    std::unique_ptr<xpn_file> xpn_api::create_file_from_part_path(const std::string &path)
    {
        std::string name_part = xpn_path::get_first_dir(path);
        auto it = m_partitions.find(name_part);
        if (it == m_partitions.end())
        {
            return nullptr;
        }
        xpn_partition& part = it->second;

        return std::make_unique<xpn_file>(xpn_path::remove_first_dir(path), part);
    }

    int xpn_api::open(const char *path, int flags, mode_t mode)
    {
        XPN_DEBUG_BEGIN_CUSTOM(path<<", "<<flags<<", "<<mode);
        int res = 0;

        auto file = create_file_from_part_path(path);
        if (!file){
            errno = ENOENT;
            XPN_DEBUG_END_CUSTOM(path<<", "<<flags<<", "<<mode);
            return -1;
        }

        if ((O_DIRECTORY != (flags & O_DIRECTORY)))
        {
            int ret = read_metadata(file->m_mdata);
            if (res < 0 && O_CREAT != (flags & O_CREAT)){
                XPN_DEBUG_END_CUSTOM(path<<", "<<flags<<", "<<mode);
                return -1;
            }
        }

        if ((O_CREAT == (flags & O_CREAT))){
            std::vector<int> v_res(file->m_part.m_data_serv.size());
            for (size_t i = 0; i < file->m_part.m_data_serv.size(); i++)
            {
                auto& serv = file->m_part.m_data_serv[i];
                m_worker->launch([i, &v_res, &serv, &file, flags, mode](){
                    v_res[i] = serv->nfi_open(file->m_path, flags, mode, file->m_data_vfh[i]);
                });
            }

            m_worker->wait();

            for (auto &aux_res : v_res)
            {
                if (aux_res < 0)
                {
                    XPN_DEBUG_END_CUSTOM(path<<", "<<flags<<", "<<mode);
                    return -1;
                }
            }

            if ((O_DIRECTORY != (flags & O_DIRECTORY)))
            {
                write_metadata(file->m_mdata, false);
            }
        }else{
            m_worker->launch([&res, &file, flags, mode](){
                int master_file = file->m_mdata.master_file();
                res = file->m_part.m_data_serv[master_file]->nfi_open(file->m_path, flags, mode, file->m_data_vfh[master_file]);
            });
            m_worker->wait();
        }

        res = m_file_table.insert(std::move(file));

        XPN_DEBUG_END_CUSTOM(path<<", "<<flags<<", "<<mode);
        return res;
    }

    int xpn_api::creat(const char *path, mode_t perm)
    {
        XPN_DEBUG_BEGIN_CUSTOM(path<<", "<<perm);
        // Like in unix we use the open with flags
        int res = open(path, O_WRONLY|O_CREAT|O_TRUNC, perm);
        XPN_DEBUG_END_CUSTOM(path<<", "<<perm);
        return res;
    }

    int xpn_api::close(int fd)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;

        

        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::unlink(const char *path)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::rename(const char *path, const char *newpath)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }
} // namespace XPN
