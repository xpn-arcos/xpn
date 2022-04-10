//#include <xpn.h>

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <dlfcn.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
   #include <dirent.h>
   #include <strings.h>
   #include <string.h>
   #include <pthread.h>
   #include <netinet/in.h>
   #include <unistd.h>
   #include <sys/time.h>
   #include <rpc/rpc.h>
   #include <rpc/clnt.h>
   #include <rpc/types.h>
        #include <fcntl.h>
#include <stdio.h>


void foo() {
	printf("Outside FUSE: sizeof(ino_t)=%d\n", sizeof(ino_t));
	printf("Outside FUSE: sizeof(off_t)=%d\n", sizeof(off_t));
}

int main(int argc, char**argv) {
	DIR *dir;
	struct dirent *dirent;
	
	dir = opendir("/home/bor/pruebas/expand1/");
	while (dirent = readdir(dir)) {
		printf("%s\n", dirent->d_name);
	}
	
	return 0;
}

