
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
#include <xpn_server/xpn_server_params.hpp>

namespace XPN
{
    namespace server_protocols
    {
        constexpr const char * mpi_server = "mpi_server";
        constexpr const char * sck_server = "sck_server";
        constexpr const char * file = "file";
    }
    class nfi_xpn_server_comm
    {
    public:
        virtual ~nfi_xpn_server_comm() = default;
        virtual int64_t write_operation(int op) = 0;
        virtual int64_t read_data(void *data, int64_t size) = 0;
        virtual int64_t write_data(const void *data, int64_t size) = 0;
    };
    class nfi_xpn_server_control_comm 
    {
    public:
        virtual ~nfi_xpn_server_control_comm() = default;

        virtual nfi_xpn_server_comm* connect(const std::string &srv_name) = 0;
        virtual void disconnect(nfi_xpn_server_comm *comm) = 0;

        static std::unique_ptr<nfi_xpn_server_control_comm> Create(const std::string& server_protocol);
    };
} // namespace XPN