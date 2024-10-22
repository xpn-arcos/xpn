
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
#include "base_cpp/xpn_path.hpp"

namespace XPN
{
    int xpn_api::fstat(int fd, struct ::stat *sb)
    {
        XPN_DEBUG_BEGIN_CUSTOM(fd);
        int res = 0;

        if (!m_file_table.has(fd))
        {
            errno = EBADF;
            XPN_DEBUG_END_CUSTOM(fd);
            return -1;
        }
        auto& file = m_file_table.get(fd);

        // Redirect to stat to not duplicate code
        std::string file_path = file.m_part.m_name + "/" + file.m_path;
        res = stat(file_path.c_str(), sb);

        XPN_DEBUG_END_CUSTOM(fd);
        return res;
    }

    int xpn_api::stat(const char *path, struct ::stat *sb)
    {
        XPN_DEBUG_BEGIN_CUSTOM(path);
        int res = 0;
        std::string file_path;
        auto name_part = check_remove_part_from_path(path, file_path);
        if (name_part.empty()){
            errno = ENOENT;
            XPN_DEBUG_END_CUSTOM(path);
            return -1;
        }

        xpn_file file(file_path, m_partitions.at(name_part));

        if (read_metadata(file.m_mdata) < 0){
            XPN_DEBUG_END_CUSTOM(path);
            return -1;
        }
        
        auto& server = file.m_part.m_data_serv[file.m_mdata.master_file()];

        auto fut = m_worker->launch([&server, &file, sb](){
            return server->nfi_getattr(file.m_path, *sb);
        });

        res = fut.get();

        // Update file_size
        if (S_ISREG(sb->st_mode)){
            sb->st_size = file.m_mdata.m_data.file_size;
        }

        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::chown([[maybe_unused]] const char *path, [[maybe_unused]] uid_t owner, [[maybe_unused]] gid_t group)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        // TODO
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::fchown([[maybe_unused]] int fd, [[maybe_unused]] uid_t owner, [[maybe_unused]] gid_t group)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        // TODO
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::chmod([[maybe_unused]] const char *path, [[maybe_unused]] mode_t mode)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        // TODO
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::fchmod([[maybe_unused]] int fd, [[maybe_unused]] mode_t mode)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        // TODO
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::truncate([[maybe_unused]] const char *path, [[maybe_unused]] off_t length)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        // TODO
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::ftruncate([[maybe_unused]] int fd, [[maybe_unused]] off_t length)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        // TODO
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::statvfs(const char * path, struct ::statvfs *buf)
    {
        XPN_DEBUG_BEGIN_CUSTOM(path);
        int res = 0;

        std::string file_path;
        auto name_part = check_remove_part_from_path(path, file_path);
        if (name_part.empty()){
            errno = ENOENT;
            res = -1;
            XPN_DEBUG_END_CUSTOM(path);
            return res;
        }

        xpn_file file(file_path, m_partitions.at(name_part));

        auto& server = file.m_part.m_data_serv[file.m_mdata.master_file()];
                
        auto fut = m_worker->launch([&server, &file, buf](){
            return server->nfi_statvfs(file.m_path, *buf);
        });

        res = fut.get();
        if (res < 0){
            XPN_DEBUG_END_CUSTOM(path);
            return res;
        }

        std::vector<std::future<int>> v_res(file.m_part.m_data_serv.size());
        std::mutex buff_mutex;
        for (size_t i = 0; i < file.m_part.m_data_serv.size(); i++)
        {
            if (static_cast<int>(i) == file.m_mdata.master_file()) continue;
            v_res[i] = m_worker->launch([i, &file, &buf, &buff_mutex](){
                struct ::statvfs aux_buf;
                int res = file.m_part.m_data_serv[i]->nfi_statvfs(file.m_path, aux_buf);
                std::unique_lock lock(buff_mutex);
                if (res >= 0){
                    buf->f_blocks += aux_buf.f_blocks;
                    buf->f_bfree += aux_buf.f_bfree;
                    buf->f_bavail += aux_buf.f_bavail;
                    
                    buf->f_files += aux_buf.f_files;
                    buf->f_ffree += aux_buf.f_ffree;
                    buf->f_favail += aux_buf.f_favail;
                }
                return res;
            });
        }

        int aux_res;
        for (auto &fut : v_res)
        {
            if (!fut.valid()) continue;
            aux_res = fut.get();
            if (aux_res < 0){
                res = aux_res;
            }
        }

        XPN_DEBUG_END_CUSTOM(path);
        return res;
    }

    int xpn_api::fstatvfs(int fd, struct ::statvfs *buf)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;

        if (!m_file_table.has(fd))
        {
            errno = EBADF;
            XPN_DEBUG_END_CUSTOM(fd);
            return -1;
        }
        auto& file = m_file_table.get(fd);

        // Redirect to statvfs to not duplicate code
        std::string file_path = file.m_part.m_name + "/" + file.m_path;
        res = statvfs(file_path.c_str(), buf);

        XPN_DEBUG_END;
        return res;
    }
} // namespace XPN
