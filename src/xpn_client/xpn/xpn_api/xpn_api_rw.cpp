
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
    ssize_t xpn_api::read(int fd, void *buffer, size_t size)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }
    ssize_t xpn_api::write(int fd, const void *buffer, size_t size)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }
    off_t xpn_api::lseek(int fd, off_t offset, int flag)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }
} // namespace XPN
