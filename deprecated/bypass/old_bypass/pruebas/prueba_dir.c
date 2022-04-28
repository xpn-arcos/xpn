/*
 * Hace pruebas con directorios de expand
 */

#include "xpn.h"
#include "expand.h"

int main(int argc, char *argv[]) {

	struct stat s;
	//char *xpn_adaptor_partition_prefix = "/PNFS/";

	printf("hola\n");

	if (argc == 2){
		xpn_init();
		printf("\nProbando que estructura devuelve xpn_stat...%d\n",xpn_stat(argv[1],&s));
		
	}
	
	printf("\nadios\n");

	return(0);
}
