
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

#include "xpn_env.hpp"
#include <cstdlib>
#include <cstring>

namespace XPN
{
    xpn_env::xpn_env()
    {
        xpn_sck_port = std::getenv("XPN_SCK_PORT");
        char *env_debug = std::getenv("XPN_DEBUG");
        if ((env_debug != NULL) && (std::strlen(env_debug) > 0)){
            xpn_debug=1;
        }
        char *env_profiler = std::getenv("XPN_PROFILER");
        if ((env_profiler != NULL) && (std::strlen(env_profiler) > 0)){
            xpn_profiler=1;
        }
        xpn_conf = std::getenv("XPN_CONF");
    }
}