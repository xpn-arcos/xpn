
#include "all_system.h"
#include "xpn.h"

#define BUFF_SIZE (1024*1024)

int main ( int argc, char *argv[] )
{
	int  ret ;
	int  fd1 ;
	char buffer[BUFF_SIZE] ;

	printf("env XPN_CONF=./xpn.conf XPN_DNS=/tmp/xpn/conf/xpn.dns ./%s\n", argv[0]);

        // xpn-init
	ret = xpn_init();
	printf("%d = xpn_init()\n", ret);
	if (ret < 0) {
	    return -1;
	}

	// test 2
	fd1 = xpn_open("/P1/test_1", O_RDWR);
	printf("%d = xpn_open('%s', %x)\n", ret, "/P1/test_1", O_RDWR);

	ret = xpn_lseek(fd1, 0, SEEK_SET);
	printf("%d = xpn_lseek(%d, %d, %d)\n", ret, fd1, 0, SEEK_SET);

	memset(buffer, 'x', BUFF_SIZE) ;
	printf("memset(buffer, 'x', %d)\n", BUFF_SIZE) ;

	ret = xpn_read(fd1, buffer, BUFF_SIZE);
	printf("%d = xpn_read(%d, %p, %lu)\n", ret, fd1, buffer, (unsigned long)BUFF_SIZE);

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
