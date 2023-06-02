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
 * @file path_misc.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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
extern "C"
{
#endif

  /************************************************
   *  ... Functions
   ***********************************************/

  /**
   * @brief 'TODO'.
   *
   * 'Server number calculation'.
   *
   * @param file 'TODO'.
   * @param nServ 'TODO'.
   * @return 'TODO'.
   */
  int base_path_misc_hash(char *file, int nServ);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param dir 'TODO'.
   * @param path 'TODO'.
   * @return 'TODO'.
   */
  int base_path_misc_get_first_dir(char *dir, char *path);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param name 'TODO'.
   * @return 'TODO'.
   */
  int base_path_misc_get_size_factor(char *name);

  /**
   * @brief 'TODO'.
   *
   * Get the last name of a path and erase the file name.
   *
   * @param file 'TODO'.
   * @param dir 'TODO'.
   * @return 'TODO'.
   */
  int base_path_misc_get_name_file(char *file, char *dir);

  /**
   * @brief 'TODO'.
   *
   * Get the first name of the path and erase the part name.
   *
   * @param part 'TODO'.
   * @param dir 'TODO'.
   * @return 'TODO'.
   */
  int base_path_misc_get_name_part(char *part, char *dir);

  /**
   * @brief 'TODO'.
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
