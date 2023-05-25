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
 * @file xpn_opendir.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _XPN_OPENDIR_H
#define _XPN_OPENDIR_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "xpn.h"
#include "xpn_open.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /** @struct __dirstream
   *  This is a struct
   *
   *  @var __dirstream::fd
   *    File descriptor.
   *  @var __dirstream::allocation
   *   Space allocated for the block.
   *  @var __dirstream::size
   *   Total valid data in the block.
   *  @var __dirstream::offset
   *    Current offset into the block.
   *  @var __dirstream::filepos
   *   Position of next entry to read.
   *  @var __dirstream::path
   *    A 'TODO'.
   */
  struct __dirstream
  {
    int fd;
    //__libc_lock_define (, lock) /* Mutex lock for this structure.  */ //TODO
    size_t allocation;
    size_t size;
    size_t offset;
    off_t filepos;
    /* Directory block.  */
    char data[0] __attribute__((aligned(__alignof__(void *))));

    char *path;
  };


	/************************************************
	 *  ... Functions
	 ***********************************************/

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param path 'TODO'.
   * @return 'TODO'.
   */
  DIR *xpn_simple_opendir(const char *path);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param dirp 'TODO'.
   * @return 'TODO'.
   */
  int xpn_simple_closedir(DIR *dirp);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param dirp 'TODO'.
   * @return 'TODO'.
   */
  struct dirent *xpn_simple_readdir(DIR *dirp);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param dirp 'TODO'.
   * @par Returns
   *    Nothing.
   */
  void xpn_simple_rewinddir(DIR *dirp);

#ifdef __cplusplus
}
#endif

#endif
