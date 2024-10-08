
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

#include <dirent.h>

// Define struct
struct __dirstream
{
    int fd;                       // File descriptor.
    //__libc_lock_define (, lock) // Mutex lock for this structure. //TODO
    size_t allocation;            // Space allocated for the block.
    size_t size;                  // Total valid data in the block.
    size_t offset;                // Current offset into the block.
    off_t  filepos;               // Position of next entry to read.
    /* Directory block.  */
    char data[0] __attribute__ ((aligned (__alignof__ (void*))));

    char * path;
};

namespace XPN
{
    DIR *xpn_api::opendir(const char *path)
    {
        XPN_DEBUG_BEGIN_CUSTOM(path);
        int res = 0;
        DIR *dirp = nullptr;
        
        std::string path_aux(path);
        path_aux += "/";

        res = open(path_aux.c_str(), O_RDONLY | O_DIRECTORY, 0);
        if (res < 0)
        {
            XPN_DEBUG_END_CUSTOM(path);
            return NULL;
        }

        dirp = new (std::nothrow) DIR;
        if (dirp == nullptr)
        {
            XPN_DEBUG_END_CUSTOM(path);
            return nullptr;
        }

        // fill the dirp info
        dirp->fd   = res;
        dirp->path = strdup(path_aux.c_str());

        XPN_DEBUG_END_CUSTOM(path);
        return dirp;
    }

    int xpn_api::closedir(DIR *dirp)
    {
        XPN_DEBUG_BEGIN_CUSTOM(dirp->fd);
        int res = 0;
        
        if((NULL == dirp)||(!m_file_table.has(dirp->fd))){
            errno = EINVAL;
            return -1;
        }

        auto& file = m_file_table.get(dirp->fd);
        XPN_DEBUG("Close : '"<<file.m_path<<"'")
        std::vector<std::future<int>> v_res(file.m_data_vfh.size());
        for (size_t i = 0; i < file.m_data_vfh.size(); i++)
        {
            if (file.m_data_vfh[i].is_initialized()){
                v_res[i] = m_worker->launch([i, &file](){
                    return file.m_part.m_data_serv[i]->nfi_closedir(file.m_data_vfh[i]);
                });
            }
        }
        
        int aux_res;
        for (auto &fut : v_res)
        {   
            if (!fut.valid()) continue;
            aux_res = fut.get();
            if (aux_res < 0)
            {
                res = aux_res;
            }
        }

        m_file_table.remove(dirp->fd);

        delete dirp->path;
        delete dirp;

        XPN_DEBUG_END;
        return res;
    }

    struct ::dirent *xpn_api::readdir(DIR *dirp)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        if((NULL == dirp)||(!m_file_table.has(dirp->fd))){
            errno = EINVAL;
            return nullptr;
        }

        auto& file = m_file_table.get(dirp->fd);

        int master_dir = file.m_mdata.master_dir();
        file.initialize_vfh_dir(master_dir);

        struct ::dirent * entry = new (std::nothrow) dirent;
        if (entry == nullptr){
            return nullptr;
        }
        auto fut = m_worker->launch([master_dir, &file, &entry](){
            return file.m_part.m_data_serv[master_dir]->nfi_readdir(file.m_data_vfh[master_dir], *entry);
        });

        res = fut.get();

        if (res < 0){
            XPN_DEBUG_END;
            delete entry;
            return nullptr;
        }

        XPN_DEBUG_END;
        return entry;
    }

    void xpn_api::rewinddir([[maybe_unused]] DIR *dirp)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        // TODO
        XPN_DEBUG_END;
    }

    int xpn_api::mkdir(const char *path, mode_t perm)
    {
        XPN_DEBUG_BEGIN_CUSTOM(path<<", "<<perm);
        int res = 0;
        
        if(path == NULL)
        {
            errno = EINVAL;
            res = -1;
            XPN_DEBUG_END_CUSTOM(path<<", "<<perm);
            return res;
        }

        std::string file_path;
        auto part_name = check_remove_part_from_path(path, file_path);
        if (part_name.empty()){
            errno = ENOENT;
            res = -1;
            XPN_DEBUG_END_CUSTOM(path<<", "<<perm);
            return res;
        }

        xpn_file file(file_path, m_partitions.at(part_name));

        std::vector<std::future<int>> v_res(file.m_part.m_data_serv.size());
        for (size_t i = 0; i < file.m_part.m_data_serv.size(); i++)
        {
            auto& serv = file.m_part.m_data_serv[i];
            v_res[i] = m_worker->launch([&serv, &file, perm](){
                return serv->nfi_mkdir(file.m_path, perm);
            });
        }

        int aux_res;
        for (auto &fut : v_res)
        {   
            aux_res = fut.get();
            if (aux_res < 0)
            {
                res = aux_res;
            }
        }

        XPN_DEBUG_END_CUSTOM(path<<", "<<perm);
        return res;
    }

    int xpn_api::rmdir(const char *path)
    {
        XPN_DEBUG_BEGIN_CUSTOM(path);
        int res = 0;
        
        if(path == NULL)
        {
            errno = EINVAL;
            res = -1;
            XPN_DEBUG_END_CUSTOM(path);
            return res;
        }

        std::string file_path;
        auto part_name = check_remove_part_from_path(path, file_path);
        if (part_name.empty()){
            errno = ENOENT;
            res = -1;
            XPN_DEBUG_END_CUSTOM(path);
            return res;
        }

        xpn_file file(file_path, m_partitions.at(part_name));

        std::vector<std::future<int>> v_res(file.m_part.m_data_serv.size());
        for (size_t i = 0; i < file.m_part.m_data_serv.size(); i++)
        {
            auto& serv = file.m_part.m_data_serv[i];
            v_res[i] = m_worker->launch([&serv, &file](){
                // Always wait and not async because it can fail in other ways
                return serv->nfi_rmdir(file.m_path, false);
                // v_res[i] = serv->nfi_rmdir(file.m_path, file.m_mdata.master_file()==static_cast<int>(i));
            });
        }
        
        int aux_res;
        for (auto &fut : v_res)
        {   
            aux_res = fut.get();
            if (aux_res < 0)
            {
                res = aux_res;
            }
        }

        XPN_DEBUG_END_CUSTOM(path);
        return res;
    }

} // namespace XPN
