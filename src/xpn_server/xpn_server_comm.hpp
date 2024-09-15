
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

  #include "all_system.h"
  #include "base/utils.h"
  #include "base/time_misc.h"
  #include "base/ns.h"
  #include "xpn_server_params.hpp"
  #include <memory>

namespace XPN
{
  class xpn_server_comm
  {
  public:
    xpn_server_comm();
    ~xpn_server_comm();
    
    virtual int64_t read_operation(int &op, int &rank_client_id, int &tag_client_id) = 0;
    virtual int64_t read_data(void *data, int64_t size, int rank_client_id, int tag_client_id) = 0;
    virtual int64_t write_data(const void *data, int64_t size, int rank_client_id, int tag_client_id) = 0;
  };

  class xpn_server_control_comm
  {
  public:
    xpn_server_control_comm(xpn_server_params &params);
    ~xpn_server_control_comm();

    virtual xpn_server_comm* accept() = 0;
    virtual void disconnect(xpn_server_comm *comm) = 0;

    static std::unique_ptr<xpn_server_control_comm> Create(xpn_server_params &params);
  protected:
    xpn_server_params &m_params;
  };
}