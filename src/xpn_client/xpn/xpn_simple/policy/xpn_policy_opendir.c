
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


#include "xpn.h"
#include "xpn/xpn_simple/xpn_opendir.h"
#include "xpn/xpn_simple/xpn_policy_open.h"
#include "xpn/xpn_simple/xpn_policy_opendir.h"


int XpnGetEntry(int fd, struct dirent *entry)
{
	int n,res;
	struct nfi_server **servers;

	XPN_DEBUG_BEGIN

	/* XpnGetServers: flag operation, partition id, absolute path, file descript., pointer to server */
	servers = NULL;
	n = XpnGetServers(op_xpn_readdir, xpn_file_table[fd]->part->id, NULL, fd, &servers, XPN_DATA_SERVER);
	if(n<=0){
	    return -1;
	}

	res = XpnGetFh(xpn_file_table[fd]->mdata, &(xpn_file_table[fd]->data_vfh->nfih[0]), servers[0], xpn_file_table[fd]->path);

	free(servers);

     // printf("xpn_file_table[fd]->vfh->nfih[0]->server %p\n",xpn_file_table[fd]->vfh->nfih[0]->server);
	res = xpn_file_table[fd]->data_vfh->nfih[0]->server->ops->nfi_readdir(xpn_file_table[fd]->data_vfh->nfih[0]->server, xpn_file_table[fd]->data_vfh->nfih[0], entry);

	XPN_DEBUG_END

	return res;
}

