
  /*
   *  Copyright 2000-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
   *
   *  This file is part of Expand.
   *
   *  Expand is free software: you can redistribute it and/or modify
   *  it under the terms of the GNU Lesser General Public License as published by
   *  the Free Software Foundation, either version 3 of the License, or
   *  (at your option) any later version.
   *
   *  Expand is distributed in the hope that it will be useful,
   *  but WITHOUT ANY WARRANTY; without even the implied warranty of
   *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   *  GNU Lesser General Public License for more details.
   *
   *  You should have received a copy of the GNU Lesser General Public License
   *  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
   *
   */


#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "all_system.h"
#include "xpn.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE_BUILD 1024 * 1024 + 1
#define BUFFER_SIZE 1024 * 1024
void print_limit(char *string, size_t num)
{
  char c = string[num];
  string[num] = 0;
  printf("%s",string);
  string[num] = c;
}

int main(int argc, char **argv)
{
	int   file;
	char  buffer[BUFFER_SIZE_BUILD];
	int   read_size;
	if (argc < 2)
	{
		fprintf(stderr, "Error: usage: ./cat filename\n");
		return (-1);
	}
	xpn_init();
	file = xpn_open(argv[1], O_RDONLY);
	if (file == -1)
	{
		fprintf(stderr, "Error: %s: file not found\n", argv[1]);
		return (-1);
	}
	while ((read_size = xpn_read(file, buffer, BUFFER_SIZE)) > 0)
		// fprintf(stdout, "read_size:%d\n%s",read_size, buffer);
		print_limit(buffer, read_size);

	xpn_close(file);
	xpn_destroy();
  	return (0);
}


// #define BAR_LENGTH 50


// struct stat st;
// ssize_t     sum;

// void usage ( char *program_name )
// {
// 	printf("Usage: %s [-h] | [-b <buffer_size_in_KB>] <source>\n", program_name);
// }

// int main(int argc, char *argv[])
// {
// 	char *source;
// 	int ret, fds;
// 	ssize_t nr;
// 	size_t buffer_size = 0;
// 	char *buffer;
// 	int isxpn = 0, xpnsource = 0;
// 	const char *xpnprefix ;
// 	int c;

// 	xpnprefix = "xpn://" ;
// 	opterr = 0;
// 	while ((c = getopt (argc, argv, "hb:")) != -1)
// 	{
// 		switch (c)
// 		{
// 			case 'h':
// 				usage(argv[0]);
// 				return 0;

// 			case 'b':
// 				buffer_size = strtol(optarg, NULL, 10)*KB;
// 				break;

// 			case '?':
// 				if (optopt == 'b')
// 					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
// 				else if (isprint (optopt))
// 					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
// 				else
// 					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
// 				usage(argv[0]);
// 				return 1;

// 			default:
// 				abort ();
// 				break;
// 		}
// 	}

// 	source=argv[optind];

// 	if (source == NULL) {
// 		fprintf(stderr, "ERROR: Incorrect number of argumets.\n");
// 		usage(argv[0]);
// 		return 1;
// 	}

// 	if (strncmp(source, xpnprefix, strlen(xpnprefix)) == 0) {
// 		source = source + strlen(xpnprefix);
// 		xpnsource = 1;
// 	}

// 	isxpn = xpnsource;
// #ifdef DEBUG
// 	printf("xpnsource=%d, isxpn=%d\n", xpnsource, isxpn);
// 	printf("source = '%s'\n", source);
// 	printf("buffer_size = %lu\n", (unsigned long)buffer_size);
// #endif

// 	if (isxpn) {
// 		ret = xpn_init();
// 		if (ret<0) {
// 			printf("Error %d while initializing expand\n", ret);
// 			exit(-1);
// 		}
// 	}

// 	if (xpnsource)
// 		xpn_stat(source, &st);
// 	else
// 		stat(source, &st);
// #ifdef DEBUG
// 	printf("st.st_size = %d\n", (int)st.st_size);
// #endif

// 	if (buffer_size <= 0) {
// 		if (st.st_size <= 0)
// 			buffer_size = 256*KB;
// 		else
// 			buffer_size = st.st_size;
// 	}
// #ifdef DEBUG
// 	printf("buffer_size = %d\n", (int)buffer_size);
// #endif
// 	buffer = (char *)malloc(buffer_size);

// 	if (xpnsource)
// 		fds = xpn_open(source, O_RDONLY);
// 	else
// 		fds = open(source, O_RDONLY);

// 	if(fds < 0) {
// 		printf("Error opening source: fd = %d\n",fds);
// 		exit(-1);
// 	}

// 	sum = 0;
// 	do {
// 		if (st.st_size-sum < (ssize_t)buffer_size)
// 			buffer_size = st.st_size-sum;
// #ifdef DEBUG
// 		printf("Leyendo un bloque de %lu bytes\n", (unsigned long)buffer_size);
// #endif

// 		if (xpnsource)
// 			nr = xpn_read(fds, buffer, buffer_size);
// 		else
// 			nr = read(fds, buffer, buffer_size);
// #ifdef DEBUG
// 		printf("Leido un bloque de %ld bytes\n", nr);
// #endif
// 		fwrite(buffer, sizeof(char), nr, stdout);

// 		sum = sum + nr;
// 	} while (nr>0);

// 	if (xpnsource)
// 		xpn_close(fds);
// 	else
// 		close(fds);

// 	if (isxpn)
// 		xpn_destroy();

// 	free(buffer);

// 	exit(0);
// }

