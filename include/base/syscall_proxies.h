
/*
 *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


#ifndef _SYSCALL_PROXIES_H
#define _SYSCALL_PROXIES_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include <dlfcn.h>
  #include <sys/stat.h>
  #include <dirent.h>
  #include <stdlib.h>

  #include "utils.h"


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  // File API
  int dlsym_open     (char *path, int flags);
  int dlsym_open2    (char *path, int flags, mode_t mode);
  int dlsym_open64   (char *path, int flags, mode_t mode);
  int dlsym___open_2 (char *path, int flags);
  int dlsym_close    (int fd);

  int dlsym_creat     (const char *path, mode_t mode);
  int dlsym_ftruncate (int fd, off_t length);

  ssize_t dlsym_read  (int fd, void *buf, size_t nbyte);
  ssize_t dlsym_write (int fd, void *buf, size_t nbyte);

  ssize_t dlsym_pread    (int fd, void *buf, size_t count, off_t offset);
  ssize_t dlsym_pwrite   (int fd, const void *buf, size_t count, off_t offset);
  ssize_t dlsym_pread64  (int fd, void *buf, size_t count, off_t offset);
  ssize_t dlsym_pwrite64 (int fd, const void *buf, size_t count, off_t offset);

  off_t   dlsym_lseek   (int fd,   off_t offset, int whence);
  off64_t dlsym_lseek64 (int fd, off64_t offset, int whence);

  int dlsym_fstat     (int ver, int fd, struct stat *buf);
  int dlsym_fxstat64  (int ver, int fd, struct stat64 *buf);
  int dlsym_stat      (int ver, const char *path, struct stat   *buf);
  int dlsym_lstat     (int ver, const char *path, struct stat   *buf);
  int dlsym_lxstat64  (int ver, const char *path, struct stat64 *buf);
  int dlsym_xstat64   (int ver, const char *path, struct stat64 *buf);
  int dlsym_fstatat   (int dfd, const char *path, struct stat   *buf, int flags);
  int dlsym_fstatat64 (int dfd, const char *path, struct stat64 *buf, int flags);

  int dlsym_rename (const char *old_path, const char *new_path);
  int dlsym_unlink (char *path);
  int dlsym_remove (char *path);


  // File API (stdio)
  FILE* dlsym_fopen   (const char *filename, const char *mode);
  FILE* dlsym_fdopen  (int fd, const char *mode);
  int   dlsym_fclose  (FILE *stream);

  size_t dlsym_fread  (void *ptr, size_t size, size_t nmemb, FILE *stream);
  size_t dlsym_fwrite (const void *ptr, size_t size, size_t nmemb, FILE *stream);

  int  dlsym_fseek    (FILE *stream, long int offset, int whence);
  long dlsym_ftell    (FILE *stream);
  int  dlsym_feof     (FILE *stream);


  // Directory API
  DIR* dlsym_opendir   (char *dirname);
  DIR* dlsym_opendir64 (char *dirname);
  int  dlsym_closedir  (DIR*);

  struct dirent * dlsym_readdir     (DIR *dirp);
  struct dirent64 * dlsym_readdir64 (DIR *dirp);

  int dlsym_mkdir (char *path, mode_t mode);
  int dlsym_rmdir (char *path);


  // Proccess API
  int dlsym_fork (void);

  int dlsym_pipe (int pipefd[2]);

  int dlsym_dup  (int fd);
  int dlsym_dup2 (int fd, int fd2);

  void dlsym_exit (int status);


  // Manager API
  int dlsym_chdir  (char * path);
  int dlsym_chmod  ( char *path, mode_t mode);
  int dlsym_fchmod (int fd, mode_t mode);
  int dlsym_chown  (char *path, uid_t owner, gid_t group);
  int dlsym_fcntl  (int fd, int cmd, long arg);
  int dlsym_access (const char *path, int mode);
  char *dlsym_realpath (const char *restrict path, char *restrict resolved_path);
  int dlsym_fsync (int fd);
  int dlsym_flock (int fd, int operation);


  // Memory API
  void *dlsym_mmap (void *addr, size_t length, int prot, int flags, int fd, off_t offset);


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
