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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/statfs.h>
#include <getopt.h>
#include <attr/xattr.h>

#include <ne_request.h>
#include <ne_basic.h>
#include <ne_props.h>
#include <ne_utils.h>
#include <ne_socket.h>
#include <ne_auth.h>
#include <ne_dates.h>
#include <ne_redirect.h>
#include <ne_uri.h>

#include <fuse.h>

#include "statcache.h"
#include "filecache.h"
#include "remoteaccess.h"
#include "session.h"
#include "fusedav.h"

const ne_propname query_properties[] = {
    { "DAV:", "resourcetype" },
    { "http://apache.org/dav/props/", "executable" },
    { "DAV:", "getcontentlength" },
    { "DAV:", "getlastmodified" },
    { "DAV:", "creationdate" },
    { NULL, NULL }
};

mode_t mask = 0;
int dav_debug = 0;
struct fuse* fuse = NULL;
ne_lock_store *lock_store = NULL;
struct ne_lock *lock = NULL;
int lock_thread_exit = 0;
int lock_timeout = 60;

int no_cache = 0;

#define MIME_XATTR "user.mime_type"

#define MAX_REDIRECTS 10

struct fill_info {
    void *buf;
    fuse_fill_dir_t filler;
    const char *root;
};

struct getdir_propfind_callback_userdata {
    struct session_info *sess;
    struct fill_info *f;
};

struct getattr_propfind_callback_userdata {
    struct session_info *sess;
    struct stat *st;
};

static int get_stat(struct session_info *sess, const char *path, struct stat *stbuf);

static pthread_once_t path_cvt_once = PTHREAD_ONCE_INIT;
static pthread_key_t path_cvt_tsd_key;

static void path_cvt_tsd_key_init(void) {
    pthread_key_create(&path_cvt_tsd_key, free);
}

static const char *path_cvt(const char *path, struct session_info *sess) {
    char *r, *t;
    int l;

    pthread_once(&path_cvt_once, path_cvt_tsd_key_init);
    
    if ((r = pthread_getspecific(path_cvt_tsd_key)))
        free(r);

    t = malloc((l = strlen(sess->base_directory)+strlen(path))+1);
    assert(t);
    sprintf(t, "%s%s", sess->base_directory, path);

    if (l > 1 && t[l-1] == '/')
        t[l-1] = 0;

    r = ne_path_escape(t);
    free(t);

    pthread_setspecific(path_cvt_tsd_key, r);
    
    return r;
}

static int simple_propfind_with_redirect(
        struct session_info *sess,
        ne_session *session,
        const char *path,
        int depth,
        const ne_propname *props,
        ne_props_result results,
        void *userdata) {

    int i, ret;
    
    for (i = 0; i < MAX_REDIRECTS; i++) {
        const ne_uri *u;

        if ((ret = ne_simple_propfind(session, path, depth, props, results, userdata)) != NE_REDIRECT)
            return ret;

        if (!(u = ne_redirect_location(session)))
            break;

        if (!session_is_local(sess, u))
            break;

        if (dav_debug)
            fprintf(stderr, "REDIRECT FROM '%s' to '%s'\n", path, u->path);
        
        path = u->path;
    }

    return ret;
}

static int getmodtime_and_length_with_redirect(
        struct session_info *sess,
        ne_session *session,
        const char *path,
        char **length,
        char **modtime) {

    int i, ret;
    
    for (i = 0; i < MAX_REDIRECTS; i++) {
        const ne_uri *u;

        if ((ret = ne_getmodtime_and_length(session, path, length, modtime)) != NE_REDIRECT)
            return ret;

        if (!(u = ne_redirect_location(session)))
            break;

        if (!session_is_local(sess, u))
            break;

        if (dav_debug)
            fprintf(stderr, "REDIRECT FROM '%s' to '%s'\n", path, u->path);
        
        path = u->path;
    }

    return ret;
}

static int proppatch_with_redirect(
        struct session_info *sess,
        ne_session *session,
        const char *path,
        const ne_proppatch_operation *ops) {
    
    int i, ret;
    
    for (i = 0; i < MAX_REDIRECTS; i++) {
        const ne_uri *u;

        if ((ret = ne_proppatch(session, path, ops)) != NE_REDIRECT)
            return ret;

        if (!(u = ne_redirect_location(session)))
            break;

        if (!session_is_local(sess, u))
            break;

        if (dav_debug)
            fprintf(stderr, "REDIRECT FROM '%s' to '%s'\n", path, u->path);
        
        path = u->path;
    }

    return ret;
}


static void fill_stat(struct stat* st, const ne_prop_result_set *results, int is_dir) {
    const char *rt, *e, *gcl, *glm, *cd;
    const ne_propname resourcetype = { "DAV:", "resourcetype" };
    const ne_propname executable = { "http://apache.org/dav/props/", "executable" };
    const ne_propname getcontentlength = { "DAV:", "getcontentlength" };
    const ne_propname getlastmodified = { "DAV:", "getlastmodified" };
    const ne_propname creationdate = { "DAV:", "creationdate" };
        
    assert(st && results);

    rt = ne_propset_value(results, &resourcetype);
    e = ne_propset_value(results, &executable);
    gcl = ne_propset_value(results, &getcontentlength);
    glm = ne_propset_value(results, &getlastmodified);
    cd = ne_propset_value(results, &creationdate);

    memset(st, 0, sizeof(struct stat));
    
    if (is_dir) {
        st->st_mode = S_IFDIR | 0777;
        st->st_nlink = 3;            /* find will ignore this directory if nlin <= and st_size == 0 */
        st->st_size = 4096;
    } else {
        st->st_mode = S_IFREG | (e && (*e == 'T' || *e == 't') ? 0777 : 0666);
        st->st_nlink = 1;
        st->st_size = gcl ? atoll(gcl) : 0;
    }

    st->st_atime = time(NULL);
    st->st_mtime = glm ? ne_rfc1123_parse(glm) : 0;
    st->st_ctime = cd ? ne_iso8601_parse(cd) : 0;
    
    st->st_blocks = (st->st_size+511)/512;
    /*fprintf(stderr, "a: %u; m: %u; c: %u\n", st->st_atime, st->st_mtime, st->st_ctime);*/

    st->st_mode &= ~mask;
    
    st->st_uid = getuid();
    st->st_gid = getgid();
}

static void fill_stat_modtime_and_length(struct stat* st, int is_dir, const char *gcl, const char *glm, const char *cd) {
        
    assert(st);

    memset(st, 0, sizeof(struct stat));
    
    if (is_dir) {
        st->st_mode = S_IFDIR | 0777;
        st->st_nlink = 3;            /* find will ignore this directory if nlin <= and st_size == 0 */
        st->st_size = 4096;
    } else {
        st->st_mode = S_IFREG | 0666;
        st->st_nlink = 1;
        st->st_size = gcl ? atoll(gcl) : 0;
    }

    st->st_atime = time(NULL);
    st->st_mtime = glm ? ne_rfc1123_parse(glm) : 0;
    st->st_ctime = cd ? ne_iso8601_parse(cd) : 0;
    
    st->st_blocks = (st->st_size+511)/512;
    /*fprintf(stderr, "a: %u; m: %u; c: %u\n", st->st_atime, st->st_mtime, st->st_ctime);*/

    st->st_mode &= ~mask;
    
    st->st_uid = getuid();
    st->st_gid = getgid();
}

static char *strip_trailing_slash(char *fn, int *is_dir) {
    size_t l = strlen(fn);
    assert(fn);
    assert(is_dir);
    assert(l > 0);
    
    if ((*is_dir = (fn[l-1] == '/')))
        fn[l-1] = 0;

    return fn;
}

static void getdir_propfind_callback(void *userdata, const ne_uri *u, const ne_prop_result_set *results) {
    struct session_info *sess = ((struct getdir_propfind_callback_userdata *)userdata)->sess;
    struct fill_info *f = ((struct getdir_propfind_callback_userdata *)userdata)->f;
    struct stat st;
    char fn[PATH_MAX], *t;
    int is_dir = 0;

    assert(f);

    strncpy(fn, u->path, sizeof(fn));
    fn[sizeof(fn)-1] = 0;
    strip_trailing_slash(fn, &is_dir);

    if (strcmp(fn, f->root) && fn[0]) {
        char *h;
        
        if ((t = strrchr(fn, '/')))
            t++;
        else
            t = fn;

        dir_cache_add(sess, f->root, t);
        f->filler(f->buf, h = ne_path_unescape(t), NULL, 0);
        free(h);
    }

    fill_stat(&st, results, is_dir);
    stat_cache_set(sess, fn, &st);
}

static void getdir_cache_callback(
        const char *root,
        const char *fn,
        void *user) {
    
    struct fill_info *f = user;
    char path[PATH_MAX];
    char *h;

    assert(f);
    
    snprintf(path, sizeof(path), "%s/%s", !strcmp(root, "/") ? "" : root, fn);
    
    f->filler(f->buf, h = ne_path_unescape(fn), NULL, 0);
    free(h);
}

int dav_readdir(
        struct session_info *sess,
        const char *path,
        void *buf,
        fuse_fill_dir_t filler,
        __unused off_t offset,
        __unused struct fuse_file_info *fi) {
    
    struct fill_info f;
    struct getdir_propfind_callback_userdata u;
    ne_session *session;

    path = path_cvt(path, sess);

    if (dav_debug)
        fprintf(stderr, "getdir(%s)\n", path);

    f.buf = buf;
    f.filler = filler;
    f.root = path;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    
    if (dir_cache_enumerate(sess, path, getdir_cache_callback, &f) < 0) {

        if (dav_debug)
            fprintf(stderr, "DIR-CACHE-MISS\n");
        
        if (!(session = session_get(sess, 1))) 
            return -EIO;

        dir_cache_begin(sess, path);
        
        u.sess = sess;
        u.f = &f;
        if (simple_propfind_with_redirect(sess, session, path, NE_DEPTH_ONE, query_properties, getdir_propfind_callback, &u) != NE_OK) {
            dir_cache_finish(sess, path, 2);
            fprintf(stderr, "dav_readdir: PROPFIND failed: %s\n", ne_get_error(session));
            return -ENOENT;
        }

        dir_cache_finish(sess, path, 1);
    }

    return 0;
}

static void getattr_propfind_callback(void *userdata, const ne_uri *u, const ne_prop_result_set *results) {
    struct session_info *sess = ((struct getattr_propfind_callback_userdata *)userdata)->sess;
    struct stat *st = ((struct getattr_propfind_callback_userdata *)userdata)->st;
    char fn[PATH_MAX];
    int is_dir;

    assert(st);

    strncpy(fn, u->path, sizeof(fn));
    fn[sizeof(fn)-1] = 0;
    strip_trailing_slash(fn, &is_dir);
    
    fill_stat(st, results, is_dir);
    stat_cache_set(sess, fn, st);
}

static int get_stat(struct session_info *sess, const char *path, struct stat *stbuf) {
    ne_session *session;
    struct getattr_propfind_callback_userdata u;

    if (!(session = session_get(sess, 1))) 
        return -EIO;

    if (stat_cache_get(sess, path, stbuf) == 0) {
        return stbuf->st_mode == 0 ? -ENOENT : 0;
    } else {
        if (dav_debug)
            fprintf(stderr, "STAT-CACHE-MISS\n");

        u.sess = sess;
        u.st = stbuf;
        if (simple_propfind_with_redirect(sess, session, path, NE_DEPTH_ZERO, query_properties, getattr_propfind_callback, &u) != NE_OK) {
            stat_cache_invalidate(sess, path);
            //fprintf(stderr, "get_stat: PROPFIND failed: %s\n", ne_get_error(session));
            return -ENOENT;
        }

        return 0;
    }
}

static int get_stat_with_head(struct session_info *sess, const char *path, struct stat *stbuf) {
    ne_session *session;
    struct getattr_propfind_callback_userdata u;
    char *length, *modtime;
    
    length = modtime = NULL;

    if (!(session = session_get(sess, 1))) 
        return -EIO;

    if (stat_cache_get(sess, path, stbuf) == 0) {
        return stbuf->st_mode == 0 ? -ENOENT : 0;
    } else {
        if (dav_debug)
            fprintf(stderr, "STAT-CACHE-MISS\n");

        u.sess = sess;
        u.st = stbuf;
        if (getmodtime_and_length_with_redirect(sess, session, path, &length, &modtime) != NE_OK) {
            stat_cache_invalidate(sess, path);
            //fprintf(stderr, "get_stat: PROPFIND failed: %s\n", ne_get_error(session));
            return -ENOENT;
        }
        else {
		char fn[PATH_MAX];
		int is_dir;

		assert(stbuf);

		strncpy(fn, path, sizeof(fn));
		fn[sizeof(fn)-1] = 0;
		strip_trailing_slash(fn, &is_dir);
		
		fill_stat_modtime_and_length(stbuf, is_dir, length, modtime, NULL);
		stat_cache_set(sess, fn, stbuf);
        }

        return 0;
    }
}

int dav_getattr(struct session_info *sess, const char *path, struct stat *stbuf) {
    int ret;
    
    //path = path_cvt(path, sess); // We don't want to remove the trailing '/' in dirs

    if (dav_debug)
        fprintf(stderr, "getattr(%s)\n", path);

    ret = get_stat(sess, path, stbuf);

    if (ret < 0)
        ret = get_stat_with_head(sess, path, stbuf); // If get_stat() fails, let's try using HEAD
    
    return ret;
}

int dav_unlink(struct session_info *sess, const char *path) {
    int r;
    struct stat st;
    ne_session *session;

    path = path_cvt(path, sess);

    if (dav_debug)
        fprintf(stderr, "unlink(%s)\n", path);

    if (!(session = session_get(sess, 1))) 
        return -EIO;

    if ((r = get_stat(sess, path, &st)) < 0)
        return r;

    if (!S_ISREG(st.st_mode))
        return -EISDIR;
    
    if (ne_delete(session, path)) {
        fprintf(stderr, "DELETE failed: %s\n", ne_get_error(session));
        return -ENOENT;
    }

    stat_cache_invalidate(sess, path);
    dir_cache_invalidate_parent(sess, path);
    
    return 0;
}

int dav_rmdir(struct session_info *sess, const char *path) {
    char fn[PATH_MAX];
    int r;
    struct stat st;
    ne_session *session;

    path = path_cvt(path, sess);

    if (dav_debug)
        fprintf(stderr, "rmdir(%s)\n", path);

    if (!(session = session_get(sess, 1))) 
        return -EIO;

    if ((r = get_stat(sess, path, &st)) < 0)
        return r;

    if (!S_ISDIR(st.st_mode))
        return -ENOTDIR;

    snprintf(fn, sizeof(fn), "%s/", path);
    
    if (ne_delete(session, fn)) {
        fprintf(stderr, "DELETE failed: %s\n", ne_get_error(session));
        return -ENOENT;
    }

    stat_cache_invalidate(sess, path);
    dir_cache_invalidate_parent(sess, path);

    return 0;
}

int dav_mkdir(struct session_info *sess, const char *path, __unused mode_t mode) {
    char fn[PATH_MAX];
    ne_session *session;

    path = path_cvt(path, sess);

    if (dav_debug)
        fprintf(stderr, "mkdir(%s)\n", path);

    if (!(session = session_get(sess, 1))) 
        return -EIO;

    snprintf(fn, sizeof(fn), "%s/", path);
    
    if (ne_mkcol(session, fn)) {
        fprintf(stderr, "MKCOL failed: %s\n", ne_get_error(session));
        return -ENOENT;
    }

    stat_cache_invalidate(sess, path);
    dir_cache_invalidate_parent(sess, path);
    
    return 0;
}

int dav_rename(struct session_info *sess, const char *from, const char *to) {
    ne_session *session;
    int r = 0;
    struct stat st;
    char fn[PATH_MAX], *_from;

    from = _from = strdup(path_cvt(from, sess));
    assert(from);
    to = path_cvt(to, sess);

    if (dav_debug)
        fprintf(stderr, "rename(%s, %s)\n", from, to);

    if (!(session = session_get(sess, 1))) {
        r = -EIO;
        goto finish;
    }

    if ((r = get_stat(sess, from, &st)) < 0)
        goto finish;

    if (S_ISDIR(st.st_mode)) {
        snprintf(fn, sizeof(fn), "%s/", from);
        from = fn;
    }
    
    if (ne_move(session, 1, from, to)) {
        fprintf(stderr, "MOVE failed: %s\n", ne_get_error(session));
        r = -ENOENT;
        goto finish;
    }
    
    stat_cache_invalidate(sess, from);
    stat_cache_invalidate(sess, to);

    dir_cache_invalidate_parent(sess, from);
    dir_cache_invalidate_parent(sess, to);

finish:

    free(_from);
    
    return r;
}

int dav_release(struct session_info *sess, const char *path, __unused struct fuse_file_info *info) {
    void *f = NULL;
    int r = 0;
    ne_session *session;

    path = path_cvt(path, sess);

    if (dav_debug)
        fprintf(stderr, "release(%s)\n", path);

    if (!(session = session_get(sess, 1))) {
        r = -EIO;
        goto finish;
    }
    
    if (!(f = file_cache_get(sess, path))) {
        //fprintf(stderr, "release() called for closed file\n");
        r = -EFAULT;
        goto finish;
    }

    if (file_cache_close(sess, f) < 0) {
        r = -errno;
        goto finish;
    }

finish:
    if (f)
        file_cache_unref(sess, f);
    
    return r;
}

int dav_fsync(struct session_info *sess, const char *path, __unused int isdatasync, __unused struct fuse_file_info *info) {
    void *f = NULL;
    int r = 0;
    ne_session *session;

    path = path_cvt(path, sess);
    if (dav_debug)
        fprintf(stderr, "fsync(%s)\n", path);

    if (!(session = session_get(sess, 1))) {
        r = -EIO;
        goto finish;
    }

    if (!(f = file_cache_get(sess, path))) {
        fprintf(stderr, "fsync() called for closed file\n");
        r = -EFAULT;
        goto finish;
    }

    if ((!no_cache) && (file_cache_sync(sess, f) < 0)) {
        r = -errno;
        goto finish;
    }

finish:
    
    if (f)
        file_cache_unref(sess, f);

    return r;
}

int dav_mknod(struct session_info *sess, const char *path, mode_t mode, __unused dev_t rdev) {
    char tempfile[PATH_MAX];
    int fd;
    ne_session *session;

    path = path_cvt(path, sess);
    if (dav_debug)
        fprintf(stderr, "mknod(%s)\n", path);

    if (!(session = session_get(sess, 1))) 
        return -EIO;

    if (!S_ISREG(mode))
        return -ENOTSUP;

    snprintf(tempfile, sizeof(tempfile), "%s/fusedav-empty-XXXXXX", "/tmp");
    if ((fd = mkstemp(tempfile)) < 0)
        return -errno;
    
    unlink(tempfile);
    
    if (ne_put(session, path, fd)) {
        fprintf(stderr, "mknod:PUT failed: %s\n", ne_get_error(session));
        close(fd);
        return -EACCES;
    }

    close(fd);

    stat_cache_invalidate(sess, path);
    dir_cache_invalidate_parent(sess, path);

    return 0;
}

int dav_open(struct session_info *sess, const char *path, struct fuse_file_info *info) {
    void *f;

    if (dav_debug)
        fprintf(stderr, "open(%s)\n", path);

    path = path_cvt(path, sess);

    if (!(f = file_cache_open(sess, path, info->flags)))
        return -errno;

    file_cache_unref(sess, f);

    return 0;
}

int dav_read(struct session_info *sess, const char *path, char *buf, size_t size, off_t offset, __unused struct fuse_file_info *info) {
    void *f = NULL;
    ssize_t r;
 
    path = path_cvt(path, sess);
    
    if (dav_debug)
        fprintf(stderr, "read(%s, %lu+%lu)\n", path, (unsigned long) offset, (unsigned long) size);
    
    if (!(f = file_cache_get(sess, path))) {
        fprintf(stderr, "read() called for closed file\n");
        r = -EFAULT;
        goto finish;
    }

    if (no_cache) {
        if ((r = remote_access_read(sess, f, buf, size, offset)) < 0) {
            r = -errno;
            goto finish;
        }
    } else {
        if ((r = file_cache_read(sess, f, buf, size, offset)) < 0) {
            r = -errno;
            goto finish;
        }
    }

finish:
    if (f)
        file_cache_unref(sess, f);

    return r;
}

int dav_write(struct session_info *sess, const char *path, const char *buf, size_t size, off_t offset, __unused struct fuse_file_info *info) {
    void *f = NULL;
    ssize_t r;

    path = path_cvt(path, sess);

    if (dav_debug)
        fprintf(stderr, "write(%s, %lu+%lu)\n", path, (unsigned long) offset, (unsigned long) size);

    if (!(f = file_cache_get(sess, path))) {
        fprintf(stderr, "write() called for closed file\n");
        r = -EFAULT;
        goto finish;
    }

    if (no_cache) {
        if ((r = remote_access_write(sess, f, buf, size, offset)) < 0) {
            r = -errno;
            goto finish;
        }
    } else {
        if ((r = file_cache_write(sess, f, buf, size, offset)) < 0) {
            r = -errno;
            goto finish;
        }
    }

finish:
    if (f)
        file_cache_unref(sess, f);

    return r;
}


int dav_truncate(struct session_info *sess, const char *path, off_t size) {
    void *f = NULL;
    int r = 0;
    ne_session *session;
    
    path = path_cvt(path, sess);
    
    if (dav_debug)
        fprintf(stderr, "truncate(%s, %lu)\n", path, (unsigned long) size);

    if (!(session = session_get(sess, 1)))
        r = -EIO;
        goto finish;
    
    if (!(f = file_cache_get(sess, path))) {
        fprintf(stderr, "truncate() called for closed file\n");
        r = -EFAULT;
        goto finish;
    }

    if (file_cache_truncate(f, size) < 0) {
        r = -errno;
        goto finish;
    }

finish:
    if (f)
        file_cache_unref(sess, f);
    
    return r;
}

int dav_utime(struct session_info *sess, const char *path, struct utimbuf *buf) {
    ne_session *session;
    const ne_propname getlastmodified = { "DAV:", "getlastmodified" };
    ne_proppatch_operation ops[2];
    int r = 0;
    char *date;
    
    assert(path);
    assert(buf);

    path = path_cvt(path, sess);
    
    if (dav_debug)
        fprintf(stderr, "utime(%s, %lu, %lu)\n", path, (unsigned long) buf->actime, (unsigned long) buf->modtime);
    
    ops[0].name = &getlastmodified;
    ops[0].type = ne_propset;
    ops[0].value = date = ne_rfc1123_date(buf->modtime);
    ops[1].name = NULL;

    if (!(session = session_get(sess, 1))) {
        r = -EIO;
        goto finish;
    }

    if (proppatch_with_redirect(sess, session, path, ops)) {
        fprintf(stderr, "PROPPATCH failed: %s\n", ne_get_error(session));
        r = -ENOTSUP;
        goto finish;
    }
    
    stat_cache_invalidate(sess, path);

finish:
    free(date);
    
    return r;
}

static const char *fix_xattr(const char *name) {
    assert(name);

    if (!strcmp(name, MIME_XATTR))
        return "user.webdav(DAV:;getcontenttype)";

    return name;
}

struct listxattr_info {
    char *list;
    size_t space, size;
};

static int listxattr_iterator(
        void *userdata,
        const ne_propname *pname,
        const char *value,
        __unused const ne_status *status) {

    struct listxattr_info *l = userdata;
    int n;
    
    assert(l);

    if (!value || !pname)
        return -1;

    if (l->list) {
        n = snprintf(l->list, l->space, "user.webdav(%s;%s)", pname->nspace, pname->name) + 1;
        
        if (n >= (int) l->space) {
            l->size += l->space;
            l->space = 0;
            return 1;
            
        } else {
            l->size += n;
            l->space -= n;
            
            if (l->list)
                l->list += n;
            
            return 0;
        }
    } else {
        /* Calculate space */
        
        l->size += strlen(pname->nspace) + strlen(pname->name) + 15;
        return 0;
    }
}

static void listxattr_propfind_callback(void *userdata, __unused const ne_uri *u, const ne_prop_result_set *results) {
    struct listxattr_info *l = userdata;
    ne_propset_iterate(results, listxattr_iterator, l);
}

int dav_listxattr(
        struct session_info *sess,
        const char *path,
        char *list,
        size_t size) {
    
    ne_session *session;
    struct listxattr_info l;
    

    assert(path);

    path = path_cvt(path, sess);

    if (dav_debug)
        fprintf(stderr, "listxattr(%s, .., %lu)\n", path, (unsigned long) size);

    if (list) {
        l.list = list;
        l.space = size-1;
        l.size = 0;

        if (l.space >= sizeof(MIME_XATTR)) {
            memcpy(l.list, MIME_XATTR, sizeof(MIME_XATTR));
            l.list += sizeof(MIME_XATTR);
            l.space -= sizeof(MIME_XATTR);
            l.size += sizeof(MIME_XATTR);
        }
        
    } else {
        l.list = NULL;
        l.space = 0;
        l.size = sizeof(MIME_XATTR);
    }
    
    if (!(session = session_get(sess, 1))) 
        return -EIO;

    if (simple_propfind_with_redirect(sess, session, path, NE_DEPTH_ZERO, NULL, listxattr_propfind_callback, &l) != NE_OK) {
        fprintf(stderr, "dav_listxattr: PROPFIND failed: %s\n", ne_get_error(session));
        return -EIO;
    }

    if (l.list) {
        assert(l.space > 0);
        *l.list = 0;
    }

    return l.size+1;
}

struct getxattr_info {
    ne_propname propname;
    char *value;
    size_t space, size;
};

static int getxattr_iterator(
        void *userdata,
        const ne_propname *pname,
        const char *value,
        __unused const ne_status *status) {

    struct getxattr_info *g = userdata;
    
    assert(g);

    if (!value || !pname)
        return -1;

    if (strcmp(pname->nspace, g->propname.nspace) ||
        strcmp(pname->name, g->propname.name))
        return 0;

    if (g->value) {
        size_t l;

        l = strlen(value);

        if (l > g->space)
            l = g->space;

        memcpy(g->value, value, l);
        g->size = l;
    } else {
        /* Calculate space */
        
        g->size = strlen(value);
        return 0;
    }
    
    return 0;
}

static void getxattr_propfind_callback(void *userdata, __unused const ne_uri *u, const ne_prop_result_set *results) {
    struct getxattr_info *g = userdata;
    ne_propset_iterate(results, getxattr_iterator, g);
}

static int parse_xattr(const char *name, char *dnspace, size_t dnspace_length, char *dname, size_t dname_length) {
    char *e;
    size_t k;
    
    assert(name);
    assert(dnspace);
    assert(dnspace_length);
    assert(dname);
    assert(dname_length);
    
    if (strncmp(name, "user.webdav(", 12) ||
        name[strlen(name)-1] != ')' ||
        !(e = strchr(name+12, ';')))
        return -1;

    if ((k = strcspn(name+12, ";")) > dnspace_length-1)
        return -1;

    memcpy(dnspace, name+12, k);
    dnspace[k] = 0;

    e++;
    
    if ((k = strlen(e)) > dname_length-1)
        return -1;

    assert(k > 0);
    k--;

    memcpy(dname, e, k);
    dname[k] = 0;

    return 0;
}

int dav_getxattr(
        struct session_info *sess,
        const char *path,
        const char *name,
        char *value,
        size_t size) {

    ne_session *session;
    struct getxattr_info g;
    ne_propname props[2];
    char dnspace[128], dname[128];
        
    assert(path);

    path = path_cvt(path, sess);
    name = fix_xattr(name);

    if (dav_debug)
        fprintf(stderr, "getxattr(%s, %s, .., %lu)\n", path, name, (unsigned long) size);

    if (parse_xattr(name, dnspace, sizeof(dnspace), dname, sizeof(dname)) < 0)
        return -ENOATTR;

    props[0].nspace = dnspace;
    props[0].name = dname;
    props[1].nspace = NULL;
    props[1].name = NULL;

    if (value) {
        g.value = value;
        g.space = size;
        g.size = (size_t) -1;
    } else {
        g.value = NULL;
        g.space = 0;
        g.size = (size_t) -1;
    }

    g.propname = props[0];
    
    if (!(session = session_get(sess, 1)))
        return -EIO;

    if (simple_propfind_with_redirect(sess, session, path, NE_DEPTH_ZERO, props, getxattr_propfind_callback, &g) != NE_OK) {
        fprintf(stderr, "dav_getxattr: PROPFIND failed: %s\n", ne_get_error(session));
        return -EIO;
    }

    if (g.size == (size_t) -1)
        return -ENOATTR;

    return g.size;
}

int dav_setxattr(
        struct session_info *sess,
        const char *path,
        const char *name,
        const char *value,
        size_t size,
        int flags) {

    ne_session *session;
    ne_propname propname;
    ne_proppatch_operation ops[2];
    int r = 0;
    char dnspace[128], dname[128];
    char *value_fixed = NULL;

    assert(path);
    assert(name);
    assert(value);

    path = path_cvt(path, sess);
    name = fix_xattr(name);
    
    if (dav_debug)
        fprintf(stderr, "setxattr(%s, %s)\n", path, name);

    if (flags) {
        r = ENOTSUP;
        goto finish;
    }
    
    if (parse_xattr(name, dnspace, sizeof(dnspace), dname, sizeof(dname)) < 0) {
        r = -ENOATTR;
        goto finish;
    }

    propname.nspace = dnspace;
    propname.name = dname;
    
    /* Add trailing NUL byte if required */
    if (!memchr(value, 0, size)) {
        value_fixed = malloc(size+1);
        assert(value_fixed);
        
        memcpy(value_fixed, value, size);
        value_fixed[size] = 0;

        value = value_fixed;
    }
    
    ops[0].name = &propname;
    ops[0].type = ne_propset;
    ops[0].value = value;
    
    ops[1].name = NULL;

    if (!(session = session_get(sess, 1))) {
        r = -EIO;
        goto finish;
    }
                 
    if (proppatch_with_redirect(sess, session, path, ops)) {
        fprintf(stderr, "PROPPATCH failed: %s\n", ne_get_error(session));
        r = -ENOTSUP;
        goto finish;
    }
    
    stat_cache_invalidate(sess, path);

finish:
    free(value_fixed);
    
    return r;
}

int dav_removexattr(struct session_info *sess, const char *path, const char *name) {
    ne_session *session;
    ne_propname propname;
    ne_proppatch_operation ops[2];
    int r = 0;
    char dnspace[128], dname[128];

    assert(path);
    assert(name);

    path = path_cvt(path, sess);
    name = fix_xattr(name);
    
    if (dav_debug)
        fprintf(stderr, "removexattr(%s, %s)\n", path, name);

    if (parse_xattr(name, dnspace, sizeof(dnspace), dname, sizeof(dname)) < 0) {
        r = -ENOATTR;
        goto finish;
    }

    propname.nspace = dnspace;
    propname.name = dname;
    
    ops[0].name = &propname;
    ops[0].type = ne_propremove;
    ops[0].value = NULL;
    
    ops[1].name = NULL;

    if (!(session = session_get(sess, 1))) {
        r = -EIO;
        goto finish;
    }
                 
    if (proppatch_with_redirect(sess, session, path, ops)) {
        fprintf(stderr, "PROPPATCH failed: %s\n", ne_get_error(session));
        r = -ENOTSUP;
        goto finish;
    }
    
    stat_cache_invalidate(sess, path);

finish:
    
    return r;
}

int dav_chmod(struct session_info *sess, const char *path, mode_t mode) {
    ne_session *session;
    const ne_propname executable = { "http://apache.org/dav/props/", "executable" };
    ne_proppatch_operation ops[2];
    int r = 0;
    
    assert(path);

    path = path_cvt(path, sess);
    
    if (dav_debug)
        fprintf(stderr, "chmod(%s, %04o)\n", path, mode);
    
    ops[0].name = &executable;
    ops[0].type = ne_propset;
    ops[0].value = mode & 0111 ? "T" : "F";
    ops[1].name = NULL;

    if (!(session = session_get(sess, 1))) {
        r = -EIO;
        goto finish;
    }

    if (proppatch_with_redirect(sess, session, path, ops)) {
        fprintf(stderr, "PROPPATCH failed: %s\n", ne_get_error(session));
        r = -ENOTSUP;
        goto finish;
    }
    
    stat_cache_invalidate(sess, path);

finish:
    
    return r;
}

/*static struct fuse_operations dav_oper = {
    .getattr	 = dav_getattr,
    .readdir	 = dav_readdir,
    .mknod	 = dav_mknod,
    .mkdir	 = dav_mkdir,
    .unlink	 = dav_unlink,
    .rmdir	 = dav_rmdir,
    .rename	 = dav_rename,
    .chmod       = dav_chmod,
    .truncate	 = dav_truncate,
    .utime       = dav_utime,
    .open	 = dav_open,
    .read	 = dav_read,
    .write	 = dav_write,
    .release	 = dav_release,
    .fsync	 = dav_fsync,
    .setxattr    = dav_setxattr,
    .getxattr    = dav_getxattr,
    .listxattr   = dav_listxattr,
    .removexattr = dav_removexattr,
};*/

static void usage(char *argv0) {
    char *e;

    if ((e = strrchr(argv0, '/')))
        e++;
    else
        e = argv0;
    
    fprintf(stderr,
            "%s [-hDLr] [-t SECS] [-u USERNAME] [-p PASSWORD] [-o OPTIONS] URL MOUNTPOINT\n"
            "\t-h Show this help\n"
            "\t-D Enable debug mode\n"
            "\t-u Username if required\n"
            "\t-p Password if required\n"
            "\t-o Additional FUSE mount options\n"
            "\t-L Locking the repository during mount\n"
            "\t-t Set lock timeout\n"
            "\t-n No file cache on disk (do not cache file data on disk, default is file cache on disk)\n",
            e);
}

static void exit_handler(__unused int sig) {
    static const char m[] = "*** Caught signal ***\n";
    /*if(fuse != NULL)
        fuse_exit(fuse);*/
    write(2, m, strlen(m));
}

static void empty_handler(__unused int sig) {}

static int setup_signal_handlers(void) {
    struct sigaction sa;
    sigset_t m;
                                                                            
    sa.sa_handler = exit_handler;
    sigemptyset(&(sa.sa_mask));
    sa.sa_flags = 0;
    
    if (sigaction(SIGHUP, &sa, NULL) == -1 ||
        sigaction(SIGINT, &sa, NULL) == -1 ||
        sigaction(SIGTERM, &sa, NULL) == -1) {
                                                                                                        
        fprintf(stderr, "Cannot set exit signal handlers: %s\n", strerror(errno));
        return -1;
    }
                                                                                                        
    sa.sa_handler = SIG_IGN;
                                                                                                        
    if (sigaction(SIGPIPE, &sa, NULL) == -1) {
        fprintf(stderr, "Cannot set ignored signals: %s\n", strerror(errno));
        return -1;
    }

    /* Used to shut down the locking thread */
    sa.sa_handler = empty_handler;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        fprintf(stderr, "Cannot set user signals: %s\n", strerror(errno));
        return -1;
    }

    sigemptyset(&m);
    pthread_sigmask(SIG_BLOCK, &m, &m);
    sigdelset(&m, SIGHUP);
    sigdelset(&m, SIGINT);
    sigdelset(&m, SIGTERM);
    sigaddset(&m, SIGPIPE);
    sigaddset(&m, SIGUSR1);
    pthread_sigmask(SIG_SETMASK, &m, NULL);
    
    return 0;
}

static int create_lock(struct session_info *sess) {
    ne_session *session;
    char _owner[64], *owner;
    int i;
    int ret;
    
    lock = ne_lock_create();
    assert(lock);

    if (!(session = session_get(sess, 0)))
        return -1;

    if (!(owner = sess->username))
        if (!(owner = getenv("USER")))
            if (!(owner = getenv("LOGNAME"))) {
                snprintf(_owner, sizeof(_owner), "%lu", (unsigned long) getuid());
                owner = owner;
            }

    ne_fill_server_uri(session, &lock->uri);
    
    lock->uri.path = strdup(sess->base_directory);
    lock->depth = NE_DEPTH_INFINITE;
    lock->timeout = lock_timeout;
    lock->owner = strdup(owner);

    if (dav_debug)
        fprintf(stderr, "Acquiring lock...\n");
    
    for (i = 0; i < MAX_REDIRECTS; i++) {
        const ne_uri *u;

        if ((ret = ne_lock(session, lock)) != NE_REDIRECT)
            break;

        if (!(u = ne_redirect_location(session)))
            break;

        if (!session_is_local(sess, u))
            break;

        if (dav_debug)
            fprintf(stderr, "REDIRECT FROM '%s' to '%s'\n", lock->uri.path, u->path);

        free(lock->uri.path);
        lock->uri.path = strdup(u->path);
    }

    if (ret) {
        fprintf(stderr, "LOCK failed: %s\n", ne_get_error(session));
        ne_lock_destroy(lock);
        lock = NULL;
        return -1;
    }

    lock_store = ne_lockstore_create();
    assert(lock_store);
    
    ne_lockstore_add(lock_store, lock);
    
    return 0;
}

static int remove_lock(struct session_info *sess) {
    ne_session *session;

    assert(lock);

    if (!(session = session_get(sess, 0)))
        return -1;

    if (dav_debug)
        fprintf(stderr, "Removing lock...\n");

    if (ne_unlock(session, lock)) {
        fprintf(stderr, "UNLOCK failed: %s\n", ne_get_error(session));
        return -1;
    }

    return 0;
}

static void *lock_thread_func(void *p) {
    ne_session *session;
    sigset_t block;
    struct session_info *sess = (struct session_info *)p;

    if (dav_debug)
        fprintf(stderr, "lock_thread entering\n");

    if (!(session = session_get(sess, 1)))
        return NULL;

    sigemptyset(&block);
    sigaddset(&block, SIGUSR1);

    assert(lock);

    while (!lock_thread_exit) {
        int r, t;
        
        lock->timeout = lock_timeout;

        pthread_sigmask(SIG_BLOCK, &block, NULL);
        r = ne_lock_refresh(session, lock);
        pthread_sigmask(SIG_UNBLOCK, &block, NULL);

        if (r) {
            fprintf(stderr, "LOCK refresh failed: %s\n", ne_get_error(session));
            break;
        }

        if (lock_thread_exit)
            break;

        t = lock_timeout/2;
        if (t <= 0)
            t = 1;
        sleep(t);
    }
    
    if (dav_debug)
        fprintf(stderr, "lock_thread exiting\n");

    return NULL;
}

struct session_info *dav_init(int argc, char *argv[]) {
    int c;
    char *u = NULL, *p = NULL, *o = NULL;
    //int fuse_fd = -1;
    //int ret = 1;
    struct session_info *sess = NULL;
    //char mountpoint[PATH_MAX];
    /*pthread_t lock_thread;
    int lock_thread_running = 0;*/
    int enable_locking = 0;
    
    /*static char *mount_args_strings[] = {
        NULL,  // path
        NULL,  // -o
        NULL,
        NULL};
    
    struct fuse_args mount_args = {
        .argc = 1,
        .argv = mount_args_strings,
        .allocated = 0
    };*/
    
    if (ne_sock_init()) {
        fprintf(stderr, "Failed to initialize libneon.\n");
        goto finish;
    }

    if (!ne_has_support(NE_FEATURE_SSL) ||
        !ne_has_support(NE_FEATURE_TS_SSL) /*||
        !ne_has_support(NE_FEATURE_LFS)*/) {
        if (!ne_has_support(NE_FEATURE_SSL)) fprintf(stderr, "!ne_has_support(NE_FEATURE_SSL)\n");
        if (!ne_has_support(NE_FEATURE_TS_SSL)) fprintf(stderr, "!ne_has_support(NE_FEATURE_TS_SSL)\n");
        if (!ne_has_support(NE_FEATURE_LFS)) fprintf(stderr, "!ne_has_support(NE_FEATURE_LFS)\n");
        fprintf(stderr, "fusedav requires libneon built with SSL, SSL thread safety and LFS enabled.\n");
        goto finish;
    }
        
    mask = umask(0);
    umask(mask);

    sess = session_create();
    if (sess == NULL)
        goto finish;

    cache_alloc(sess);

    if (setup_signal_handlers() < 0)
        goto finish;

    optind = 1;
    while ((c = getopt(argc, argv, "hu:p:Do:Lt:n")) != -1) {

        switch(c) {
            case 'u':
                u = optarg;
                break;
                
            case 'p':
                p = optarg;
                break;
                
            case 'D':
                dav_debug = !dav_debug;
                break;

            case 'o':
                o = optarg;
                break;

            case 'L':
                enable_locking = 1;
                break;

            case 't':
                if ((lock_timeout = atoi(optarg)) < 0) {
                    fprintf(stderr, "Invalid lock timeout '%s'\n", optarg);
                    goto finish;
                }
                break;
                
            case 'n':
                no_cache = 1;
                break;

            case 'h':
                sess = NULL;

                /* fall through */
            default:
                usage(argv[0]);
                goto finish;
        }
    }

    if (optind != argc-2) {
        usage(argv[0]);
        goto finish;
    }

    if (dav_debug)
        ne_debug_init(stderr, 2);

    if (session_set_uri(sess, argv[optind], u, p) < 0) {
        sess = NULL;
        usage(argv[0]);
        goto finish;
    }

    /*if (argv[optind+1][0] == '/')
        snprintf(mountpoint, sizeof(mountpoint), "%s", argv[optind+1]);
    else {
        char *pwd = get_current_dir_name();
        snprintf(mountpoint, sizeof(mountpoint), "%s/%s", pwd, argv[optind+1]);
        free(pwd);
    }

    mount_args_strings[0] = argv[optind];

    if (o) {
        mount_args_strings[1] = (char*) "-o";
        mount_args_strings[2] = o;
        mount_args.argc += 2;
    }
    
    if ((fuse_fd = fuse_mount(mountpoint, &mount_args)) < 0) {
        fprintf(stderr, "Failed to mount FUSE file system.\n");
        goto finish;
    }

    if (!(fuse = fuse_new(fuse_fd, &mount_args, &dav_oper, sizeof(dav_oper)))) {
        fprintf(stderr, "Failed to create FUSE object.\n");
        goto finish;
    }*/
    
    if (enable_locking && create_lock(sess) >= 0) {
        int r;
        if ((r = pthread_create(&sess->lock_thread, NULL, lock_thread_func, sess)) < 0) {
            fprintf(stderr, "pthread_create(): %s\n", strerror(r));
            goto finish;
        }
        
        sess->lock_thread_running = 1;
    }
    
    /*fuse_loop_mt(fuse);

    if (dav_debug)
        fprintf(stderr, "Exiting cleanly.\n");
    
    ret = 0;*/

    return sess;

finish:
    sess = NULL;
    return sess;
}

void dav_destroy(struct session_info *sess) {
    if (sess->lock_thread_running) {
        lock_thread_exit = 1;
        pthread_kill(sess->lock_thread, SIGUSR1);
        pthread_join(sess->lock_thread, NULL);
        remove_lock(sess);
        ne_lockstore_destroy(lock_store);
    }

    /*if (fuse)
        fuse_destroy(fuse);
    
    if (fuse_fd >= 0)
        fuse_unmount(mountpoint);*/
    
    file_cache_close_all(sess);
    cache_free(sess);
    session_free(sess);
    //free(sess); // to avoid a double free! Why???
}

