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
 * @file xpn_policy_rw.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _XPN_POLICY_RW_H
#define _XPN_POLICY_RW_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "xpn_file.h"
#include "xpn_policy_open.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /************************************************
     *  ... Functions
     ***********************************************/

    /**
     * @brief Calculates the server and the offset.
     *
     * Calculates the server and the offset in that server of the given offset of a file.
     *
     * @param fd [in] A file descriptor.
     * @param global_offset [in] The original offset.
     * @param local_offset [out] The offset in the server.
     * @param serv [out] The server in which is located the given offset.
     * @return Returns 0 on success or -1 on error.
     */
    int XpnGetBlock(int fd, off_t global_offset, off_t *local_offset, int *serv);

    /**
     * @brief Read blocks.
     *
     * Calculates how the blocks have to be read from the servers. io_out is an operation matrix. io_out[i] (row 'i' in io_out)
     * contains the required operations in server 'i'. While ion_out[i] is the number of operations in server 'i' (io_out[i]).
     *
     * @param fd [in] A file descriptor.
     * @param buffer [in] The original buffer.
     * @param size [in] The original size.
     * @param offset [in] The original offset.
     * @param io_out [out] The operation matrix.
     * @param ion_out [out] The length of every row in io_out.
     * @param num_servers [in] The number of servers.
     *
     * @return Returns a pointer to a new buffer on success, or NULL on error.
     */
    void *XpnReadBlocks(int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd 'TODO'.
     * @param buffer 'TODO'.
     * @param size 'TODO'.
     * @param offset 'TODO'.
     * @param io_out 'TODO'.
     * @param ion_out 'TODO'.
     * @param num_servers 'TODO'.
     * @param new_buffer 'TODO'.
     * @return 'TODO'.
     */
    int XpnReadBlocksFinish(int fd, void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers, void *new_buffer);

    /**
     * @brief Write blocks.
     *
     * Calculates how the blocks have to be written to the servers. io_out is an operation matrix. io_out[i] (row 'i' in io_out)
     * contains the required operations in server 'i'. While ion_out[i] is the number of operations in server 'i' (io_out[i]).
     *
     * @param fd [in] A file descriptor.
     * @param buffer [in] The original buffer.
     * @param size [in] The original size.
     * @param offset [in] The original offset.
     * @param io_out [out] The operation matrix.
     * @param ion_out [out] The length of every row in io_out.
     * @param num_servers [in] The number of servers.
     *
     * @return Returns 0 on success or -1 on error.
     */
    void *XpnWriteBlocks(int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd 'TODO'.
     * @param buffer 'TODO'.
     * @param size 'TODO'.
     * @param offset 'TODO'.
     * @param io_out 'TODO'.
     * @param ion_out 'TODO'.
     * @param num_servers 'TODO'.
     * @param new_buffer 'TODO'.
     * @return 'TODO'.
     */
    int XpnWriteBlocksFinish(int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers, void *new_buffer);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd 'TODO'.
     * @param res_v 'TODO'.
     * @param num_servers 'TODO'.
     * @return 'TODO'.
     */
    ssize_t XpnReadGetTotalBytes(int fd, ssize_t *res_v, int num_servers);

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd 'TODO'.
     * @param res_v 'TODO'.
     * @param num_servers 'TODO'.
     * @return 'TODO'.
     */
    ssize_t XpnWriteGetTotalBytes(int fd, ssize_t *res_v, int num_servers);

#ifdef __cplusplus
}
#endif

#endif
