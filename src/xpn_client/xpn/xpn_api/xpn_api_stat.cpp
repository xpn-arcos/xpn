
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

        auto& file = m_file_table.get(fd);
        if (!file)
        {
            errno = EBADF;
            XPN_DEBUG_END_CUSTOM(fd);
            return -1;
        }

        // Redirect to stat to not duplicate code
        res = stat(file->m_path.c_str(), sb);

        XPN_DEBUG_END_CUSTOM(fd);
        return res;
    }

    int xpn_api::stat(const char *path, struct ::stat *sb)
    {
        XPN_DEBUG_BEGIN_CUSTOM(path);
        int res = 0;

        auto file = create_file_from_part_path(path);
        if (!file){
            errno = ENOENT;
            XPN_DEBUG_END_CUSTOM(path);
            return -1;
        }

        if (read_metadata(file->m_mdata) < 0){
            XPN_DEBUG_END_CUSTOM(path);
            return -1;
        }
        
        auto& server = file->m_part.m_data_serv[file->m_mdata.master_file()];

        m_worker->launch([&server, &file, sb, &res](){res = server->nfi_getattr(file->m_path, *sb);});

        m_worker->wait();

        // Update file_size
        if (S_ISREG(sb->st_mode)){
            sb->st_size = file->m_mdata.m_data.file_size;
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

    int xpn_api::dup([[maybe_unused]] int fd)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        // TODO
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::dup2([[maybe_unused]] int fd, [[maybe_unused]] int fd2)
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

        auto file = create_file_from_part_path(path);
        if (file)
        {
            errno = ENOENT;
            res = -1;
            XPN_DEBUG_END_CUSTOM(path);
            return res;
        }

        auto& server = file->m_part.m_data_serv[file->m_mdata.master_file()];
                
        m_worker->launch([&server, &file, buf, &res](){res = server->nfi_statvfs(file->m_path, *buf);});

        std::vector<int> v_res(file->m_part.m_data_serv.size());
        for (size_t i = 0; i < file->m_part.m_data_serv.size(); i++)
        {
            if (static_cast<int>(i) == file->m_mdata.master_file()) continue;
            struct ::statvfs aux_buf;
            m_worker->launch([i, &server, &file, &aux_buf, &v_res](){v_res[i] = server->nfi_statvfs(file->m_path, aux_buf);});
            buf->f_blocks += aux_buf.f_blocks;
            buf->f_bfree += aux_buf.f_bfree;
            buf->f_bavail += aux_buf.f_bavail;
        }
        
        m_worker->wait();

        for (auto &aux_res : v_res)
        {
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

        auto& file = m_file_table.get(fd);
        if (!file)
        {
            errno = EBADF;
            XPN_DEBUG_END_CUSTOM(fd);
            return -1;
        }

        // Redirect to statvfs to not duplicate code
        res = statvfs(file->m_path.c_str(), buf);

        XPN_DEBUG_END;
        return res;
    }
} // namespace XPN
