
#include "xpn/xpn_simple/xpn_cwd.h"

struct xpn_cwd xpn_cwddir;

void xpn_init_cwd()
{
	xpn_cwddir.xpn_cwd_path[0] ='\0';
}


char* xpn_getcwd(char *path, size_t size)
{
	strncpy(path, xpn_cwddir.xpn_cwd_path, size);
	return xpn_cwddir.xpn_cwd_path;
}

int xpn_chdir(char *path)
{
	/* comprobar que la particion existe */
	/* comprobar . y .. */
	strcpy(xpn_cwddir.xpn_cwd_path ,path);
	return 0;
}

