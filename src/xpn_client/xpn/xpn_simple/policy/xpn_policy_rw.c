
  /*
   *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Dario Muñoz Muñoz
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


#include "xpn/xpn_simple/xpn_policy_rw.h"

/**
 * Calculates the server and the offset (in server) of the given offset (origin file) of a file with replication.
 *
 * @param block_size[in] The block size of the file.
 * @param replication_level[in] The replication level of the file.
 * @param nserv[in] The number of servers.
 * @param offset[in] The original offset.
 * @param replication[in] The replication of actual offset.
 * @param local_offset[out] The offset in the server.
 * @param serv[out] The server in which is located the given offset.
 */
void XpnCalculateBlock(int block_size, int replication_level, int nserv, off_t offset, int replication, off_t *local_offset, int *serv)
{
	off_t block = offset / block_size;
	off_t block_replication = block * (replication_level + 1) + replication;
	off_t block_line = block_replication / nserv;
	
	// Calculate the server	
	(*serv) = (block_replication) % nserv;
	
	// Calculate the offset in the server
	(*local_offset) = block_line * block_size + (offset % block_size);
}

/**
 * Calculates the offset (in file) of the given offset (file in server) of a file with replication.
 *
 * @param block_size[in] The block size of the file.
 * @param replication_level[in] The replication level of the file.
 * @param nserv[in] The number of servers.
 * @param serv[in] The server in which is located the given offset.
 * @param local_offset[in] The offset in the server.
 * @param offset[out] The original offset.
 */
void XpnCalculateBlockInvert(int block_size, int replication_level, int nserv, int serv, off_t local_offset, off_t *offset, int *replication)
{
    off_t block_line = local_offset / block_size;
	off_t block_replication = block_line * nserv + serv;
	// round down
    off_t block = block_replication / (replication_level + 1);
	// Calculate the offset
	(*offset) = block * block_size + (local_offset % block_size);
	// Calculate the actual replication block
	(*replication) = block_replication % (replication_level + 1);
}

/**
 * Calculates the server and the offset (in server) for reads of the given offset (origin file) of a file with replication.
 *
 * @param fd[in] A file descriptor.
 * @param offset[in] The original offset.
 * @param serv_client[in] To optimize: the server where the client is.
 * @param replication[in] The replication of actual offset.
 * @param local_offset[out] The offset in the server.
 * @param serv[out] The server in which is located the given offset.
 *
 * @return Returns 0 on success or -1 on error.
 */
int XpnReadGetBlock(int fd, off_t offset, int serv_client, off_t *local_offset, int *serv)
{	
	int retries = 0;
	int replication = 0;
	if (serv_client != -1){
		do{
			XpnCalculateBlock(xpn_file_table[fd]->block_size, xpn_file_table[fd]->part->replication_level, xpn_file_table[fd]->part->data_nserv, offset, replication, local_offset, serv);
			if ((*serv) == serv_client && xpn_file_table[fd]->part->data_serv[(*serv)].error != -1 ){
				return 0;
			}
			replication++;
		}while(replication <= xpn_file_table[fd]->part->replication_level);
	}
	
	replication = 0;
	if (xpn_file_table[fd]->part->replication_level != 0)
		replication = rand() % (xpn_file_table[fd]->part->replication_level + 1);

	do{
		XpnCalculateBlock(xpn_file_table[fd]->block_size, xpn_file_table[fd]->part->replication_level, xpn_file_table[fd]->part->data_nserv, offset, replication, local_offset, serv);
		if (xpn_file_table[fd]->part->replication_level != 0)
			replication = (replication + 1) % (xpn_file_table[fd]->part->replication_level + 1);
		retries++;
	}while(xpn_file_table[fd]->part->data_serv[*serv].error == -1 && retries <= xpn_file_table[fd]->part->replication_level);
	
	return 0;
}

/**
 * Calculates the server and the offset (in server) for writes of the given offset (origin file) of a file with replication.
 *
 * @param fd[in] A file descriptor.
 * @param offset[in] The original offset.
 * @param replication[in] The replication of actual offset.
 * @param local_offset[out] The offset in the server.
 * @param serv[out] The server in which is located the given offset.
 *
 * @return Returns 0 on success or -1 on error.
 */
int XpnWriteGetBlock(int fd, off_t offset, int replication, off_t *local_offset, int *serv)
{
	XpnCalculateBlock(xpn_file_table[fd]->block_size, xpn_file_table[fd]->part->replication_level, xpn_file_table[fd]->part->data_nserv, offset, replication, local_offset, serv);
	return xpn_file_table[fd]->part->data_serv[*serv].error;
}

/**
 * The blocks that have to be read from the servers are selected by round-robin: one block from each server. Using this method the nfi module will perform one read operation for every single block on every server, which is not optimal.
 *
 * @param fd[in] A file descriptor.
 * @param buffer[in] The original buffer.
 * @param size[in] The original size.
 * @param offset[in] The original offset.
 * @param serv_client[in] To optimize: the server where the client is.
 * @param io_out[out] The operation matrix. io_out[i] (row 'i' in io_out) contains the required operations in server 'i'.
 * @param ion_out[out] The length of every row in io_out. ion_out[i] is the number of operations in server 'i' (io_out[i]).
 * @param num_servers[in] The number of servers.
 *
 */
void XpnReadBlocksBlockByBlock(int fd, const void *buffer, size_t size, off_t offset, int serv_client, struct nfi_worker_io ***io_out, int **ion_out, int num_servers)
{
	struct nfi_worker_io **io = *io_out;
	int *ion = *ion_out;
	off_t new_offset, l_offset;
	int l_serv, i;
	size_t l_size, count;

	for (i = 0 ; i < num_servers ; i++) {
		ion[i] = 0;
	}

	new_offset = offset;
	count = 0;

	while(size>count)
	{
		XpnReadGetBlock(fd, new_offset, serv_client, &l_offset, &l_serv);

		// l_size is the remaining bytes from new_offset until the end of the block
		l_size = xpn_file_table[fd]->block_size -
			(new_offset%xpn_file_table[fd]->block_size);

		// If l_size > the remaining bytes to read/write, then adjust l_size
		if ((size - count) < l_size)
			l_size = size - count;

		io[l_serv][ion[l_serv]].offset = l_offset;
		io[l_serv][ion[l_serv]].size   = l_size;
		io[l_serv][ion[l_serv]].buffer = ((char *)buffer + count);

		ion[l_serv]++; // Increment the number of operations in server 'l_serv'
		XPN_DEBUG("l_serv = %d, l_offset = %lld, l_size = %lld, ion[l_serv] = %d", l_serv, (long long)l_offset, (long long)l_size, ion[l_serv]);
		
		count = l_size + count;
		new_offset = offset + count;
	}
}

/**
 * The blocks that need to be written to the servers are selected in round-robin: one block from each server. With this method, the nfi module will perform one write operation per replication level for each block (blocks * replication_level) on each server, which is not optimal.
 *
 * @param fd[in] A file descriptor.
 * @param buffer[in] The original buffer.
 * @param size[in] The original size.
 * @param offset[in] The original offset.
 * @param io_out[out] The operation matrix. io_out[i] (row 'i' in io_out) contains the required operations in server 'i'.
 * @param ion_out[out] The length of every row in io_out. ion_out[i] is the number of operations in server 'i' (io_out[i]).
 * @param num_servers[in] The number of servers.
 *
 */
void XpnWriteBlocksBlockByBlock(int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers)
{
	struct nfi_worker_io **io = *io_out;
	int *ion = *ion_out;
	off_t new_offset, l_offset;
	int l_serv, i;
	size_t l_size = 0, count;

	for (i = 0 ; i < num_servers ; i++) {
		ion[i] = 0;
	}

	new_offset = offset;
	count = 0;

	while(size>count)
	{
		
		for (int j = 0; j < xpn_file_table[fd]->part->replication_level + 1; j++)
		{
			XpnWriteGetBlock(fd, new_offset, j, &l_offset, &l_serv);
			// l_size is the remaining bytes from new_offset until the end of the block
			l_size = xpn_file_table[fd]->block_size -
				(new_offset%xpn_file_table[fd]->block_size);

			// If l_size > the remaining bytes to read/write, then adjust l_size
			if ((size - count) < l_size)
				l_size = size - count;

			io[l_serv][ion[l_serv]].offset = l_offset;
			io[l_serv][ion[l_serv]].size   = l_size;
			io[l_serv][ion[l_serv]].buffer = ((char *)buffer + count);

			ion[l_serv]++; // Increment the number of operations in server 'l_serv'
			XPN_DEBUG("l_serv = %d, l_offset = %lld, l_size = %lld, ion[l_serv] = %d", l_serv, (long long)l_offset, (long long)l_size, ion[l_serv]);
		}
		count = l_size + count;
		new_offset = offset + count;
	}
}

/**
 * The blocks that have to be read from the servers are selected by round-robin (one block from each server), and then, grouped in one operation per server. Using this method the nfi module will perform at most one read operation per server.
 *
 * @param fd[in] A file descriptor.
 * @param buffer[in] The original buffer.
 * @param size[in] The original size.
 * @param offset[in] The original offset.
 * @param serv_client[in] To optimize: the server where the client is.
 * @param io_out[out] The operation matrix. io_out[i] (row 'i' in io_out) contains the required operations in server 'i'.
 * @param ion_out[out] The length of every row in io_out. ion_out[i] is the number of operations in server 'i' (io_out[i]).
 * @param num_servers[in] The number of servers.
 *
 */
void XpnReadBlocksAllInOne(int fd, void *buffer, size_t size, off_t offset, int serv_client, struct nfi_worker_io ***io_out, int **ion_out, int num_servers)
{
	struct nfi_worker_io **io = *io_out;
	int *ion = *ion_out;
	int i, j;
	size_t count;

	XpnReadBlocksBlockByBlock(fd, buffer, size, offset, serv_client, io_out, ion_out, num_servers);

	count = 0;
	for (i = 0 ; i < num_servers ; i++)
	{
		if (ion[i] > 0) {
			for (j = 1 ; j < ion[i] ; j++) {
				io[i][0].size += io[i][j].size;
			}
			io[i][0].buffer = ((char *)buffer + count);
			ion[i] = 1;

			count += io[i][0].size;
		}
	}
}

/**
 * The blocks that have to be written to the servers are selected by round-robin (one block from each server), and then, grouped in one operation per server. Using this policy the nfi module will perform at most one write operation per server.
 *
 * @param fd[in] A file descriptor.
 * @param buffer[in] The original buffer.
 * @param size[in] The original size.
 * @param offset[in] The original offset.
 * @param io_out[out] The operation matrix. io_out[i] (row 'i' in io_out) contains the required operations in server 'i'.
 * @param ion_out[out] The length of every row in io_out. ion_out[i] is the number of operations in server 'i' (io_out[i]).
 * @param num_servers[in] The number of servers.
 * @param send_buffer[in] The buffer used to send data (it will be modified to store the disorganized blocks before sending).
 *
 */
void XpnWriteBlocksAllInOne(int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers, void *send_buffer)
{
	struct nfi_worker_io **io = *io_out;
	int *ion = *ion_out;
	int i, j;
	size_t count, server_count;

	XpnWriteBlocksBlockByBlock(fd, buffer, size, offset, io_out, ion_out, num_servers);

	count = 0;
	server_count = 0;
	for (i = 0 ; i < num_servers ; i++) {
		if (ion[i] > 0) {
			for (j = 0 ; j < ion[i] ; j++) {
				memcpy(((char *)send_buffer) + count, io[i][j].buffer, io[i][j].size);

				count += io[i][j].size;

				if (j >= 1)
					io[i][0].size += io[i][j].size;
			}
			io[i][0].buffer = ((char *)send_buffer + server_count);
			ion[i] = 1;

			server_count += io[i][0].size;
		}
	}
}

/**
 * Calculates how the blocks have to be read from the servers. io_out is an operation matrix. io_out[i] (row 'i' in io_out)
 * contains the required operations in server 'i'. While ion_out[i] is the number of operations in server 'i' (io_out[i]).
 *
 * @param fd[in] A file descriptor.
 * @param buffer[in] The original buffer.
 * @param size[in] The original size.
 * @param offset[in] The original offset.
 * @param serv_client[in] To optimize: the server where the client is.
 * @param io_out[out] The operation matrix.
 * @param ion_out[out] The length of every row in io_out.
 * @param num_servers[in] The number of servers.
 *
 * @return Returns the a pointer to buffer on success, or NULL on error.
 */
void *XpnReadBlocks(int fd, const void *buffer, size_t size, off_t offset, int serv_client, struct nfi_worker_io ***io_out, int **ion_out, int num_servers)
{	
	int optimize = 1; // Optimize by default
	if (xpn_file_table[fd]->part->replication_level > 0){
    	optimize = 0; // Do not optimize
	}

	void *new_buffer = (void *)buffer;
	new_buffer = malloc(size * sizeof(char));
	if (new_buffer == NULL){
		XPN_DEBUG("Error in malloc");
		perror("XpnReadBlocks: Error in malloc");
		return new_buffer;
	}

	if (optimize)
		XpnReadBlocksAllInOne(fd, new_buffer, size, offset, serv_client, io_out, ion_out, num_servers);
	else 
		XpnReadBlocksBlockByBlock(fd, buffer, size, offset, serv_client, io_out, ion_out, num_servers);
	return new_buffer;
}

/**
 * This is the complementary operation to XpnReadBlocks. The blocks that have been read and grouped by XpnReadBlocksAllInOne are now reordered before delivering them to the user.
 *
 * @param fd[in] A file descriptor.
 * @param buffer[out] The ordered buffer to be delivered to the user. This buffer will contain the same blocks that new_buffer, but in the right order.
 * @param size[in] The original size.
 * @param offset[in] The original offset.
 * @param serv_client[in] To optimize: the server where the client is.
 * @param io_out[out] The operation matrix. io_out[i] (row 'i' in io_out) contains the required operations in server 'i'.
 * @param ion_out[out] The length of every row in io_out. ion_out[i] is the number of operations in server 'i' (io_out[i]).
 * @param num_servers[in] The number of servers.
 * @param new_buffer[in] The disorganized buffer that needs to be ordered.
 *
 */
void XpnReadBlocksFinish(int fd, void *buffer, size_t size, off_t offset, int serv_client, struct nfi_worker_io ***io_out, int **ion_out, int num_servers, const void *new_buffer)
{
	struct nfi_worker_io **io = *io_out;
	int *ion = *ion_out;
	int i, j;
	size_t count;

	int optimize = 1; // Optimize by default
	if (xpn_file_table[fd]->part->replication_level > 0){
    	optimize = 0; // Do not optimize
	}

	if (optimize){
		XpnReadBlocksBlockByBlock(fd, buffer, size, offset, serv_client, io_out, ion_out, num_servers);

		count = 0;
		for (i = 0 ; i < num_servers ; i++) {
			for (j = 0 ; j < ion[i] ; j++) {
				memcpy(io[i][j].buffer, ((char *)new_buffer) + count, io[i][j].size);

				count += io[i][j].size;
			}
		}
	}
}

/**
 * Calculates how the blocks have to be written to the servers. io_out is an operation matrix. io_out[i] (row 'i' in io_out)
 * contains the required operations in server 'i'. While ion_out[i] is the number of operations in server 'i' (io_out[i]).
 *
 * @param fd[in] A file descriptor.
 * @param buffer[in] The original buffer.
 * @param size[in] The original size.
 * @param offset[in] The original offset.
 * @param io_out[out] The operation matrix.
 * @param ion_out[out] The length of every row in io_out.
 * @param num_servers[in] The number of servers.
 *
 * @return Returns the a pointer to buffer on success, or NULL on error.
 */
void *XpnWriteBlocks ( int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers)
{
	int optimize = 1; // Optimize by default
	// if (xpn_file_table[fd]->part->replication_level > 0){
    // 	optimize = 0; // Do not optimize
	// }

	void *new_buffer = (void *)buffer;
	new_buffer = malloc(size * (xpn_file_table[fd]->part->replication_level + 1));
	if (new_buffer == NULL){
		XPN_DEBUG("Error in malloc");
		perror("XpnWriteBlocks: Error in malloc");
		return new_buffer;
	}

	if (optimize) 
		XpnWriteBlocksAllInOne(fd, buffer, size, offset, io_out, ion_out, num_servers, new_buffer);
	else
		XpnWriteBlocksBlockByBlock(fd, buffer, size, offset, io_out, ion_out, num_servers);
	return new_buffer;
}


/**
 * Calculates the total bytes the servers read.
 *
 * @param res_v[in] The response array of the servers.
 * @param num_servers[in] The number of servers.
 *
 * @return Returns total bytes read/write.
 */
ssize_t XpnReadGetTotalBytes(ssize_t *res_v, int num_servers) 
{
	ssize_t res = -1;
	int i;

	res = 0;
	for (i = 0 ; i < num_servers ; i++){
		res += res_v[i];
		XPN_DEBUG("res_v[%d] = %ld",i,res_v[i]);
	}
	return res;
}

/**
 * Calculates the total bytes the servers read/write.
 *
 * @param res_v[in] The response array of the servers.
 * @param num_servers[in] The number of servers.
 *
 * @return Returns total bytes read/write.
 */
ssize_t XpnWriteGetTotalBytes(ssize_t *res_v, int num_servers, struct nfi_worker_io ***io, int *ion, struct nfi_server *servers) 
{
	ssize_t res = -1;
	int i;

	int total_send = 0;
	int total_write = 0;

	res = 0;
	for (i = 0 ; i < num_servers ; i++){
		res += res_v[i];

		for (int j = 0; j < ion[i]; j++)
		{
			if (servers[i].error != -1){
				total_send += (*io)[i][j].size;
			}
			total_write += (*io)[i][j].size;
		}
	}
	XPN_DEBUG("res = %ld total_send = %d total_write = %d", res, total_send, total_write);
	if (res == total_send)
		return total_write;

	return res;
}

/**
 * Calculates the real size in bytes of a file.
 *
 * @param part[in] The part of the file.
 * @param attr[in] The response array of the servers.
 * @param n_serv[in] The number of servers.
 *
 * @return Returns real size in bytes.
 */
ssize_t XpnGetRealFileSize(struct xpn_partition *part, struct nfi_attr *attr, int n_serv)
{
	int i = 0;
	int serv_to_calc = 0;
	size_t total = 0;
	// Check if have incomplete blocks
	int have_incompete_blocks = 0;
	for(i=0;i<n_serv;i++){
		total += attr[i].at_size;
		if (attr[i].at_size != 0 &&
		(attr[i].at_size - XPN_HEADER_SIZE) % part->block_size != 0){
			have_incompete_blocks = 1;
			serv_to_calc = i;
			break;
		}
	}
	if (total == 0){
		return 0;
	}
	// Get serv with the last block
	for(i=0;i<n_serv;i++){
		if (have_incompete_blocks){
			if (attr[i].at_size != 0 &&
				(attr[i].at_size - XPN_HEADER_SIZE) % part->block_size != 0 && 
				attr[i].at_size <= attr[serv_to_calc].at_size){
					serv_to_calc = i;
				}
		}else{
			if (attr[i].at_size >= attr[serv_to_calc].at_size)
				serv_to_calc = i;
		}
	}

	off_t offset = attr[serv_to_calc].at_size - XPN_HEADER_SIZE;
	int replication;
	if (offset > 0){
		// The fix -1 and then +1 is because in the size of blocksize it calculate the next block, and with the -1 calculate the correct block that then with +1 is corrected
		XpnCalculateBlockInvert(part->block_size, part->replication_level, part->data_nserv, serv_to_calc, attr[serv_to_calc].at_size - XPN_HEADER_SIZE - 1, &offset, &replication);
		offset += 1;
	}else{
		offset = 0;
	}
	return offset;
}