
/*
 *  Copyright 2020-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
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

  #include <stdio.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <stdlib.h>
  #include <string.h>
  #include <fcntl.h>
  #include <linux/limits.h>
  #include <sys/stat.h>
  #include <dirent.h>
  #include "mpi.h"
  #include "xpn/xpn_simple/xpn_policy_rw.h"

/* ... Const / Const ................................................. */

  #ifndef _LARGEFILE_SOURCE
  #define _LARGEFILE_SOURCE
  #endif

  #ifndef _FILE_OFFSET_BITS
  #define _FILE_OFFSET_BITS 64
  #endif

  #define MIN(a,b) (((a)<(b))?(a):(b))
  #define HEADER_SIZE 8192

  int xpn_path_len = 0;

/* ... Functions / Funciones ......................................... */

  int copy(char * entry, int rank, int size, int last_size)
  {  
    debug_info("copy entry %s rank %d size %d last_size %d\n", entry, rank, size, last_size);
    int ret;
    int fd_src;
    struct stat st;
    struct xpn_metadata mdata;

    if (rank == 0){
      printf("%s\n", entry);
    }

    int master_node_old = hash(&entry[xpn_path_len], last_size, 1);
    int master_node_new = hash(&entry[xpn_path_len], size, 1);
    int has_new_mdata = 0;

    debug_info("master_node_old %d master_node_new %d\n", master_node_old, master_node_new);

    if (master_node_old == rank){
      fd_src = open(entry, O_RDONLY);
      if (fd_src < 0){
        perror("open :");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
      ret = filesystem_read(fd_src, &mdata, sizeof(mdata));
      if (ret < 0){
        perror("read mdata :");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
      ret = close(fd_src);
      if (ret < 0){
        perror("close :");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
      ret = stat(entry, &st);
      if (ret < 0){
        perror("stat :");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
      
      // Check correct mdata
      if (!XPN_CHECK_MAGIC_NUMBER(&mdata)){
        printf("Error: metadata incorrect\n");
        int len;
        char processor_name[MPI_MAX_PROCESSOR_NAME];
        MPI_Get_processor_name(processor_name, &len);
        printf("Rank %d processor_name %s ", rank, processor_name); 
        XpnPrintMetadata(&mdata);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
    }

    MPI_Bcast(&mdata, sizeof(mdata), MPI_CHAR, master_node_old, MPI_COMM_WORLD);
    MPI_Bcast(&st, sizeof(st), MPI_CHAR, master_node_old, MPI_COMM_WORLD);
    
    // Calculate where to write the new metadata
    int aux_serv;
    for (int i = 0; i < mdata.replication_level+1; i++)
    { 
      aux_serv = ( master_node_new + i ) % size;
      if (aux_serv == rank){
        has_new_mdata = 1;
        break;
      }
    }

    // Modify the metadata
    if (mdata.data_nserv[XPN_METADATA_MAX_RECONSTURCTIONS-1] != 0){
      printf("Error: it cannot be more expansion in servers it not fit in metadata\n");
    }

    for (int i = 1; i < XPN_METADATA_MAX_RECONSTURCTIONS; i++)
    {
      if (mdata.data_nserv[i] == 0){
        int actual_blocks = mdata.file_size / mdata.block_size;
        int limit_actual_blocks = (actual_blocks + mdata.data_nserv[i-1] - 1) / mdata.data_nserv[i-1] * mdata.data_nserv[i-1];
        limit_actual_blocks -= 1;
        mdata.data_nserv[i] = size;
        mdata.offsets[i] = limit_actual_blocks;
        break;
      }
    }
    
      debug_info("Write metadata for %s in rank %d\n", entry, rank);
    fd_src = open(entry, O_WRONLY | O_CREAT, st.st_mode);
    if (fd_src < 0){
      perror("open :");
      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    if (has_new_mdata == 1){
      ret = filesystem_write(fd_src, &mdata, sizeof(mdata));
      if (ret < 0){
        perror("write mdata :");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
    }
    ret = close(fd_src);
    if (ret < 0){
      perror("close :");
      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    return 0;
  }


  int list (char * dir_name, int rank, int size, int last_size)
  {
    debug_info("dir_name %s rank %d size %d last_size %d\n", dir_name, rank, size, last_size);
    
    int ret;
    DIR* dir = NULL;
    struct stat stat_buf;
    char path [PATH_MAX];
    int buff_coord = 1;
    
    int master_node = hash(&dir_name[xpn_path_len], last_size, 1);
    debug_info("for %s master_node %d\n", dir_name, master_node);
    if (rank == master_node){
      dir = opendir(dir_name);
      if(dir == NULL)
      {
        perror("opendir:");
        return -1;
      }
      struct dirent*  entry;
      entry = readdir(dir);
      

      while(entry != NULL)
      {
        debug_info("Rank %d readdir %s readed %s\n", rank, dir_name, entry->d_name);
        if (! strcmp(entry->d_name, ".")){
          entry = readdir(dir);
          continue;
        }

        if (! strcmp(entry->d_name, "..")){
          entry = readdir(dir);
          continue;
        }

        sprintf(path, "%s/%s", dir_name, entry->d_name);

        ret = stat(path, &stat_buf);
        if (ret < 0) 
        {
          perror("stat: ");
          printf("%s\n", path);
          entry = readdir(dir);
          continue;
        }

        MPI_Bcast(&buff_coord, 1, MPI_INT, master_node, MPI_COMM_WORLD);
        MPI_Bcast(&path, sizeof(path), MPI_CHAR, master_node, MPI_COMM_WORLD);
        MPI_Bcast(&stat_buf, sizeof(stat_buf), MPI_CHAR, master_node, MPI_COMM_WORLD);
        if (S_ISDIR(stat_buf.st_mode)){
          list(path, rank, size, last_size);
        }else{
          copy(path, rank, size, last_size);
        }

        entry = readdir(dir);
      }
      buff_coord = 0;
      MPI_Bcast(&buff_coord, 1, MPI_INT, master_node, MPI_COMM_WORLD);
      closedir(dir);
    }else{
      while(buff_coord == 1){
        MPI_Bcast(&buff_coord, 1, MPI_INT, master_node, MPI_COMM_WORLD);
        if (buff_coord == 0) break;
        MPI_Bcast(&path, sizeof(path), MPI_CHAR, master_node, MPI_COMM_WORLD);
        MPI_Bcast(&stat_buf, sizeof(stat_buf), MPI_CHAR, master_node, MPI_COMM_WORLD);

        if (S_ISDIR(stat_buf.st_mode)){
          list(path, rank, size, last_size);
        }else{
          copy(path, rank, size, last_size);
        }
      }
    }

    return 0;
  }


  int main(int argc, char *argv[])
  {   
    int rank, size;
    double start_time;
    int last_size;
    //
    // Check arguments...
    //
    if ( argc < 3 )
    {
      printf("Usage:\n");
      printf(" ./%s <path to dir> <last size>\n", argv[0]);
      printf("\n");
      return -1;
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    last_size = atoi(argv[2]);
    if (last_size >= size){
      if (rank == 0){
        printf("Error: the last size '%d' is bigger or equal to the new size '%d'\n", last_size, size);
      }
      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    start_time = MPI_Wtime();
    if (rank == 0){
      printf("Expand in path %s from %d servers to %d servers\n", argv[1], last_size, size);
    }
    xpn_path_len = strlen(argv[1]);
    list (argv[1], rank, size, last_size);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0){
      printf("Expand elapsed time %f mseg\n", (MPI_Wtime() - start_time)*1000);
    }
    MPI_Finalize();

    return 0;
  }

/* ................................................................... */
