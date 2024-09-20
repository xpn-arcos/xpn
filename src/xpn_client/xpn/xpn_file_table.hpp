
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

#pragma once

#include <queue>
#include <unordered_map>

#include "xpn_file.hpp"

namespace XPN
{
    class xpn_file_table
    {
    public:
        static constexpr const int MAX_FILES = 1000000;
    public:
        bool has(int fd)
        {
            return m_files.find(fd) != m_files.end();
        }

        int insert(std::unique_ptr<xpn_file> file)
        {
            int fd;
            if (m_free_keys.empty()){
                fd = secuencial_key++;
            }else{
                fd = m_free_keys.front();
                m_free_keys.pop();
            }
            m_files[fd] = std::move(file);
            return fd;
        }

        std::unique_ptr<xpn_file>& get(int fd)
        {
            return m_files[fd]; 
        }

        bool remove(int fd)
        {   
            int res = m_files.erase(fd);
            if (res == 1){
                m_free_keys.push(fd);
            }
            return res == 1 ? true : false;
        }

        std::string to_string()
        {
            std::stringstream out;
            for (auto &[key, file] : m_files)
            {
                out << "fd: " << key << " : " << file->m_path << std::endl;
            }
            return out.str();
        }

    private:
        std::unordered_map<int, std::unique_ptr<xpn_file>> m_files;
        std::queue<int> m_free_keys;
        int secuencial_key = 1;
    };
} // namespace XPN