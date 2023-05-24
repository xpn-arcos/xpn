/*
 *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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

/**
 * @file xpn_file.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _XPN_FILE_H
#define _XPN_FILE_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "xpn.h"
#include "xpn_err.h"
#include "xpn_policy_init.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* max number of file descriptors */
#define XPN_MAX_FILE 1024

/* FILE or DIR */
#define XPN_FILE 0
#define XPN_DIR 1

        /** @struct xpn_fh
         *  'TODO'
         *
         *  @var xpn_fh::n_nfih
         *    'TODO'.
         *  @var xpn_fh::nfih
         *    NFI handler.
         */
        struct xpn_fh
        {
                int n_nfih;
                struct nfi_fhandle **nfih;
        };

        /** @struct xpn_attr
         *  File table
         *
         *  @var xpn_attr::at_type
         *    FILE or DIR.
         *  @var xpn_attr::at_mode
         *    Protection.
         *  @var xpn_attr::at_nlink
         *    Number of hard links.
         *  @var xpn_attr::at_uid
         *    User ID of owner.
         *  @var xpn_attr::at_gid
         *    Group ID of owner.
         *  @var xpn_attr::at_size
         *    Total size, in bytes.
         *  @var xpn_attr::at_blksize
         *    Blocksize for filesystem I/O.
         *  @var xpn_attr::at_blocks
         *    Number of blocks allocated.
         *  @var xpn_attr::at_atime
         *    Time of last access.
         *  @var xpn_attr::at_mtime
         *    Time of last modification.
         *  @var xpn_attr::at_ctime
         *    Time of last status change.
         *  @var xpn_attr::private_info
         *    'TODO'.
         */
        struct xpn_attr
        {
                int at_type;
                mode_t at_mode;
                nlink_t at_nlink;
                uid_t at_uid;
                gid_t at_gid;
                off_t at_size;
                u_long at_blksize;
                u_long at_blocks;
                time_t at_atime;
                time_t at_mtime;
                time_t at_ctime;
                void *private_info;
        };

        /** @struct xpn_filedesc
         *  File table
         *
         *  @var xpn_filedesc::id
         *    Id of file.
         *  @var xpn_filedesc::path
         *    Absolute path.
         *  @var xpn_filedesc::type
         *    Indicate FILE or DIR.
         *  @var xpn_filedesc::links
         *    Number of links that this file has.
         *  @var xpn_filedesc::mode
         *    O_RDONLY, O_WRONLY,...
         *  @var xpn_filedesc::part
         *    Partition.
         *  @var xpn_filedesc::mdata
         *    Metadata.
         *  @var xpn_filedesc::attr
         *    Attributes of the open file.
         *  @var xpn_filedesc::offset
         *    Offset of the open file.
         *  @var xpn_filedesc::block_size
         *    Size of distribution used.
         *  @var xpn_filedesc::size_threads
         *    A 'TODO'.
         *  @var xpn_filedesc::data_vfh
         *    Virtual FH.
         *  @var xpn_filedesc::meta_vfh
         *    Virtual METADATA FH.
         *  @var xpn_filedesc::st
         *    A 'TODO'.
         */
        struct xpn_filedesc
        {
                int id;
                char path[PATH_MAX];
                int type;
                int links;
                mode_t mode;
                struct xpn_partition *part;
                struct xpn_metadata *mdata;
                struct xpn_attr attr;
                off_t offset;
                ssize_t block_size;
                ssize_t size_threads;
                struct xpn_fh *data_vfh;
                struct xpn_fh *meta_vfh;
                struct stat st;
        };

        /* global  */
        extern struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];

/************************************************
 *  ... Functions
 ***********************************************/

        /**
         * @brief 'TODO'.
         *
         * 'TODO'.
         *
         * @par Parameters
         *    None.
         * @return 'TODO'.
         */
        int xpn_init_file_table(void);

        /**
         * @brief 'TODO'.
         *
         * 'TODO'.
         *
         * @par Parameters
         *    None.
         * @return 'TODO'.
         */
        int xpn_destroy_file_table(void);

#ifdef __cplusplus
}
#endif

#endif
