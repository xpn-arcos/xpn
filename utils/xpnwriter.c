
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>

#include "xpn.h"

#define BAR_LENGTH 50

#ifndef KB
	#define KB	(1024)
#endif

#ifndef MB
	#define MB	(KB*KB)
#endif

char *program_name;

void usage() {
	printf("Usage: %s [-h] | [-b <buffer_size_in_KB>] <dest>\n", program_name);
}

int main(int argc, char *argv[])
{
	char *dest;
	int ret, fdd;
	ssize_t nr;
	size_t buffer_size = 64*KB;
	char *buffer;
	int isxpn = 0, xpndest = 0;
	const char *xpnprefix = "xpn://";
	int c;
	
	program_name = argv[0];
	
	opterr = 0;
	
	while ((c = getopt (argc, argv, "hb:")) != -1)
		switch (c) {
			case 'h':
				usage();
				return 0;
			case 'b':
				buffer_size = strtol(optarg, NULL, 10)*KB;
				break;
			case '?':
				if (optopt == 'b')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				usage();
				return 1;
			default:
				abort ();
		}
	
	dest=argv[optind];
	
	if (dest == NULL) {
		fprintf(stderr, "ERROR: Incorrect number of argumets.\n");
		usage();
		return 1;
	}

	buffer = (char *)malloc(buffer_size);

	if (strncmp(dest, xpnprefix, strlen(xpnprefix)) == 0) {
		dest = dest + strlen(xpnprefix);
		xpndest = 1;
	}
	
	isxpn = xpndest;
#ifdef DEBUG
	printf("xpndest=%d, isxpn=%d\n", xpndest, isxpn);
	
	printf("dest = '%s'\n", dest);

	printf("buffer_size = %d\n", buffer_size);
#endif
	if (isxpn) {
		ret = xpn_init();
		if (ret<0) {
			printf("Error %d while initializing expand\n", ret);
			exit(-1);
		}
	}
	
	if (xpndest)
		fdd = xpn_open(dest, O_CREAT|O_WRONLY|O_TRUNC, 00644);
	else
		fdd = open(dest, O_CREAT|O_WRONLY|O_TRUNC, 00644);
	
	if(fdd < 0) {
		printf("Error opening dest: fd = %d\n",fdd);
		exit(-1);
	}
	
	do {
		nr = fread(buffer, sizeof(char), buffer_size, stdin);
		
#ifdef DEBUG
		printf("Escribiendo un bloque de %d bytes\n", nr);
#endif
		if (xpndest)
			xpn_write(fdd, buffer, nr);
		else
			write(fdd, buffer, nr);
#ifdef DEBUG
		printf("Escrito un bloque de %d bytes\n", nr);
#endif
	} while (nr>0);
	
	if (xpndest)
		xpn_close(fdd);
	else
		close(fdd);
	
	if (isxpn)
		xpn_destroy();
	
	free(buffer);
	
	exit(0);
}

