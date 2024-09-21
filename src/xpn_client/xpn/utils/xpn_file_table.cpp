
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

#include "xpn/xpn_file_table.hpp"
#include "xpn/xpn_api.hpp"

namespace XPN
{
    int xpn_file_table::insert(const xpn_file& file)
    {
        int fd;
        if (m_free_keys.empty()){
            fd = secuencial_key++;
        }else{
            fd = m_free_keys.front();
            m_free_keys.pop();
        }
        auto pair = std::make_pair(fd, new xpn_file(file));
        pair.second->m_links++;
        m_files.insert(pair);
        return fd;
    }

    int xpn_file_table::insert(xpn_file* file)
    {
        int fd;
        if (m_free_keys.empty()){
            fd = secuencial_key++;
        }else{
            fd = m_free_keys.front();
            m_free_keys.pop();
        }
        auto pair = std::make_pair(fd, file);
        pair.second->m_links++;
        m_files.insert(pair);
        return fd;
    }

    bool xpn_file_table::remove(int fd)
    {   
        bool has_fd = has(fd);
        if (!has_fd){
            return false;
        }
        auto file = m_files.at(fd);
        int res = m_files.erase(fd);
        if (res == 1){
            m_free_keys.push(fd);
        }
        file->m_links--;
        if (file->m_links <= 0){
            delete file;
        }
        return res == 1 ? true : false;
    }

    // It must be checked if fd is in the file_table with has(fd)
    int xpn_file_table::dup(int fd, int new_fd)
    {   
        int ret = -1;
        auto file = m_files.at(fd);
        file->m_links++;
        if (new_fd != -1){
            // Like posix dup2 close silently if its open
            if (has(new_fd)){
                xpn_api::get_instance().close(new_fd);
            }
            auto pair = std::make_pair(new_fd, file);
            pair.second->m_links++;
            m_files.insert(pair);
            ret = new_fd;
        }else{
            ret = insert(file);
        }
        return ret;
    }

    std::string xpn_file_table::to_string()
    {
        std::stringstream out;
        for (auto &[key, file] : m_files)
        {
            out << "fd: " << key << " : " << (*file).m_path << std::endl;
        }
        return out.str();
    }
}