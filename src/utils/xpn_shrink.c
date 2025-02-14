
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
  #include "base/ns.h"


/* ... Const / Const ................................................. */

  #ifndef _LARGEFILE_SOURCE
  #define _LARGEFILE_SOURCE
  #endif

  #ifndef _FILE_OFFSET_BITS
  #define _FILE_OFFSET_BITS 64
  #endif

  #define MIN(a,b) (((a)<(b))?(a):(b))
  #define THREAD_WRITER 1

  struct xpn_metadata new_mdata;
  char *t_entry;
  struct stat st;
  int xpn_path_len = 0;

  int rank, size, new_size, pos_in_shrink;


  void *recv_thread(void * arg);
/* ... Functions / Funciones ......................................... */
  int is_prev_first_node(int first_node, int node, int nserv){
    if (first_node == 0 && node == nserv-1){
      return 1;
    }
    if (first_node != 0 && node == first_node-1){
      return 1;
    }
    return 0;
  }

  int copy(char * entry)
  {  
    debug_info("copy entry %s rank %d size %d new_size %d pos_in_shrink %d\n", entry, rank, size, new_size, pos_in_shrink);
    int ret;
    int fd_src;
    struct xpn_metadata mdata;

    if (rank == 0){
      printf("%s\n", entry);
    }

    int master_node_old = hash(&entry[xpn_path_len], size, 1);
    int master_node_new = hash(&entry[xpn_path_len], new_size, 1);
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
    debug_info("before bcast\n");
    MPI_Bcast(&mdata, sizeof(mdata), MPI_CHAR, master_node_old, MPI_COMM_WORLD);
    new_mdata = mdata;
    MPI_Bcast(&st, sizeof(st), MPI_CHAR, master_node_old, MPI_COMM_WORLD);
    debug_info("after bcast\n");
    
    if (rank == 0){
      #ifdef DEBUG
      XpnPrintMetadata(&mdata);
      XpnPrintBlockDistribution(20, &mdata); 
      #endif
    }

    // Modify the metadata
    int rank_bcast = -1, aux_rank_bcast = -1;
    int current_serv = 0;
    int times_do_it = 0;
    int trans_rank = rank;
    int actual_last_block = mdata.file_size / mdata.block_size;
    int *trans_rank_array = malloc(sizeof(int) * size);
    if (trans_rank_array == NULL){
      perror("malloc: ");
      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    for (int i = 1; i < XPN_METADATA_MAX_RECONSTURCTIONS; i++)
    {
      if (new_mdata.data_nserv[i] == 0 && i+1 < XPN_METADATA_MAX_RECONSTURCTIONS){
        if (current_serv == pos_in_shrink){
          aux_rank_bcast = rank;
        }else{
          aux_rank_bcast = -1;
        }
        MPI_Allreduce(&aux_rank_bcast, &rank_bcast, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
        // aux_st = local_st;
        // MPI_Bcast(&aux_st, sizeof(aux_st), MPI_CHAR, rank_bcast, MPI_COMM_WORLD);
        MPI_Allgather(&trans_rank, 1, MPI_INT, trans_rank_array, 1, MPI_INT, MPI_COMM_WORLD);
        if (rank == 0){
          debug_info("Rank %d rank_bcast %d\n", rank, rank_bcast);
          debug_info("Trans_rank_array ");
          for (int i = 0; i < size; i++)
          {
            #ifdef DEBUG
            printf("%d ",trans_rank_array[i]);
            if (i == size-1){
              printf("\n");
            }
            #endif
          }
        }
        
        int prev_nserv;
        int prev_block = new_mdata.data_nserv[i-1];
        if (new_mdata.data_nserv[i-1] < 0){
          prev_nserv = new_mdata.data_nserv[i-2];
        }else{
          prev_nserv = new_mdata.data_nserv[i-1];
        }
        if (rank == 0){ debug_info("prev_nserv %d\n", prev_nserv); }
          
        if (actual_last_block < prev_block){
          actual_last_block = prev_block;
        }
        actual_last_block++;
        int replication = 0;
        off_t local_offset;
        int aux_serv;
        int counter = 0;
        for (int j = 0; j < prev_nserv*2; j++)
        {
          XpnCalculateBlockMdata(&new_mdata, actual_last_block*new_mdata.block_size, replication, &local_offset, &aux_serv);
          if (rank == 0){ debug_info("actual_last_block %d aux_serv %d\n",actual_last_block, aux_serv); }
          if (is_prev_first_node(new_mdata.first_node, aux_serv, prev_nserv) == 1){
            if (counter == 1){
              // break;
              goto tag1;
            }else{
              counter ++;
            }
          }
          replication++;
          if (replication > new_mdata.replication_level){
            actual_last_block ++;
            replication = 0;
          }
        }
        tag1:

        replication = new_mdata.replication_level;
        int aux_actual_last_block = actual_last_block;
        for (int j = prev_nserv-1; j >= 0; j--)
        {
          XpnCalculateBlockMdata(&new_mdata, aux_actual_last_block*new_mdata.block_size, replication, &local_offset, &aux_serv);
          if (rank == 0){ debug_info("aux_actual_last_block %d aux_serv %d local_offset_blocks %d\n",aux_actual_last_block, aux_serv, local_offset/new_mdata.block_size); }
          replication--;
          if (replication < 0){
            aux_actual_last_block --;
            replication = new_mdata.replication_level;
          }
          if (aux_serv == trans_rank_array[rank_bcast]){
            goto tag2;
            // break;
          }
        }
        tag2:

        // Server id in negative
        new_mdata.data_nserv[i] = (trans_rank_array[rank_bcast] + 1) * -1;
        // Num blocks to redistribute
        new_mdata.offsets[i] = local_offset / new_mdata.block_size + 1;
        // New nserv
        new_mdata.data_nserv[i+1] = prev_nserv - 1;
        // New last block
        new_mdata.offsets[i+1] = actual_last_block;
        debug_info("Rank %d trans_rank %d rank_bcast %d\n", rank, trans_rank, rank_bcast);
        if (rank > rank_bcast){
          trans_rank--;
        }else if (rank == rank_bcast){
          trans_rank=-1;
        }

        if (rank == 0){
          #ifdef DEBUG
          XpnPrintMetadata(&new_mdata);
          XpnPrintBlockDistribution(20, &new_mdata); 
          #endif
        }
        current_serv++;
        // Check when finish
        times_do_it++;
        if (times_do_it == size-new_size){
          break;
        }
      }
    }
    
    MPI_Allgather(&trans_rank, 1, MPI_INT, trans_rank_array, 1, MPI_INT, MPI_COMM_WORLD);

    MPI_Comm comm_separated;
    
    if (pos_in_shrink == -1){
      MPI_Comm_split(MPI_COMM_WORLD, 0, rank, &comm_separated);
    }else{
      MPI_Comm_split(MPI_COMM_WORLD, 1, size-rank, &comm_separated);
    }

    if (rank == 0){
      #ifdef DEBUG
      XpnPrintMetadata(&new_mdata);
      XpnPrintBlockDistribution(20, &new_mdata); 
      printf("Trans_rank_array ");
      for (int i = 0; i < size; i++)
      {
        printf("%d ",trans_rank_array[i]);
      }
      printf("\n");
      #endif
    }
    //Start the data move
    char *buf;    
    int has_remaining_blocks = 1;
    t_entry = entry;
    if (pos_in_shrink == -1){
      // If is writer
      if (THREAD_WRITER == 1){
        recv_thread(NULL);
      }else{
        pthread_t threads[THREAD_WRITER];
        for (int i = 0; i < THREAD_WRITER; i++)
        {
		      pthread_create(&threads[i], NULL, (void * (*)(void *))recv_thread, NULL);
        }
        for (int i = 0; i < THREAD_WRITER; i++)
        {
		      pthread_join(threads[i], NULL);
        }
      }
      
    }else{
      // If is reader
      int fd; 
      buf = malloc(sizeof(char)*new_mdata.block_size);
      if (buf == NULL){
        perror("malloc: ");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }

      fd = open64(t_entry, O_RDWR | O_CREAT | O_LARGEFILE, st.st_mode);
      if (fd < 0){
        perror("open reader :");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
      off_t offset_dest = -new_mdata.block_size; 
      off_t offset_src;
      off_t local_offset;
      int replication = 0;
      int read_size = 0;
      int aux_serv;
      do{
        // First calcule from where
        do{
          offset_dest += mdata.block_size;
          for (replication = 0; replication < mdata.replication_level+1; replication++)
          {
            XpnCalculateBlockMdata(&mdata, offset_dest, replication, &local_offset, &aux_serv);
            
            // debug_info("try rank %d offset_dest %ld offset_src %ld aux_server %d\n", rank, offset_dest, offset_src, aux_serv);
            if (rank == aux_serv){
              goto exit_search;
            }
          }
        }while(offset_dest < mdata.file_size);
        exit_search:
        if (rank != aux_serv){
          continue;
        }

        
        off64_t ret_2;
        ret_2 = lseek64(fd, local_offset + XPN_HEADER_SIZE, SEEK_SET);
        if (ret_2 < 0){
          printf("lseek offset %ld\n", local_offset + XPN_HEADER_SIZE);
          perror("lseek :");
          MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        read_size = filesystem_read(fd, buf, new_mdata.block_size);
        // debug_info("Rank %d local_offset %ld read_size %d\n", rank, local_offset, read_size);
        if (read_size <= 0){
          has_remaining_blocks = 0;
          break;
        }
        int rank_to_send = 0;
        XpnCalculateBlockMdata(&new_mdata, offset_dest, replication, &offset_src, &aux_serv);
        for (int i = 0; i < size; i++)
        {
          if (aux_serv == trans_rank_array[i]){
            rank_to_send = i;
            break;
          }
        }
        
        // debug_info("Rank %d send to %d has_remaining_blocks %d local_offset %ld read_size %d\n", rank, rank_to_send, has_remaining_blocks, local_offset, read_size);
        MPI_Send(&has_remaining_blocks, 1, MPI_INT, rank_to_send, 0, MPI_COMM_WORLD);
        MPI_Send(&offset_src, 1, MPI_LONG, rank_to_send, 1, MPI_COMM_WORLD);
        MPI_Send(&read_size, 1, MPI_INT, rank_to_send, 2, MPI_COMM_WORLD);
        MPI_Send(buf, read_size, MPI_CHAR, rank_to_send, 3, MPI_COMM_WORLD);

        // MPI_Recv(&has_remaining_blocks, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        // if (has_remaining_blocks == 0) break;

        // MPI_Recv(&offset, 1, MPI_LONG, status.MPI_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // MPI_Recv(&buf_size, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // MPI_Recv(buf[0], buf_size, MPI_CHAR, status.MPI_SOURCE, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }while(has_remaining_blocks == 1);

      debug_info("Rank %d after barrier readers\n", rank);
      MPI_Barrier(comm_separated);

      // Select master to stop writers
      int master_send_stop;
      for (master_send_stop = 0; master_send_stop < size; master_send_stop++)
      {
        if (trans_rank_array[master_send_stop] == -1) break;
      }

      debug_info("Rank %d Before barrier readers master_send_stop %d\n", rank, master_send_stop);

      if (rank == master_send_stop){
        for (int i = 0; i < size; i++)
        {
          if (trans_rank_array[i] != -1){
            for (int j = 0; j < THREAD_WRITER; j++)
            {
              MPI_Send(&has_remaining_blocks, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
          }
        }      
      }
        
      ret = close(fd);
      if (ret < 0){
        perror("close :");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }

      ret = unlink(t_entry);
      if (ret < 0){
        perror("unlink: ");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
      
    }
        
    // Calculate where to write the new metadata
    int aux_serv = 0;
    for (int i = 0; i < mdata.replication_level+1; i++)
    { 
      aux_serv = ( master_node_new + i ) % size;
      if (trans_rank_array[rank] == aux_serv){
        has_new_mdata = 1;
        break;
      }
    }
    debug_info("Rank %d Write metadata master_node_new %d\n", rank, master_node_new);
    if (has_new_mdata == 1){
      debug_info("Write metadata for %s in rank %d new rank %d\n", entry, rank, trans_rank_array[rank]);
      fd_src = open(entry, O_WRONLY | O_CREAT, st.st_mode);
      if (fd_src < 0){
        perror("open :");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
      ret = filesystem_write(fd_src, &new_mdata, sizeof(new_mdata));
      if (ret < 0){
        perror("write mdata :");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
      ret = close(fd_src);
      if (ret < 0){
        perror("close :");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
    }
    
    free(trans_rank_array);
    MPI_Comm_free(&comm_separated);

    return 0;
  }

  void *recv_thread(__attribute__((__unused__)) void * arg){
    int ret;
    char *buf;    
    int has_remaining_blocks = 1;
    off_t offset;
    int buf_size;
    MPI_Status status;
    int fd;

    buf = malloc(sizeof(char)*new_mdata.block_size);
    if (buf == NULL){
      perror("malloc: ");
      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    fd = open64(t_entry, O_WRONLY | O_CREAT | O_LARGEFILE, st.st_mode);
    if (fd < 0){
      perror("open writer:");
      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    debug_info("Rank %d starting listening\n", rank);
    do{
      MPI_Recv(&has_remaining_blocks, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      // debug_info("Rank %d recv has_remaining_blocks %d\n", rank, has_remaining_blocks);
      if (has_remaining_blocks == 0) break;


      MPI_Recv(&offset, 1, MPI_LONG, status.MPI_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&buf_size, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(buf, buf_size, MPI_CHAR, status.MPI_SOURCE, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // debug_info("Rank %d recv offset %ld buf_size %d buf\n", rank, offset, buf_size);

      off64_t ret_2;
      ret_2 = lseek64(fd, offset + XPN_HEADER_SIZE, SEEK_SET);
      if (ret_2 < 0){
        printf("lseek offset %ld\n", offset + XPN_HEADER_SIZE);
        perror("lseek :");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }

      ret = filesystem_write(fd, buf, buf_size);
      if (ret < 0){
        perror("write buf :");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
    }while(has_remaining_blocks == 1);
    
    ret = close(fd);
    if (ret < 0){
      perror("close :");
      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    debug_info("Exit recv_thread rank %d\n", rank);
    return NULL;
  }


  int list (char * dir_name)
  {
    debug_info("dir_name %s rank %d size %d new_size %d pos_in_shrink %d\n", dir_name, rank, size, new_size, pos_in_shrink);
    
    int ret;
    DIR* dir = NULL;
    struct stat stat_buf;
    char path [PATH_MAX];
    int buff_coord = 1;
    

    int master_node = hash(&dir_name[xpn_path_len], size, 1);
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
          list(path);
        }else{
          copy(path);
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
          list(path);
        }else{
          copy(path);
        }
      }
    }

    return 0;
  }


  int main(int argc, char *argv[])
  {   
    double start_time;
    const char *delim = ";";
    char *servers[XPN_METADATA_MAX_RECONSTURCTIONS] = {NULL};
    char hostname[MPI_MAX_PROCESSOR_NAME];
    char *hostip;
    int provided;
    
    //
    // Check arguments...
    //
    if ( argc < 3 )
    {
      printf("Usage:\n");
      printf(" ./%s <path to dir> <servers ip to shrink separated by ';'>\n", argv[0]);
      printf("\n");
      return -1;
    }

    if (THREAD_WRITER == 1){
      MPI_Init(&argc, &argv);
    }else{
      MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    
    if (strchr(argv[2], delim[0])==NULL){
      servers[0] = argv[2];
      new_size = 1;
    }else{
      servers[0] = strtok(argv[2], delim);
      while (servers[new_size] != NULL) {
        new_size++;
        if (new_size >= XPN_METADATA_MAX_RECONSTURCTIONS){
          printf("Too much serv %s\n", argv[2]);
          exit(EXIT_FAILURE);
        }
        servers[new_size] = strtok(NULL, delim);
      }
    }

    new_size = size - new_size;
    pos_in_shrink = -1;

    for (int i = 0; i < XPN_METADATA_MAX_RECONSTURCTIONS; i++)
    {
      if (servers[i] == NULL) break;

      hostip = ns_get_host_ip();
      ns_get_hostname(hostname);
      if (strstr(servers[i], hostip) != NULL || strstr(servers[i], hostname) != NULL)
      {
        debug_info("Rank %d Server %d '%s'\n", rank, i, servers[i]);
        pos_in_shrink = i;
        break;
      }
    }

    if (new_size >= size){
      if (rank == 0){
        printf("Error: the new size '%d' is greatter or equal to the old size '%d'\n", new_size, size);
      }
      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    start_time = MPI_Wtime();
    if (rank == 0){
      printf("Shrink in path %s from %d servers to %d servers\n", argv[1], size, new_size);
    }
    xpn_path_len = strlen(argv[1]);
    list (argv[1]);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0){
      printf("Shrink elapsed time %f mseg\n", (MPI_Wtime() - start_time)*1000);
    }
    MPI_Finalize();

    return 0;
  }

/* ................................................................... */
