#include <features.h>
#include <sys/types.h>
#include <sys/time.h>

#include "all_system.h"
#include "xpn.h"

#define MAX_PATH 512
#define MAX_HEADER 128

double get_time(void)

{
    struct timeval tp;
    struct timezone tzp;

    gettimeofday(&tp,&tzp);
    return((double) tp.tv_sec + .000001 * (double) tp.tv_usec);

}


int main ( int argc, char *argv[] )
{

    if (argc != 5) {
        fprintf(stderr, "Usage: %s <directory> <filename> <num_packets> <packet_size>\n", argv[0]);
        return 1;
    }

    const char *dir_name = argv[1];
    const char *file_name = argv[2];
    int num_packets = atoi(argv[3]);
    int packet_size = atoi(argv[4]);
    int ret = 0;

    char topic[MAX_PATH];
    char dir[MAX_PATH];
    char *filename = strrchr(argv[2], '/');
   
    ret = xpn_init();
    if (ret < 0)
    {
        return -1;
    }

    if (filename) 
    {
        filename++;
    } else 
    {
        filename = argv[2];
    }

    snprintf(dir, sizeof(dir), "/P1/%s", argv[1]);
    snprintf(topic, sizeof(topic), "/P1/%s/%s", argv[1], filename);

    xpn_mkdir(dir, 0777);

    int fd1 = xpn_creat(topic, 0777);
    if (fd1 < 0) 
    {
        perror ("ERROR CREATE");
        return -1;
    }


    char packet[packet_size];

    for (int i = 0; i < num_packets; i++) 
    {
        // Build header
        char header[MAX_HEADER];
        snprintf(header, sizeof(header), "%s-%d ", file_name, i);
        int header_len = strlen(header);

        // Fill packet buffer
        memset(packet, 'A', packet_size);           // Fill all with 'A'
        memcpy(packet, header, header_len);         // Overwrite start with header

        // Write to file
        ret = xpn_write(fd1, packet, packet_size);
        usleep(100000); // Pausa de 100 ms
        bzero(packet, packet_size);
    }

    ret = xpn_close(fd1);

    if (ret < 0) 
    {
        xpn_destroy();
        printf("ERROR: xpn_close()\n");
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
