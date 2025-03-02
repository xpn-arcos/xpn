
  /*
   *  Copyright 2000-2025 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Dario Muñoz Muñoz
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
#include <sys/types.h>
#include <sys/stat.h>

char *bufferWrite;
char *bufferRead ;
char *bufferRead2;

double get_time(void)
{
    struct timeval tp;
    struct timezone tzp;

    gettimeofday(&tp,&tzp);
    return((double) tp.tv_sec + .000001 * (double) tp.tv_usec);
}

int cmpBuffers(const char *buffer1, const char *buffer2, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (buffer1[i] != buffer2[i]) {
			printf("Diferent [%ld] 1 %d 2 %d\n",i, buffer1[i],buffer2[i]);
            return 1; 
        }
    }
    return 0; 
}

void freeBuffers(){
	free(bufferWrite);
	free(bufferRead);
	free(bufferRead2);
}


void print_stat(int fd){
	struct stat sb;
	xpn_fstat(fd, &sb);
	printf("File type:                ");

	switch (sb.st_mode & S_IFMT) {
		case S_IFBLK:  printf("block device\n");            break;
		case S_IFCHR:  printf("character device\n");        break;
		case S_IFDIR:  printf("directory\n");               break;
		case S_IFIFO:  printf("FIFO/pipe\n");               break;
		case S_IFLNK:  printf("symlink\n");                 break;
		case S_IFREG:  printf("regular file\n");            break;
		case S_IFSOCK: printf("socket\n");                  break;
		default:       printf("unknown?\n");                break;
		}

	printf("I-node number:            %ld\n", (long) sb.st_ino);

	printf("Mode:                     %lo (octal)\n",
				(unsigned long) sb.st_mode);

	printf("Link count:               %ld\n", (long) sb.st_nlink);
	printf("Ownership:                UID=%ld   GID=%ld\n",
			(long) sb.st_uid, (long) sb.st_gid);

	printf("Preferred I/O block size: %ld bytes\n",
			(long) sb.st_blksize);
	printf("File size:                %lld bytes\n",
			(long long) sb.st_size);
	printf("Blocks allocated:         %lld\n",
			(long long) sb.st_blocks);

	printf("Last status change:       %s", ctime(&sb.st_ctime));
	printf("Last file access:         %s", ctime(&sb.st_atime));
	printf("Last file modification:   %s", ctime(&sb.st_mtime));

}

int main ( int argc, char *argv[] )
{
	int    ret, fd1;
	double t_bc, t_ac, t_bw, t_aw ;
	char random_file[PATH_MAX];
	sprintf(random_file,"/tmp/randomfile%d.txt",rand());

    if (argc < 3)
	{
	    printf("\n") ;
	    printf(" Usage: %s <full path> <megabytes to write-read>\n", argv[0]) ;
	    printf("\n") ;
	    printf(" Example:") ;
	    printf(" env XPN_CONF=./xpn.conf  %s /P1/test_1 2\n", argv[0]);
	    printf("\n") ;
	    return -1 ;
	}	

	// xpn-init
	ret = xpn_init();
	printf("%d = xpn_init()\n", ret);
	if (ret < 0) {
	    return -1;
	}

    double mb_file = atof(argv[2]);
    double buff_size_mb = atof(argv[3]);
    size_t buff_size = buff_size_mb*MB;

	bufferWrite = malloc(buff_size*sizeof(char)) ;
	bufferRead  = malloc(buff_size*sizeof(char)) ;
	bufferRead2 = malloc(buff_size*sizeof(char)) ;

	long file_size = mb_file*MB;
	printf("MB to test: %f bytes %ld\n", mb_file, file_size);
	printf("File to save data and check xpn: %s", random_file);

	// xpn-creat
	t_bc = get_time();

	fd1 = xpn_creat(argv[1], 00777);
	if (fd1 < 0) {
	    printf("%d = xpn_creat('%s', %o)\n", ret, argv[1], 00777) ;
	    return -1 ;
	}

	print_stat(fd1);
	
	t_bw = get_time();

	// xpn-write
	size_t bytesWritten = 0;
    int fd_random = open(random_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	while (bytesWritten < file_size) {
        size_t bytesToWrite = (file_size - bytesWritten < buff_size) ? (file_size - bytesWritten) : buff_size;
		for (int i = 0; i < buff_size; i++)
		{
			bufferWrite[i] = rand() % 1000;
		}

        ssize_t result = xpn_write(fd1, bufferWrite, bytesToWrite);
        ssize_t result2 = write(fd_random, bufferWrite, bytesToWrite);

        if (result == -1 || result2 == -1 || result != result2) {
            printf("Error when writing xpn_write: %lld write: %lld\n",(long long)result, (long long)result2);
            break;
        }

        bytesWritten += result;
    }

	t_aw = get_time() - t_bw;

	print_stat(fd1);
    close(fd_random);
	ret = xpn_close(fd1);

	t_ac = get_time() - t_bc;

	printf("Bytes (KiB); Total time (ms); Write time (ms)\n") ;
	printf("%f;%f;%f\n", (double)file_size / ((double)KB), t_ac * 1000, t_aw * 1000) ;

	
	// printf("xpn_mark_error_server 1\n") ;
	// ret = xpn_mark_error_server(1);


    // xpn-open
	t_bc = get_time();

	fd1 = xpn_open(argv[1], O_RDWR);
	if (fd1 < 0) {
	    printf("%d = xpn_open('%s', %o)\n", ret, argv[1], 00777) ;
	    freeBuffers();
		return -1 ;
	}
	
	print_stat(fd1);

	t_bw = get_time();


	// xpn-read
	size_t bytesReadTotal = 0;
    int is_the_same = 1;
	ssize_t bytesRead, bytesRead2;
	fd_random = open(random_file, O_RDWR);
    while ((bytesRead = xpn_read(fd1, bufferRead, buff_size)) > 0) {
		bytesRead2 = read(fd_random, bufferRead2, buff_size);
		// printf("bytesRead = %lld bytesRead2 = %lld\n", (long long)bytesRead, (long long)bytesRead2);
		if (bytesRead != bytesRead2){
			printf("Error when reading xpn_read: %lld read: %lld\n",(long long)bytesRead, (long long)bytesRead2);
		}
		if (strncmp(bufferRead,bufferRead2,bytesRead)==0){
			// printf("The buffers are the same strncmp\n");
		}else{
			printf("The buffers are different strncmp\n"); 
			is_the_same = 0; 
		}
		if (memcmp(bufferRead,bufferRead2,bytesRead)==0){
			// printf("The buffers are the same memcmp\n");
		}else{
			printf("The buffers are different memcmp\n"); 
			is_the_same = 0; 
		}
		if (cmpBuffers(bufferRead,bufferRead2,bytesRead)==0){
			// printf("The buffers are the same cmpBuffers\n");
		}else{
			printf("The buffers are different cmpBuffers\n"); 
			is_the_same = 0; 
		}

		memset(bufferRead,1,buff_size);
		memset(bufferRead2,2,buff_size);
        bytesReadTotal += bytesRead;
    }

	if (is_the_same) {
        printf("The files contain the same data.\n");
    } else {
        printf("File contents do not match.\n");
    }


    close(fd_random);

	t_aw = get_time() - t_bw;
	
	struct stat sb;
	xpn_fstat(fd1, &sb);
	printf("File size: %lld bytes, real size: %ld bytes\n", (long long) sb.st_size, file_size);
	struct stat sb2;
	xpn_stat(argv[1], &sb2);
	printf("File size: %lld bytes, real size: %ld bytes\n", (long long) sb2.st_size, file_size);

	ret = xpn_close(fd1);

	t_ac = get_time() - t_bc;

	printf("Bytes; Total time (ms); Read time (ms)\n") ;
	printf("%f;%f;%f\n", (double)file_size, t_ac * 1000, t_aw * 1000) ;

	
	// xpn-destroy
	ret = xpn_destroy();
	printf("%d = xpn_destroy()\n", ret);
	if (ret < 0) {
		freeBuffers();
	    return -1;
	}
	
	if (is_the_same == 0){
		freeBuffers();
		return 1;
	}
	freeBuffers();
	return 0;
}

