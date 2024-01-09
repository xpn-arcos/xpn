
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


#include "xpn/xpn_simple/xpn_cwd.h"

struct xpn_cwd xpn_cwddir;

void xpn_init_cwd()
{
  xpn_cwddir.xpn_cwd_path[0] ='\0';
}

char* xpn_simple_getcwd(char *path, size_t size)
{
  strncpy(path, xpn_cwddir.xpn_cwd_path, size);
  return xpn_cwddir.xpn_cwd_path;
}

int xpn_simple_chdir(char *path)
{
  /* comprobar que la particion existe */
  /* comprobar . y .. */
  strcpy(xpn_cwddir.xpn_cwd_path ,path);
  return 0;
}

