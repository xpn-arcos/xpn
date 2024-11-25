
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


/* ... Include / Inclusion ........................................... */

#include <iostream>
#include "nfi_xpn_server_comm.hpp"
#include "nfi_mpi_server/nfi_mpi_server_comm.hpp"
#include "nfi_sck_server/nfi_sck_server_comm.hpp"

namespace XPN
{
    std::unique_ptr<nfi_xpn_server_control_comm> nfi_xpn_server_control_comm::Create(const std::string &server_protocol)
    {
        if (server_protocol == server_protocols::mpi_server)
        {
            return std::make_unique<nfi_mpi_server_control_comm>();
        }else
        if (server_protocol == server_protocols::sck_server)
        {
            return std::make_unique<nfi_sck_server_control_comm>();
        }else
        if (server_protocol == server_protocols::file)
        {
            // return std::make_unique<mpi_server_control_comm>(params.get_argc(), params.get_argv(), params.have_threads());
        }
        std::cerr << "Error: server protocol '"<< server_protocol << "' is not defined." << std::endl;
        return nullptr;
    }

} // namespace XPN