/*
 * Hace una prueba con directorios de expand
 */

#include "xpn.h"
#include "expand.h"

int main(int argc, char *argv[]) {

	//char *path;
	DIR *dirp;
	struct dirent *direntry;
	
	printf("hola\n");
	
	if (argc == 2){
		xpn_init();
		//vsprintf(path, "/export/temp/pato11/", argv[1]);
		//dirp = (DIR *)malloc(sizeof(DIR));
		dirp = xpn_opendir(argv[1]);
		//printf("\n");
		//printf("peta 1 antes\n");
		//printf("dirp = %p\n", dirp);
		//printf("dirp->fd = %d\n", dirp->fd);
		//printf("peta 1 despues\n");
		//direntry = (struct dirent *)malloc(sizeof(struct dirent));
		//printf("antes de direntry\n");
		//do{
			//direntry = xpn_readdir(dirp);
			//if(direntry != NULL)
				//printf("direntry->d_name = %s type = %d\n",direntry->d_name,direntry->d_type);
			//else				
				//printf("cerrado.....\n");
		//}while(direntry != NULL);
		//xpn_closedir(dirp);
		//printf("peta 2 antes\n");
		//printf("El directorio es %u\n", (int)direntry);
		//printf("El directorio tiene tipo %u\n", direntry->d_type);
		//printf("peta 2 despues\n");
		//printf("Probando que devuelve S_ISDIR...%d\n", S_ISDIR(direntry->d_type));
	}
	
	printf("adios\n");

	return(0);
}
