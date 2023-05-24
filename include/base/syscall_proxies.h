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
 * @file syscall_proxies.h
 * @brief Header file to 'TODO'.
 *
 * Header file to 'TODO'.
 *
 * @authors Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
 * @date  Jul 22, 2021
 * @bug No known bugs.
 */

#ifndef _SYSCALL_PROXIES_H
#define _SYSCALL_PROXIES_H

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************
 *  ... Includes
 ***********************************************/
#include <dlfcn.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include "utils.h"

  /************************************************
   *  ... Functions
   ***********************************************/

  /************************************************
   *  ... Functions: File API
   ***********************************************/

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param path 'TODO'.
   * @param flags 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_open(char *path, int flags);

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
  int dlsym_open2(char *path, int flags, mode_t mode);

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
  int dlsym_open64(char *path, int flags, mode_t mode);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param path 'TODO'.
   * @param flags 'TODO'.
   * @return 'TODO'.
   */
  int dlsym___open_2(char *path, int flags);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param fd 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_close(int fd);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param path 'TODO'.
   * @param mode 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_creat(const char *path, mode_t mode);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param fd 'TODO'.
   * @param length 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_ftruncate(int fd, off_t length);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param fd 'TODO'.
   * @param buf 'TODO'.
   * @param nbyte 'TODO'.
   * @return 'TODO'.
   */
  ssize_t dlsym_read(int fd, void *buf, size_t nbyte);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param fd 'TODO'.
   * @param buf 'TODO'.
   * @param nbyte 'TODO'.
   * @return 'TODO'.
   */
  ssize_t dlsym_write(int fd, void *buf, size_t nbyte);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param fd 'TODO'.
   * @param offset 'TODO'.
   * @param whence 'TODO'.
   * @return 'TODO'.
   */
  off_t dlsym_lseek(int fd, off_t offset, int whence);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param fd 'TODO'.
   * @param offset 'TODO'.
   * @param whence 'TODO'.
   * @return 'TODO'.
   */
  off64_t dlsym_lseek64(int fd, off64_t offset, int whence);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param ver 'TODO'.
   * @param fd 'TODO'.
   * @param buf 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_fstat(int ver, int fd, struct stat *buf);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param ver 'TODO'.
   * @param fd 'TODO'.
   * @param buf 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_fxstat64(int ver, int fd, struct stat64 *buf);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param ver 'TODO'.
   * @param path 'TODO'.
   * @param buf 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_stat(int ver, const char *path, struct stat *buf);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param ver 'TODO'.
   * @param path 'TODO'.
   * @param buf 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_lstat(int ver, const char *path, struct stat *buf);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param ver 'TODO'.
   * @param path 'TODO'.
   * @param buf 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_lxstat64(int ver, const char *path, struct stat64 *buf);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param ver 'TODO'.
   * @param path 'TODO'.
   * @param buf 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_xstat64(int ver, const char *path, struct stat64 *buf);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param dfd 'TODO'.
   * @param path 'TODO'.
   * @param buf 'TODO'.
   * @param flags 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_fstatat(int dfd, const char *path, struct stat *buf, int flags);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param dfd 'TODO'.
   * @param path 'TODO'.
   * @param buf 'TODO'.
   * @param flags 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_fstatat64(int dfd, const char *path, struct stat64 *buf, int flags);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param old_path 'TODO'.
   * @param new_path 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_rename(const char *old_path, const char *new_path);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param path 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_unlink(char *path);

  /************************************************
   *  ... Functions: Directory API
   ***********************************************/

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param dirname 'TODO'.
   * @return 'TODO'.
   */
  DIR *dlsym_opendir(char *dirname);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param dirname 'TODO'.
   * @return 'TODO'.
   */
  DIR *dlsym_opendir64(char *dirname);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param dirp 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_closedir(DIR *);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param dirp 'TODO'.
   * @return 'TODO'.
   */
  struct dirent *dlsym_readdir(DIR *dirp);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param dirp 'TODO'.
   * @return 'TODO'.
   */
  struct dirent64 *dlsym_readdir64(DIR *dirp);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param path 'TODO'.
   * @param mode 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_mkdir(char *path, mode_t mode);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param path 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_rmdir(char *path);

  /************************************************
   *  ... Functions: Proccess API
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
  int dlsym_fork(void);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param pipefd 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_pipe(int pipefd[2]);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param fd 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_dup(int fd);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param fd 'TODO'.
   * @param fd2 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_dup2(int fd, int fd2);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param status 'TODO'.
   * @par Returns
   *    Nothing.
   */
  void dlsym_exit(int status);

  /************************************************
   *  ... Functions: Manager - File/Directory Metadata API
   ***********************************************/

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param path 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_chdir(char *path);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param path 'TODO'.
   * @param mode 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_chmod(char *path, mode_t mode);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param fd 'TODO'.
   * @param mode 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_fchmod(int fd, mode_t mode);

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
  int dlsym_chown(char *path, uid_t owner, gid_t group);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param fd 'TODO'.
   * @param cmd 'TODO'.
   * @param arg 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_fcntl(int fd, int cmd, long arg);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param path 'TODO'.
   * @param mode 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_access(const char *path, int mode);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param path 'TODO'.
   * @param resolved_path 'TODO'.
   * @return 'TODO'.
   */
  char *dlsym_realpath(const char *restrict path, char *restrict resolved_path);

  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param fd 'TODO'.
   * @return 'TODO'.
   */
  int dlsym_fsync(int fd);

  /************************************************
   *  ... Functions: Memory API
   ***********************************************/
  /**
   * @brief 'TODO'.
   *
   * 'TODO'.
   *
   * @param addr 'TODO'.
   * @param length 'TODO'.
   * @param prot 'TODO'.
   * @param flags 'TODO'.
   * @param fd 'TODO'.
   * @param offset 'TODO'.
   * @par Returns
   *    Nothing.
   */
  void *dlsym_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

#ifdef __cplusplus
}
#endif

#endif
