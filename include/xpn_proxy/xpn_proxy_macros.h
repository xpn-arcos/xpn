/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
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
 *  Macro remapping for PROXY_XPN_XXXX functions.
 * If USE_XPN_FUNCTIONS is defined, PROXY_XPN_XXXX maps to xpn_xxxx.
 * Otherwise, PROXY_XPN_XXXX maps to POSIX functions.
 */

#ifndef PROXY_XPN_MACROS_H
#define PROXY_XPN_MACROS_H



#ifdef USE_XPN_FUNCTIONS
    // Map to xpn_xxxx functions
    #define PROXY_XPN_OPEN      xpn_open
    #define PROXY_XPN_CREAT     xpn_creat
    #define PROXY_XPN_CLOSE     xpn_close
    #define PROXY_XPN_READ      xpn_read
    #define PROXY_XPN_WRITE     xpn_write
    #define PROXY_XPN_UNLINK    xpn_unlink
    #define PROXY_XPN_RENAME    xpn_rename
    #define PROXY_XPN_STAT      xpn_stat
    #define PROXY_XPN_MKDIR     xpn_mkdir
    #define PROXY_XPN_OPENDIR   xpn_opendir
    #define PROXY_XPN_CLOSEDIR  xpn_closedir
    #define PROXY_XPN_READDIR   xpn_readdir
    #define PROXY_XPN_RMDIR     xpn_rmdir
#else

    #include <fcntl.h>      // for open()
    #include <unistd.h>     // for close(), read(), write()

    // Map to POSIX functions
    #define PROXY_XPN_OPEN      open
    #define PROXY_XPN_CREAT     creat
    #define PROXY_XPN_CLOSE     close
    #define PROXY_XPN_READ      filesystem_read
    #define PROXY_XPN_WRITE     write
    #define PROXY_XPN_UNLINK    unlink
    #define PROXY_XPN_RENAME    rename
    #define PROXY_XPN_STAT      stat
    #define PROXY_XPN_MKDIR     mkdir
    #define PROXY_XPN_OPENDIR   opendir
    #define PROXY_XPN_CLOSEDIR  closedir
    #define PROXY_XPN_READDIR   readdir
    #define PROXY_XPN_RMDIR     rmdir
#endif

#endif // PROXY_XPN_MACROS_H
