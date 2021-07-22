#ifndef foostatcachehfoo
#define foostatcachehfoo

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

#include <sys/stat.h>

#include "session.h"

int stat_cache_get(struct session_info *sess, const char *fn, struct stat *st);
void stat_cache_set(struct session_info *sess, const char *fn, const struct stat *st);
void stat_cache_invalidate(struct session_info *sess, const char*fn);

void dir_cache_invalidate(struct session_info *sess, const char*fn);
void dir_cache_invalidate_parent(struct session_info *sess, const char *fn);
void dir_cache_begin(struct session_info *sess, const char *fn);
void dir_cache_finish(struct session_info *sess, const char *fn, int success);
void dir_cache_add(struct session_info *sess, const char *fn, const char *subdir);
int dir_cache_enumerate(struct session_info *sess, const char *fn, void (*f) (const char*fn, const char *subdir, void *user), void *user);

void cache_free(struct session_info *sess);
void cache_alloc(struct session_info *sess);

#endif
