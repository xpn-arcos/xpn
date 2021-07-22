
#ifdef DEBUG_PRUEBA_LIB_XPN
	#ifdef DEBUG_PRUEBA_XPN
		#undef DEBUG_PRUEBA_XPN
	#endif
#endif


#ifdef DEBUG_PRUEBA_XPN
	#include <xpn.h>
#else
	#include "xpn_intercept-limpia.h"
#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
	//#include <bits/stat.h>
#endif

//#include <string.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <errno.h>


//extern int errno;


int main(int argc, char *argv[]) {

	//const char *pepe2 = "$HOME/intercept/pepe";
	
	#ifdef DEBUG_PRUEBA_LIB_XPN
		const char *pepe = "/xpn/pepe.txt";
	#else
		const char *pepe = "pepe.txt";
	#endif
	
	//const char *pepe = "/xpn/pepe.txt";
        const char *cadena = "Probando que expand funciona\n\0";
	char *cadenaret;
	//char *jare;
	//struct stat64 *buf64;
	struct stat64 s64;
	//struct stat *buf;
	struct stat s;
	int fildes;
	//int fildes2;
	//struct flock *lock;

	printf("hola\n");

	printf("*pepe = %s\n",pepe);
	
	#ifdef DEBUG_PRUEBA_XPN
		xpn_init();
	#endif

	cadenaret=(char *) malloc(50);

	printf("\nProbando creat...");
	#ifdef DEBUG_PRUEBA_XPN
		printf("%d\n", fildes=xpn_creat((char *)pepe,S_IRWXU) );
		perror("xpn_creat");
	#else
		printf("%d\n", fildes=creat(pepe,S_IRWXU) );
		perror("creat");
	#endif
	
	printf("\nProbando close...");
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

	printf("\nProbando stat64...");
	#ifdef DEBUG_PRUEBA_XPN
		printf("%d\n",xpn_stat(pepe,&s));
		perror("xpn_stat");
	#else
		printf("%d\n",stat64(pepe,&s64));
		perror("stat64");
	#endif

	printf("\nProbando open...");
	#ifdef DEBUG_PRUEBA_XPN
		printf("%d\n",fildes=xpn_open((char *)pepe,O_RDWR));
		perror("xpn_open");
	#else
		printf("%d\n",fildes=open(pepe,O_RDWR));
		perror("open");
	#endif

	printf("\nProbando fstat...");
	#ifdef DEBUG_PRUEBA_XPN
		printf("%d\n",xpn_fstat(fildes,&s));
		perror("xpn_fstat");
	#else
		printf("%d\n",fstat(fildes,&s));
		perror("fstat");
	#endif

	printf("\nProbando write...se han escrito");
	#ifdef DEBUG_PRUEBA_XPN
		printf(" %d caracteres\n",xpn_write(fildes,(char *)cadena,30));
		perror("xpn_write");
	#else
		printf(" %d caracteres\n",write(fildes,cadena,30));
		perror("write");
	#endif

	printf("\nProbando lseek...ponemos el puntero a");
	#ifdef DEBUG_PRUEBA_XPN
		printf(" %ld\n",xpn_lseek(fildes,0,SEEK_SET));
		perror("xpn_lseek");
	#else
		printf(" %ld\n",lseek(fildes,0,SEEK_SET));
		perror("lseek");
	#endif

	printf("\nProbando read...se han leido");
	#ifdef DEBUG_PRUEBA_XPN
		printf(" %d caracrteres\n",xpn_read(fildes,cadenaret,30));
		perror("xpn_read");
	#else
		printf(" %d caracteres\n",read(fildes,cadenaret,30));
		perror("read");
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
		perror("close");
	#endif

	//printf("%d\n",unlink(pepe));

	printf("\nadios\n");

	return(1234);
}
