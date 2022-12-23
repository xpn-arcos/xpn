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

