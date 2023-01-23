
#include "all_system.h"
#include "xpn.h"

#define BUFF_SIZE (1024*1024)

int main ( int argc, char *argv[] )
{
	int  ret ;
	int  fd1 ;
	char buffer[BUFF_SIZE] ;

	printf("setenv XPN_CONF ./\n\n");
	setenv("XPN_CONF", "./prueba.conf", 1);

        // xpn-init
	printf("ANTES XPN_INIT\n\n");
	ret = xpn_init();
	printf("%d = xpn_init()\n\n", ret);
	if (ret < 0) {
	    return -1;
	}

	// test 1
	printf("ANTES XPN_OPEN\n\n");

	int fd = open("quijote.txt", O_RDONLY);
  if (fd == -1) return -1;


	fd1 = xpn_open("/xpn/copia_quijote.txt", O_CREAT|O_WRONLY, 00777);
	printf("%d = xpn_open('%s', O_CREAT|O_WRONLY, %o)\n\n", fd1, "/XPN/test_1", 00777);

	//memset(buffer, 'b', BUFF_SIZE) ;
	//printf("memset(buffer, 'b', %d)\n\n", BUFF_SIZE) ;

	printf("LEO EL QUIJOTE\n");
	int sz = read(fd, buffer, BUFF_SIZE);
	printf("ANTES XPN_WRITE\n\n");
	ret = xpn_write(fd1, buffer, BUFF_SIZE);
	printf("%d = xpn_write(%d, %p, %lu)\n\n", ret, fd1, buffer, (unsigned long)BUFF_SIZE);

	printf("ANTES XPN_CLOSE\n\n");
	ret = xpn_close(fd1);
	printf("%d = xpn_close(%d)\n\n", ret, fd1) ;

	ret = close(fd);

        // xpn-destroy
	printf("xpn_destroy()\n\n");
	ret = xpn_destroy();
	if (ret < 0) {
	    printf("ERROR: xpn_destroy()\n");
	    return -1;
	}

	return 0;
}
