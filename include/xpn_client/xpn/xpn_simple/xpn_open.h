
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


#ifndef _XPN_OPEN_H
#define _XPN_OPEN_H

  #ifdef  __cplusplus
    extern "C" {
  #endif


  /* ... Include / Inclusion ........................................... */

  #include "xpn.h"
  #include "xpn_file.h"
  #include "xpn_policy_open.h"
  #include "xpn_policy_cwd.h"
  #include "xpn_init.h"
  #include "xpn_rw.h"
  #include "base/workers.h"


  /* ... Const / Const ................................................. */


  /* ... Data structures / Estructuras de datos ........................ */


  /* ... Functions / Funciones ......................................... */

  //int   xpn_open(const char *path, int flags, ...);
  FILE *xpn_fopencookie(const char *path, const char *mode);

  int   xpn_simple_open(const char *path, int flags, mode_t mode);
  int   xpn_simple_creat(const char *path, mode_t perm);
  FILE *xpn_simple_fopen(const char *filename, const char *mode);
  int   xpn_simple_close(int fd);
  int   xpn_simple_unlink(const char *path);
  int   xpn_simple_rename(const char *path, const char *newpath);
  int   xpn_simple_fstat(int fd, struct stat *sb);
  int   xpn_simple_fclose(FILE *fp);
  int   xpn_simple_stat(const char *path, struct stat *sb);
  int   xpn_simple_chown( const char *path,  uid_t owner,  gid_t group);
  int   xpn_simple_fchown(int  fd,  uid_t owner,  gid_t group);
  int   xpn_simple_chmod( const char *path,  mode_t mode);
  int   xpn_simple_fchmod( int fd,  mode_t mode);
  int   xpn_simple_truncate( const char *path,  off_t length);
  int   xpn_simple_ftruncate( int fd, off_t length);

  int   xpn_simple_dup(int fd);
  int   xpn_simple_dup2(int fd, int fd2);

  int   xpn_simple_flush(const char *virtual_path, const char *storage_path);
  int   xpn_simple_preload(const char *virtual_path, const char *storage_path);


  /* ................................................................... */

  #ifdef  __cplusplus
    }
  #endif

#endif
