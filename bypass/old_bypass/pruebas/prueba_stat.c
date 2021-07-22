/*
 * Hace un stat si el fichero es local y un xpn_stat si el fichero es de expand
 */

#include <xpn.h>
#include <expand.h>

int main(int argc, char *argv[]) {

	struct stat s;
	char *xpn_adaptor_partition_prefix = "/PNFS/";

	printf("hola\n");

	if (argc == 2){
		if(!strncmp(xpn_adaptor_partition_prefix,argv[1],strlen(xpn_adaptor_partition_prefix))){
			xpn_init();
			printf("\nProbando que estructura devuelve xpn_stat...%d\n",xpn_stat(argv[1],&s));
		} else {
			printf("\nProbando que estructura devuelve stat...%d\n",stat(argv[1],&s));
		}
		
	        printf("dev_t         st_dev     (device)                        = %d\n",(int)s.st_dev);  // unsigned long int __dev_t;
	        printf("ino_t         st_ino     (inode)                         = %d\n",(int)s.st_ino);  // unsigned long int __ino_t;
	        printf("mode_t        st_mode    (protection)                    = %hx\n",    s.st_mode); // unsigned long int __mode_t;
	        printf("nlink_t       st_nlink   (number of hard links)          = %d\n",     s.st_nlink);// unsigned long int __nlink_t;
	        printf("uid_t         st_uid     (user ID of owner)              = %d\n",(int)s.st_uid);  // long int __uid_t;
	        printf("gid_t         st_gid     (group ID of owner)             = %d\n",(int)s.st_gid);  // long int __gid_t;
	        printf("dev_t         st_rdev    (device type (if inode device)) = %d\n",(int)s.st_rdev); // unsigned long int __dev_t;
	        printf("off_t         st_size    (total size, in bytes)          = %d\n",(int)s.st_size); // long int __off_t;
        	printf("unsigned long st_blksize (blocksize for filesystem I/O)  = %d\n",(int)s.st_blksize);
	        printf("unsigned long st_blocks  (number of blocks allocated)    = %d\n",(int)s.st_blocks); 
	        printf("time_t        st_atime   (time of last access)           = %d\n",(int)s.st_atime);// long int __time_t;
	        printf("time_t        st_mtime   (time of last modification)     = %d\n",(int)s.st_mtime);// long int __time_t;
	        printf("time_t        st_ctime   (time of last change)           = %d\n",(int)s.st_ctime);// long int __time_t;
	}
	
	printf("\nadios\n");

	return(0);
}
