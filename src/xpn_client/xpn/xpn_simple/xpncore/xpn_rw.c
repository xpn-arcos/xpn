
  /*
   *  Copyright 2020-2022 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


#include "xpn/xpn_simple/xpn_rw.h"


#ifdef POSIX_THREADS
//extern pthread_mutex_t global_mt;
#endif

void XpnShowFileTable();


ssize_t xpn_read(int fd, void *buffer, size_t size)
{
	ssize_t res = -1;

	XPN_DEBUG_BEGIN_CUSTOM("%d, %zu", fd, size)

	if (xpn_file_table[fd] != NULL)	
	{
		if ((unsigned long)(size) > (unsigned long)(xpn_file_table[fd]->block_size))
		{
			res = xpn_pread(fd, buffer, size, xpn_file_table[fd]->offset);
		}
		else {
			res = xpn_sread(fd, buffer, size, xpn_file_table[fd]->offset);
		}
	}
	else {
		XpnShowFileTable();
		errno = EBADF;
	}

	XPN_DEBUG_END_CUSTOM("%d, %zu", fd, size)
	return res;
}

ssize_t xpn_write(int fd, const void *buffer, size_t size)
{
	ssize_t res = -1;
	
	XPN_DEBUG_BEGIN_CUSTOM("%d, %zu", fd, size)

	if (xpn_file_table[fd] != NULL)
	{
		if ((unsigned long)(size) >= (unsigned long)(xpn_file_table[fd]->block_size))
		{
			res = xpn_pwrite(fd, buffer, size, xpn_file_table[fd]->offset);
		}
		else {
			res = xpn_swrite(fd, buffer, size, xpn_file_table[fd]->offset);
		}
	}
	else {
		XpnShowFileTable();
		errno = EBADF;
	}

	XPN_DEBUG_END_CUSTOM("%d, %zu", fd, size)
	return res;
}

ssize_t xpn_sread(int fd, const void *buffer, size_t size, off_t offset)
{
	off_t new_offset, l_offset;
	size_t initial_size;
	size_t l_size;
	int l_serv;
	size_t count = 0;
	struct nfi_server **servers;
	int n, res;

	XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

	if((fd<0)||(fd>XPN_MAX_FILE)||(xpn_file_table[fd] == NULL)){
		XpnShowFileTable();
		errno = EBADF;
		res = -1;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	if(buffer == NULL){
		errno = EFAULT;
		res = -1;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	if(size == 0){
		res = 0;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	if(xpn_file_table[fd]->mode == O_WRONLY){
		res = -1;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	if(xpn_file_table[fd]->type == XPN_DIR){
		res = -1;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}

	/* params:
	 * flag operation, partition id, absolute path, file descriptor,
	 * pointer to servers */
	servers = NULL;
	n = XpnGetServers(op_xpn_read,
		   	          xpn_file_table[fd]->part->id,
			          NULL,
			          fd,
			          &servers,
			          XPN_DATA_SERVER);
	if(n<=0){
		if (servers != NULL){
			free(servers);
		}
		return -1;
	}

	initial_size = size;
	new_offset = offset;
	count = 0;

	while(size > count){
		XpnGetBlock(fd, new_offset, &l_offset, &l_serv);

		l_size = xpn_file_table[fd]->block_size - (new_offset%xpn_file_table[fd]->block_size);

		// If l_size > the remaining bytes to read/write, then adjust l_size
		if ((size - count) < l_size){
			l_size = size - count;
		}

		if (xpn_file_table[fd]->data_vfh->nfih[l_serv] == NULL)
		{
			res = XpnGetFh( xpn_file_table[fd]->mdata,
					&(xpn_file_table[fd]->data_vfh->nfih[l_serv]),
					servers[l_serv],
					xpn_file_table[fd]->path);

			if(res<0){
				break;
			}
		}
		
		res = servers[l_serv]->ops->nfi_read(servers[l_serv],
											  xpn_file_table[fd]->data_vfh->nfih[l_serv],
											  (char *)buffer + count,
											  l_offset,
											  l_size) ;
		if(res<0){
			break;
		}

		count = l_size + count;
		new_offset = offset + count;
	}

	free(servers);

	if (size == count)
		return initial_size;
	else
		return -1;
}

ssize_t xpn_pread(int fd, void *buffer, size_t size, off_t offset)
{
	ssize_t *res_v, total;
	int res, n, i, j, err;	
	struct nfi_server **servers;
	struct nfi_worker_io **io;
	int *ion, max;
	void *new_buffer;
	
	XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
	
	if((fd<0)||(fd>XPN_MAX_FILE)||(xpn_file_table[fd] == NULL)){
		XpnShowFileTable();
		errno = EBADF;
		res = -1;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	if(buffer == NULL){
		errno = EFAULT;
		res = -1;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	if(size == 0){
		res = 0;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	if(xpn_file_table[fd]->mode == O_WRONLY){
		res = -1;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	if(xpn_file_table[fd]->type == XPN_DIR){
		res = -1;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	/* params:
	 * flag operation, partition id, absolute path, file descriptor,
	 * pointer to servers */
	servers = NULL;
	n = XpnGetServers(op_xpn_read,
			  xpn_file_table[fd]->part->id,
			  NULL,
			  fd,
			  &servers,
			  XPN_DATA_SERVER);
	if(n<=0){
		if (servers != NULL)
			free(servers);

		res = -1;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	io = (struct nfi_worker_io **)malloc(sizeof(struct nfi_worker_io *)*n);
	if(io == NULL){
		free(servers);

		res = -1;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	ion = (int *)malloc(sizeof(int)*n);
	if(ion == NULL){
		free(io);
		free(servers);

		res = -1;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	bzero(ion, sizeof(int)*n);
	
	// calculate the maximum number of operations
	max = (size/xpn_file_table[fd]->block_size) + 1;
	if(size%xpn_file_table[fd]->block_size != 0){
		max++;
	}

	// create
	for(i=0; i<n; i++){
		io[i] = (struct nfi_worker_io *)malloc(sizeof(struct nfi_worker_io)*max);
		if(io[i] == NULL){
			free(servers);
			for(j=0;j<i;j++){
				free(io[j]);
			}
			free(io);
			free(ion);

			res = -1;
			XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
			return res;
		}
	}
	
	for(i=0; i<n; i++){
		io[i][0].offset = 0;
		io[i][0].size = 0;
	}

	// Calculate which blocks to read from each server
	new_buffer = XpnReadBlocks(fd, buffer, size, offset, &io, &ion, n);
	if(new_buffer == NULL){
		free(servers);
		for(i=0;i<n;i++){
			free(io[i]);
		}
		free(io);
		free(ion);
		return -1;
	}
	
#ifdef POSIX_THREADS
	// TODO: Expand is not reentrant. Using only one 'wrk' (struct nfi_worker) inside servers[i] (struct nfi_server) does not allow two nfi operations at the same time
	//pthread_mutex_lock(&(global_mt)); // It is better to do this in clients when necessary
#endif

	// operation
	for(j=0; j<n; j++){
		i = XpnGetMetadataPos(xpn_file_table[fd]->mdata, j);
		
		if(ion[i] != 0){
			res = XpnGetFh( xpn_file_table[fd]->mdata,
					&(xpn_file_table[fd]->data_vfh->nfih[i]),
					servers[i],
					xpn_file_table[fd]->path);
			if(res<0){
				free(servers);
				for(i=0;i<n;i++){
					free(io[i]);
				}
				free(io);
				free(ion);

				res = -1;
				XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
				return res;
			}
			
			// Default Value
			nfi_worker_thread(servers[i]->wrk, XpnGetThreads(op_xpn_read, xpn_file_table[fd]->part->id, size));	

			nfi_worker_do_read(servers[i]->wrk,
					xpn_file_table[fd]->data_vfh->nfih[i],
					io[i],
					ion[i]);
		}
	}
	
	res_v = (ssize_t *)malloc(sizeof(ssize_t)*n);
	if(res_v == NULL){
		free(servers);
		for(j=0;j<n;j++){
			free(io[j]);
		}
		free(io);
		free(ion);

		return -1;
	}
	bzero(res_v, sizeof(ssize_t)*n);

	err = 0;
	for(i=0; i<n; i++){
		res_v[i] = nfi_worker_wait(servers[i]->wrk);

		if(res_v[i] < 0)
			err = 1;
	}
	
#ifdef POSIX_THREADS
	//pthread_mutex_unlock(&(global_mt));
#endif

	XpnReadBlocksFinish(fd, buffer, size, offset, &io, &ion, n, new_buffer);

	total = -1;
	if (!err) {
		total = XpnReadGetTotalBytes(fd, res_v, n);
	
		if(total > 0)
			xpn_file_table[fd]->offset += total;
	}

	free(servers);
	for(j=0;j<n;j++){
		free(io[j]);
	}
	free(io);
	free(ion);
	
	res = total;
	XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
	return res;
}

ssize_t xpn_swrite(int fd, const void *buffer, size_t size, off_t offset)
{
	off_t new_offset, l_offset;
	size_t initial_size;
	size_t l_size;
	int l_serv;
	size_t count = 0;
	struct nfi_server **servers;
	int n, res;

	XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)

	if((fd<0)||(fd>XPN_MAX_FILE)){
		return -1;
	}
	
	if((xpn_file_table[fd] == NULL)||(buffer == NULL)){
		return -1;
	}
	
	if(size == 0){
		return 0;
	}
	
	if(xpn_file_table[fd]->mode == O_RDONLY){
		return -1;
	}
	
	if(xpn_file_table[fd]->type == XPN_DIR){
		return -1;
	}

	/* params:
	 * flag operation, partition id, absolute path, file descriptor,
	 * pointer to servers */
	servers = NULL;
	n = XpnGetServers(op_xpn_write,
		   	          xpn_file_table[fd]->part->id,
			          NULL,
			          fd,
			          &servers,
			          XPN_DATA_SERVER);
	if(n<=0){
		if (servers != NULL){
			free(servers);
		}
		return -1;
	}

	initial_size = size;
	new_offset = offset;
	count = 0;

	while(size > count){
		XpnGetBlock(fd, new_offset, &l_offset, &l_serv);
	
		l_size = xpn_file_table[fd]->block_size - (new_offset%xpn_file_table[fd]->block_size);

		// If l_size > the remaining bytes to read/write, then adjust l_size
		if ((size - count) < l_size){
			l_size = size - count;
		}

		if (xpn_file_table[fd]->data_vfh->nfih[l_serv] == NULL)
		{
			res = XpnGetFh( xpn_file_table[fd]->mdata,
					&(xpn_file_table[fd]->data_vfh->nfih[l_serv]),
					servers[l_serv],
					xpn_file_table[fd]->path);

			if(res<0){
				break;
			}
		}

		res = servers[l_serv]->ops->nfi_write(servers[l_serv],
											  xpn_file_table[fd]->data_vfh->nfih[l_serv],
											  (char *)buffer + count,
											  l_offset,
											  l_size) ;
		if(res<0){
			break;
		}

		count = l_size + count;
		new_offset = offset + count;
	}

	free(servers);

	if (size == count)
		return initial_size;
	else
		return -1;
}

ssize_t xpn_pwrite(int fd, const void *buffer, size_t size, off_t offset)
{
	ssize_t *res_v, total;
	int res, n, i, j, err;
	struct nfi_server **servers;
	struct nfi_worker_io **io;
	int *ion, max;
	void *new_buffer;
	
	XPN_DEBUG_BEGIN_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
	
	if((fd<0)||(fd>XPN_MAX_FILE)){
		return -1;
	}
	
	if((xpn_file_table[fd] == NULL)||(buffer == NULL)){
		return -1;
	}
	
	if(size == 0){
		return 0;
	}
	
	if(xpn_file_table[fd]->mode == O_RDONLY){
		return -1;
	}
	
	if(xpn_file_table[fd]->type == XPN_DIR){
		return -1;
	}

	/* params:
	 * flag operation, partition id, absolute path, file descriptor,
	 * pointer to servers */
	servers = NULL;
	n = XpnGetServers(op_xpn_write,
		   	  xpn_file_table[fd]->part->id,
			  NULL,
			  fd,
			  &servers,
			  XPN_DATA_SERVER);
	if(n<=0){
		if (servers != NULL)
			free(servers);

		res = -1;
		XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
		return res;
	}
	
	io = (struct nfi_worker_io **)malloc(sizeof(struct nfi_worker_io *)*n);
	if(io == NULL){
		free(servers);
		return -1;
	}
	
	ion = (int *)malloc(sizeof(int)*n);
	if(ion == NULL){
		free(io);
		free(servers);
		return -1;
	}
	bzero(ion, sizeof(int)*n);
	
	// calculate the maximum number of operations
	max = (size/xpn_file_table[fd]->block_size) + 1;
	if(size%xpn_file_table[fd]->block_size != 0){
		max++;
	}
	
	// create
	for(i=0; i<n; i++){
		io[i] = (struct nfi_worker_io *)malloc(sizeof(struct nfi_worker_io)*max);
		if(io[i] == NULL){
			free(servers);
			for(j=0;j<i;j++){
				free(io[j]);
			}
			free(io);
			free(ion);
			return -1;
		}
	}
	
	for(i=0; i<n; i++){
		io[i][0].offset = 0;
		io[i][0].size = 0;
	}
	
	// Calculate which blocks to write to each server
	new_buffer = XpnWriteBlocks(fd, buffer, size, offset, &io, &ion, n);
	if(new_buffer == NULL){
		free(servers);
		for(i=0;i<n;i++){
			free(io[i]);
		}
		free(io);
		free(ion);
		return -1;
	}

#ifdef POSIX_THREADS
	// TODO: Expand is not reentrant. Using only one 'wrk' (struct nfi_worker) inside servers[i] (struct nfi_server) does not allow two nfi operations at the same time
	//pthread_mutex_lock(&(global_mt)); // It is better to do this in clients when necessary
#endif

	for(j=0; j<n; j++){
		i = XpnGetMetadataPos(xpn_file_table[fd]->mdata, j);
		
		if(ion[i] != 0){
			res = XpnGetFh( xpn_file_table[fd]->mdata,
					&(xpn_file_table[fd]->data_vfh->nfih[i]),
					servers[i],
					xpn_file_table[fd]->path);
			
			if(res<0){
				free(servers);
				for(i=0;i<n;i++){
					free(io[i]);
				}
				free(io);
				free(ion);
				return -1;
			}
/*
			switch(xpn_file_table[fd]->size_threads){
				case -1:
					flag_thread = 0;
					break;
				case 0:
					flag_thread = 1;
					break;
				default:
					if(xpn_file_table[fd]->size_threads >= size){
						flag_thread = 1;
					}else{
						flag_thread = 0;
					}
			}
*/
			// Default Value
			nfi_worker_thread(servers[i]->wrk, XpnGetThreads(op_xpn_write ,xpn_file_table[fd]->part->id, size));
			
			nfi_worker_do_write(servers[i]->wrk,
					xpn_file_table[fd]->data_vfh->nfih[i],
					io[i],
					ion[i]);
		}
	}
	
	res_v = (ssize_t *)malloc(sizeof(ssize_t)*n);
	if(res_v == NULL){
		free(servers);
		for(j=0;j<n;j++){
			free(io[j]);
		}
		free(io);
		free(ion);

		return -1;
	}
	bzero(res_v, sizeof(ssize_t)*n);

	err = 0;
	for(i=0; i<n; i++){
		res_v[i] = nfi_worker_wait(servers[i]->wrk);

		if(res_v[i] < 0)
			err = 1;
	}

#ifdef POSIX_THREADS
	//pthread_mutex_unlock(&(global_mt));
#endif

	XpnWriteBlocksFinish(fd, buffer, size, offset, &io, &ion, n, new_buffer);

	total = -1;
	if (!err) {
		total = XpnWriteGetTotalBytes(fd, res_v, n);
	
		if(total > 0)
			xpn_file_table[fd]->offset += total;
	}
	
	free(servers);
	for(j=0;j<n;j++){
		free(io[j]);
	}
	free(io);
	free(ion);
	
	res = total;
	XPN_DEBUG_END_CUSTOM("%d, %zu, %lld", fd, size, (long long int)offset)
	return res;
}

off_t xpn_lseek(int fd, off_t offset, int flag)
{
	struct stat st;
	
	switch (flag) {
		case SEEK_SET:
			if(offset<0) {
				errno = EINVAL;
				return (off_t)-1;
			}
			else{
				xpn_file_table[fd]->offset = offset;
			}
			break;
		case SEEK_CUR:
			if(xpn_file_table[fd]->offset+offset<0) {
				errno = EINVAL;
				return (off_t)-1;
			}
			else{
				xpn_file_table[fd]->offset += offset;
			}
			break;
		case SEEK_END:
			if(xpn_fstat(fd, &st)<0) {
				errno = EBADF;
				return (off_t)-1;
			}
			if(st.st_size + offset<0) {
				errno = EINVAL;
				return (off_t)-1;
			}
			else{
				xpn_file_table[fd]->offset = st.st_size + offset;
			}
			break;
		default:
			errno = EINVAL;
			return (off_t)-1;
	}
	
	return xpn_file_table[fd]->offset;
}

