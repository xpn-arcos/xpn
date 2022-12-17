
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
struct stat st;
ssize_t sum;

void usage()
{
	printf("Usage: %s [-h] | [-b <buffer_size_in_KB>] <source>\n", program_name);
}

int main(int argc, char *argv[])
{
	char *source;
	int ret, fds;
	ssize_t nr;
	size_t buffer_size = 0;
	char *buffer;
	int isxpn = 0, xpnsource = 0;
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
	printf("buffer_size = %lu\n", (unsigned long)buffer_size);
#endif

	if (isxpn) {
		ret = xpn_init();
		if (ret<0) {
			printf("Error %d while initializing expand\n", ret);
			exit(-1);
		}
	}

	if (xpnsource)
		xpn_stat(source, &st);
	else
		stat(source, &st);
#ifdef DEBUG
	printf("st.st_size = %d\n", (int)st.st_size);
#endif

	if (buffer_size <= 0) {
		if (st.st_size <= 0)
			buffer_size = 256*KB;
		else
			buffer_size = st.st_size;
	}
#ifdef DEBUG
	printf("buffer_size = %d\n", (int)buffer_size);
#endif
	buffer = (char *)malloc(buffer_size);

	if (xpnsource)
		fds = xpn_open(source, O_RDONLY);
	else
		fds = open(source, O_RDONLY);

	if(fds < 0) {
		printf("Error opening source: fd = %d\n",fds);
		exit(-1);
	}

	sum = 0;
	do {
		if (st.st_size-sum < (ssize_t)buffer_size)
			buffer_size = st.st_size-sum;
#ifdef DEBUG
		printf("Leyendo un bloque de %lu bytes\n", (unsigned long)buffer_size);
#endif

		if (xpnsource)
			nr = xpn_read(fds, buffer, buffer_size);
		else
			nr = read(fds, buffer, buffer_size);
#ifdef DEBUG
		printf("Leido un bloque de %ld bytes\n", nr);
#endif
		fwrite(buffer, sizeof(char), nr, stdout);

		sum = sum + nr;
	} while (nr>0);

	if (xpnsource)
		xpn_close(fds);
	else
		close(fds);

	if (isxpn)
		xpn_destroy();

	free(buffer);

	exit(0);
}

