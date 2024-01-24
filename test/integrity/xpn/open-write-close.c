
#include "all_system.h"
#include "xpn.h"

#define BUFF_SIZE (128)
char buffer[BUFF_SIZE] ;

int main ( int argc, char *argv[] )
{
	int  ret ;
	int  fd, fd1, num_read ;

	// Arguments
	if (argc < 2)
	{
	    printf("Usage: %s <number of write requests>\n", argv[0]) ;
	    return -1 ;
	}

	//printf("env XPN_CONF=./xpn.conf XPN_DNS=/tmp/tcp_server.dns %s\n", argv[0]);

	// xpn-init
	ret = xpn_init();
	printf("%d = xpn_init()\n", ret);
	if (ret < 0) {
	    return -1;
	}

	// xpn-creat
	fd1 = xpn_creat("/P1/test_1", 00777);
	printf("%d = xpn_creat('%s', %o)\n", ret, "/P1/test_1", 00777);
	if (fd1 < 0) {
	    return -1;
	}

	// xpn-write
	memset(buffer, 'a', BUFF_SIZE) ;
	printf("memset(buffer, 'a', %d)\n", BUFF_SIZE) ;

	for (int i = 0; i < atoi(argv[1]); i++)
	{
		ret = xpn_write(fd1, buffer, BUFF_SIZE);
		printf("%d = xpn_write_%d(%d, %p, %lu)\n", ret, i, fd1, buffer, (unsigned long)BUFF_SIZE);
	}

	ret = xpn_close(fd1);
	printf("%d = xpn_close(%d)\n", ret, fd1) ;

	// xpn-destroy
	ret = xpn_destroy();
	printf("%d = xpn_destroy()\n", ret);
	if (ret < 0) {
	    return -1;
	}

	return 0;
}
