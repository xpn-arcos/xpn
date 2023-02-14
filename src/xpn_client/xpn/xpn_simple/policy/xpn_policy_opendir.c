
#include "xpn.h"
#include "xpn/xpn_simple/xpn_opendir.h"
#include "xpn/xpn_simple/xpn_policy_open.h"
#include "xpn/xpn_simple/xpn_policy_opendir.h"


int XpnGetEntry(int fd, char *entry, unsigned char *type)
{
	int n,res;
	struct nfi_server **servers;

	XPN_DEBUG_BEGIN

	/* params:
         * flag operation, partition id, absolute path, file descript., pointer to server
	 */
	servers = NULL;
	n = XpnGetServers(op_xpn_readdir,
		   	  xpn_file_table[fd]->part->id,
			  NULL,
			  fd,
			  &servers,
			  XPN_DATA_SERVER);
	if(n<=0){
	    return -1;
	}

	res = XpnGetFh(xpn_file_table[fd]->mdata,
			&(xpn_file_table[fd]->data_vfh->nfih[0]),
			servers[0],
			xpn_file_table[fd]->path);

	free(servers);

	//printf("xpn_file_table[fd]->vfh->nfih[0]->server %p\n",xpn_file_table[fd]->vfh->nfih[0]->server);
	res = xpn_file_table[fd]->data_vfh->nfih[0]->server->ops->nfi_readdir(xpn_file_table[fd]->data_vfh->nfih[0]->server,
										xpn_file_table[fd]->data_vfh->nfih[0],
										entry,
										type);

	XPN_DEBUG_END

	return res;
}

