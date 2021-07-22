#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>

#include "libXpnUploaderClient.h"

char *program_name;

void usage() {
	printf("Usage: %s [-h] | [-b <buffer_size_in_KB>] [-e server] [-p port] [-z 1-9] [-l] [-a container] [-c certificate] [-k password] [-v] <source> <destination> <partition>\n", program_name);
}

int main(int argc, char *argv[]) {
	char *source, *dest, *part_file, *partition;
	int fd;
	int ret;
	struct ioservice *ioservice;
	int c;
	char* server;
	int port;			/** Port where the server will listen the request */
	int enableSSL;			/** Enables SSL auntenthication and encryption */
	int enableGZIP;			/** Enables GZIP compression */
	int serverCertificateFlag;
	int serverPassFlag;
	int serverCacertFlag;
	char* serverCacert;		/** Trusted certificates container. "CA/cacert.pem" */
	char* serverCertificate;	/** Server certificate. "CA/server.pem" */
	char* serverPass;		/** Server pass certificate. "password" */
	int debug;
	struct stat st;
	
	program_name = argv[0];
	
	debug = opterr = port =  enableGZIP = enableSSL = 0;
	serverCacertFlag = serverCertificateFlag = serverPassFlag = 0;
	server = serverCacert = serverCertificate = serverPass = NULL;
	
	while ((c = getopt (argc, argv, "he:p:z:la:c:k:v")) != -1)
		switch (c) {
			case 'h': // Help
				usage();
				return 0;
			case 'e': // Server
				if (optarg != NULL) {
					server = (char *)malloc(strlen(optarg)+1);
					strcpy (server, optarg);
				}
				break;
			case 'p': // Port
				port = atoi (optarg);
				if ((port < 0) || (port > 65535)) {
					usage();
					exit(-1);
				}
				break;
			case 'z': // GZIP
				enableGZIP = atoi(optarg);
				if ((enableGZIP < 1) || (enableGZIP > 9)) {
					usage();
					exit(-1);
				}
				break;
			case 'l': // SSL
				enableSSL = 1;
				break;
			case 'a': // Container
				serverCacertFlag = 1;
				if (optarg != NULL) {
					serverCacert = (char *)malloc(strlen(optarg)+1);
					strcpy (serverCacert, optarg);
				}
				break;
			case 'c': // Certificate
				serverCertificateFlag = 1;
				if (optarg != NULL) {
					serverCertificate = (char *)malloc(strlen(optarg)+1);
					strcpy (serverCertificate, optarg);
				}
				break;
			case 'k': // Password
				serverPassFlag = 1;
				if (optarg != NULL) {
					serverPass = (char *)malloc(strlen(optarg)+1);
					strcpy (serverPass, optarg);
				}
				break;
			case 'v': // Verbose
				debug = 1;
				break;
			case '?': // None of the above
				if ((optopt == 'b') || (optopt == 'p') || (optopt == 'z') || (optopt == 'a') || (optopt == 'c') || (optopt == 'k'))
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
	dest=argv[optind+1];
	part_file=argv[optind+2];
	
	if ((source == NULL) || (dest == NULL) || (part_file == NULL)) {
		fprintf(stderr, "ERROR: Incorrect number of argumets.\n");
		usage();
		return 1;
	}
	
	if (stat(part_file, &st) < 0) {
		fprintf(stderr, "Error at stat(%s)\n", part_file);
		exit(-1);
	}
	
	partition = (char *)malloc(st.st_size+1);
	fd = open(part_file, O_RDONLY);
	read(fd, partition, st.st_size);
	partition[st.st_size] = '\0';
	close(fd);
	
#ifdef DEBUG
	printf("source = '%s'\n", source);
	printf("dest = '%s'\n", dest);
	printf("partition = '%s'\n", partition);
#endif
	ioservice = xpnuploader_ws_init(server, port, enableGZIP, enableSSL, serverCacert, serverCertificate, serverPass, debug);
	if (ioservice <= 0) {
		printf("Error %d while initializing xpnuploader web service\n", ret);
		exit(-1);
	}
	
	xpnuploader_ws_upload(ioservice, source, dest, partition);
	
	xpnuploader_ws_end(ioservice);
	
	if (server != NULL)
		free(server);
	free(partition);
	
	exit(0);
}

