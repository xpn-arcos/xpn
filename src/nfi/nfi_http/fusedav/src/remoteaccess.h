#ifndef fooremoteaccesshfoo
#define fooremoteaceesshfoo

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

#include <sys/types.h>

#include <ne_session.h>

int remote_access_read(struct session_info *sess, void *f, char *buf, size_t size, off_t offset);
int remote_access_write(struct session_info *sess, void *f, const char *buf, size_t size, off_t offset);

#endif
