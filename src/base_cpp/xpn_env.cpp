
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
        // XPN_SCK_PORT
        xpn_sck_port = std::getenv("XPN_SCK_PORT");
        // XPN_CONF
        xpn_conf = std::getenv("XPN_CONF");
        // XPN_DEBUG
        char *env_debug = std::getenv("XPN_DEBUG");
        if ((env_debug != NULL) && (std::strlen(env_debug) > 0)){
            xpn_debug=1;
        }
        // XPN_PROFILER
        char *env_profiler = std::getenv("XPN_PROFILER");
        if ((env_profiler != NULL) && (std::strlen(env_profiler) > 0)){
            xpn_profiler=1;
        }
        // XPN_THREAD
        char *env_thread = std::getenv("XPN_THREAD");
        if ((env_thread != NULL) && (std::strlen(env_thread) > 0)){
            xpn_thread=atoi(env_thread);
        }
        // XPN_LOCALITY
        char *env_locality = std::getenv("XPN_LOCALITY");
        if ((env_locality != NULL) && (std::strlen(env_locality) > 0)){
            xpn_locality=atoi(env_locality);
        }
        // XPN_SESSION_DIR
        char *env_session_dir = std::getenv("XPN_SESSION_DIR");
        if ((env_session_dir != NULL) && (std::strlen(env_session_dir) > 0)){
            xpn_session_dir=atoi(env_session_dir);
        }
        // XPN_SESSION_FILE
        char *env_session_file = std::getenv("XPN_SESSION_FILE");
        if ((env_session_file != NULL) && (std::strlen(env_session_file) > 0)){
            xpn_session_file=atoi(env_session_file);
        }
        // XPN_SESSION_CONNECT
        char *env_session_connect = std::getenv("XPN_SESSION_CONNECT");
        if ((env_session_connect != NULL) && (std::strlen(env_session_connect) > 0)){
            xpn_session_connect=atoi(env_session_connect);
        }
    }
}