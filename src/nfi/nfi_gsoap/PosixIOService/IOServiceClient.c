#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>

#include "libIOServiceClient.h"

#define BAR_LENGTH 50

#ifndef KB
	#define KB	(1024)
#endif

#ifndef MB
	#define MB	(KB*KB)
#endif

char *program_name;
struct stat st;
struct timeval t_ini_transfer, t_end_transfer, t_old_transfer;
double transfer_t, transfer_bw;
size_t sum, sum_old;
char bar[BAR_LENGTH+1];
int bar_length;
int silent = 0;
int very_silent = 0;
int debug;

void print_bar() {
	int bar_expected_length;
	
	memcpy(&t_old_transfer, &t_end_transfer, sizeof(struct timeval));

	gettimeofday(&t_end_transfer, NULL);
	
	transfer_t = (t_end_transfer.tv_sec-t_old_transfer.tv_sec)+(double)(t_end_transfer.tv_usec-t_old_transfer.tv_usec)/1000000;
	transfer_bw = (sum-sum_old)/transfer_t;
	if (st.st_size > 0) {
		bar_expected_length = sum / (float)(st.st_size/BAR_LENGTH);
		while (bar_expected_length > bar_length) {
			bar[bar_length] = '=';
			bar_length++;
		}
	}
	
	if (!silent) {
		printf("%c[%s] %d %.3f MB/s", (char)13, bar, sum, transfer_bw/MB);
		fflush(NULL);
	}
}

void progression_bar() {
	int i;
	
	for (i = 0 ; i < BAR_LENGTH ; i++)
		bar[i] = ' ';
	bar[BAR_LENGTH] = '\0';
	
	memcpy(&t_end_transfer, &t_ini_transfer, sizeof(struct timeval));
	
	bar_length = 0;
	sum_old = 0;
	
	if (!very_silent) {
		printf("\n");
	}
	
	while(1) {
		print_bar();
		sum_old = sum;
		sleep(1);
	}

	pthread_exit(0);
}

void usage() {
	printf("Usage: %s [-h] | [-b <buffer_size_in_KB>] [-s | -S] [-e server] [-p port] [-z 1-9] [-l] [-a container] [-c certificate] [-k password] [-r <number_of_retries>] [-v] <source> <destination>\n", program_name);
}

int main(int argc, char *argv[]) {
	char *source, *dest;
	int ret = -1, fds, fdd;
	size_t nr, nw;
	size_t buffer_size = 64*KB;
	char *buffer;
	int isws = 0, wssource = 0, wsdest = 0;
	const char *wsprefix = "gsoap://";
	struct ioservice *ioservice = NULL;
	struct timeval t_ini, t_end;
	double total_t, total_bw;
	pthread_t thread;
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
	int retries;
	
	program_name = argv[0];
	
	opterr = port =  enableGZIP = enableSSL = debug = retries = 0;
	serverCacertFlag = serverCertificateFlag = serverPassFlag = 0;
	server = serverCacert = serverCertificate = serverPass = NULL;
	
	while ((c = getopt (argc, argv, "hb:sSe:p:z:la:c:k:r:v")) != -1)
		switch (c) {
			case 'h': // Help
				usage();
				return 0;
			case 'b': // Buffer size
				buffer_size = strtol(optarg, NULL, 10)*KB;
				break;
			case 's': // Silent mode
				silent = 1;
				break;
			case 'S': // Very silent mode
				silent = 1;
				very_silent = 1;
				break;
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
			case 'r': // Retries
				retries = atoi (optarg);
				if (retries < 0) {
					usage();
					exit(-1);
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
	
	if ((source == NULL) || (dest == NULL)) {
		fprintf(stderr, "ERROR: Incorrect number of argumets.\n");
		usage();
		return 1;
	}
	
	buffer = (char *)malloc(buffer_size);
	
	if (strncmp(source, wsprefix, strlen(wsprefix)) == 0) {
		source = source + strlen(wsprefix);
		wssource = 1;
	}
	
	if (strncmp(dest, wsprefix, strlen(wsprefix)) == 0) {
		dest = dest + strlen(wsprefix);
		wsdest = 1;
	}
	
	isws = wssource | wsdest;
#ifdef DEBUG
	printf("wssource=%d, wsdest=%d, isws=%d\n", wssource, wsdest, isws);
	
	printf("source = '%s'\n", source);
	printf("dest = '%s'\n", dest);

	printf("buffer_size = %d\n", buffer_size);
#endif
	if (isws) {
		gettimeofday(&t_ini, NULL);
		ioservice = posix_ws_init(server, port, enableGZIP, enableSSL, serverCacert, serverCertificate, serverPass, retries, debug);
		if (ret<0) {
			printf("Error %d while initializing posix I/O web services\n", ret);
			exit(-1);
		}
	}
	
	if (wssource)
		posix_ws_stat(ioservice, source, &st);
	else
		stat(source, &st);
#ifdef DEBUG
	printf("st.st_size = %d\n", (int)st.st_size);
#endif
	
	if (!very_silent) {
		gettimeofday(&t_ini_transfer, NULL);
	}

	if (!silent)
		pthread_create(&thread, NULL, (void * (*)(void *))progression_bar, NULL);
	
	if (wssource)
		fds = posix_ws_open(ioservice, source, O_RDONLY);
	else
		fds = open(source, O_RDONLY);
	
	if(fds < 0) {
		printf("Error opening source: fd = %d\n",fds);
		exit(-1);
	}
	
	if (wsdest)
		fdd = posix_ws_open(ioservice, dest, O_TRUNC|O_CREAT|O_RDWR, 0644);
	else
		fdd = open(dest, O_TRUNC|O_CREAT|O_RDWR, 0644);
	
	if(fdd < 0) {
		printf("Error opening dest: fd = %d\n",fdd);
		exit(-1);
	}

	sum = 0;
	do {
		if (st.st_size-sum < buffer_size)
			buffer_size = st.st_size-sum;
#ifdef DEBUG
		printf("Reading a block of size %d bytes\n", buffer_size);
#endif
		
		if (wssource)
			nr = posix_ws_read(ioservice, fds, buffer, buffer_size);
		else
			nr = read(fds, buffer, buffer_size);
#ifdef DEBUG
		printf("Has been read a block of size %d bytes\n", nr);
#endif

		if (wsdest)
			nw = posix_ws_write(ioservice, fdd, buffer, nr);
		else
			nw = write(fdd, buffer, nr);
#ifdef DEBUG
		printf("Written a block of size %d bytes\n", nw);
#endif
		
		sum = sum + nw;
	} while ((nr==buffer_size) && (nw > 0));
	
	if (wssource)
		posix_ws_close(ioservice, fds);
	else
		close(fds);
	
	if (wsdest)
		posix_ws_close(ioservice, fdd);
	else
		close(fdd);
	
	if (!silent) {
		pthread_cancel(thread);
		print_bar();
	}

	if (!very_silent) {
		transfer_t = (t_end_transfer.tv_sec-t_ini_transfer.tv_sec)+(double)(t_end_transfer.tv_usec-t_ini_transfer.tv_usec)/1000000;
		transfer_bw = sum/transfer_t;
		printf("\n\n");
		printf("Transfer time = %.3f s\n", transfer_t);
		printf("Transfer bandwidth = %.3f B/s = %.3f KB/s = %.3f MB/s\n", transfer_bw, transfer_bw/KB, transfer_bw/MB);
	}
	
	if (isws) {
		posix_ws_end(ioservice);
		if (!very_silent) {
			gettimeofday(&t_end, NULL);
			total_t = (t_end.tv_sec-t_ini.tv_sec)+(double)(t_end.tv_usec-t_ini.tv_usec)/1000000;
			total_bw = sum/total_t;
			printf("\n");
			printf("Total time = %.3f s\n", total_t);
			printf("Total bandwidth = %.3f B/s = %.3f KB/s = %.3f MB/s\n", total_bw, total_bw/KB, total_bw/MB);
		}
	}

	if (!very_silent) {
		printf("\n");
	}
	
	if (server != NULL)
		free(server);
	
	free(buffer);
	
	exit(0);
}

