
#include "all_system.h"
#include "xpn.h"

#define BUFF_SIZE (1024*1024)

int main ( int argc, char *argv[] )
{
	int  ret ;
	//char buffer[BUFF_SIZE] ;

	printf("env XPN_CONF=./xpn.conf %s\n", argv[0]);

	// xpn_init
	ret = xpn_init();
	printf("%d = xpn_init()\n", ret);
	if (ret < 0) {
	    printf("ERROR: xpn_init()\n");
	    return -1;
	}

	ret = xpn_unlink("/P1/test_1");
	printf("%d = xpn_unlink(%s)\n", ret, "/P1/test_1") ;
	if (ret < 0) {
	    printf("ERROR: xpn_unlink()\n");
	    return -1;
	}

	// xpn_destroy
	ret = xpn_destroy();
	printf("%d = xpn_destroy()\n", ret);
	if (ret < 0) {
	    printf("ERROR: xpn_destroy()\n");
	    return -1;
	}

	return 0;
}

