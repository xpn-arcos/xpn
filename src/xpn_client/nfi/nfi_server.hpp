
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

#include <string>
#include <memory>
#include <sys/vfs.h>
#include <sys/stat.h>

#include "nfi_xpn_server_comm.hpp"

namespace XPN
{
    // Fordward declaration
    class xpn_file;
    class xpn_metadata;

    class nfi_server 
    {
    public:
        nfi_server(const std::string &url);
        int init_comm();
        int destroy_comm();
    public:
        std::string m_protocol; // protocol of the server: mpi_server sck_server
        std::string m_server;   // server address
        std::string m_path;     // path of the server

        int m_error = 0;        // For fault tolerance
    private:
        const std::string m_url;// URL of this server -> protocol
                                // + server
                                // + path + more info (port, ...)

        std::unique_ptr<nfi_xpn_server_control_comm> m_control_comm;
        nfi_xpn_server_comm                         *m_comm;

    public:
        int nfi_reconnect   ();
        int nfi_disconnect  ();
        int nfi_destroy     ();
        int nfi_getattr     (const xpn_file &fh, struct ::stat &st);
        int nfi_setattr     (const xpn_file &fh, struct ::stat &st);
        int nfi_open        (const std::string &path, int flags, mode_t mode, xpn_file &fho); 
        int nfi_create      (const std::string &path, mode_t mode, xpn_file &fh);
        int nfi_close       (const xpn_file &fh);
        int nfi_remove      (const std::string &path);
        int nfi_rename      (const std::string &path, const std::string &new_path);
        int64_t nfi_read    (const xpn_file &fh, void *buffer, off_t offset, size_t size);
        int64_t nfi_write   (const xpn_file &fh, void *buffer, off_t offset, size_t size);
        int nfi_mkdir       (const std::string &path, mode_t mode);
        int nfi_rmdir       (const std::string &path);
        int nfi_opendir     (const std::string &path, xpn_file &fho);
        int nfi_readdir     (const xpn_file &fhd, struct dirent &entry);
        int nfi_closedir    (const xpn_file &fh);
        int nfi_statfs      (struct ::statfs &inf);
        int nfi_read_mdata  (const std::string &path, xpn_metadata &mdata);
        int nfi_write_mdata (const std::string &path, const xpn_metadata &mdata, bool only_file_size);
    };
} // namespace XPN