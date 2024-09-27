
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

#include "nfi/nfi_server.hpp"
#include "base_cpp/debug.hpp"
#include "base_c/filesystem.h"

namespace XPN
{
    // Fordward declaration
    class xpn_fh;
    class xpn_metadata;

    class nfi_local : public nfi_server
    {
    public:
        nfi_local(const std::string &url) : nfi_server(url) 
        {
            filesystem_low_set(RTLD_NEXT);
        }
    public:
        // Operations 
        int nfi_open        (const std::string &path, int flags, mode_t mode, xpn_fh &fho) override;
        int nfi_create      (const std::string &path, mode_t mode, xpn_fh &fho) override;
        int nfi_close       (const xpn_fh &fh) override;
        int64_t nfi_read    (const xpn_fh &fh,       char *buffer, int64_t offset, uint64_t size) override;
        int64_t nfi_write   (const xpn_fh &fh, const char *buffer, int64_t offset, uint64_t size) override;
        int nfi_remove      (const std::string &path, bool is_async) override;
        int nfi_rename      (const std::string &path, const std::string &new_path) override;
        int nfi_getattr     (const std::string &path, struct ::stat &st) override;
        int nfi_setattr     (const std::string &path, struct ::stat &st) override;
        int nfi_mkdir       (const std::string &path, mode_t mode) override;
        int nfi_opendir     (const std::string &path, xpn_fh &fho) override;
        int nfi_readdir     (xpn_fh &fhd, struct ::dirent &entry) override;
        int nfi_closedir    (const xpn_fh &fhd) override;
        int nfi_rmdir       (const std::string &path, bool is_async) override;
        int nfi_statvfs     (const std::string &path, struct ::statvfs &inf) override;
        int nfi_read_mdata  (const std::string &path, xpn_metadata &mdata) override;
        int nfi_write_mdata (const std::string &path, const xpn_metadata &mdata, bool only_file_size) override;
    };
} // namespace XPN