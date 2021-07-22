#ifndef foosessionhfoo
#define foosessionhfoo

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

#include <ne_session.h>
#include <ne_locks.h>

struct session_info {
    pthread_t lock_thread;
    int lock_thread_running;// = 0;

    pthread_once_t session_once;// = PTHREAD_ONCE_INIT;
    pthread_key_t session_tsd_key;

    ne_uri uri;
    int b_uri;// = 0;

    char *username;// = NULL;
    char *password;// = NULL;
    char *base_directory;// = NULL;

    pthread_mutex_t credential_mutex;// = PTHREAD_MUTEX_INITIALIZER;

    struct file_info *files;// = NULL;
    pthread_mutex_t files_mutex;// = PTHREAD_MUTEX_INITIALIZER;

    struct cache_entry *cache;// = NULL;
    pthread_mutex_t stat_cache_mutex;// = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t dir_cache_mutex;// = PTHREAD_MUTEX_INITIALIZER;
};

#include "statcache.h"

ne_session *session_get(struct session_info *sess, int with_lock);
int session_set_uri(struct session_info *sess, const char *s, const char *u, const char *p);
struct session_info *session_create(void);
void session_free(struct session_info *sess);

int session_is_local(struct session_info *sess, const ne_uri *u);

/*extern char *base_directory;
extern ne_uri uri;
extern char *username;*/

#endif
