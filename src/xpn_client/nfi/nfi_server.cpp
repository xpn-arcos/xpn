
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
#include "nfi_server.hpp"
#include "xpn/xpn_api.hpp"

#include <iostream>
#include <csignal>

namespace XPN
{
    nfi_server::nfi_server(const std::string &url) : m_url(url)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        // Find the position of "://"
        size_t protocol_pos = m_url.find("://");

        if (protocol_pos == std::string::npos) {
            std::cerr << "Invalid format of server_url: '://' not found '" << m_url << "'" << std::endl;
            std::raise(SIGTERM);
        }

        // Extract the first part (before "://")
        m_protocol = m_url.substr(0, protocol_pos);

        // Extract the second part (after "://")
        std::string remainder = m_url.substr(protocol_pos + 3);

        // Find the position of the first '/'
        size_t ip_pos = remainder.find('/');

        if (ip_pos == std::string::npos) {
            std::cerr << "Invalid format: '/' not found after IP '" << m_url << "'" << std::endl;
            std::raise(SIGTERM);
        }

        // Extract the IP address
        m_server = remainder.substr(0, ip_pos);

        // Extract the path (after the first '/')
        m_path = remainder.substr(ip_pos);

        XPN_DEBUG("Parse '"<<m_url<<"' to protocol '"
        << m_protocol <<"' server '"
        << m_server << "' path '"
        << m_path << "'");
        XPN_DEBUG_END;
    }

    int nfi_server::init_comm()
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        // Init the comunication
        m_control_comm = nfi_xpn_server_control_comm::Create(m_protocol);

        // Connect to the server
        m_comm = m_control_comm->connect(m_server);
        if(m_comm){
            XPN_DEBUG("Connected successfull to "<<m_server);
        }

        if (m_comm == nullptr){
            m_error = -1;
            res = -1;
        }
        XPN_DEBUG_END;
        return res;
    }

    int nfi_server::destroy_comm()
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        m_control_comm->disconnect(m_comm);

        m_control_comm.reset();

        XPN_DEBUG_END;
        return res;
    }

    int nfi_reconnect   ()
    {

    }

    int nfi_disconnect  ()
    {
        int res = 0;
        return res;
    }

    int nfi_destroy     ()
    {
        int res = 0;
        return res;
    }

    int nfi_getattr     (const xpn_file &fh, struct ::stat &st)
    {
        int res = 0;
        return res;
    }

    int nfi_setattr     (const xpn_file &fh, struct ::stat &st)
    {
        int res = 0;
        return res;
    }

    int nfi_open        (const std::string &path, int flags, mode_t mode, xpn_file &fho)
    {
        int res = 0;
        return res;
    }

    int nfi_create      (const std::string &path, mode_t mode, xpn_file &fh)
    {
        int res = 0;
        return res;
    }

    int nfi_close       (const xpn_file &fh)
    {
        int res = 0;
        return res;
    }

    int nfi_remove      (const std::string &path)
    {
        int res = 0;
        return res;
    }

    int nfi_rename      (const std::string &path, const std::string &new_path)
    {
        int res = 0;
        return res;
    }

    int64_t nfi_read    (const xpn_file &fh, void *buffer, off_t offset, size_t size)
    {
        int64_t res = 0;
        return res;
    }

    int64_t nfi_write   (const xpn_file &fh, void *buffer, off_t offset, size_t size)
    {
        int64_t res = 0;
        return res;
    }

    int nfi_mkdir       (const std::string &path, mode_t mode)
    {
        int res = 0;
        return res;
    }

    int nfi_rmdir       (const std::string &path)
    {
        int res = 0;
        return res;
    }

    int nfi_opendir     (const std::string &path, xpn_file &fho)
    {
        int res = 0;
        return res;
    }

    int nfi_readdir     (const xpn_file &fhd, struct dirent &entry)
    {
        int res = 0;
        return res;
    }

    int nfi_closedir    (const xpn_file &fh)
    {
        int res = 0;
        return res;
    }

    int nfi_statfs      (struct ::statfs &inf)
    {
        int res = 0;
        return res;
    }

    int nfi_read_mdata  (const std::string &path, xpn_metadata &mdata)
    {
        int res = 0;
        return res;
    }

    int nfi_write_mdata (const std::string &path, const xpn_metadata &mdata, bool only_file_size)
    {
        int res = 0;
        return res;
    }

} // namespace XPN