#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>

#include "xpn.h"

char *program_name;

void usage() {
	printf("Usage: %s [-h] <file>\n", program_name);
}

int main(int argc, char *argv[]) {
	char *source;
	int ret;
	int isxpn = 0, xpnsource = 0;
	const char *xpnprefix = "xpn://";
	int c;

	program_name = argv[0];
	
	opterr = 0;
	
	while ((c = getopt (argc, argv, "h")) != -1)
		switch (c) {
			case 'h':
				usage();
				return 0;
			case '?':
				if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				usage();
				return 1;
			default:
				abort ();
		}
	
	source=argv[optind];
	
	if (source == NULL) {
		fprintf(stderr, "ERROR: Incorrect number of argumets.\n");
		usage();
		return 1;
	}

	if (strncmp(source, xpnprefix, strlen(xpnprefix)) == 0) {
		source = source + strlen(xpnprefix);
		xpnsource = 1;
	}
	
	isxpn = xpnsource;
#ifdef DEBUG
	printf("xpnsource=%d, isxpn=%d\n", xpnsource, isxpn);
	
	printf("source = '%s'\n", source);
#endif
	if (isxpn) {
		ret = xpn_init();
		if (ret<0) {
			printf("Error %d while initializing expand\n", ret);
			exit(-1);
		}
	}
	
	if (xpnsource)
		xpn_unlink(source);
	else
		unlink(source);
	
	if (isxpn) {
		xpn_destroy();
	}

	exit(0);
}

