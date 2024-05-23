
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

/* ... Include / Inclusion ........................................... */

#include <dirent.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "mpi.h"
#include "ns.h"
#include "xpn/xpn_simple/xpn_policy_rw.h"

/* ... Const / Const ................................................. */

#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif

#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#define HEADER_SIZE 8192

int *rank_actual_to_new = NULL;
int *rank_actual_to_old = NULL;
int *rank_new_to_actual = NULL;
int *rank_old_to_actual = NULL;
int old_size, new_size;
struct mpi_msg_info {
    int rank_send;
    int rank_recv;
    ssize_t read_size;
    off64_t offset;
};

/* ... Functions / Funciones ......................................... */

int copy(char *entry, int is_file, int blocksize, int replication_level, int rank, int size) {
    struct stat st;
    int res;
    off64_t ret_2;
    int master_old = 0;
    char dest_path[PATH_MAX];
    int fd_dest = -1, fd_src = -1;
    ssize_t read_size, write_size;
    off_t offset_src, offset_dest, offset_real;
    int replication, rank_to_send;
    char *buf = NULL, *buf_recv = NULL;
    struct mpi_msg_info *all_msg_info = NULL;
    struct mpi_msg_info local_msg_info;
    int buf_len;

    MPI_Barrier(MPI_COMM_WORLD);

    // Calculate the master node of the old file
    for (int i = 0; i < size; i++) {
        if (rank_actual_to_old[i] == 0) {
            master_old = i;
        }
    }
    // Get stat of file only in master node of the old file
    if (rank == master_old) {
        res = stat(entry, &st);
        if (res < 0) {
            perror("stat: ");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
    }
    // Broadcast stat
    res = MPI_Bcast(&st, sizeof(struct stat), MPI_BYTE, master_old, MPI_COMM_WORLD);
    if (res != MPI_SUCCESS) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    if (!is_file) {
        if (rank_actual_to_new[rank] != -1) {
            // Not error when mkdir because it can exist
            res = mkdir(entry, st.st_mode);
            if (res == -1 && errno != EEXIST) {
                perror("mkdir");
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
        }
    } else {
        // Only create tmp file in new_ranks
        if (rank_actual_to_new[rank] != -1) {
            sprintf(dest_path, "%s_XXXXXX", entry);
            fd_dest = mkstemp(dest_path);
            if (fd_dest < 0) {
                perror("mkstemp");
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
            close(fd_dest);

            fd_dest = open64(dest_path, O_CREAT | O_WRONLY | O_TRUNC | O_LARGEFILE, st.st_mode);
            if (fd_dest < 0) {
                perror("open fd_dest");
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
        }
        // Only open file in old_ranks
        if (rank_actual_to_old[rank] != -1) {
            fd_src = open64(entry, O_RDONLY | O_LARGEFILE);
            if (fd_src < 0) {
                perror("open fd_src");
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
        }
        // Alocate buffers
        buf_len = blocksize;
        buf = (char *)malloc(blocksize);
        if (NULL == buf) {
            perror("malloc");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        buf_recv = (char *)malloc(blocksize);
        if (NULL == buf_recv) {
            perror("malloc");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        all_msg_info = malloc(size * sizeof(struct mpi_msg_info));
        if (NULL == all_msg_info) {
            perror("malloc");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        // Write header only in new_ranks
        if (rank_actual_to_new[rank] != -1) {
            char header_buf[HEADER_SIZE];
            memset(header_buf, 0, HEADER_SIZE);
            write_size = filesystem_write(fd_dest, header_buf, HEADER_SIZE);
            if (write_size != HEADER_SIZE) {
                perror("write header");
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
        }

        offset_src = 0;
        read_size = 0;
        int finish = 0;
        do {
            // Read in old_ranks
            if (rank_actual_to_old[rank] != -1) {
                // Calculate the block
                XpnCalculateBlockInvert(blocksize, replication_level, old_size, rank_actual_to_old[rank], offset_src,
                                        &offset_real, &replication);
                XpnCalculateBlock(blocksize, replication_level, new_size, offset_real, replication, &offset_dest,
                                  &rank_to_send);
                ret_2 = lseek64(fd_src, offset_src + HEADER_SIZE, SEEK_SET);
                if (ret_2 < 0) {
                    perror("lseek");
                    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
                }
                read_size = filesystem_read(fd_src, buf, buf_len);
                // Construct mpi_msg_info to send
                local_msg_info.rank_send = rank;
                local_msg_info.rank_recv = rank_new_to_actual[rank_to_send];
                local_msg_info.read_size = read_size;
                local_msg_info.offset = offset_dest;
            } else {
                // Construct mpi_msg_info when rank is not in the old ranks
                local_msg_info.rank_send = -1;
                local_msg_info.rank_recv = -1;
                local_msg_info.read_size = -1;
                local_msg_info.offset = -1;
            }
            // Send the structure of mpi_msg_info to all processes
            res = MPI_Allgather(&local_msg_info, sizeof(struct mpi_msg_info), MPI_BYTE, all_msg_info, sizeof(struct mpi_msg_info), MPI_BYTE, MPI_COMM_WORLD);
            if (res != MPI_SUCCESS) {
                fprintf(stderr, "Error: %s\n", strerror(errno));
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }

            // For all messages
            for (int i = 0; i < size; i++) {
                // Send if the rank has to send and not is the same as recv
                if (all_msg_info[i].rank_send == rank) {
                    if (all_msg_info[i].rank_send != all_msg_info[i].rank_recv) {
                        res = MPI_Send(buf, all_msg_info[i].read_size, MPI_CHAR, all_msg_info[i].rank_recv, 0, MPI_COMM_WORLD);
                        if (res != MPI_SUCCESS) {
                            fprintf(stderr, "Error: %s\n", strerror(errno));
                            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
                        }
                    }
                }
                // Recv if rank has to recv and not is the same as send
                // then write the recv buf
                if (all_msg_info[i].rank_recv == rank) {
                    if (all_msg_info[i].rank_send != all_msg_info[i].rank_recv) {
                        res = MPI_Recv(buf_recv, all_msg_info[i].read_size, MPI_CHAR, all_msg_info[i].rank_send, 0, MPI_COMM_WORLD,
                                       MPI_STATUS_IGNORE);
                        if (res != MPI_SUCCESS) {
                            fprintf(stderr, "Error: %s\n", strerror(errno));
                            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
                        }
                    }
                    ret_2 = lseek64(fd_dest, all_msg_info[i].offset + HEADER_SIZE, SEEK_SET);
                    if (ret_2 < 0) {
                        perror("lseek: ");
                        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
                    }
                    // Use diferent buf if recv or if the same rank use directly
                    if (all_msg_info[i].rank_send != all_msg_info[i].rank_recv) {
                        write_size = filesystem_write(fd_dest, buf_recv, all_msg_info[i].read_size);
                    } else {
                        write_size = filesystem_write(fd_dest, buf, all_msg_info[i].read_size);
                    }

                    if (write_size != all_msg_info[i].read_size) {
                        perror("write: ");
                        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
                    }
                }
            }
            // Check if all mpi_msg_info dont have read_size
            finish = 1;
            for (int i = 0; i < size; i++) {
                if (all_msg_info[i].read_size > 0) {
                    finish = 0;
                }
            }

            offset_src += blocksize;
        } while (finish == 0);

        // Only in old_ranks
        if (rank_actual_to_old[rank] != -1) {
            close(fd_src);
            unlink(entry);
        }
        // Only in new_ranks
        if (rank_actual_to_new[rank] != -1) {
            close(fd_dest);
            rename(dest_path, entry);
        }

        if (buf != NULL) {
            free(buf);
        }
        if (buf_recv != NULL) {
            free(buf_recv);
        }
        if (all_msg_info != NULL) {
            free(all_msg_info);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    return 0;
}

int list(char *dir_name, int blocksize, int replication_level, int rank, int size) {
    int res;
    DIR *dir = NULL;
    struct stat stat_buf;
    struct dirent *entry;
    char path[PATH_MAX];
    int while_loop = 1;
    // Calculate the master rank of the old file
    int master_old = 0;
    for (int i = 0; i < size; i++) {
        if (rank_actual_to_old[i] == 0) {
            master_old = i;
        }
    }
    // Only on master open and readdir (because there are no dir structure in some of new ranks)
    // then send to the others ranks
    if (rank == master_old) {
        dir = opendir(dir_name);
        if (dir == NULL) {
            perror("opendir:");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        entry = readdir(dir);
        while (entry != NULL) {
            if (!strcmp(entry->d_name, ".")) {
                entry = readdir(dir);
                continue;
            }

            if (!strcmp(entry->d_name, "..")) {
                entry = readdir(dir);
                continue;
            }

            sprintf(path, "%s/%s", dir_name, entry->d_name);

            res = stat(path, &stat_buf);
            if (res < 0) {
                perror("stat: ");
                printf("%s\n", path);
                entry = readdir(dir);
                continue;
            }
            while_loop = 1;
            // Broadcast the values to run copy()
            res = MPI_Bcast(&while_loop, 1, MPI_INT, master_old, MPI_COMM_WORLD);
            if (res != MPI_SUCCESS) {
                fprintf(stderr, "Error: %s\n", strerror(errno));
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
            int is_file = !S_ISDIR(stat_buf.st_mode);
            res = MPI_Bcast(path, PATH_MAX, MPI_CHAR, master_old, MPI_COMM_WORLD);
            if (res != MPI_SUCCESS) {
                fprintf(stderr, "Error: %s\n", strerror(errno));
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
            res = MPI_Bcast(&is_file, 1, MPI_INT, master_old, MPI_COMM_WORLD);
            if (res != MPI_SUCCESS) {
                fprintf(stderr, "Error: %s\n", strerror(errno));
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
            res = MPI_Bcast(dir_name, PATH_MAX, MPI_CHAR, master_old, MPI_COMM_WORLD);
            if (res != MPI_SUCCESS) {
                fprintf(stderr, "Error: %s\n", strerror(errno));
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
            copy(path, is_file, blocksize, replication_level, rank, size);

            if (S_ISDIR(stat_buf.st_mode)) {
                list(path, blocksize, replication_level, rank, size);
            }

            entry = readdir(dir);
        }
        while_loop = 0;
        res = MPI_Bcast(&while_loop, 1, MPI_INT, master_old, MPI_COMM_WORLD);
        if (res != MPI_SUCCESS) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        closedir(dir);
    } else {
        while_loop = 1;
        while (1) {
            // Get the broadcast values to run copy()
            res = MPI_Bcast(&while_loop, 1, MPI_INT, master_old, MPI_COMM_WORLD);
            if (res != MPI_SUCCESS) {
                fprintf(stderr, "Error: %s\n", strerror(errno));
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
            if (while_loop == 0) {
                break;
            }
            int is_file;
            res = MPI_Bcast(path, PATH_MAX, MPI_CHAR, master_old, MPI_COMM_WORLD);
            if (res != MPI_SUCCESS) {
                fprintf(stderr, "Error: %s\n", strerror(errno));
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
            res = MPI_Bcast(&is_file, 1, MPI_INT, master_old, MPI_COMM_WORLD);
            if (res != MPI_SUCCESS) {
                fprintf(stderr, "Error: %s\n", strerror(errno));
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
            res = MPI_Bcast(dir_name, PATH_MAX, MPI_CHAR, master_old, MPI_COMM_WORLD);
            if (res != MPI_SUCCESS) {
                fprintf(stderr, "Error: %s\n", strerror(errno));
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
            copy(path, is_file, blocksize, replication_level, rank, size);
        }
    }

    return 0;
}

void calculate_ranks_sizes(char *path_old_hosts, char *path_new_hosts, int *old_rank, int *new_rank) {
    // Get ip and hostname
    char *hostip = ns_get_host_ip();
    char hostname[HOST_NAME_MAX];
    ns_get_hostname(hostname);
    // Open host files
    FILE *file_old = NULL;
    FILE *file_new = NULL;
    file_old = fopen(path_old_hosts, "r");
    if (file_old == NULL) {
        perror("Error opening old_hosts");
        goto cleanup_calculate_ranks_sizes;
    }
    file_new = fopen(path_new_hosts, "r");
    if (file_new == NULL) {
        perror("Error opening new_hosts");
        goto cleanup_calculate_ranks_sizes;
    }

    // Read line by line to get the new and old rank
    char line[HOST_NAME_MAX];
    int rank = 0;
    *old_rank = -1;
    while (fscanf(file_old, "%s", line) == 1) {
        if (strstr(line, hostip) != NULL || strstr(line, hostname) != NULL) {
            *old_rank = rank;
        }
        rank++;
    }
    old_size = rank;
    rank = 0;
    *new_rank = -1;
    while (fscanf(file_new, "%s", line) == 1) {
        if (strstr(line, hostip) != NULL || strstr(line, hostname) != NULL) {
            *new_rank = rank;
        }
        rank++;
    }
    new_size = rank;
cleanup_calculate_ranks_sizes:
    if (file_old != NULL) {
        fclose(file_old);
    }
    if (file_new != NULL) {
        fclose(file_new);
    }
}

// TODO: think if MPI_Abort is the desired error handler
int main(int argc, char *argv[]) {
    int rank, size, old_rank, new_rank;
    int replication_level = 0;
    int blocksize = 524288;
    double start_time;
    int res = 0;
    //
    // Check arguments...
    //
    if (argc < 4) {
        printf("Usage:\n");
        printf(
            " ./%s <xpn local path> <path to old hostfile> <path to new hostfile> <optional destination block size> "
            "<optional replication level>\n",
            argv[0]);
        printf("\n");
        return -1;
    }

    if (argc >= 6) {
        replication_level = atoi(argv[5]);
    }
    if (argc >= 5) {
        blocksize = atoi(argv[4]);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    start_time = MPI_Wtime();
    if (rank == 0) {
        printf("Rebuild from %s blocksize %d replication_level %d \n", argv[1], blocksize, replication_level);
    }
    calculate_ranks_sizes(argv[2], argv[3], &old_rank, &new_rank);

    // Allocate buffers to comunication
    rank_actual_to_old = malloc(size * sizeof(int));
    rank_actual_to_new = malloc(size * sizeof(int));
    rank_old_to_actual = malloc(size * sizeof(int));
    rank_new_to_actual = malloc(size * sizeof(int));
    if (rank_actual_to_new == NULL || rank_actual_to_old == NULL || rank_new_to_actual == NULL ||
        rank_old_to_actual == NULL) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Transfer the old and new ranks
    res = MPI_Allgather(&old_rank, 1, MPI_INT, rank_actual_to_old, 1, MPI_INT, MPI_COMM_WORLD);
    if (res != MPI_SUCCESS) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    res = MPI_Allgather(&new_rank, 1, MPI_INT, rank_actual_to_new, 1, MPI_INT, MPI_COMM_WORLD);
    if (res != MPI_SUCCESS) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    // Construct maps
    for (int i = 0; i < size; i++) {
        rank_old_to_actual[i] = -1;
        rank_new_to_actual[i] = -1;
    }
    for (int i = 0; i < size; i++) {
        if (rank_actual_to_old[i] != -1) {
            rank_old_to_actual[rank_actual_to_old[i]] = i;
        }
        if (rank_actual_to_new[i] != -1) {
            rank_new_to_actual[rank_actual_to_new[i]] = i;
        }
    }

    char dir_name[PATH_MAX];
    strncpy(dir_name, argv[1], PATH_MAX - 1);

    list(dir_name, blocksize, replication_level, rank, size);

    if (rank == 0) {
        printf("Rebuild elapsed time %f mseg\n", (MPI_Wtime() - start_time) * 1000);
    }

    if (rank_actual_to_new != NULL) {
        free(rank_actual_to_new);
    }
    if (rank_actual_to_old != NULL) {
        free(rank_actual_to_old);
    }
    if (rank_new_to_actual != NULL) {
        free(rank_new_to_actual);
    }
    if (rank_old_to_actual != NULL) {
        free(rank_old_to_actual);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return res;
}

/* ................................................................... */
