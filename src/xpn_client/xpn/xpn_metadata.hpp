
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

#include <string>
#include <vector>

#include "nfi/nfi_server.hpp"

namespace XPN
{
    #define HEADER_SIZE 8192
    
    #define MAGIC_NUMBER "XPN"
    #define METADATA_VERSION 1
    #define METADATA_MAX_RECONSTURCTIONS 40
    #define METADATA_DISTRIBUTION_ROUND_ROBIN 1

    // inline bool CHECK_MAGIC_NUMBER(xpn_metadata &mdata){
    //     return (((mdata).magic_number[0] == MAGIC_NUMBER[0]) && 
    //         ((mdata).magic_number[1] == MAGIC_NUMBER[1]) && 
    //         ((mdata).magic_number[2] == MAGIC_NUMBER[2])) 
    // }
            
    class xpn_metadata
    {
    public:
        xpn_metadata() = default;
    private:
        std::array<int, 3> magic_number = {0};
        int      version = 0;                                      // Version number
        int      type = 0;                                         // Type of file: file or directory
        uint64_t block_size = 0;                                   // Size of block used
        uint64_t file_size = 0;                                    // Size of the file
        int      replication_level = 0;                            // Replication level of files: 0, 1, 2, ...
        int      first_node = 0;                                   // Server which has the first block
        int      distribution_policy = 0;                          // Distribution policy of blocks, default: round-robin
        std::array<int, METADATA_MAX_RECONSTURCTIONS> data_nserv = {0}; // Array of number of servers to reconstruct
        std::array<int, METADATA_MAX_RECONSTURCTIONS> offsets = {0};    // Array indicating the block where new server configuration starts

        bool check_magic_number() 
        { 
            return magic_number[0] == MAGIC_NUMBER[0] && 
                   magic_number[1] == MAGIC_NUMBER[1] && 
                   magic_number[2] == MAGIC_NUMBER[2];
        } 
    };
} // namespace XPN