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
 * @file xpn_policy_cwd.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _XPN_POLICY_CWD_H
#define _XPN_POLICY_CWD_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "all_system.h"
#include "xpn_cwd.h"

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
    * @param path 'TODO'.
    * @param path_part 'TODO'.
    * @return 'TODO'.
    */
   int XpnGetAbsolutePath(const char *path, char *path_part);

#ifdef __cplusplus
}
#endif

#endif
