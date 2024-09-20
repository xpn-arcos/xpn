
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
#include "xpn/xpn_rw.hpp"
#include <iomanip>

namespace XPN
{
    ssize_t xpn_api::read(int fd, void *buffer, size_t size)
    {
        XPN_DEBUG_BEGIN_CUSTOM(fd<<", "<<buffer<<", "<<size);
        int res = 0;

        if (!m_file_table.has(fd)){
            errno = EBADF;
            res = -1;
            XPN_DEBUG_END_CUSTOM(fd<<", "<<buffer<<", "<<size);
            return res;
        }

        auto& file = m_file_table.get(fd);

        if (buffer == NULL){
            errno = EFAULT;
            XPN_DEBUG_END_CUSTOM(fd<<", "<<buffer<<", "<<size);
            return -1;
        }

        if (size == 0){
            XPN_DEBUG_END_CUSTOM(fd<<", "<<buffer<<", "<<size);
            return 0;
        }

        if (file.m_flags == O_WRONLY){
            errno = EBADF;
            XPN_DEBUG_END_CUSTOM(fd<<", "<<buffer<<", "<<size);
            return -1;
        }

        if (file.m_type == file_type::dir){
            errno = EISDIR;
            XPN_DEBUG_END_CUSTOM(fd<<", "<<buffer<<", "<<size);
            return -1;
        }

        if (size > file.m_part.m_block_size){
            res = parallel_read(file, buffer, size);
        }else {
            res = secuencial_read(file, buffer, size);
        }

        XPN_DEBUG_END_CUSTOM(fd<<", "<<buffer<<", "<<size);
        return res;
    }

    ssize_t xpn_api::secuencial_read (xpn_file &file, void *buffer, size_t _size)
    {
        ssize_t size = static_cast<ssize_t>(_size);
        XPN_DEBUG_BEGIN_CUSTOM(file.m_path<<", "<<buffer<<", "<<size);
        ssize_t res = 0;

        ssize_t new_offset = file.m_offset;
        ssize_t count = 0;
        ssize_t l_offset = 0;
        int l_serv = 0;
        ssize_t l_size;

        do
        {
            xpn_rw::read_get_block(file, new_offset, l_offset, l_serv);

            l_size = file.m_part.m_block_size - (new_offset%file.m_part.m_block_size);

            // If l_size > the remaining bytes to read/write, then adjust l_size
            if ((size - count) < l_size){
                l_size = size - count;
            }

            file.initialize_vfh(l_serv);
            m_worker->launch([&res, &file, l_serv, buffer, count, l_offset, l_size](){
                res = file.m_part.m_data_serv[l_serv]->nfi_read(file.m_data_vfh[l_serv], (char *)buffer + count, l_offset+xpn_metadata::HEADER_SIZE, l_size);
            });
            m_worker->wait();

            if (res < 0) {
                count = (0 == count) ? -1 : count;
            }else{
                count = count + res;
                new_offset = file.m_offset + count;
            }
        }
        while((size > count) && (res > 0));

        if(count > 0){
            file.m_offset += count;
        }

        res = count;
        XPN_DEBUG_END_CUSTOM(file.m_path<<", "<<buffer<<", "<<size);
        return res;
    }

    ssize_t xpn_api::parallel_read   (xpn_file &file, void *buffer, size_t size)
    {
        XPN_DEBUG_BEGIN_CUSTOM(file.m_path<<", "<<buffer<<", "<<size);
        int res = 0;
        // TODO
        XPN_DEBUG_END_CUSTOM(file.m_path<<", "<<buffer<<", "<<size);
        return size;
    }

    ssize_t xpn_api::write(int fd, const void *buffer, size_t size)
    {
        XPN_DEBUG_BEGIN_CUSTOM(fd<<", "<<buffer<<", "<<size);
        int res = 0;

        if (!m_file_table.has(fd)){
            errno = EBADF;
            res = -1;
            XPN_DEBUG_END_CUSTOM(fd<<", "<<buffer<<", "<<size);
            return res;
        }

        auto& file = m_file_table.get(fd);

        if (buffer == NULL){
            errno = EFAULT;
            XPN_DEBUG_END_CUSTOM(fd<<", "<<buffer<<", "<<size);
            return -1;
        }

        if (size == 0){
            XPN_DEBUG_END_CUSTOM(fd<<", "<<buffer<<", "<<size);
            return 0;
        }

        if (file.m_flags == O_RDONLY){
            errno = EBADF;
            XPN_DEBUG_END_CUSTOM(fd<<", "<<buffer<<", "<<size);
            return -1;
        }

        if (file.m_type == file_type::dir){
            errno = EISDIR;
            XPN_DEBUG_END_CUSTOM(fd<<", "<<buffer<<", "<<size);
            return -1;
        }
        XPN_DEBUG("Path file: "<<file.m_path);
        if (size > file.m_part.m_block_size){
            res = parallel_write(file, buffer, size);
        }else {
            res = secuencial_write(file, buffer, size);
        }

        XPN_DEBUG_END_CUSTOM(fd<<", "<<buffer<<", "<<size);
        return res;
    }

    ssize_t xpn_api::secuencial_write (xpn_file &file, const void *buffer, size_t _size)
    {
        ssize_t size = static_cast<ssize_t>(_size);
        XPN_DEBUG_BEGIN_CUSTOM(file.m_path<<", "<<buffer<<", "<<size);
        ssize_t res = 0;

        ssize_t new_offset = file.m_offset;
        ssize_t count = 0;
        ssize_t l_offset = 0;
        int l_serv = 0;
        ssize_t l_size;
        int res_aux = 0;

        do
        {
            for (int i = 0; i < file.m_part.m_replication_level + 1; i++)
            {
                res_aux = xpn_rw::write_get_block(file, new_offset, i, l_offset, l_serv);
                if (res_aux != -1){        
                    l_size = file.m_part.m_block_size - (new_offset%file.m_part.m_block_size);

                    // If l_size > the remaining bytes to read/write, then adjust l_size
                    if ((size - count) < l_size){
                        l_size = size - count;
                    }

                    res = file.initialize_vfh(l_serv);
                    if (res < 0){
                        count = -1;
                        goto cleanup_xpn_swrite;
                    }
                    m_worker->launch([&res, &file, l_serv, buffer, count, l_offset, l_size](){
                        res = file.m_part.m_data_serv[l_serv]->nfi_write(file.m_data_vfh[l_serv], (char *)buffer + count, l_offset+xpn_metadata::HEADER_SIZE, l_size);
                    });
                    m_worker->wait();
                    XPN_DEBUG("l_serv = "<<l_serv<<", l_offset = "<<l_offset<<", l_size = "<<l_size);
                    if (res < 0) {
                        count = (0 == count) ? -1 : count;
                        goto cleanup_xpn_swrite;
                    }
                }
            }
            count = count + res;
            new_offset = file.m_offset + count;
        }
        while((size > count) && (res > 0));
    cleanup_xpn_swrite:
        if(count > 0){
            file.m_offset += count;
            // Update file_size in metadata
            if (file.m_offset > static_cast<off_t>(file.m_mdata.m_data.file_size)){
                file.m_mdata.m_data.file_size = file.m_offset;
                write_metadata(file.m_mdata, true);
            }
        }
        res = count;

        XPN_DEBUG_END_CUSTOM(file.m_path<<", "<<buffer<<", "<<size);
        return res;
    }

    ssize_t xpn_api::parallel_write   (xpn_file &file, const void *buffer, size_t size)
    {

        XPN_DEBUG_BEGIN_CUSTOM(file.m_path<<", "<<buffer<<", "<<size);
        int res = 0;
        // TODO
        XPN_DEBUG_END_CUSTOM(file.m_path<<", "<<buffer<<", "<<size);
        return size;
    }

    off_t xpn_api::lseek(int fd, off_t offset, int flag)
    {
        XPN_DEBUG_BEGIN_CUSTOM(fd<<", "<<offset<<", "<<flag);
        int res = 0;
        struct ::stat st;

        if (!m_file_table.has(fd)){
            errno = EBADF;
            XPN_DEBUG_END_CUSTOM(fd<<", "<<offset<<", "<<flag);
            return -1;
        }
        auto& file = m_file_table.get(fd);
        
        switch (flag)
        {
            case SEEK_SET:
                if (offset < 0)
                {
                    errno = EINVAL;
                    return (off_t)-1;
                }
                else {
                    file.m_offset = offset;
                }
                break;

            case SEEK_CUR:
                if (file.m_offset+offset<0)
                {
                    errno = EINVAL;
                    return (off_t)-1;
                }
                else {
                    file.m_offset += offset;
                }
                break;

            case SEEK_END:
                if (fstat(fd, &st)<0)
                {
                    errno = EBADF;
                    return (off_t)-1;
                }
                if (st.st_size + offset<0)
                {
                    errno = EINVAL;
                    return (off_t)-1;
                }
                else {
                    file.m_offset = st.st_size + offset;
                }
            break;

            default:
                errno = EINVAL;
                return (off_t)-1;
        }

        XPN_DEBUG_END_CUSTOM(fd<<", "<<offset<<", "<<flag);
        return res;
    }
} // namespace XPN
