
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
#include <string.h>
#include "xpn.h"

void print_tree(const char *path, int depth) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = xpn_opendir(path)))
        return;

    while ((entry = xpn_readdir(dir)) != NULL) {
        // Ignore "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Print spaces according to the depth
        for (int i = 0; i < depth; i++) {
            printf(" |   ");
        }

        // Print the file/directory name
        printf(" |-- %s\n", entry->d_name);


        // Construct the new path
        char new_path[1024];
        snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);

        // Check if it's a directory and make a recursive call
        struct stat info;
        if (xpn_stat(new_path, &info) == 0 && S_ISDIR(info.st_mode)) {
            print_tree(new_path, depth + 1);
        }
    }
    xpn_closedir(dir);
}

int main(int argc, char *argv[])
{
    int ret;
    if (argc < 2){
        printf("Usage: %s <path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    ret = xpn_init();
    if (ret < 0) {
        printf("Error %d while initializing expand\n", ret);
        exit(-1);
    }

    printf("Path:\n%s\n", argv[1]);
    print_tree(argv[1], 0);

    xpn_destroy();

    exit(EXIT_SUCCESS);
}
