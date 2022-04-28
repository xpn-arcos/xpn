#include "xpn_intercept-limpia.h"
int main(int argc, char *argv[]) {
	int fildes;
#ifdef DEBUG_PRUEBA_LIB_XPN
	char *pepe1 = "/xpn/pepe.txt";
        char *pepe2 = "/xpn/pepe2.txt";
#else
	char *pepe1 = "pepe.txt";
        char *pepe2 = "pepe2.txt";
#endif
	const char *cadena1 = "Probando que expand funciona 1\n\0";
	const char *cadena2 = "Probando que expand funciona 2\n\0";
	
	if (fork() == 0){
		printf("Soy %d: mi LD_PRELOAD es %s\n", getpid(), getenv("LD_PRELOAD"));
		printf("Soy %d: creat->%d\n", getpid(), fildes=creat(pepe1,S_IRWXU));
		printf("Soy %d: close->%d\n", getpid(), close(fildes));
		printf("Soy %d: open-->%d\n", getpid(), fildes=open((char *)pepe1,O_RDWR));
		printf("Soy %d: write->%d\n", getpid(), write(fildes,(char *)cadena1,32));
		printf("Soy %d: close->%d\n", getpid(), close(fildes));
		printf("Soy %d: close->%d\n", getpid(), close(fildes));
	} else {
		printf("Soy %d: mi LD_PRELOAD es %s\n", getpid(), getenv("LD_PRELOAD"));
		printf("Soy %d: creat->%d\n", getpid(), fildes=creat(pepe2,S_IRWXU));
		printf("Soy %d: close->%d\n", getpid(), close(fildes));
		printf("Soy %d: open-->%d\n", getpid(), fildes=open((char *)pepe2,O_RDWR));
		printf("Soy %d: write->%d\n", getpid(), write(fildes,(char *)cadena2,32));
		printf("Soy %d: close->%d\n", getpid(), close(fildes));
	}
	exit(0);
}
