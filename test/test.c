#include "xpn.h" //added
//include "expand.h"


#include <stdlib.h> //added
#include <fcntl.h> //added

#define MAXIM (1024*1024)

int main()
{
	int ret,fd1,fd2;
	char buffer[MAXIM];
	
	setenv("XPN_CONF", "/work/xpn.conf.xml", 1);
	printf("Antes ..... init \n");
	ret = xpn_init();
	if(ret<0){
		printf("Casco ..... init \n");
		return -1;
	}


	printf("Despues ..... init \n");
	fd1 = xpn_unlink("/xpn1/jj.txt");
	printf("xpn_unlink jj = %d\n",fd1);
	fd1 = xpn_unlink("/xpn1/jj1.txt");
	printf("xpn_unlink jj = %d\n",fd1);
	
	fd1 = xpn_creat("/xpn1/jj.txt",00777);
	printf("xpn_creat jj = %d\n",fd1);
	
	fd2 = xpn_creat("/xpn1/jj1.txt",0777);
	printf("xpn_creat jj1= %d\n",fd2);
	
	ret = xpn_write(fd1, buffer,MAXIM);	
	printf("WR1 = %d\n",ret);

	printf("empiezan los closes\n");
	xpn_close(fd1);	
	xpn_close(fd2);	
	
	printf("des. closes\n");
	
	fd1 = xpn_open("/xpn1/jj.txt",O_RDWR);
	printf("xpn_open jj = %d\n",fd1);

	xpn_lseek(fd1,MAXIM,0);
	ret = xpn_write(fd1, buffer,MAXIM);
	printf("WR3 = %d\n",ret);

	printf("***********LECTURAS ******\n");
	xpn_lseek(fd1,0,0);
	ret = xpn_read(fd1, buffer,MAXIM);
	printf("RD1 = %d\n",ret);
	
	xpn_lseek(fd1,MAXIM+MAXIM,0);
	ret = xpn_read(fd1, buffer, MAXIM);
	printf("RD2 = %d\n",ret);
	
	printf("En close\n");
	xpn_close(fd1);
	
	printf("antes de destroy\n");
	
	//ret = xpn_destroy();
	printf("Destroy = %d\n",ret);
	return 0;
}
