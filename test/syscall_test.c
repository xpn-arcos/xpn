//#include "xpn.h" //added
//include "expand.h"

//#include "/expand/bypass/xpn_bypass.h"


#include <stdlib.h> //added
#include <fcntl.h> //added
#include <sys/unistd.h>
#include <stdio.h>
#include <sys/stat.h>

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
	
	fd1 = creat("xpn:///xpn1/jj.txt",00777);
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


	struct stat stat_buf;
	printf("%d\n", stat("xpn:///xpn1/jj.txt", &stat_buf));
}
