
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

    FILE *xpn_api::fopen(const char *filename, const char *mode)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return nullptr;
    }

    int xpn_api::fclose(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    size_t xpn_api::fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    size_t xpn_api::fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::fseek(FILE *stream, long offset, int whence)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    long xpn_api::ftell(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::fflush(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::fgetc(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    char *xpn_api::fgets(char *s, int tam, FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return nullptr;
    }

    int xpn_api::getc(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    void xpn_api::rewind(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return nullptr;
    }

    int xpn_api::fileno(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::ferror(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

} // namespace XPN
