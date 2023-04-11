
#include "all_system.h"
#include "xpn.h"

#define BUFF_SIZE (128)

int main ( int argc, char *argv[] )
{
	int  ret ;
	int  fd, fd1, num_read ;
	char buffer[BUFF_SIZE] ;

	//printf("env XPN_CONF=./xpn.conf XPN_DNS=/tmp/tcp_server.dns %s\n", argv[0]);

	// xpn-init
	ret = xpn_init();
	printf("%d = xpn_init()\n", ret);
	if (ret < 0) {
		return -1;
	}

	// test 1

	//fd = open ("./quijote.txt", O_RDONLY);

	fd1 = xpn_creat("/P1/test_1", 00777);
	printf("%d = xpn_creat('%s', %o)\n", ret, "/P1/test_1", 00777);

	memset(buffer, 'a', BUFF_SIZE) ;
	printf("memset(buffer, 'a', %d)\n", BUFF_SIZE) ;

	
	/*while ((num_read = read(fd, buffer, BUFF_SIZE)) > 0) {
        if (xpn_write(fd1, buffer, num_read)) {
            perror("Error al escribir en el archivo de destino");
            exit(EXIT_FAILURE);
        }
    }

    close(fd);*/

	for (int i = 0; i < atoi(argv[1]); i++)
	{
		ret = xpn_write(fd1, buffer, BUFF_SIZE);
		printf("%d = xpn_write_%d(%d, %p, %lu)\n", ret, i, fd1, buffer, (unsigned long)BUFF_SIZE);
	}
	//sleep(1);*/
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
