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

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include <ne_uri.h>
#include <ne_request.h>
#include <ne_basic.h>
#include <ne_props.h>
#include <ne_utils.h>
#include <ne_socket.h>
#include <ne_auth.h>
#include <ne_dates.h>
#include <ne_redirect.h>

#include "session.h"
#include "fusedav.h"

static pthread_key_t *session_tsd_key = NULL;
static pthread_mutex_t session_tsd_key_mutex = PTHREAD_MUTEX_INITIALIZER;
/*
static pthread_once_t session_once = PTHREAD_ONCE_INIT;
static pthread_key_t session_tsd_key;

ne_uri uri;
static int b_uri = 0;

char *username = NULL;
static char *password = NULL;
char *base_directory = NULL;

static pthread_mutex_t credential_mutex = PTHREAD_MUTEX_INITIALIZER;
*/
static char* ask_user(const char *p, int hidden) {
    char q[256], *r;
    struct termios t;
    int c = 0, l;

    if (hidden) {
        if (!isatty(fileno(stdin)))
            hidden = 0;
        else {
            if (tcgetattr(fileno(stdin),  &t) < 0)
                hidden = 0;
            else {
                c = t.c_lflag;
                t.c_lflag &= ~ECHO;
                if (tcsetattr(fileno(stdin), TCSANOW, &t) < 0)
                    hidden = 0;
            }
        }
    }
    
    fprintf(stderr, "%s: ", p);
    r = fgets(q, sizeof(q), stdin);
    l = strlen(q);
    if (l && q[l-1] == '\n')
        q[l-1] = 0;

    if (hidden) {
        t.c_lflag = c;
        tcsetattr(fileno(stdin), TCSANOW, &t);
        fprintf(stderr, "\n");
    }
    
    return r ? strdup(r) : NULL;
}

static int ssl_verify_cb(__unused void *userdata, __unused int failures, __unused const ne_ssl_certificate *cert) {
    return 0;
}

static int ne_auth_creds_cb(void *userdata, const char *realm, int attempt, char *u, char *p) {
    int r = -1;
    struct session_info *sess = (struct session_info *)userdata;
    pthread_mutex_t *credential_mutex = &sess->credential_mutex;
    char *username = sess->username;
    char *password = sess->password;
    
    
    
    pthread_mutex_lock(credential_mutex);

    if (attempt) {
        fprintf(stderr, "Authentication failure!\n");
        free((void*) username);
        free((void*) password);
        username = password = NULL;
    }

    if (!username || !password)
        fprintf(stderr, "Realm '%s' requires authentication.\n", realm);
    
    if (!username)
        username = ask_user("Username", 0);
    
    if (username && !password)
        password = ask_user("Password", 1);

    if (username && password) {
        snprintf(u, NE_ABUFSIZ, "%s", username);
        snprintf(p, NE_ABUFSIZ, "%s", password);
        r  = 0;
    }

    pthread_mutex_unlock(credential_mutex);
    return r;
}

static ne_session *session_open(struct session_info *sess, int with_lock) {
    const char *scheme = NULL;
    ne_session *session;
    ne_uri *uri = &sess->uri;
    int b_uri = sess->b_uri;

    extern ne_lock_store *lock_store;

    if (!b_uri)
        return NULL;

    scheme = uri->scheme ? uri->scheme : "http";
    
    if (!(session = ne_session_create(scheme, uri->host, uri->port ? uri->port : ne_uri_defaultport(scheme)))) {
        fprintf(stderr, "Failed to create session\n");
        return NULL;
    }

    ne_ssl_set_verify(session, ssl_verify_cb, NULL);
    ne_set_server_auth(session, ne_auth_creds_cb, sess);
    ne_redirect_register(session);

    if (with_lock && lock_store)
        ne_lockstore_register(lock_store, session);
    
    return session;
}

static void session_destroy(void *s) {
    ne_session *session = s;
    assert(s);
    ne_session_destroy(session);
}

static void session_tsd_key_init(void) {
    pthread_key_create(session_tsd_key, session_destroy);
}

ne_session *session_get(struct session_info *sess, int with_lock) {
    ne_session *session;
    
    pthread_mutex_lock(&session_tsd_key_mutex);
    session_tsd_key = &sess->session_tsd_key;
    pthread_once(&sess->session_once, session_tsd_key_init);
    session_tsd_key = NULL;
    pthread_mutex_unlock(&session_tsd_key_mutex);

    if ((session = pthread_getspecific(sess->session_tsd_key)))
        return session;

    session = session_open(sess, with_lock);
    pthread_setspecific(sess->session_tsd_key, session);

    return session;
}

int session_set_uri(struct session_info *sess, const char *s, const char *u, const char *p) {
    int l;
        
    assert(!sess->b_uri);
    assert(!sess->username);
    assert(!sess->password);

    if (ne_uri_parse(s, &sess->uri)) {
        fprintf(stderr, "Invalid URI <%s>\n", s);
        goto finish;
    }

    sess->b_uri = 1;

    if (!sess->uri.host) {
        fprintf(stderr, "Missing host part in URI <%s>\n", s);
        goto finish;
    }

    sess->base_directory = strdup(sess->uri.path);
    l = strlen(sess->base_directory);
    if (sess->base_directory[l-1] == '/')
        ((char*) sess->base_directory)[l-1] = 0;

    if (u)
        sess->username = strdup(u);

    if (p)
        sess->password = strdup(p);

    return 0;
    
finish:
    
    if (sess->b_uri) {
        ne_uri_free(&sess->uri);
        sess->b_uri = 0;
    }

    return -1;
}

struct session_info *session_create(void) {
    struct session_info *sess = NULL;
    pthread_once_t tmp_once = PTHREAD_ONCE_INIT;
    pthread_mutex_t tmp_mutex = PTHREAD_MUTEX_INITIALIZER;

    sess = malloc (sizeof(struct session_info));
    memset (sess, 0, sizeof(struct session_info));

    memcpy(&sess->session_once, &tmp_once, sizeof(pthread_once_t));
    memcpy(&sess->credential_mutex, &tmp_mutex, sizeof(pthread_mutex_t));

    memcpy(&sess->files_mutex, &tmp_mutex, sizeof(pthread_mutex_t));

    memcpy(&sess->stat_cache_mutex, &tmp_mutex, sizeof(pthread_mutex_t));
    memcpy(&sess->dir_cache_mutex, &tmp_mutex, sizeof(pthread_mutex_t));

    return sess;
}

void session_free(struct session_info *sess) {
    if (sess->b_uri) {
        ne_uri_free(&sess->uri);
        sess->b_uri = 0;
    }

    free((char*) sess->username);
    free((char*) sess->password);
    free((char*) sess->base_directory);

    sess->username = sess->password = sess->base_directory = NULL;

    free(sess);
}

int session_is_local(struct session_info *sess, const ne_uri *u) {
    assert(u);
    assert(sess->b_uri);

    return
        strcmp(u->scheme, sess->uri.scheme) == 0 &&
        strcmp(u->host, sess->uri.host) == 0 &&
        u->port == sess->uri.port;
}

