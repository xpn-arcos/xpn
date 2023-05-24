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
 * @file math_misc.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _MATH_MISC_H
#define _MATH_MISC_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "all_system.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /************************************************
     *  ... Functions
     ***********************************************/

    /**
     * @brief Compute server index.
     *
     * Compute server index associated to a file.
     *
     * @param file the file name.
     * @param nServ number of servers for this file.
     * @return hash value computed.
     */
    int MATH_MISC_hash(
        char *file,
        int nServ);

    /**
     * @brief Compute location of block.
     *
     * Compute location ('SP','IP','SD','ID') of 'i' block with 'm' servers.
     *
     * @param i Block index.
     * @param m Number of servers.
     * @param SP Server for parity.
     * @param IP Index for parity.
     * @param SD Server for data.
     * @param ID Index for data.
     * @return true (1) if element is removed and error (-1) if
     *         a problem is found.
     */
    int MATH_MISC_locateInRAID5withInternalParity(
        int i,
        int m,
        int *SP,
        int *IP,
        int *SD,
        int *ID);

    /**
     * @brief Compute location of block.
     *
     * Compute location ('SP','IP','SD','ID') of 'i' block with 'm' servers.
     *
     * @param i Block index.
     * @param m Number of servers.
     * @param SP Server for parity.
     * @param IP Index for parity.
     * @param SD Server for data.
     * @param ID Index for data.
     * @return true (1) if element is removed and error (-1) if
     *         a problem is found.
     */
    int MATH_MISC_locateInRAID5withExternalParity(
        int i,
        int m,
        int *SP,
        int *IP,
        int *SD,
        int *ID);

    /**
     * @brief Compute Xor operation.
     *
     * Compute Xor operation ('block_result') for
     * two blocks ('block_1' and 'block_2') of 'block_size' bytes.
     * 
     * ...
     * char res[1024], b1[1024], b2[1024];
     * ...
     * memset(b1, 'a', 1024);
     * memset(b1, 'b', 1024);
     * MATH_MISC_Xor(res, b1, b2, 1024);
     * ... // res[i] = b1[i] ^ b2[i]
     * ...
     * MATH_MISC_Xor(b1, res, b2, 1024);
     * ... // b1 = res:(b1 ^ b2) ^ b2 = b1
     *
     * @param block_result Resulting block.
     * @param block_1 First block of data.
     * @param block_2 Second block of data.
     * @param block_size Number of bytes.
     * @return true (1).
     */
    int MATH_MISC_Xor(
        char *block_result,
        char *block_1,
        char *block_2,
        int block_size);

    /**
     * @brief Compute Xor operation.
     *
     * Compute Xor operation ('block_result') for
     * three blocks ('block_1', 'block_2' and 'block_3') of 'block_size' bytes.
     *
     * @param block_result Resulting block.
     * @param block_1 First block of data.
     * @param block_2 Second block of data.
     * @param block_3 Third block of data.
     * @param block_size Number of bytes of each block.
     * @return true (1).
     */
    int MATH_MISC_Xor3(
        char *block_result,
        char *block_1,
        char *block_2,
        char *block_3,
        int block_size);

    /**
     * @brief Compute Xor operation.
     *
     * Compute Xor operation ('block_result') for
     * 'nblocks' blocks at 'blocks' array of 'block_size' bytes each block.
     *
     * @param block_result Resulting block.
     * @param blocks Array with all blocks.
     * @param nblocks Number of blocks.
     * @param block_size Number of bytes of each block.
     * @return true (1).
     */
    int MATH_MISC_XorN(
        char *block_result,
        char **blocks,
        int nblocks,
        int block_size);

#ifdef __cplusplus
}
#endif

#endif /* _MATH_MISC_H */
