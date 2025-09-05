
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Elías Del Pozo Puñal
 *
 *  This file is part of Expand.
 *
 *  Expand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Expand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "all_system.h"
#include "xpn.h"
#include <sys/time.h>


#define BUFF_SIZE (1024*1024)
char buffer[BUFF_SIZE];

double get_time(void)
{
    struct timeval tp;
    struct timezone tzp;

    gettimeofday( & tp, & tzp);
    return ((double) tp.tv_sec + .000001 * (double) tp.tv_usec);
}

int main(int argc, char * argv[])
{
    int ret = 0, fd1;
    double t_bc, t_ac, t_bw, t_aw;
    long mb ;

    if (argc < 3) {
        printf("\n");
        printf(" Usage: %s <full path> <megabytes to write>\n", argv[0]);
        printf("\n");
        printf(" Example:");
        printf(" env XPN_CONF=./xpn.conf  %s /P1/test_1 2\n", argv[0]);
        printf("\n");
        return -1;
    }

    // xpn-init
    ret = xpn_init();
    printf("%d = xpn_init()\n", ret);
    if (ret < 0) {
        return -1;
    }

    memset(buffer, 'a', BUFF_SIZE);
    printf("memset(buffer, 'a', %d)\n", BUFF_SIZE);

    // xpn-creat
    t_bc = get_time();

    fd1 = xpn_creat(argv[1], 00777);
    if (fd1 < 0) {
        printf("%d = xpn_creat('%s', %o)\n", ret, argv[1], 00777);
        return -1;
    }

    t_bw = get_time();

    // xpn-write
    mb = atoi(argv[2]);
    for (int i = 0; i < mb; i++) {
        ret = xpn_write(fd1, buffer, BUFF_SIZE);
        printf("%d = xpn_write_%d(%d, %p, %lu)\n", ret, i, fd1, buffer, (unsigned long) BUFF_SIZE);
        if (ret < 0) {
            return -1;
        }
    }

    t_aw = get_time() - t_bw;

    ret = xpn_close(fd1);
    if (ret < 0) {
        return -1;
    }
    // printf("%d = xpn_close(%d)\n", ret, fd1) ;

    t_ac = get_time() - t_bc;

    printf("Bytes; Total time (ms); Write time (ms)\n");
    printf("%f;%f;%f\n", (double) mb * (double) BUFF_SIZE, t_ac * 1000, t_aw * 1000);

    // xpn-destroy
    ret = xpn_destroy();
    printf("%d = xpn_destroy()\n", ret);
    if (ret < 0) {
        return -1;
    }

    return 0;
}

