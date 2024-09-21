
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
        XPN_DEBUG_BEGIN_CUSTOM(filename<<", "<<mode);
        int res = 0;
        FILE *stream = NULL;
        int flags = -1;

        if (strstr(mode, "r+") != NULL){
            flags = O_RDWR;
        }
        else if (strstr(mode, "r") != NULL){
            flags = O_RDONLY;
        }
        else if (strstr(mode, "w+") != NULL){
            flags = O_RDWR | O_CREAT | O_TRUNC;
        }
        else if (strstr(mode, "w") != NULL){
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        }
        else if (strstr(mode, "a+") != NULL){
            flags = O_RDWR | O_CREAT | O_APPEND;
        }
        else if (strstr(mode, "a") != NULL){
            flags = O_WRONLY | O_CREAT | O_APPEND;
        }

        if (flags >= 0)
        {
            res = open(filename, flags, 07000);
            if (res >= 0)
            {
                stream = new FILE;
                memset(stream, 0, sizeof(FILE));
                stream->_fileno = res;
            }
        }
        XPN_DEBUG_END_CUSTOM(filename<<", "<<mode);
        return stream;
    }

    int xpn_api::fclose(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        
        if (stream != NULL)
        {
            fflush(stream);

            res = close(stream->_fileno);
            if (stream->_IO_buf_base != NULL) {
                stream->_IO_buf_base = NULL;
            }
            free(stream);
        }
        else
        {
            res = EOF;
            errno = EBADF;
        }
        XPN_DEBUG_END;
        return res;
    }

    size_t xpn_api::fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        size_t res = 0;
        res = read(stream->_fileno, ptr, size*nmemb);
        XPN_DEBUG_END;
        return res;
    }

    size_t xpn_api::fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        size_t res = 0;
        res = write(stream->_fileno, ptr, size*nmemb);
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::fseek(FILE *stream, long offset, int whence)
    {
        XPN_DEBUG_BEGIN;
        off_t res;
        res = lseek(stream->_fileno, offset, whence);
        XPN_DEBUG_END;
        return static_cast<int>(res);
    }

    long xpn_api::ftell(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        long res = 0;
        if (m_file_table.has(stream->_fileno)){
            res = m_file_table.get(stream->_fileno).m_offset;
        }
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::fflush([[maybe_unused]] FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::fgetc(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        unsigned char res;
        read(stream->_fileno, &res, 1*sizeof(unsigned char));
        XPN_DEBUG_END;
        return (int)res;
    }

    char *xpn_api::fgets(char *s, int tam, FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        memset(s, 0, tam*sizeof(char));

        fread(s, sizeof(char), tam-1, stream);

        XPN_DEBUG_END;
        return s;
    }

    int xpn_api::getc(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = fgetc(stream);
        XPN_DEBUG_END;
        return res;
    }

    void xpn_api::rewind(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = 0;
        lseek(stream->_fileno, 0, SEEK_SET);
        XPN_DEBUG_END;
        return;
    }

    int xpn_api::fileno(FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = stream->_fileno;
        XPN_DEBUG_END;
        return res;
    }

    int xpn_api::ferror([[maybe_unused]] FILE *stream)
    {
        XPN_DEBUG_BEGIN;
        int res = errno;
        // TODO: update errno or xpn_errno?
        XPN_DEBUG_END;
        return res;
    }

} // namespace XPN
