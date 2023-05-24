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
 * @file xpn_policy_opendir.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _XPN_POLICY_DIR_H
#define _XPN_POLICY_DIR_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "xpn_file.h"
#include "xpn.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /************************************************
     *  ... Functions
     ***********************************************/

    /**
     * @brief 'TODO'.
     *
     * 'TODO'.
     *
     * @param fd 'TODO'.
     * @param entry 'TODO'.
     * @return 'TODO'.
     */
    int XpnGetEntry(int fd, struct dirent *entry);

#ifdef __cplusplus
}
#endif

#endif
