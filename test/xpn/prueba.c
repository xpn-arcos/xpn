#include "expand.h"

#define MAXIM (1024*1024)

int main()
{
	int ret,fd1,fd2,fd3;
	char buffer[MAXIM];
	
	setenv("XPN_CONF", "./", 1);
	printf("Antes ..... init \n");
	ret = xpn_init();
	if(ret<0){
		printf("Casco ..... init \n");
		return -1;
	}

	fd1 = xpn_unlink("/PNFS/jj");
	printf("xpn_unlink jj = %d\n",fd1);
	fd1 = xpn_unlink("/PNFS/jj1");
	printf("xpn_unlink jj = %d\n",fd1);
	
	fd1 = xpn_creat("/PNFS/jj",00777);
	printf("xpn_creat jj = %d\n",fd1);
	
	fd2 = xpn_creat("/PNFS/jj",0777);
	printf("xpn_creat jj = %d\n",fd2);
	
	fd3 = xpn_creat("/PNFS/jj1",0777);
	printf("xpn_creat jj1= %d\n",fd3);
	
	ret = xpn_write(fd2, buffer,MAXIM);
		
	printf("WR1 = %d\n",ret);

	printf("empiezan los closes\n");	
	xpn_close(fd1);	
	xpn_close(fd2);	
	xpn_close(fd3);	
	
	printf("des. closes\n");
	
	fd1 = xpn_open("/PNFS/jj",O_RDWR);
	printf("xpn_open jj = %d\n",fd1);

	xpn_lseek(fd1,MAXIM,0);
	ret = xpn_write(fd1, buffer,MAXIM);
	printf("WR2 = %d\n",ret);

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
	
	ret = xpn_destroy();
	return 0;
}
