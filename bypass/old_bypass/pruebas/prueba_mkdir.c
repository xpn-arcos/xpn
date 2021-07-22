/*
 * Hace una prueba consistente en crear directorios en expand
 */

#include "xpn.h"
#include "expand.h"

int main(int argc, char *argv[]) {

	char *xpn_adaptor_partition_prefix = "/PNFS/";

	printf("hola\n");

	if (argc == 2){
		if(!strncmp(xpn_adaptor_partition_prefix,argv[1],strlen(xpn_adaptor_partition_prefix))){
			xpn_init();
			printf("\nProbando que devuelve xpn_mkdir...%d\n", xpn_mkdir(argv[1], 0777));
		} else {
			printf("\nProbando que devuelve mkdir...%d\n", mkdir(argv[1], 0777));
		}
	}
	
	printf("\nadios\n");

	return(0);
}
