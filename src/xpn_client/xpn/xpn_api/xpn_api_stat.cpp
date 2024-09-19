
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

namespace XPN
{
    int xpn_api::fstat(int fd, struct ::stat *sb)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;

        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::stat(const char *path, struct ::stat *sb)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::chown(const char *path,  uid_t owner,  gid_t group)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::fchown(int  fd,  uid_t owner,  gid_t group)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::chmod(const char *path,  mode_t mode)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::fchmod(int fd,  mode_t mode)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::truncate(const char *path,  off_t length)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::ftruncate(int fd, off_t length)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::dup(int fd)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::dup2(int fd, int fd2)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }
} // namespace XPN
