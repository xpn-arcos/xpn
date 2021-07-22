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

#include <errno.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <inttypes.h>
#include <limits.h>

#include <ne_props.h>
#include <ne_uri.h>
#include <ne_session.h>
#include <ne_utils.h>
#include <ne_socket.h>
#include <ne_auth.h>
#include <ne_dates.h>
#include <ne_basic.h>

#include "filecache.h"
#include "statcache.h"
#include "remoteaccess.h"
#include "fusedav.h"
#include "session.h"

struct file_info {
    char *filename;
    int fd;
    off_t server_length, length, present;
    
    int readable;
    int writable;

    int modified;

    int ref, dead;

    pthread_mutex_t mutex;

    /* This field is locked by files_mutex, not by file_info->mutex */
    struct file_info *next;
};

extern int no_cache;

int remote_access_read(struct session_info *sess, void *f, char *buf, size_t size, off_t offset) {
    struct file_info *fi = f;
    ssize_t r = -1;
    off_t l;
    ne_content_range range;
    ne_session *session;
    ne_generic_data data;
    
    if (dav_debug)
        fprintf(stderr, "Called remote_access_read (size=%u, offset=%ld)\n", (unsigned int)size, (long int)offset);
    assert(fi && buf && size);

    pthread_mutex_lock(&fi->mutex);

    if (!(session = session_get(sess, 1))) {
        errno = EIO;
        goto finish;
    }

    l = offset+size;
    if (l > fi->server_length)
        l = fi->server_length;
    
    range.start = offset;
    range.end = l-1;
    range.total = 0;
    
    data.type = NE_GENERIC_DATA_TYPE_BUFFER;
    data.generic_data.buf.buffer = buf;

    if (ne_get_range_generic(session, fi->filename, &range, &data) != NE_OK) {
        fprintf(stderr, "GET failed: %s\n", ne_get_error(session));
        errno = ENOENT;
        goto finish;
    }

    r = data.generic_data.buf.length;

finish:
    
    pthread_mutex_unlock(&fi->mutex);

    return r;
}

int remote_access_write(struct session_info *sess, void *f, const char *buf, size_t size, off_t offset) {
    struct file_info *fi = f;
    ssize_t r = -1;
    off_t l;
    ne_content_range range;
    ne_session *session;
    ne_generic_data data;

    if (dav_debug)
        fprintf(stderr, "Called remote_access_write (size=%u, offset=%ld)\n", (unsigned int)size, (long int)offset);
    assert (fi);

    pthread_mutex_lock(&fi->mutex);

    if (!(session = session_get(sess, 1))) {
        errno = EIO;
        goto finish;
    }

    l = offset+size;

    range.start = offset;
    range.end = l-1;
    range.total = size;

    data.type = NE_GENERIC_DATA_TYPE_BUFFER;
    data.generic_data.buf.buffer = (char *) buf;
    data.generic_data.buf.length = size;

    if (ne_put_range_generic(session, fi->filename, &range, &data) != NE_OK) {
        fprintf(stderr, "remote_access_write: PUT failed: %s\n", ne_get_error(session));
        errno = ENOENT;
        goto finish;
    }

    r = size;

    if (offset+(long)size > fi->present)
        fi->present = offset+size;

    if (offset+(long)size > fi->length)
        fi->length = offset+size;

    fi->modified = 1;

finish:
    pthread_mutex_unlock(&fi->mutex);

    return r;
}

//TODO
/*int remote_access_truncate(void *f, off_t s) {
    int r = -1;
    return r;
}*/
