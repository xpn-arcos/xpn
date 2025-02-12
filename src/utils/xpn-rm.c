
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
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>

#include "all_system.h"
#include "xpn.h"


void usage ( char * program_name )
{
	printf("Usage: %s [-h] <file>\n", program_name);
}

int main(int argc, char *argv[])
{
	char *source;
	int ret;
	int isxpn = 0, xpnsource = 0;
	const char *xpnprefix ;
	int c;

	xpnprefix = "xpn://";
	opterr = 0;
	while ((c = getopt (argc, argv, "h")) != -1)
	{
		switch (c)
		{
			case 'h':
				usage(argv[0]);
				return 0;
			case '?':
				if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				usage(argv[0]);
				return 1;
			default:
				abort ();
		}
	}
	
	source=argv[optind];
	
	if (source == NULL) {
		fprintf(stderr, "ERROR: Incorrect number of argumets.\n");
		usage(argv[0]);
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

