
#include <features.h>
#include <sys/types.h>
#include <sys/time.h>

#include "all_system.h"
#include "xpn.h"

#define MAX_LINE_LENGTH (512)

double get_time(void)

{
    struct timeval tp;
    struct timezone tzp;

    gettimeofday(&tp,&tzp);
    return((double) tp.tv_sec + .000001 * (double) tp.tv_usec);

}


int main ( int argc, char *argv[] )
{

	if (argc != 3) 
	{
        fprintf(stderr, "Uso: %s <project> <circ_name>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[2], O_RDONLY);
    if (fd == -1) 
    {
        perror("Error al abrir el fichero");
        return 1;
    }

	int  ret ;
	int  fd1 ;

	ret = xpn_init();
	if (ret < 0) 
	{
		return -1;
	}

    char topic[256];
    char dir[256];
    char *filename = strrchr(argv[2], '/');
    
    if (filename) 
    {
        filename++; // Avanza el puntero para saltar el '/'
    } else 
    {
        filename = argv[2]; // No hay '/' en el nombre del fichero
    }

    snprintf(dir, sizeof(dir), "/P1/%s", argv[1]);
    snprintf(topic, sizeof(topic), "/P1/%s/%s", argv[1], filename);

    xpn_mkdir(dir, 0777);

    fd1 = xpn_creat(topic, 0777);
    if (fd1 < 0) 
    {
        perror ("ERROR CREATE");
        return -1;
    }

    char buffer[MAX_LINE_LENGTH];
    bzero(buffer, MAX_LINE_LENGTH);
    ssize_t bytes_read;

    //ret = xpn_write(fd1, "a", strlen("a"));

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) 
    {
        ret = xpn_write(fd1, buffer, bytes_read);
        usleep(100000); // Pausa de 100 ms
        bzero(buffer, MAX_LINE_LENGTH);
    }

	ret = xpn_close(fd1);

    if (ret < 0) 
    {
        xpn_destroy();
        printf("ERROR: xpn_close()\n");
        return -1;
    }

    ret = close(fd);

    if (ret < 0) 
    {
        xpn_destroy();
        printf("ERROR: close()\n");
        return -1;
    }

	ret = xpn_destroy();
	if (ret < 0) 
	{
		printf("ERROR: xpn_destroy()\n");
		return -1;
	}

	return 0;
}
