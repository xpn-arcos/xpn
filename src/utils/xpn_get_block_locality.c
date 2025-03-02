
/*
 *  Copyright 2000-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

#include <stdlib.h>
#include "xpn.h"
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int ret;
    int url_c;
    char **url_v;
    struct stat st;
    if (argc < 2){
        printf("Usage: %s <file> <offset>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    ret = xpn_init();
    if (ret<0) {
        printf("Error %d while initializing expand\n", ret);
        exit(-1);
    }
    
    ret = xpn_stat(argv[1], &st);
    if (ret < 0){
        printf("Error in xpn_stat %s %s\n", argv[1], strerror(errno));
        goto error_main;
    }

    ret = xpn_get_block_locality( argv[1], atol(argv[2]), &url_c, &url_v );
    if (ret < 0){
        printf("Error in xpn_get_block_locality %s %s\n", argv[1], strerror(errno));
        goto error_main;
    }
    
    printf("Path: %s\nOffset: %ld\nServers urls:\n", argv[1], atol(argv[2]));
    for (int i = 0; i < url_c; i++)
    {
        printf("    %s\n",url_v[i]);
    }
    
    xpn_free_block_locality( &url_c, &url_v );

    xpn_destroy();

    exit(EXIT_SUCCESS);

error_main:
    xpn_destroy();

    exit(EXIT_FAILURE);
}
