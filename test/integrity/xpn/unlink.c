
#include "all_system.h"
#include "xpn.h"

#define BUFF_SIZE (1024*1024)

int main ( int argc, char *argv[] )
{
	int  ret ;
	int  fd1 ;
	//char buffer[BUFF_SIZE] ;

	printf("env XPN_CONF=./xpn.conf XPN_DNS=/tmp/tcp_server.dns %s\n", argv[0]);

	// xpn-init
	ret = xpn_init();
	printf("%d = xpn_init()\n", ret);
	if (ret < 0) {
		return -1;
	}

	ret = xpn_unlink("/P1/test_1");
	printf("%d = xpn_unlink(%s)\n", ret, "/P1/test_1") ;

		// xpn-destroy
	printf("xpn_destroy()\n");
	ret = xpn_destroy();
	if (ret < 0) {
		printf("ERROR: xpn_destroy()\n");
		return -1;
	}

	return 0;
}
