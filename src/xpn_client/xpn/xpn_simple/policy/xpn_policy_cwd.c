
  /*
   *  Copyright 2000-2023 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


#include "xpn/xpn_simple/xpn_policy_cwd.h"

extern struct xpn_cwd xpn_cwddir;

int XpnGetAbsolutePath(const char *path, char *path_part)
{
	path_part[0] ='\0';
	if(path[0] != '/'){
		sprintf(path_part,"%s%s%s", "/", xpn_cwddir.xpn_cwd_path, path);
	}else{
		//sprintf(path_part,"%s",path);
		strcpy(path_part, path);
	}
	path_part[strlen(path)+1] = '\0';
	return 0;
}

