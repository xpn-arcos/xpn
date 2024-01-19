
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


/* ... Include / Inclusion ........................................... */

#include "nfi_local.h"

/* ... Const / Const ................................................. */


/* ... Global variables / Variables globales ........................ */

extern int errno;


/* ... Functions / Funciones ......................................... */

// TODO: this interface must be changed
void local_err ( int err )
{
  debug_info("[LOCAL_ERR] [local_err] >> Begin\n");

  switch (err)
  {
    case LOCAL_ERR_PARAM:
      errno = -1;
      break;
    case LOCAL_ERR_MEMORY:
      errno = -1;
      break;
    case LOCAL_ERR_URL:
      errno = -1;
      break;
    case LOCAL_ERR_MNTCONNECTION:
      errno = -1;
      break;
    case LOCAL_ERR_MOUNT:
      errno = -1;
      break;
    case LOCAL_ERR_NFSCONNECTION:
      errno = -1;
      break;
    case LOCAL_ERR_GETATTR:
      errno = -1;
      break;
  }

  debug_info("[LOCAL_ERR] [local_err] >> End\n");
}


/* ................................................................... */
