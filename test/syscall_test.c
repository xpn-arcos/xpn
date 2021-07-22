//#include "xpn.h" //added
//include "expand.h"

//#include "/expand/bypass/xpn_bypass.h"


#include <stdlib.h> //added
#include <fcntl.h> //added
#include <sys/unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

#define MAXIM (1024*1024)

int main()
{
	int ret,fd1,fd2;
	char buffer[MAXIM];
 
  	//setenv("XPN_CONF", "/work/xpn.conf.xml", 1);
	

	/*fd1 = unlink("/xpn1/jj.txt");
	printf("xpn_unlink jj = %d\n",fd1);
	fd1 = unlink("/xpn1/jj1.txt");
	printf("xpn_unlink jj = %d\n",fd1);*/
	
	fd1 = creat("xpn:///xpn1/jj.txt",00747);
	printf("xpn_creat jj = %d\n",fd1);
	
	fd2 = creat("xpn:///xpn1/jj1.txt",0777);
	printf("xpn_creat jj1= %d\n",fd2);
	
	ret = write(fd1, buffer,MAXIM);	
	printf("WR1 = %d\n",ret);

	printf("empiezan los closes\n");
	close(fd1);	
	close(fd2);	
	
	printf("des. closes\n");
	
	fd1 = open("xpn:///xpn1/jj.txt",O_RDWR);
	printf("xpn_open jj = %d\n",fd1);

	lseek(fd1,MAXIM,0);
	ret = write(fd1, buffer,MAXIM);
	printf("WR3 = %d\n",ret);

	printf("***********LECTURAS ******\n");
	lseek(fd1,0,0);
	ret = read(fd1, buffer,MAXIM);
	printf("RD1 = %d\n",ret);
	
	lseek(fd1,MAXIM+MAXIM,0);
	ret = read(fd1, buffer, MAXIM);
	printf("RD2 = %d\n",ret);
	
	printf("En close\n");
	close(fd1);


	struct stat file_stats;
	stat("xpn:///xpn1/jj.txt", &file_stats);

	printf(" device: %lld\n",                       file_stats.st_dev);
	printf(" inode: %ld\n",                         file_stats.st_ino);
	printf(" protection: %o\n",                     file_stats.st_mode);
	printf(" number of hard links: %d\n",           file_stats.st_nlink);
	printf(" user ID of owner: %d\n",               file_stats.st_uid);
	printf(" group ID of owner: %d\n",              file_stats.st_gid);
	printf(" device type (if inode device): %lld\n",file_stats.st_rdev);
	printf(" total size, in bytes: %ld\n",          file_stats.st_size);
	printf(" blocksize for filesystem I/O: %ld\n",  file_stats.st_blksize);
	printf(" number of blocks allocated: %ld\n",    file_stats.st_blocks);
	printf(" time of last access: %ld : %s",        file_stats.st_atime, ctime(&file_stats.st_atime));
	printf(" time of last modification: %ld : %s",  file_stats.st_mtime, ctime(&file_stats.st_mtime));
	printf(" time of last change: %ld : %s",        file_stats.st_ctime, ctime(&file_stats.st_ctime));
}
