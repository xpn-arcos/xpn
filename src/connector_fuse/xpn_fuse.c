/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
  Copyright (C) 2011       Sebastian Pipping <sebastian@pipping.org>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  mpicc -Wall xpn_fuse.c libxpn.a -I ../../include/xpn_client `pkg-config fuse3 --cflags --libs` -o xpn_fuse
*/

#define FUSE_USE_VERSION 314
// #define DEBUG

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include "xpn.h"
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif
#include <sys/file.h> /* flock(2) */

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifdef DEBUG
#define debug_info(...)               \
    do {                              \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n");        \
    } while (0)
#else
#define debug_info(...) \
    do {                \
    } while (0)
#endif

static int xpn_path(char* dst, size_t size, const char* path)
{
    int n = snprintf(dst, size, "xpn%s", path);
    if (n < 0) return -EINVAL;
    if ((size_t)n >= size) return -ENAMETOOLONG;
    return 0;
}

static void* xmp_init(struct fuse_conn_info* conn, struct fuse_config* cfg)
{
    int res;
    (void)conn;

    debug_info("[FUSE-XPN] Init");
    debug_info("[FUSE-XPN] proto_major %u", conn->proto_major);
    debug_info("[FUSE-XPN] proto_minor %u", conn->proto_minor);
    debug_info("[FUSE-XPN] max_write %u", conn->max_write);
    debug_info("[FUSE-XPN] max_read %u", conn->max_read);
    debug_info("[FUSE-XPN] max_readahead %u", conn->max_readahead);
    debug_info("[FUSE-XPN] capable %u", conn->capable);
    debug_info("[FUSE-XPN] want %u", conn->want);
    debug_info("[FUSE-XPN] max_background %u", conn->max_background);
    debug_info("[FUSE-XPN] congestion_threshold %u", conn->congestion_threshold);
    debug_info("[FUSE-XPN] time_gran %u", conn->time_gran);

    res = xpn_init();

    cfg->direct_io = 1;
    cfg->attr_timeout = 0;
    cfg->entry_timeout = 0;
    cfg->negative_timeout = 0;

    debug_info("[FUSE-XPN] End");

    if (res == -1) return (void*)(size_t)(-errno);

    return NULL;
}

static void xmp_destroy(void* private_data)
{
    (void)private_data;
    debug_info("[FUSE-XPN] Init");

    xpn_destroy();

    debug_info("[FUSE-XPN] End");
}

static int xmp_getattr(const char* path, struct stat* stbuf, struct fuse_file_info* fi)
{
    int res;
    char p[PATH_MAX];
    (void)fi;

    debug_info("[FUSE-XPN] Init getattr(%s, %p)", path, (void*)stbuf);

    res = xpn_path(p, sizeof(p), path);
    if (res < 0) return res;
    res = xpn_stat(p, stbuf);

    debug_info("[FUSE-XPN] End getattr(%s, %p)=%ld", path, (void*)stbuf, (long)stbuf->st_size);

    if (res == -1) return -errno;
    return 0;
}

// TODO
// static int xmp_access(const char *path, int mask) {
//     int res;
//
//     res = xpn_access(path, mask);
//     if (res == -1) return -errno;
//
//     return 0;
// }

// TODO
// static int xmp_readlink(const char *path, char *buf, size_t size) {
//     int res;
//
//     res = xpn_readlink(path, buf, size - 1);
//     if (res == -1) return -errno;
//
//     buf[res] = '\0';
//     return 0;
// }

struct xmp_dirp
{
    DIR* dp;
    struct dirent* entry;
    off_t offset;
};

static int xmp_opendir(const char* path, struct fuse_file_info* fi)
{
    int res;
    char p[PATH_MAX];
    struct xmp_dirp* d;

    debug_info("[FUSE-XPN] Init opendir(%s, %llu)", path, (unsigned long long)fi->fh);

    res = xpn_path(p, sizeof(p), path);
    if (res < 0) return res;

    d = (struct xmp_dirp*)malloc(sizeof(struct xmp_dirp));
    if (d == NULL) return -ENOMEM;

    d->dp = xpn_opendir(p);
    if (d->dp == NULL) {
        res = -errno;
        free(d);
        debug_info("[FUSE-XPN] End opendir(%s, %llu)", path, (unsigned long long)fi->fh);
        return res;
    }

    d->offset = 0;
    d->entry = NULL;

    fi->fh = (uintptr_t)d;

    debug_info("[FUSE-XPN] End opendir(%s, %llu)", path, (unsigned long long)fi->fh);

    return 0;
}

static inline struct xmp_dirp* get_dirp(struct fuse_file_info* fi) { return (struct xmp_dirp*)(uintptr_t)fi->fh; }

static int xmp_readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi,
                       enum fuse_readdir_flags flags)
{
    struct xmp_dirp* d = get_dirp(fi);
    (void)path;
    (void)offset;
    (void)flags;

    debug_info("[FUSE-XPN] Init readdir(%s, %p, %lld, %llu)", path, buf, (long long)offset, (unsigned long long)fi->fh);

    // if (offset != d->offset) {
    //     xpn_seekdir(d->dp, offset);
    //     d->entry = NULL;
    //     d->offset = offset;
    // }

    while (1) {
        struct stat st;
        off_t nextoff = 0;

        if (!d->entry) {
            d->entry = xpn_readdir(d->dp);
            if (!d->entry) break;
        }

        memset(&st, 0, sizeof(st));
        st.st_ino = d->entry->d_ino;
        st.st_mode = d->entry->d_type << 12;
        // nextoff = telldir(d->dp);
        if (filler(buf, d->entry->d_name, &st, nextoff, (enum fuse_fill_dir_flags)0)) break;

        d->entry = NULL;
        // d->offset = nextoff;
    }

    debug_info("[FUSE-XPN] End readdir(%s, %p, %lld, %llu)", path, buf, (long long)offset, (unsigned long long)fi->fh);

    return 0;
}

static int xmp_releasedir(const char* path, struct fuse_file_info* fi)
{
    struct xmp_dirp* d = get_dirp(fi);
    (void)path;

    debug_info("[FUSE-XPN] Init releasedir(%s, %llu)", path, (unsigned long long)fi->fh);

    xpn_closedir(d->dp);
    free(d);

    debug_info("[FUSE-XPN] End releasedir(%s, %llu)", path, (unsigned long long)fi->fh);

    return 0;
}

// TODO
// static int xmp_mknod(const char *path, mode_t mode, dev_t rdev) {
//     int res;
//
//     if (S_ISFIFO(mode))
//         res = mkfifo(path, mode);
//     else
//         res = mknod(path, mode, rdev);
//     if (res == -1) return -errno;
//
//     return 0;
// }

static int xmp_mkdir(const char* path, mode_t mode)
{
    int res;
    char p[PATH_MAX];

    debug_info("[FUSE-XPN] Init mkdir(%s, %o)", path, (unsigned int)mode);

    res = xpn_path(p, sizeof(p), path);
    if (res < 0) return res;
    res = xpn_mkdir(p, mode);

    debug_info("[FUSE-XPN] End mkdir(%s, %o)", path, (unsigned int)mode);

    if (res == -1) return -errno;
    return 0;
}

static int xmp_unlink(const char* path)
{
    int res;
    char p[PATH_MAX];

    debug_info("[FUSE-XPN] Init unlink(%s)", path);

    res = xpn_path(p, sizeof(p), path);
    if (res < 0) return res;
    res = xpn_unlink(p);

    debug_info("[FUSE-XPN] End unlink(%s)", path);

    if (res == -1) return -errno;
    return 0;
}

static int xmp_rmdir(const char* path)
{
    int res;
    char p[PATH_MAX];

    debug_info("[FUSE-XPN] Init rmdir(%s)", path);

    res = xpn_path(p, sizeof(p), path);
    if (res < 0) return res;
    res = xpn_rmdir(p);

    debug_info("[FUSE-XPN] End rmdir(%s)", path);

    if (res == -1) return -errno;
    return 0;
}

// TODO
// static int xmp_symlink(const char *from, const char *to) {
//     int res;
//
//     res = xpn_symlink(from, to);
//     if (res == -1) return -errno;
//
//     return 0;
// }

static int xmp_rename(const char* from, const char* to, unsigned int flags)
{
    int res;
    char f[PATH_MAX];
    char t[PATH_MAX];
    (void)flags;  // TODO: fix the correct use of flags

    debug_info("[FUSE-XPN] Init rename(%s, %s)", from, to);

    res = xpn_path(f, sizeof(f), from);
    if (res < 0) return res;
    res = xpn_path(t, sizeof(t), to);
    if (res < 0) return res;
    res = xpn_rename(f, t);

    debug_info("[FUSE-XPN] End rename(%s, %s)", from, to);

    if (res == -1) return -errno;
    return 0;
}

// TODO
// static int xmp_link(const char *from, const char *to) {
//     int res;
//
//     res = xpn_link(from, to);
//     if (res == -1) return -errno;
//
//     return 0;
// }

static int xmp_chmod(const char* path, mode_t mode, struct fuse_file_info* fi)
{
    int res;
    char p[PATH_MAX];
    (void)fi;

    debug_info("[FUSE-XPN] Init chmod(%s, %o)", path, (unsigned int)mode);

    res = xpn_path(p, sizeof(p), path);
    if (res < 0) return res;
    res = xpn_chmod(p, mode);

    debug_info("[FUSE-XPN] End chmod(%s, %o)", path, (unsigned int)mode);

    if (res == -1) return -errno;
    return 0;
}

// TODO
// static int xmp_chown(const char *path, uid_t uid, gid_t gid) {
//     int res;
//
//     res = xpn_lchown(path, uid, gid);
//     if (res == -1) return -errno;
//
//     return 0;
// }

static int xmp_truncate(const char* path, off_t size, struct fuse_file_info* fi)
{
    int res;
    char p[PATH_MAX];
    (void)fi;

    debug_info("[FUSE-XPN] Init truncate(%s, %lld)", path, (long long)size);

    res = xpn_path(p, sizeof(p), path);
    if (res < 0) return res;
    res = xpn_truncate(p, size);

    debug_info("[FUSE-XPN] End truncate(%s, %lld)", path, (long long)size);

    if (res == -1) return -errno;
    return 0;
}

#ifdef HAVE_UTIMENSAT
static int xmp_utimens(const char* path, const struct timespec ts[2])
{
    int res;

    /* don't use utime/utimes since they follow symlinks */
    res = utimensat(0, path, ts, AT_SYMLINK_NOFOLLOW);
    if (res == -1) return -errno;

    return 0;
}
#endif

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi)
{
    int fd;
    int res;
    char p[PATH_MAX];

    debug_info("[FUSE-XPN] Init create(%llu, %s, %o)", (unsigned long long)fi->fh, path, (unsigned int)mode);

    res = xpn_path(p, sizeof(p), path);
    if (res < 0) return res;
    fd = xpn_open(p, fi->flags, mode);

    debug_info("[FUSE-XPN] End create(%d, %s, %o)", fd, path, (unsigned int)mode);

    if (fd == -1) return -errno;

    fi->fh = fd;
    fi->direct_io = 1;
    return 0;
}

static int xmp_open(const char* path, struct fuse_file_info* fi)
{
    int fd;
    int res;
    char p[PATH_MAX];

    debug_info("[FUSE-XPN] Init open(%llu, %s)", (unsigned long long)fi->fh, path);

    res = xpn_path(p, sizeof(p), path);
    if (res < 0) return res;
    fd = xpn_open(p, fi->flags);

    debug_info("[FUSE-XPN] End open(%d, %s)", fd, path);

    if (fd == -1) return -errno;

    fi->fh = fd;
    fi->direct_io = 1;
    return 0;
}

static int xmp_read(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* fi)
{
    int res;
    (void)path;

    debug_info("[FUSE-XPN] Init read(%llu, %s, %zu, %lld)", (unsigned long long)fi->fh, path, size, (long long)offset);

    res = xpn_pread(fi->fh, buf, size, offset);

    debug_info("[FUSE-XPN] End read(%llu, %s, %zu, %lld)", (unsigned long long)fi->fh, path, size, (long long)offset);

    if (res == -1) res = -errno;
    return res;
}

// static int xmp_read_buf(const char *path, struct fuse_bufvec **bufp, size_t size, off_t offset, struct fuse_file_info
// *fi) {
//     struct fuse_bufvec *src;
//
//     (void)path;
//
//     src = (struct fuse_bufvec *)malloc(sizeof(struct fuse_bufvec));
//     if (src == NULL) return -ENOMEM;
//
//     *src = FUSE_BUFVEC_INIT(size);
//
//     src->buf[0].flags = (enum fuse_buf_flags)(FUSE_BUF_IS_FD | FUSE_BUF_FD_SEEK);
//     src->buf[0].fd = fi->fh;
//     src->buf[0].pos = offset;
//
//     *bufp = src;
//
//     return 0;
// }

static int xmp_write(const char* path, const char* buf, size_t size, off_t offset, struct fuse_file_info* fi)
{
    int res;
    (void)path;

    debug_info("[FUSE-XPN] Init write(%llu, %s, %zu, %lld)", (unsigned long long)fi->fh, path, size, (long long)offset);

    res = xpn_pwrite(fi->fh, buf, size, offset);
    if (res == -1) res = -errno;

    debug_info("[FUSE-XPN] End write(%llu, %s, %zu, %lld)", (unsigned long long)fi->fh, path, size, (long long)offset);

    return res;
}

// static int xmp_write_buf(const char *path, struct fuse_bufvec *buf, off_t offset, struct fuse_file_info *fi) {
//     struct fuse_bufvec dst = FUSE_BUFVEC_INIT(fuse_buf_size(buf));
//
//     (void)path;
//
//     dst.buf[0].flags = (enum fuse_buf_flags)(FUSE_BUF_IS_FD | FUSE_BUF_FD_SEEK);
//     dst.buf[0].fd = fi->fh;
//     dst.buf[0].pos = offset;
//
//     return fuse_buf_copy(&dst, buf, FUSE_BUF_SPLICE_NONBLOCK);
// }

static int xmp_statfs(const char* path, struct statvfs* stbuf)
{
    int res;
    char p[PATH_MAX];

    debug_info("[FUSE-XPN] Init statfs(%s, %p)", path, (void*)stbuf);

    res = xpn_path(p, sizeof(p), path);
    if (res < 0) return res;
    res = xmp_statfs(p, stbuf);

    debug_info("[FUSE-XPN] End statfs(%s, %p)", path, (void*)stbuf);

    if (res == -1) return -errno;
    return 0;
}

static int xmp_flush(const char* path, struct fuse_file_info* fi)
{
    (void)path;
    (void)fi;

    debug_info("[FUSE-XPN] Init flush(%llu, %s)", (unsigned long long)fi->fh, path);
    /* This is called from every close on an open file, so call the
       close on the underlying filesystem.  But since flush may be
       called multiple times for an open file, this must not really
       close the file.  This is important if used on a network
       filesystem like NFS which flush the data/metadata on close() */
    // res = xpn_fflush(fi->fh);
    // res = xpn_close(xpn_fldup(fi->fh));
    // if (res == -1) return -errno;
    debug_info("[FUSE-XPN] End flush(%llu, %s)", (unsigned long long)fi->fh, path);

    return 0;
}

static int xmp_release(const char* path, struct fuse_file_info* fi)
{
    (void)path;

    debug_info("[FUSE-XPN] Init release(%llu, %s)", (unsigned long long)fi->fh, path);

    xpn_close(fi->fh);

    debug_info("[FUSE-XPN] End release(%llu, %s)", (unsigned long long)fi->fh, path);

    return 0;
}

static int xmp_fsync(const char* path, int isdatasync, struct fuse_file_info* fi)
{
    (void)path;
    (void)fi;
    (void)isdatasync;

    debug_info("[FUSE-XPN] Init fsync(%llu, %s, %d)", (unsigned long long)fi->fh, path, isdatasync);

    /* TODO: no hay xpn_fsync/xpn_fdatasync todavía */
    // int res;
    // if (isdatasync)
    //     res = xpn_fdatasync(fi->fh);
    // else
    //     res = xpn_fsync(fi->fh);
    // if (res == -1) return -errno;

    debug_info("[FUSE-XPN] End fsync(%llu, %s, %d)", (unsigned long long)fi->fh, path, isdatasync);

    return 0;
}

static off_t xmp_lseek(const char* path, off_t off, int whence, struct fuse_file_info* fi)
{
    off_t res;
    (void)path;

    debug_info("[FUSE-XPN] Init lseek(%llu, %ld, %d)", (unsigned long long)fi->fh, off, whence);

    res = xpn_lseek(fi->fh, off, whence);

    debug_info("[FUSE-XPN] End lseek(%llu, %ld, %d)", (unsigned long long)fi->fh, off, whence);

    if (res == -1) return -errno;
    return res;
}

#ifdef HAVE_POSIX_FALLOCATE
static int xmp_fallocate(const char* path, int mode, off_t offset, off_t length, struct fuse_file_info* fi)
{
    (void)path;

    if (mode) return -EOPNOTSUPP;

    return -posix_fallocate(fi->fh, offset, length);
}
#endif

#ifdef HAVE_SETXATTR
/* xattr operations are optional and can safely be left unimplemented */
static int xmp_setxattr(const char* path, const char* name, const char* value, size_t size, int flags)
{
    int res = lsetxattr(path, name, value, size, flags);
    if (res == -1) return -errno;
    return 0;
}

static int xmp_getxattr(const char* path, const char* name, char* value, size_t size)
{
    int res = lgetxattr(path, name, value, size);
    if (res == -1) return -errno;
    return res;
}

static int xmp_listxattr(const char* path, char* list, size_t size)
{
    int res = llistxattr(path, list, size);
    if (res == -1) return -errno;
    return res;
}

static int xmp_removexattr(const char* path, const char* name)
{
    int res = lremovexattr(path, name);
    if (res == -1) return -errno;
    return 0;
}
#endif /* HAVE_SETXATTR */

// static int xmp_lock(const char *path, struct fuse_file_info *fi, int cmd, struct flock *lock) {
//     (void)path;
//
//     return ulockmgr_op(fi->fh, cmd, lock, &fi->lock_owner, sizeof(fi->lock_owner));
// }

// static int xmp_flock(const char *path, struct fuse_file_info *fi, int op) {
//     int res;
//     (void)path;
//
//     res = flock(fi->fh, op);
//     if (res == -1) return -errno;
//
//     return 0;
// }

static const struct fuse_operations xmp_oper = {
    .init = xmp_init,
    .destroy = xmp_destroy,

    .getattr = xmp_getattr,
    .chmod = xmp_chmod,
    .truncate = xmp_truncate,
    .statfs = xmp_statfs,

    .mkdir = xmp_mkdir,
    .unlink = xmp_unlink,
    .rmdir = xmp_rmdir,
    .rename = xmp_rename,

    .create = xmp_create,
    .open = xmp_open,
    .read = xmp_read,
    .write = xmp_write,
    .flush = xmp_flush,
    .release = xmp_release,
    .fsync = xmp_fsync,
    .lseek = xmp_lseek,

    .opendir = xmp_opendir,
    .readdir = xmp_readdir,
    .releasedir = xmp_releasedir,
};

int main(int argc, char* argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
