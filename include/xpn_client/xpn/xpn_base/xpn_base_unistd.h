
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

#ifndef _XPN_BASE_UNISTD_H
#define _XPN_BASE_UNISTD_H


 #ifdef  __cplusplus
    extern "C" {
 #endif


     #include "xpn_base.h"
     #include "xpn_policy_rw.h"
     #include "xpn_base_filetable.h"
     #include "xpn_base_err.h"
     #include "xpn/xpn_base/xpn_unistd.h"


     extern struct xpn_filedesc *xpn_file_table[XPN_MAX_FILE];

/*
 * Internal
 */

int xpn_internal_creat(const char *path, mode_t perm, struct xpn_fh **vfh, struct xpn_metadata **mdata) ;
int xpn_internal_open ( const char *path, struct xpn_fh *vfh, struct xpn_metadata *mdata, int mode ) ;
int xpn_internal_remove(const char *path) ;


/*
 * API
 */

int xpn_base_creat(const char *path, mode_t perm) ;
int xpn_base_open(const char *path, int flags, mode_t mode) ;
int xpn_base_close(int fd) ;

ssize_t xpn_base_read(int fd, void *buffer, size_t size) ;
ssize_t xpn_base_write ( int fd, const void *buffer, size_t size ) ;
ssize_t xpn_sread(int fd, const void *buffer, size_t size, off_t offset) ;
ssize_t xpn_pread(int fd, void *buffer, size_t size, off_t offset) ;
ssize_t xpn_swrite(int fd, const void *buffer, size_t size, off_t offset) ;
ssize_t xpn_pwrite(int fd, const void *buffer, size_t size, off_t offset) ;
off_t xpn_base_lseek ( int fd, off_t offset, int flag ) ;


int xpn_base_unlink(const char *path) ;
int xpn_base_rename(const char *path, const char *newpath) ;
int xpn_base_fstat(int fd, struct stat *sb) ;

int xpn_base_stat(const char *path, struct stat *sb) ;
int xpn_base_chown(const char *path, uid_t owner, gid_t group) ;
int xpn_base_fchown(int  fd,  uid_t owner, gid_t group) ;
int xpn_base_chmod(const char *path, mode_t mode) ;
int xpn_base_fchmod(int fd, mode_t mode) ;
int xpn_base_truncate(const char *path, off_t length) ;
int xpn_base_ftruncate(int fd, off_t length) ;
int xpn_base_dup(int fd) ;
int xpn_base_dup2(int fd, int fd2) ;


 #ifdef  __cplusplus
     }
 #endif


#endif

