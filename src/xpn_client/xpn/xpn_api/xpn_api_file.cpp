
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
#include "xpn/xpn_api.hpp"

#include <iostream>

namespace XPN
{
    #ifndef O_WRONLY
    #define O_WRONLY             01
    #endif
    #ifndef O_CREAT
    #define O_CREAT            0100
    #endif
    #ifndef O_TRUNC
    #define O_TRUNC           01000
    #endif
    #ifndef O_DIRECTORY
    #define O_DIRECTORY    00200000
    #endif

    int xpn_api::open(const char *path, int flags, mode_t mode)
    {
        XPN_DEBUG_BEGIN_CUSTOM(path<<", "<<flags<<", "<<mode);
        int res = 0;


        
        XPN_DEBUG_END_CUSTOM(path<<", "<<flags<<", "<<mode);
        return res;
    }

    int xpn_api::creat(const char *path, mode_t perm)
    {
        XPN_DEBUG_BEGIN_CUSTOM(path<<", "<<perm);
        // Like in unix we use the open with flags
        int res = open(path, O_WRONLY|O_CREAT|O_TRUNC, perm);
        XPN_DEBUG_END_CUSTOM(path<<", "<<perm);
        return res;
    }

    int xpn_api::close(int fd)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::unlink(const char *path)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::rename(const char *path, const char *newpath)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }
} // namespace XPN
