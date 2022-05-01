#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <argp.h>

#include "xpn.h"

#define BAR_LENGTH 50

#ifndef KB
	#define KB	(1024)
#endif

#ifndef MB
	#define MB	(KB*KB)
#endif

struct stat st;
struct timeval t_ini_transfer, t_end_transfer, t_old_transfer;
double transfer_t, transfer_bw;
ssize_t sum, sum_old;
char bar[BAR_LENGTH+1];
int bar_length;
int silent = 0;
int very_silent = 0;
int debug = 0;

void print_bar()
{
	int bar_expected_length;
	
	memcpy(&t_old_transfer, &t_end_transfer, sizeof(struct timeval));
	
	gettimeofday(&t_end_transfer, NULL);
	
	transfer_t = (t_end_transfer.tv_sec-t_old_transfer.tv_sec)+(double)(t_end_transfer.tv_usec-t_old_transfer.tv_usec)/1000000;
	transfer_bw = (sum-sum_old)/transfer_t;
	if (st.st_size > 0) {
		bar_expected_length = sum / ((float)(st.st_size)/BAR_LENGTH);
		while (bar_expected_length > bar_length) {
			bar[bar_length] = '=';
			bar_length++;
		}
	}
	
	if (!silent) {
		printf("%c[%s] %zd %.3f MB/s", (char)13, bar, sum, transfer_bw/MB);
		fflush(NULL);
	}
}

void *progression_bar ( __attribute__((unused)) void *arg )
{
	for (int i = 0 ; i < BAR_LENGTH ; i++)
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
	return NULL ;
}

const char *argp_program_version = "xpncp_f 1.0";
const char *argp_program_bug_address = "<https://github.com/xpn-arcos/xpn>" ;
static char doc[] = "A copy program for Expand partitions that uses fread/fwrite";

char *help_filter (int key, const char *text, __attribute__((__unused__)) void *input)
{
  switch (key)
  {
    case ARGP_KEY_HELP_PRE_DOC:
      //printf("\nARGP_KEY_HELP_PRE_DOC text='%s' input='%s'\n", text, (char *)input);
      return (char *)text;
      break;
    case ARGP_KEY_HELP_POST_DOC:
      //printf("\nARGP_KEY_HELP_POST_DOC text='%s' input='%s'\n", text, (char *)input);
      return (char *)text;
      break;
    case ARGP_KEY_HELP_HEADER:
      //printf("\nARGP_KEY_HELP_HEADER text='%s' input='%s'\n", text, (char *)input);
      return (char *)text;
      break;
    case ARGP_KEY_HELP_EXTRA:
      //printf("\nARGP_KEY_HELP_EXTRA text='%s' input='%s'\n", text, (char *)input);
      return (char *)text;
      break;
    case ARGP_KEY_HELP_DUP_ARGS_NOTE:
      //printf("\nARGP_KEY_HELP_DUP_ARGS_NOTE text='%s' input='%s'\n", text, (char *)input);
      return (char *)text;
      break;
    case ARGP_KEY_HELP_ARGS_DOC:
      //printf("\nARGP_KEY_HELP_ARGS_DOC text='%s' input='%s'\n", text, (char *)input);
      return strdup("<source> <destination>");
      break;
    default:
      return (char *)text;
  }
}

/* The options we understand. */
static struct argp_option options[] = {
  {"buffer-size",          'b', "BUFFER_SIZE",          0, "Buffer size", 0 },
  {"internal-buffer-size", 'n', "INTERNAL_BUFFER_SIZE", 0, "Internal buffer size", 0 },
  {"file-size",            'i', "FILE_SIZE",            0, "File size", 0 },
  {"processing",           'p', "PROCESSING_TIME",      0, "Processing time", 0 },
  {"read-only",            'r', 0,                      0, "Read-only", 0 },
  {"prefetch",             'e', 0,                      0, "Prefetch", 0 },
  {"silent",               's', 0,                      0, "Silent", 0 },
  {"very-silent",          'S', 0,                      0, "Very silent", 0 },
  {"debug",                'd', 0,                      0, "Debug", 0 },
  { 0, 0, 0, 0, 0, 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments {
	size_t buffer_size;
	size_t internal_buffer_size;
	struct stat st;
	int do_stat;
	useconds_t processing_time;
	int do_processing;
	int read_only;
	int do_prefetch;
	int silent;
	int very_silent;
	int debug;
	char *source;
	char *dest;
};

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;
  char *suffix;
  int units = 0/*, i*/;

  switch (key)
  {
    case 'b':
      units = 1;
      arguments->buffer_size = strtoul(arg, &suffix, 10);
      if ( strlen(suffix) > 0 ) {
        if ( strlen(suffix) > 1 ) argp_usage (state);
        switch(*suffix) {
          case 'k': case 'K':
            units = 1024;
            break;
          case 'm': case 'M':
            units = 1024*1024;
            break;
          case 'g': case 'G':
            units = 1024*1024*1024;
            break;
          default:
            fprintf(stderr, "Unknown suffix '%s' when parsing buffer_size '%s'\n", suffix, arg);
            argp_state_help (state, stdout, ARGP_HELP_STD_HELP);
            argp_usage (state);
        }
        if (arguments->buffer_size*(long long)units < SSIZE_MAX)
            arguments->buffer_size *= units;
        else
            arguments->buffer_size = SSIZE_MAX;
      }
      break;
    case 'n':
      units = 1;
      arguments->internal_buffer_size = strtoul(arg, &suffix, 10);
      if ( strlen(suffix) > 0 ) {
        if ( strlen(suffix) > 1 ) argp_usage (state);
        switch(*suffix) {
          case 'k': case 'K':
            units = 1024;
            break;
          case 'm': case 'M':
            units = 1024*1024;
            break;
          case 'g': case 'G':
            units = 1024*1024*1024;
            break;
          default:
            fprintf(stderr, "Unknown suffix '%s' when parsing internal_buffer_size '%s'\n", suffix, arg);
            argp_state_help (state, stdout, ARGP_HELP_STD_HELP);
            argp_usage (state);
        }
        if (arguments->internal_buffer_size*(long long)units < SSIZE_MAX)
            arguments->internal_buffer_size *= units;
        else
            arguments->internal_buffer_size = SSIZE_MAX;
      }
      break;
    case 'i':
      units = 1;
      arguments->st.st_size = strtoll(arg, &suffix, 10);
      arguments->do_stat = 0;
      if ( strlen(suffix) > 0 ) {
        if ( strlen(suffix) > 1 ) argp_usage (state);
        switch(*suffix) {
          case 'k': case 'K':
            units = 1024;
            break;
          case 'm': case 'M':
            units = 1024*1024;
            break;
          case 'g': case 'G':
            units = 1024*1024*1024;
            break;
          default:
            fprintf(stderr, "Unknown suffix '%s' when parsing file_size '%s'\n", suffix, arg);
            argp_state_help (state, stdout, ARGP_HELP_STD_HELP);
            argp_usage (state);
        }
        arguments->st.st_size *= units;
      }
      break;
    case 'p':
      units = 1;
      arguments->processing_time = strtol(arg, &suffix, 10);
      arguments->do_processing = 1;
      if ( strlen(suffix) > 0 ) {
        if ( strlen(suffix) > 1 ) argp_usage (state);
        switch(*suffix) {
          case 'u': case 'U':
            units = 1;
            break;
          case 'm': case 'M':
            units = 1000;
            break;
          case 's': case 'S':
            units = 1000*1000;
            break;
          default:
            fprintf(stderr, "Unknown suffix '%s' when parsing processing_time '%s'\n", suffix, arg);
            argp_state_help (state, stdout, ARGP_HELP_STD_HELP);
            argp_usage (state);
        }
        arguments->processing_time *= units;
      }
      break;
    case 'r':
      arguments->read_only = 1;
      break;
    case 'e':
      arguments->do_prefetch = 1;
      break;
    case 's':
      arguments->silent = 1;
      break;
    case 'S':
      arguments->silent = 1;
      arguments->very_silent = 1;
      break;
    case 'd':
      arguments->debug = 1;
      break;
    case ARGP_KEY_ARG:
      return ARGP_ERR_UNKNOWN;
      break;
    case ARGP_KEY_ARGS:
      arguments->source = state->argv[state->next];
      //TODO: Handle several input files
      //for (i = state->next ; i < state->argc ; i++ )
      //  printf("arg=%s\n", state->argv[i]);
      arguments->dest = state->argv[state->argc-1];
      break;
    case ARGP_KEY_END:
      //if (state->arg_num < 2)
      if (state->arg_num != 2)
        /* Not enough arguments. */
        argp_usage (state);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, 0, doc, 0, help_filter, 0 };

int main(int argc, char *argv[]) {
	int do_stat = 1;
	char *source, *dest;
	int ret;
	FILE *fds, *fdd;
	ssize_t nr, nw;
	size_t buffer_size = 0;
	size_t internal_buffer_size = 0;
	int do_processing = 0;
	useconds_t processing_time = 0;
	int read_only = 0;
	int do_prefetch = 0;
	char *buffer;
	int isxpn = 0, xpnsource = 0, xpndest = 0;
	const char *xpnprefix = "xpn://";
	struct timeval t_ini, t_end;
	double total_t, total_bw;
	pthread_t thread;
	struct arguments arguments;

	/* Default values. */
	arguments.buffer_size = 0;
	arguments.internal_buffer_size = 0;
	memset(&arguments.st, 0, sizeof(struct stat));
	arguments.do_stat = 1;
	arguments.processing_time = 0;
	arguments.do_processing = 0;
	arguments.read_only = 0;
	arguments.do_prefetch = 0;
	arguments.silent = 0;
	arguments.very_silent = 0;
	arguments.debug = 0;
	arguments.source = NULL;
	arguments.dest = NULL;

	/* Parse our arguments; every option seen by parse_opt will
	   be reflected in arguments. */
	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	/* Set globals based on command-line arguments */
	buffer_size = arguments.buffer_size;
	internal_buffer_size = arguments.internal_buffer_size;
	memcpy(&st, &arguments.st, sizeof(struct stat));
	do_stat = arguments.do_stat;
	processing_time = arguments.processing_time;
	do_processing = arguments.do_processing;
	read_only = arguments.read_only;
	do_prefetch = arguments.do_prefetch;
	silent= arguments.silent;
	very_silent = arguments.very_silent;
	debug = arguments.debug;
	source = arguments.source;
	dest = arguments.dest;
	
	if (strncmp(source, xpnprefix, strlen(xpnprefix)) == 0) {
		source = source + strlen(xpnprefix);
		xpnsource = 1;
	}
	
	if ((!read_only) && (strncmp(dest, xpnprefix, strlen(xpnprefix)) == 0)) {
		dest = dest + strlen(xpnprefix);
		xpndest = 1;
	}
	
	isxpn = xpnsource | xpndest;

	if (debug) {
		printf("xpnsource=%d, xpndest=%d, isxpn=%d\n", xpnsource, xpndest, isxpn);
	
		printf("source = '%s'\n", source);
		printf("dest = '%s'\n", dest);
	
		printf("buffer_size = %zu\n", buffer_size);
		printf("internal_buffer_size = %zu\n", internal_buffer_size);
	}

	if (isxpn) {
		gettimeofday(&t_ini, NULL);
		ret = xpn_init();
		if (ret<0) {
			fprintf(stderr, "Error %d while initializing expand\n", ret);
			exit(-1);
		}
	}
	
	if (do_stat) {
		if (xpnsource)
			xpn_stat(source, &st);
		else
			stat(source, &st);
	}

	if (debug)
		printf("st.st_size = %d\n", (int)st.st_size);

	if (buffer_size <= 0) {
		if (st.st_size <= 0)
			buffer_size = 256*KB;
		else
			buffer_size = st.st_size;
	}

	if (debug)
		printf("buffer_size = %d\n", (int)buffer_size);

	buffer = (char *)malloc(buffer_size);
	if (buffer == NULL) {
		fprintf(stderr, "Error allocating %zu bytes\n", buffer_size);
		exit(1);
	}

	if (!very_silent) {
		gettimeofday(&t_ini_transfer, NULL);
	}
	
	if (!silent)
		pthread_create(&thread, NULL, (void * (*)(void *))progression_bar, NULL);
	
	if (xpnsource)
		fds = xpn_fopen(source, "r");
	else
		fds = fopen(source, "r");
	
	if(fds == NULL) {
		perror("Error opening source");
		fprintf(stderr, "Error opening source '%s': fd = %p\n", source, fds);
		exit(-1);
	}

	if (!read_only) {
		if (xpndest)
			fdd = xpn_fopen(dest, "w");
		else
			fdd = fopen(dest, "w");
	
		if(fdd == NULL) {
			fprintf(stderr, "Error opening dest: fd = %p\n", fdd);
			exit(-1);
		}
	}
	
	if (internal_buffer_size > 0) {
		if (xpnsource) {
			xpn_setvbuf(fds, (char *) NULL, _IOFBF, internal_buffer_size);
			//char *ibuf = malloc(internal_buffer_size);
			//xpn_setbuffer(fds, ibuf, internal_buffer_size);
		} else
			setvbuf(fds, (char *) NULL, _IOFBF, internal_buffer_size);

		if (xpndest) {
			xpn_setvbuf(fdd, (char *) NULL, _IOFBF, internal_buffer_size);
			//char *ibuf = malloc(internal_buffer_size);
			//xpn_setbuffer(fdd, ibuf, internal_buffer_size);
		} else
			setvbuf(fdd, (char *) NULL, _IOFBF, internal_buffer_size);
	}
	
	sum = 0;
	do {
		if (st.st_size-sum < (ssize_t)buffer_size)
			buffer_size = st.st_size-sum;

		if (debug)
			printf("Copying a block of %zu bytes\n", buffer_size);
		
		if (xpnsource) {
			if (do_prefetch)
				nr = xpn_fread_prefetch(buffer, 1, buffer_size, fds);
			else
				nr = xpn_fread(buffer, 1, buffer_size, fds);
		} else
			nr = fread(buffer, 1, buffer_size, fds);

		if (debug)
			printf("Read a block of %zu bytes\n", nr);

		if (do_processing)
			usleep(processing_time);

		if (!read_only)
			if (xpndest)
				nw = xpn_fwrite(buffer, 1, nr, fdd);
			else
				nw = fwrite(buffer, 1, nr, fdd);
		else
			nw = nr;

		if (debug)
			printf("Written a block of %zu bytes\n", nw);
		
		sum = sum + nw;
	} while ((nr==(ssize_t)buffer_size) && (nw > 0));
	
	if (xpnsource)
		xpn_fclose(fds);
	else
		fclose(fds);
	
	if ((!read_only) && (xpndest))
		xpn_fclose(fdd);
	else
		fclose(fdd);
	
	if (!silent) {
		pthread_cancel(thread);
		print_bar();
	}
	
	if (!very_silent) {
		gettimeofday(&t_end_transfer, NULL);
		transfer_t = (t_end_transfer.tv_sec-t_ini_transfer.tv_sec)+(double)(t_end_transfer.tv_usec-t_ini_transfer.tv_usec)/1000000;
		transfer_bw = sum/transfer_t;
		printf("\n\n");
		printf("Transfer time = %.3f s\n", transfer_t);
		printf("Transfer bandwidth = %.3f B/s = %.3f KB/s = %.3f MB/s\n", transfer_bw, transfer_bw/KB, transfer_bw/MB);
	}
	
	if (isxpn) {
		xpn_destroy();
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
	
	free(buffer);
	
	exit(0);
}

