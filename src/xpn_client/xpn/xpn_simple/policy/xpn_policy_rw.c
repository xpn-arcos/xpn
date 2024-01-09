
  /*
   *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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
 * Calculates the server and the offset in that server of the given offset of a file.
 *
 * @param fd[in] A file descriptor.
 * @param offset[in] The original offset.
 * @param local_offset[out] The offset in the server.
 * @param serv[out] The server in which is located the given offset.
 *
 * @return Returns 0 on success or -1 on error.
 */
int XpnGetBlock(int fd, off_t offset, off_t *local_offset, int *serv)
{
	struct policy *p;

	switch(xpn_file_table[fd]->mdata->type_policy)
	{
		case POLICY_RAID0:
			// Calculate the server
			p = (struct policy *)xpn_file_table[fd]->mdata->policy;
			(*serv) = (((offset/xpn_file_table[fd]->block_size)
			% xpn_file_table[fd]->mdata->data_nserv)
			+ p->first_node) % xpn_file_table[fd]->mdata->data_nserv;

			// Calculate the offset in the above server
			(*local_offset) = (((offset/xpn_file_table[fd]->block_size)
				/ xpn_file_table[fd]->mdata->data_nserv)
				* xpn_file_table[fd]->block_size)
				+ (offset % xpn_file_table[fd]->block_size);
			break;

		case POLICY_RAID1:
			// The server is calculated like in RAID0
			p = (struct policy *)xpn_file_table[fd]->mdata->policy;
			(*serv) = (((offset/xpn_file_table[fd]->block_size)
			% xpn_file_table[fd]->mdata->data_nserv)
			+ p->first_node) % xpn_file_table[fd]->mdata->data_nserv;

			// The offset does not need any calculation
			(*local_offset) = offset;
			break;

		default:
			return -1;
	}

	//XPN_DEBUG("XpnGetBlock(%lld) -> local_offset = %lld, serv = %d", (long long)offset, (long long)(*local_offset), *serv)

	return 0;
}

/**
 * The blocks that have to be read/written from/to the servers are selected by round-robin: one block from each server. Using this policy the nfi module will perform one read/write operation for every single block on every server, which is not optimal.
 * This policy is valid for read/write operations on a RAID0 partition, and for read operations on a RAID1 partition.
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
void XpnRWBlocksPolicyBlockByBlock(int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers)
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
		XpnGetBlock(fd, new_offset, &l_offset, &l_serv); // l_offset and l_serv will differ depending on the policy

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
		count = l_size + count;
		new_offset = offset + count;
	}
}

/**
 * The blocks that have to be read from the servers are selected by round-robin (one block from each server), and then, grouped in one operation per server. Using this policy the nfi module will perform at most one read operation per server. This policy requires the execution of XpnReadBlocksPolicyRAID0_AllInOne_Finish when the data have arrived, which will reorder the blocks.
 * This policy is valid for read operations on a RAID0 partition.
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
void XpnReadBlocksPolicyRAID0_AllInOne(int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers)
{
	struct nfi_worker_io **io = *io_out;
	int *ion = *ion_out;
	int i, j;
	size_t count;

	XpnRWBlocksPolicyBlockByBlock(fd, buffer, size, offset, io_out, ion_out, num_servers);

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
 * This is the complementary operation to XpnRWBlocksPolicyRAID0_AllInOne. The blocks that have been read and grouped by XpnReadBlocksPolicyRAID0_AllInOne are now reordered before delivering them to the user.
 * This policy is valid for read operations on a RAID0 partition.
 *
 * @param fd[in] A file descriptor.
 * @param buffer[out] The ordered buffer to be delivered to the user. This buffer will contain the same blocks that new_buffer, but in the right order.
 * @param size[in] The original size.
 * @param offset[in] The original offset.
 * @param io_out[out] The operation matrix. io_out[i] (row 'i' in io_out) contains the required operations in server 'i'.
 * @param ion_out[out] The length of every row in io_out. ion_out[i] is the number of operations in server 'i' (io_out[i]).
 * @param num_servers[in] The number of servers.
 * @param new_buffer[in] The disorganized buffer that needs to be ordered.
 *
 */
void XpnReadBlocksPolicyRAID0_AllInOne_Finish(int fd, void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers, const void *new_buffer)
{
	struct nfi_worker_io **io = *io_out;
	int *ion = *ion_out;
	int i, j;
	size_t count;

	XpnRWBlocksPolicyBlockByBlock(fd, buffer, size, offset, io_out, ion_out, num_servers);

	count = 0;
	for (i = 0 ; i < num_servers ; i++) {
		for (j = 0 ; j < ion[i] ; j++) {
			memcpy(io[i][j].buffer, ((char *)new_buffer) + count, io[i][j].size);

			count += io[i][j].size;
		}
	}
}

/**
 * The blocks that have to be written to the servers are selected by round-robin (one block from each server), and then, grouped in one operation per server. Using this policy the nfi module will perform at most one write operation per server.
 * This policy is valid for write operations on a RAID0 partition.
 *
 * @param fd[in] A file descriptor.
 * @param buffer[in] The original buffer.
 * @param size[in] The original size.
 * @param offset[in] The original offset.
 * @param io_out[out] The operation matrix. io_out[i] (row 'i' in io_out) contains the required operations in server 'i'.
 * @param ion_out[out] The length of every row in io_out. ion_out[i] is the number of operations in server 'i' (io_out[i]).
 * @param num_servers[in] The number of servers.
 * @param new_buffer[in] The buffer used to send data (it will be modified to store the disorganized blocks before sending).
 *
 */
void XpnWriteBlocksPolicyRAID0_AllInOne(int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers, void *new_buffer)
{
	struct nfi_worker_io **io = *io_out;
	int *ion = *ion_out;
	int i, j;
	size_t count, server_count;

	XpnRWBlocksPolicyBlockByBlock(fd, buffer, size, offset, io_out, ion_out, num_servers);

	count = 0;
	server_count = 0;
	for (i = 0 ; i < num_servers ; i++) {
		if (ion[i] > 0) {
			for (j = 0 ; j < ion[i] ; j++) {
				memcpy(((char *)new_buffer) + count, io[i][j].buffer, io[i][j].size);

				count += io[i][j].size;

				if (j >= 1)
					io[i][0].size += io[i][j].size;
			}
			io[i][0].buffer = ((char *)new_buffer + server_count);
			ion[i] = 1;

			server_count += io[i][0].size;
		}
	}
}

/**
 * The blocks that have to be read from the servers are 1/n of the file from each server (being 'n' the number of servers).
 * This policy is only valid for read operations on a RAID1 partition.
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
void XpnReadBlocksPolicyRAID1_AllInOne(__attribute__((__unused__)) int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers)
{
	struct nfi_worker_io **io = *io_out;
	int *ion = *ion_out;
	int i;
	size_t l_size;

	l_size = size/num_servers;

	for (i = 0 ; i < num_servers ; i++) {
		io[i][0].offset = offset + i*l_size;
		io[i][0].size   = l_size;
		io[i][0].buffer = ((char *)buffer + i*l_size);

		ion[i] = 1; // One operation per server
	}

	// If the division 'size/num_servers' is not integer, add the remaining bytes to the last server
	if (size%num_servers > 0) {
		io[num_servers-1][0].size += size%num_servers;
	}
}

/**
 * The blocks that have to be read/written from/to the servers are every block from every server. Every block requires one operation.
 * This policy is suitable for write operations on a RAID1 partition.
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
void XpnWriteBlocksPolicyRAID1_BlockByBlock(int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers)
{
	struct nfi_worker_io **io = *io_out;
	int *ion = *ion_out;
	off_t new_offset, l_offset;
	int l_serv, i;
	size_t l_size, count;

	for (i = 0 ; i < num_servers ; i++)
		ion[i] = 0;

	new_offset = offset;
	count = 0;

	while(size>count){
		XpnGetBlock(fd, new_offset, &l_offset, &l_serv); // l_offset and l_serv will differ depending on the policy

		// l_size = Remaining bytes from new_offset until the end of the block
		l_size = xpn_file_table[fd]->block_size -
			(new_offset%xpn_file_table[fd]->block_size);

		if ((size - count) < l_size)
			l_size = size - count;

		for (i = 0 ; i < num_servers ; i++) {
			io[i][ion[i]].offset = l_offset;
			io[i][ion[i]].size   = l_size;
			io[i][ion[i]].buffer = ((char *)buffer + count);

			ion[i]++; // Increment the number of operations in server i
		}
		count += l_size;
		new_offset = offset + count;
	}
}

/**
 * The blocks that have to be written to the servers are every block from every server. All the blocks are written in one operation per server.
 * This policy is suitable for write operations on a RAID1 partition.
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
void XpnWriteBlocksPolicyRAID1_AllInOne(__attribute__((__unused__)) int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers)
{
	struct nfi_worker_io **io = *io_out;
	int *ion = *ion_out;
	int i;

	for (i = 0 ; i < num_servers ; i++) {
		io[i][0].offset = offset;
		io[i][0].size   = size;
		io[i][0].buffer = ((char *)buffer);

		ion[i] = 1; // One operation per server
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
 * @param io_out[out] The operation matrix.
 * @param ion_out[out] The length of every row in io_out.
 * @param num_servers[in] The number of servers.
 *
 * @return Returns a pointer to a new buffer on success, or NULL on error.
 */
void *XpnReadBlocks(int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers)
{
	int optimize = 1; // Optimize by default
      //int optimize = 0; // Do not optimize
	void *new_buffer ;

	new_buffer = (void *)buffer;
	switch(xpn_file_table[fd]->mdata->type_policy)
	{
		case POLICY_RAID0:
			if (optimize)
			{
				new_buffer = malloc(size);
				if (NULL == new_buffer) {
					perror("XpnReadBlocks: Error in malloc");
					return new_buffer ;
				}

				XpnReadBlocksPolicyRAID0_AllInOne(fd, (const void *)new_buffer, size, offset, io_out, ion_out, num_servers);

				// free(new_buffer) ??????
			}
			else {
				XpnRWBlocksPolicyBlockByBlock(fd, buffer, size, offset, io_out, ion_out, num_servers);
			}
			break;

		case POLICY_RAID1:
			if (optimize)
			{
				off_t l_offset;
				int l_serv;

				// First, do the assignment block by block
				XpnRWBlocksPolicyBlockByBlock(fd, buffer, size, offset, io_out, ion_out, num_servers);

				// Second, check if I need to do 2 or more operations in the first server
				XpnGetBlock(fd, offset, &l_offset, &l_serv); // l_offset and l_serv will differ depending on the policy
				if ((*ion_out)[l_serv] >= 2) {
					// If so, then apply the optimizaction
					XpnReadBlocksPolicyRAID1_AllInOne(fd, buffer, size, offset, io_out, ion_out, num_servers);
				}
			}
			else {
				XpnRWBlocksPolicyBlockByBlock(fd, buffer, size, offset, io_out, ion_out, num_servers);
			}
			break;

		default:
			return NULL;
	}

	return new_buffer;
}

int XpnReadBlocksFinish(int fd, void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers, void *new_buffer)
{
	switch(xpn_file_table[fd]->mdata->type_policy)
	{
		case POLICY_RAID0:
			if (buffer != new_buffer) {
				XpnReadBlocksPolicyRAID0_AllInOne_Finish(fd, buffer, size, offset, io_out, ion_out, num_servers, new_buffer);
				free(new_buffer);
			}
			break;

		case POLICY_RAID1:
			break;

		default:
			return -1;
	}

	return 0;
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
 * @return Returns 0 on success or -1 on error.
 */
void *XpnWriteBlocks ( int fd, const void *buffer, size_t size, off_t offset, struct nfi_worker_io ***io_out, int **ion_out, int num_servers)
{
	int optimize = 1; // Optimize by default
	//int optimize = 0; // Do not optimize
	void *new_buffer = (void *)buffer;

	switch(xpn_file_table[fd]->mdata->type_policy){
		case POLICY_RAID0:
			if (optimize) {
				new_buffer = malloc(size);
				if (new_buffer == NULL)
					perror("XpnWriteBlocks: Error in malloc");
				else
					XpnWriteBlocksPolicyRAID0_AllInOne(fd, buffer, size, offset, io_out, ion_out, num_servers, new_buffer);
			} else
				XpnRWBlocksPolicyBlockByBlock(fd, buffer, size, offset, io_out, ion_out, num_servers);
			break;

		case POLICY_RAID1:
			if (optimize)
				XpnWriteBlocksPolicyRAID1_AllInOne(fd, buffer, size, offset, io_out, ion_out, num_servers);
			else
				XpnWriteBlocksPolicyRAID1_BlockByBlock(fd, buffer, size, offset, io_out, ion_out, num_servers);
			break;

		default:
			return NULL;
	}

	return new_buffer;
}

int XpnWriteBlocksFinish ( int fd,
		           const void *buffer, 
			   __attribute__((__unused__)) size_t size, 
			   __attribute__((__unused__)) off_t offset, 
			   __attribute__((__unused__)) struct nfi_worker_io ***io_out, 
			   __attribute__((__unused__)) int **ion_out, 
			   __attribute__((__unused__)) int num_servers, 
			   void *new_buffer)
{
	switch(xpn_file_table[fd]->mdata->type_policy)
	{
		case POLICY_RAID0:
			if (buffer != new_buffer)
				free(new_buffer);
			break;

		case POLICY_RAID1:
			break;

		default:
			return -1;
	}

	return 0;
}

ssize_t XpnReadGetTotalBytes(int fd, ssize_t *res_v, int num_servers) 
{
	ssize_t res = -1;
	int i;

	switch(xpn_file_table[fd]->mdata->type_policy)
	{
		case POLICY_RAID0:
		case POLICY_RAID1:
			res = 0;
			for (i = 0 ; i < num_servers ; i++)
				res += res_v[i];
			break;

		default:
			return -1;
	}

	return res;
}

ssize_t XpnWriteGetTotalBytes(int fd, ssize_t *res_v, int num_servers) 
{
	ssize_t res = -1;
	int i;

	switch(xpn_file_table[fd]->mdata->type_policy)
	{
		case POLICY_RAID0:
			res = 0;
			for (i = 0 ; i < num_servers ; i++)
				res += res_v[i];
			break;

		case POLICY_RAID1:
			res = res_v[0];
			for (i = 1 ; i < num_servers ; i++)
				res = (res_v[i]<res)?res_v[i]:res;
			break;

		default:
			return -1;
	}

	return res;
}

