
#include "all_system.h"
#include "xpn.h"
#include <sys/time.h>

#define BUFF_SIZE (MB)
char bufferWrite[BUFF_SIZE] ;
char bufferRead[BUFF_SIZE] ;


double get_time(void)
{
    struct timeval tp;
    struct timezone tzp;

    gettimeofday(&tp,&tzp);
    return((double) tp.tv_sec + .000001 * (double) tp.tv_usec);
}


int main ( int argc, char *argv[] )
{
	int    ret, fd1 ;
	double t_bc, t_ac, t_bw, t_aw ;

        if (argc < 3)
	{
	    printf("\n") ;
	    printf(" Usage: %s <full path> <megabytes to write-read>\n", argv[0]) ;
	    printf("\n") ;
	    printf(" Example:") ;
	    printf(" env XPN_CONF=./xpn.conf XPN_DNS=/shared/tcp_server.dns %s /P1/test_1 2\n", argv[0]);
	    printf("\n") ;
	    return -1 ;
	}	

	// xpn-init
	ret = xpn_init();
	printf("%d = xpn_init()\n", ret);
	if (ret < 0) {
	    return -1;
	}

    // fill buffer with random
    int fd = open("/dev/random", O_RDONLY);
    read(fd, bufferWrite, BUFF_SIZE);
    close(fd);
	printf("buffer fill with random data, %d\n", bufferWrite, BUFF_SIZE) ;

	// xpn-creat
	t_bc = get_time();

	fd1 = xpn_creat(argv[1], 00777);
	if (fd1 < 0) {
	    printf("%d = xpn_creat('%s', %o)\n", ret, argv[1], 00777) ;
	    return -1 ;
	}

	t_bw = get_time();

	// xpn-write
    long mb_file = atoi(argv[2]) ;
	for (int i = 0; i < mb_file; i++)
	{
	    ret = xpn_write(fd1, bufferWrite, BUFF_SIZE);
	}
	
	t_aw = get_time() - t_bw;

	ret = xpn_close(fd1);

	t_ac = get_time() - t_bc;

	printf("Bytes (KiB); Total time (ms); Read time (ms)\n") ;
	printf("%f;%f;%f\n", ((double)mb_file * (double)BUFF_SIZE) / ((double)KB), t_ac * 1000, t_aw * 1000) ;

    // xpn-creat
	t_bc = get_time();

	fd1 = xpn_open(argv[1], O_RDWR);
	if (fd1 < 0) {
	    printf("%d = xpn_open('%s', %o)\n", ret, argv[1], 00777) ;
	    return -1 ;
	}

	t_bw = get_time();

	// xpn-write
	for (int i = 0; i < mb_file; i++)
	{
	    ret = xpn_read(fd1, bufferRead, BUFF_SIZE);
	}
	
	t_aw = get_time() - t_bw;

	ret = xpn_close(fd1);

	t_ac = get_time() - t_bc;

	printf("Bytes; Total time (ms); Read time (ms)\n") ;
	printf("%f;%f;%f\n", (double)mb_file * BUFF_SIZE, t_ac * 1000, t_aw * 1000) ;

    // Compare the buffers using strcmp
    if (memcmp(bufferWrite, bufferRead, BUFF_SIZE) == 0) {
        printf("The buffers are equal.\n");
    } else {
        printf("The buffers are different %d.\n", strcmp(bufferWrite, bufferRead));
    }

	// xpn-destroy
	ret = xpn_destroy();
	printf("%d = xpn_destroy()\n", ret);
	if (ret < 0) {
	    return -1;
	}

	return 0;
}

