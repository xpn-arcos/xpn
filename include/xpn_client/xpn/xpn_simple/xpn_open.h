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
 * @file xpn_open.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _XPN_OPEN_H
#define _XPN_OPEN_H

/************************************************
 *  ... Includes
 ***********************************************/
#include "xpn.h"
#include "xpn_file.h"
#include "xpn_policy_open.h"
#include "xpn_policy_cwd.h"
#include "xpn_init.h"
#include "xpn_rw.h"
#include "base/workers.h"

#ifdef __cplusplus
extern "C"
{
#endif

   /************************************************
    *  ... Functions
    ***********************************************/

   // int   xpn_open(const char *path, int flags, ...);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param path 'TODO'.
    * @param mode 'TODO'.
    * @return 'TODO'.
    */
   FILE *xpn_fopencookie(const char *path, const char *mode);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param path 'TODO'.
    * @param flags 'TODO'.
    * @param mode 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_open(const char *path, int flags, mode_t mode);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param path 'TODO'.
    * @param perm 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_creat(const char *path, mode_t perm);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param filename 'TODO'.
    * @param mode 'TODO'.
    * @return 'TODO'.
    */
   FILE *xpn_simple_fopen(const char *filename, const char *mode);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param fd 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_close(int fd);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param path 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_unlink(const char *path);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param path 'TODO'.
    * @param newpath 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_rename(const char *path, const char *newpath);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param fd 'TODO'.
    * @param sb 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_fstat(int fd, struct stat *sb);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param fp 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_fclose(FILE *fp);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param path 'TODO'.
    * @param sb 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_stat(const char *path, struct stat *sb);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param path 'TODO'.
    * @param owner 'TODO'.
    * @param group 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_chown(const char *path, uid_t owner, gid_t group);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param fd 'TODO'.
    * @param owner 'TODO'.
    * @param group 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_fchown(int fd, uid_t owner, gid_t group);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param path 'TODO'.
    * @param mode 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_chmod(const char *path, mode_t mode);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param fd 'TODO'.
    * @param mode 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_fchmod(int fd, mode_t mode);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param path 'TODO'.
    * @param length 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_truncate(const char *path, off_t length);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param fd 'TODO'.
    * @param length 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_ftruncate(int fd, off_t length);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param fd 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_dup(int fd);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param fd 'TODO'.
    * @param fd2 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_dup2(int fd, int fd2);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param virtual_path 'TODO'.
    * @param storage_path 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_flush(const char *virtual_path, const char *storage_path);

   /**
    * @brief 'TODO'.
    *
    * 'TODO'.
    *
    * @param virtual_path 'TODO'.
    * @param storage_path 'TODO'.
    * @return 'TODO'.
    */
   int xpn_simple_preload(const char *virtual_path, const char *storage_path);

#ifdef __cplusplus
}
#endif

#endif
