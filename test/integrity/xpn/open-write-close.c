
#include "all_system.h"
#include "xpn.h"

#define BUFF_SIZE (1024*1024)

int main ( int argc, char *argv[] )
{
	int  ret ;
	int  fd1 ;
	char buffer[BUFF_SIZE] ;

	printf("env XPN_CONF=./xpn.conf\n");
	setenv("XPN_CONF",  "./xpn.conf", 1);
	setenv("XPN_DNS",  "/tmp/tcp_server.dns", 1);

        // xpn-init
	ret = xpn_init();
	printf("%d = xpn_init()\n", ret);
	if (ret < 0) {
	    return -1;
	}

	// test 1
	fd1 = xpn_creat("/P1/test_1", 00777);
	printf("%d = xpn_creat('%s', %o)\n", ret, "/P1/test_1", 00777);

	memset(buffer, 'a', BUFF_SIZE) ;
	printf("memset(buffer, 'a', %d)\n", BUFF_SIZE) ;

	ret = xpn_write(fd1, buffer, BUFF_SIZE);
	printf("%d = xpn_write(%d, %p, %lu)\n", ret, fd1, buffer, (unsigned long)BUFF_SIZE);

	ret = xpn_close(fd1);
	printf("%d = xpn_close(%d)\n", ret, fd1) ;

        // xpn-destroy
	printf("xpn_destroy()\n");
	ret = xpn_destroy();
	if (ret < 0) {
	    printf("ERROR: xpn_destroy()\n");
	    return -1;
	}

	return 0;
}
