
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "all_system.h"
#include "xpn.h"
#include <signal.h>

#define MAX_LINE_LENGTH 512
#define BASE_PATH "P1/"

volatile sig_atomic_t stop = 0;

void handle_sigint(int sig) {
    stop = 1;
}


int main(int argc, char *argv[]) 
{
    if (argc != 3) 
    {
        fprintf(stderr, "Uso: %s <nombre_de_proyecto> <directorio_destino>\n", argv[0]);
        return 1;
    }

    char src_full_path[PATH_MAX];
    if (snprintf(src_full_path, sizeof(src_full_path), "%s%s", BASE_PATH, argv[1]) >= sizeof(src_full_path)) 
    {
        fprintf(stderr, "Ruta del directorio de origen demasiado larga: %s%s\n", BASE_PATH, argv[1]);
        return 1;
    }

    char *dest_dirpath = argv[2];
    struct stat st = {0};
    if (stat(dest_dirpath, &st) == -1) 
    {
        if (mkdir(dest_dirpath, 0700) == -1) 
        {
            perror("Error al crear el directorio de destino");
            return 1;
        }
    }

    int  ret ;

    ret = xpn_init();
    if (ret < 0) 
    {
        return -1;
    }

    char full_path[PATH_MAX];

    snprintf(full_path, sizeof(full_path), "%s%s", BASE_PATH, argv[1]);

    
    signal(SIGINT, handle_sigint);

    while (!stop)
    {

        DIR *dir = xpn_opendir(full_path);
        if (dir == NULL) 
        {
            perror("Error al abrir el directorio");
            return 1;
        }

        struct dirent *entry;
        char filepath[PATH_MAX];
        char dest_filepath[PATH_MAX];
        while ((entry = xpn_readdir(dir)) != NULL) 
        {
            if (entry->d_type == DT_REG) 
            { 
                if (snprintf(filepath, sizeof(filepath), "%s/%s", full_path, entry->d_name) >= sizeof(filepath)) 
                {
                    printf("Ruta del fichero demasiado larga: %s/%s\n", full_path, entry->d_name);
                    continue;
                }

                snprintf(dest_filepath, sizeof(dest_filepath), "%s/%s", dest_dirpath, entry->d_name);

                int fd = xpn_open(filepath, O_RDONLY);
                if (fd == -1) 
                {
                    perror("Error al abrir el fichero");
                    continue;
                }
                
                int dest_fd = open(dest_filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (dest_fd == -1) 
                {
                    perror("Error al abrir el fichero de destino");
                    xpn_close(fd);
                    continue;
                }

                char buffer[MAX_LINE_LENGTH];
                //bzero(buffer, MAX_LINE_LENGTH);
                ssize_t bytes_read;

                while ((bytes_read = xpn_read(fd, buffer, sizeof(buffer))) > 0) 
                {
                    if (write(dest_fd, buffer, bytes_read) == -1) 
                    {
                        perror("Error al escribir en el fichero de destino");
                        break;
                    }
                    //printf("%s", buffer);
                    //bzero(buffer, MAX_LINE_LENGTH);
                }

                if (bytes_read == -1) 
                {
                    perror("Error al leer el fichero");
                }

                xpn_close(fd);
                close(dest_fd);
            }
        }

        sleep(10);
        xpn_closedir(dir);
    }

    ret = xpn_destroy();
    if (ret < 0) 
    {
        printf("ERROR: xpn_destroy()\n");
        return -1;
    }

    
    return 0;
}
