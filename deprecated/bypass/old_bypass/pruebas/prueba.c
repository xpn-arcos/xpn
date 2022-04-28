/*
 * Si esta definido DEBUG_PRUEBA_XPN se genera un ejecutable que prueba expand.
 * Si esta definido DEBUG_PRUEBA_LIB_XPN se genera un ejecutable que prueba
 * expand usando la biblioteca de interceptacion.
 * En otro caso se genera un ejecutable que prueba las llamadas al sistema.
 *
 */

#ifdef DEBUG_PRUEBA_LIB_XPN
#ifdef DEBUG_PRUEBA_XPN
#undef DEBUG_PRUEBA_XPN
#endif
#endif


#ifdef DEBUG_PRUEBA_XPN
#include <expand.h>
//#include <xpn.h>
#include "xpn.h"
#else
#include "xpn_intercept-limpia.h"
//#include "xpn_struct_stat64.h"
#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
#endif

//#include <string.h>
//#include <sys/types.h>
#define __USE_LARGEFILE64 1
#include <sys/stat.h>
//#include <fcntl.h>
//#include <errno.h>


//extern int errno;


int main(int argc, char *argv[]) {

#ifdef DEBUG_PRUEBA_LIB_XPN
	const char *pepe = "/xpn/pepe.txt";
	char *pepe2 = "/xpn/pepe.txt";
#else
	const char *pepe = "pepe.txt";
	char *pepe2 = "pepe.txt";
#endif
        const char *cadena = "Probando que expand funciona\n\0";
	char *cadenaret;
	struct stat s;
	struct stat64 *s64;
	//struct stat64 s64_prueba;
	int fildes;
#ifndef DEBUG_PRUEBA_XPN
	struct stat s2;
	struct stat64 s642;
	int fd1=0;
	FILE *fd2;
	int ficher=0;
	int i;
	char *ld_preload;
	char linea[13][256];
	//char *linea[13];
#endif

	printf("hola\n");

	s64=(struct stat64 *)malloc(sizeof(s));
	//s64=(struct stat64 *)malloc(sizeof(s64_prueba));
	printf("*pepe = %s\n",pepe);
	cadenaret=(char *) malloc(50);

#ifndef DEBUG_PRUEBA_XPN
	//s2=(struct stat *)malloc(sizeof(struct stat));
	//s642=(struct stat64 *)malloc(sizeof(struct stat64));
#endif

#ifdef DEBUG_PRUEBA_XPN
	xpn_init();
#endif

#ifndef DEBUG_PRUEBA_XPN
	ld_preload=(char *)malloc(256);
	ld_preload=(char *)getenv("LD_PRELOAD");
	printf("%s\n",ld_preload);
	printf("Probando que estructura devuelve stat...%d\n",stat("/export/home/pato11-1/users/bbergua/httpd-2.0.48",&s2));
	//ficher=open("/export/home/pato11-1/users/bbergua/httpd-2.0.48-dl/conf/httpd.conf",O_RDONLY);
	//printf("Probando que estructura devuelve fstat...%d\n",fstat(ficher,&s2));
	//close(ficher);
	if (ld_preload == NULL){
		fd1=creat("prueba_stat_normal.bin", 00644);
		//printf("descriptor fd1: %d\n",fd1);
	} else{
		fd1=creat("prueba_stat_intercept.bin", 00644);
	}
	//printf("descriptor fd1: %d\n",fd1);
	write(fd1,&s2,sizeof(struct stat64));
	close(fd1);
	if (ld_preload == NULL){
		fd2=fopen("prueba_stat_normal.txt", "w");
		//printf("descriptor fd2: %d\n",fd2);
	} else{
		fd2=fopen("prueba_stat_intercept.txt", "w");
	}
	//printf("descriptor fd2: %d\n",fd2);
        fprintf(fd2, "dev_t         st_dev     (device)                        = %d\n",s2.st_dev);// unsigned long int __dev_t;
        fprintf(fd2, "ino_t         st_ino     (inode)                         = %d\n",s2.st_ino);// unsigned long int __ino_t;
        fprintf(fd2, "mode_t        st_mode    (protection)                    = %hx\n",s2.st_mode);// unsigned long int __mode_t;
        fprintf(fd2, "nlink_t       st_nlink   (number of hard links)          = %d\n",s2.st_nlink);// unsigned long int __nlink_t;
        fprintf(fd2, "uid_t         st_uid     (user ID of owner)              = %d\n",(int)s2.st_uid);// long int __uid_t;
        fprintf(fd2, "gid_t         st_gid     (group ID of owner)             = %d\n",(int)s2.st_gid);// long int __gid_t;
        fprintf(fd2, "dev_t         st_rdev    (device type (if inode device)) = %d\n",s2.st_rdev);// unsigned long int __dev_t;
        fprintf(fd2, "off_t         st_size    (total size, in bytes)          = %d\n",(int)s2.st_size);// long int __off_t;
        fprintf(fd2, "unsigned long st_blksize (blocksize for filesystem I/O)  = %d\n",s2.st_blksize);
        fprintf(fd2, "unsigned long st_blocks  (number of blocks allocated)    = %d\n",s2.st_blocks); 
        fprintf(fd2, "time_t        st_atime   (time of last access)           = %d\n",(int)s2.st_atime);// long int __time_t;
        fprintf(fd2, "time_t        st_mtime   (time of last modification)     = %d\n",(int)s2.st_mtime);// long int __time_t;
        fprintf(fd2, "time_t        st_ctime   (time of last change)           = %d\n",(int)s2.st_ctime);// long int __time_t;
	fclose(fd2);
#endif

	printf("\nProbando creat...");
#ifdef DEBUG_PRUEBA_XPN
	printf("%d\n", fildes=xpn_creat(pepe2,S_IRWXU) );
	perror("xpn_creat");
#else
	printf("%d\n", fildes=creat((const char *)pepe2,S_IRWXU) );
	perror("creat");
#endif

	printf("\nProbando close con fildes=%d...",fildes);
#ifdef DEBUG_PRUEBA_XPN
	printf("%d\n",xpn_close(fildes));
	perror("xpn_close");
#else
	printf("%d\n",close(fildes));
	perror("close");
#endif

	printf("\nProbando stat...");
#ifdef DEBUG_PRUEBA_XPN
	printf("%d\n",xpn_stat((char *)pepe,&s));
	perror("xpn_stat");
#else
	printf("%d\n",stat(pepe,&s));
	perror("stat");
#endif

	printf("\nProbando lstat...");
#ifdef DEBUG_PRUEBA_XPN
	printf("%d\n",xpn_stat((char *)pepe,&s));
	perror("xpn_stat");
#else
	printf("%d\n",lstat(pepe,&s));
	perror("lstat");
#endif

	printf("\nProbando stat64...");
#ifdef DEBUG_PRUEBA_XPN
	printf("%d\n",xpn_stat((char *)pepe,&s));
	perror("xpn_stat");
#else
	printf("stat64->%d\n",stat64(pepe,s64));
	//perror("stat64");
#endif

	printf("\nProbando lstat64...");
#ifdef DEBUG_PRUEBA_XPN
	printf("%d\n",xpn_stat((char *)pepe,&s));
	perror("xpn_stat");
#else
	printf("lstat64->%d\n",lstat64(pepe,s64));
	//perror("lstat64");
#endif

	printf("\nProbando open...");
#ifdef DEBUG_PRUEBA_XPN
	printf("%d\n",fildes=xpn_open((char *)pepe,O_RDWR));
	perror("xpn_open");
#else
	printf("%d\n",fildes=open(pepe,O_RDWR));
	//perror("open");
#endif

	printf("\nProbando fstat...");
#ifdef DEBUG_PRUEBA_XPN
	printf("%d\n",xpn_fstat(fildes,&s));
	perror("xpn_fstat");
#else
	printf("fstat->%d\n",fstat(fildes,&s));
	//perror("fstat");
#endif

	printf("\nProbando fstat64...");
#ifdef DEBUG_PRUEBA_XPN
	printf("%d\n",xpn_fstat(fildes,&s));
	perror("xpn_fstat");
#else
	printf("fstat64->%d\n",fstat64(fildes,s64));
	//perror("fstat64");
#endif

	printf("\nProbando write...se han escrito");
#ifdef DEBUG_PRUEBA_XPN
	printf(" %d caracteres\n",xpn_write(fildes,(char *)cadena,30));
	perror("xpn_write");
#else
	printf(" %d caracteres\n",write(fildes,cadena,30));
	//perror("write");
#endif

	printf("\nProbando lseek...ponemos el puntero a");
#ifdef DEBUG_PRUEBA_XPN
	printf(" %ld\n",xpn_lseek(fildes,0,SEEK_SET));
	perror("xpn_lseek");
#else
	printf(" %ld\n",lseek(fildes,0,SEEK_SET));
	//perror("lseek");
#endif

	printf("\nProbando read...se han leido");
#ifdef DEBUG_PRUEBA_XPN
	printf(" %d caracrteres\n",xpn_read(fildes,cadenaret,30));
	perror("xpn_read");
#else
	printf(" %d caracteres\n",read(fildes,cadenaret,30));
	//perror("read");
#endif
	printf("\nLa cadena leida es...%s",cadenaret);

	//printf("%d\n",lstat64("pepin",&s));
	//printf("%d\n",lstat("pepin",&s2));
	//printf("%d\n",close(fildes));
	//fildes2 = open(pepe,O_CREAT,S_IRWXU);
	//printf("Aqui va un fstat64...%d\n",fstat64(fildes2,&s));
	//printf("Aqui va un fstat...%d\n",fstat(fildes2,&s2));
	//printf("%d\n",fcntl(fildes2,F_GETFD));
	//printf("%d\n",fcntl(fildes2,F_DUPFD,35));
	//&printf("%d\n",fcntl(fildes2,F_GETLK,lock));

	printf("\nProbando close...");
#ifdef DEBUG_PRUEBA_XPN
	printf("%d\n",xpn_close(fildes));
	perror("xpn_close");
#else
	printf("%d\n",close(fildes));
	//perror("close");
#endif

	//printf("%d\n",unlink(pepe));

	printf("\nadios\n");

	return(0);
	
}
