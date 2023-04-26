
#include "all_system.h"
#include "xpn.h"
#include <sys/time.h>

#define BUFF_SIZE (128)

double get_time(void)

{
    struct timeval tp;
    struct timezone tzp;

    gettimeofday(&tp,&tzp);
    return((double) tp.tv_sec + .000001 * (double) tp.tv_usec);

}


int main ( int argc, char *argv[] )
{
	int  ret ;
	int  fd, fd1, num_read ;
	char buffer[BUFF_SIZE] ;
	double t_bc, t_ac, t_bw, t_aw ;


	//printf("env XPN_CONF=./xpn.conf XPN_DNS=/tmp/tcp_server.dns %s\n", argv[0]);

	// xpn-init
	ret = xpn_init();
	printf("%d = xpn_init()\n", ret);
	if (ret < 0) {
		return -1;
	}

	// test 1

	//fd = open ("./hola.txt", O_RDONLY);

	t_bc = get_time();

	//fd1 = xpn_creat("/P1/test_1", 00777);
	fd1 = xpn_creat(argv[2], 00777);
	printf("%d = xpn_creat('%s', %o)\n", ret, argv[2], 00777);

	memset(buffer, 'a', BUFF_SIZE) ;
	printf("memset(buffer, 'a', %d)\n", BUFF_SIZE) ;

	
	/*while ((num_read = read(fd, buffer, BUFF_SIZE)) > 0) {
		printf("NUM_READ -- %d\n", num_read);
        if ((ret = xpn_write(fd1, buffer, num_read)) < 0) {
        	printf ("RET ---------- %d\n", ret);
            perror("Error al escribir en el archivo de destino");
            exit(EXIT_FAILURE);
        }
    }

    close(fd);*/

	t_bw = get_time();

	for (int i = 0; i < atoi(argv[1]); i++)
	{
		ret = xpn_write(fd1, buffer, BUFF_SIZE);
		//printf("%d = xpn_write_%d(%d, %p, %lu)\n", ret, i, fd1, buffer, (unsigned long)BUFF_SIZE);
	}
	
	t_aw = get_time() - t_bw;

	ret = xpn_close(fd1);

	t_ac = get_time() - t_bc;

	printf("%d = xpn_close(%d)\n", ret, fd1) ;

	printf("Prueba: %d (128B), Tiempo Total: %fms; Tiempo Escritura: %f ms\n", atoi(argv[1]), t_ac * 1000, t_aw * 1000);

		// xpn-destroy
	printf("xpn_destroy()\n");
	ret = xpn_destroy();
	if (ret < 0) {
		printf("ERROR: xpn_destroy()\n");
		return -1;
	}

	return 0;
}
