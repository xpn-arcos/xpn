
  /*
   *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Dario Muñoz Muñoz
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


#include "xpn.h"
#include "xpn/xpn_simple/xpn_opendir.h"
#include "xpn/xpn_simple/xpn_policy_open.h"
#include "xpn/xpn_simple/xpn_policy_opendir.h"


int XpnGetEntry(int fd, struct dirent *entry)
{
	int n,res,serv_num;
	struct nfi_server *servers;

	XPN_DEBUG_BEGIN

	servers = NULL;
	n = XpnGetServers(xpn_file_table[fd]->part->id, fd, &servers);
	if(n<=0){
	    return -1;
	}
  serv_num = 0;
  while(serv_num < n && servers[serv_num].error == -1)
  {
    serv_num++;
  }

	res = XpnGetFhDir(xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[serv_num]), &servers[serv_num], xpn_file_table[fd]->path);

	res = xpn_file_table[fd]->data_vfh->nfih[serv_num]->server->ops->nfi_readdir(xpn_file_table[fd]->data_vfh->nfih[serv_num]->server, xpn_file_table[fd]->data_vfh->nfih[serv_num], entry);

	XPN_DEBUG_END

	return res;
}

