/*
 *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez
 * Garcia, Borja Bergua Guerra
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
 * @file path_misc.h
 * @brief Header file to Paths.
 *
 * Header file to perform operations on the path (hash, first directory, factor size, get filename, etc.).
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia,
 * Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _PATH_MISC_H_
#define _PATH_MISC_H_

/************************************************
 *  ... Includes
 ***********************************************/
#include "all_system.h"
#include "base/string_misc.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************
 *  ... Functions
 ***********************************************/

/**
 * @brief Server number calculation.
 *
 * This function is used to determine where (server) the first block of a file is located.
 * It receives as input the file name, applies a hash (to distribute the workload) 
 * and determines which server number corresponds to the master server 
 * where the first block of the file will be located.
 *
 * @param file File name.
 * @param nServ Server number.
 *
 * @return Master server number.
 */
int base_path_misc_hash(char *file, int nServ);

/**
 * @brief Get first directory.
 *
 * Gets from the path the name of the first directory.
 *
 * @param dir 'TODO'.
 * @param path 'TODO'.
 * @return 'TODO'.
 */
int base_path_misc_get_first_dir(char *dir, char *path);

/**
 * @brief Get size factor.
 *
 * 'TODO'.
 *
 * @param name 'TODO'.
 * @return 'TODO'.
 */
int base_path_misc_get_size_factor(char *name);

/**
 * @brief Get name file.
 *
 * Get the last name of a path and erase the file name.
 *
 * @param file 'TODO'.
 * @param dir 'TODO'.
 * @return 'TODO'.
 */
int base_path_misc_get_name_file(char *file, char *dir);

/**
 * @brief Get name part.
 *
 * Get the first name of the path and erase the part name.
 *
 * @param part 'TODO'.
 * @param dir 'TODO'.
 * @return 'TODO'.
 */
int base_path_misc_get_name_part(char *part, char *dir);

/**
 * @brief Get directory with URL.
 *
 * Erase the initial dir used in the url.
 *
 * @param url 'TODO'.
 * @param dir 'TODO'.
 * @return 'TODO'.
 */
int get_dir_with_url(char *url, char *dir);

#ifdef __cplusplus
}
#endif

#endif /* _PATH_MISC_H */
