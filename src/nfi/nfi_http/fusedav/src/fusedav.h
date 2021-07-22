#ifndef foofusedevhfoo
#define foofusedevhfoo

/* $Id$ */

/***
  This file is part of fusedav.

  fusedav is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  fusedav is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
  License for more details.
  
  You should have received a copy of the GNU General Public License
  along with fusedav; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
***/

#include <fuse.h>

#include "session.h"

extern int dav_debug;

#ifdef __GNUC__
#define __unused __attribute__ ((unused))
#else
#define __unused
#endif

int dav_readdir(
        struct session_info *sess,
        const char *path,
        void *buf,
        fuse_fill_dir_t filler,
        __unused off_t offset,
        __unused struct fuse_file_info *fi);
int dav_getattr(struct session_info *sess, const char *path, struct stat *stbuf);
int dav_unlink(struct session_info *sess, const char *path);
int dav_rmdir(struct session_info *sess, const char *path);
int dav_mkdir(struct session_info *sess, const char *path, __unused mode_t mode);
int dav_rename(struct session_info *sess, const char *from, const char *to);
int dav_release(struct session_info *sess, const char *path, __unused struct fuse_file_info *info);
int dav_fsync(struct session_info *sess, const char *path, __unused int isdatasync, __unused struct fuse_file_info *info);
int dav_mknod(struct session_info *sess, const char *path, mode_t mode, __unused dev_t rdev);
int dav_open(struct session_info *sess, const char *path, struct fuse_file_info *info);
int dav_read(struct session_info *sess, const char *path, char *buf, size_t size, off_t offset, __unused struct fuse_file_info *info);
int dav_write(struct session_info *sess, const char *path, const char *buf, size_t size, off_t offset, __unused struct fuse_file_info *info);
int dav_truncate(struct session_info *sess, const char *path, off_t size);
int dav_utime(struct session_info *sess, const char *path, struct utimbuf *buf);
int dav_listxattr(
        struct session_info *sess,
        const char *path,
        char *list,
        size_t size);
int dav_getxattr(
        struct session_info *sess,
        const char *path,
        const char *name,
        char *value,
        size_t size);
int dav_setxattr(
        struct session_info *sess,
        const char *path,
        const char *name,
        const char *value,
        size_t size,
        int flags);
int dav_removexattr(struct session_info *sess, const char *path, const char *name);
int dav_chmod(struct session_info *sess, const char *path, mode_t mode);
struct session_info *dav_init(int argc, char *argv[]);
void dav_destroy(struct session_info *sess);

#endif
