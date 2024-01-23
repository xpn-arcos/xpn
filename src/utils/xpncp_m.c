
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
#include <sys/mman.h>
#include <argp.h>

#include "all_system.h"
#include "xpn.h"

#define BAR_LENGTH 50


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
	int is_true = 1;
	for (int i = 0 ; i < BAR_LENGTH ; i++)
		bar[i] = ' ';
	bar[BAR_LENGTH] = '\0';

	memcpy(&t_end_transfer, &t_ini_transfer, sizeof(struct timeval));

	bar_length = 0;
	sum_old = 0;

	if (!very_silent) {
		printf("\n");
	}

	while(is_true) {
		print_bar();
		sum_old = sum;
		sleep(1);
	}

	pthread_exit(0);
	return NULL ;
}

struct buffer_st {
	ssize_t buffer_length;
	char *buffer;
};

struct write_args {
	int fdd;
	int xpnsource;
	int xpndest;
	double *out_transfer_t;
};

struct buffer_st **array_bufs = NULL;
ssize_t next = 0, last = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t xpn_mutex = PTHREAD_MUTEX_INITIALIZER;

void *write_func(void *func_args)
{
	struct write_args *args = (struct write_args *)func_args;
	struct timeval t_ini_trans, t_end_trans;
	size_t sum_w;
	ssize_t nw;

	while (sum < st.st_size)
	{
		pthread_mutex_lock(&mutex);
		while (next >= last) {
			//fprintf(stderr, "write_func: cond not ready next %lu last %lu\n", next, last);
			pthread_cond_wait(&cond, &mutex);
		}
		pthread_mutex_unlock(&mutex);

		sum_w = 0;
		do {
			if (!very_silent)
				gettimeofday(&t_ini_trans, NULL);
			if (args->xpndest) {
				// Expand is not reentrant
				if (args->xpnsource)
					pthread_mutex_lock(&xpn_mutex);
				nw = xpn_write(args->fdd, &(array_bufs[next]->buffer)[sum_w], array_bufs[next]->buffer_length);
				if (args->xpnsource)
					pthread_mutex_unlock(&xpn_mutex);
			} else {
				nw = write(args->fdd, &(array_bufs[next]->buffer)[sum_w], array_bufs[next]->buffer_length);
				fsync(args->fdd); // Improves performance
			}

			if (!very_silent) {
				gettimeofday(&t_end_trans, NULL);
				*(args->out_transfer_t) += (t_end_trans.tv_sec-t_ini_trans.tv_sec)+(double)(t_end_trans.tv_usec-t_ini_trans.tv_usec)/1000000;
			}

				//fprintf(stderr, "write(%zd) written block %zd (%p) of %zd bytes %u %u %u %u ... %u %u %u %u\n", array_bufs[next]->buffer_length, next, array_bufs[next]->buffer, nw, (unsigned char)array_bufs[next]->buffer[0], (unsigned char)array_bufs[next]->buffer[1], (unsigned char)array_bufs[next]->buffer[2], (unsigned char)array_bufs[next]->buffer[3], (unsigned char)array_bufs[next]->buffer[nw-4], (unsigned char)array_bufs[next]->buffer[nw-3], (unsigned char)array_bufs[next]->buffer[nw-2], (unsigned char)array_bufs[next]->buffer[nw-1]);
			if (debug)
				fprintf(stderr, "write(%zd) written block %zd (%p) of %zd bytes\n", array_bufs[next]->buffer_length, next, array_bufs[next]->buffer, nw);

			if (nw > 0) {
				sum_w += nw;
				sum   += nw;
				array_bufs[next]->buffer_length -= nw;
			}
		} while (array_bufs[next]->buffer_length > 0);

		free(array_bufs[next]->buffer);
		array_bufs[next]->buffer = NULL;
		free(array_bufs[next]);
		array_bufs[next] = NULL;

		next++;

		if (debug)
			printf("Written a block of %lu bytes\n", (unsigned long)sum_w);
	}

	pthread_exit(0);
        return NULL;
}

const char *argp_program_version = "xpncp_m 1.0";
const char *argp_program_bug_address = "<https://github.com/xpn-arcos/xpn>" ;
static char doc[] = "A fast copy program for Expand partitions that uses mmap for local files";

char *help_filter (int key, const char *text, __attribute__((__unused__)) void *input)
{
	char * ret;
  switch (key)
  {
    case ARGP_KEY_HELP_PRE_DOC:
      //printf("\nARGP_KEY_HELP_PRE_DOC text='%s' input='%s'\n", text, (char *)input);
      ret = (char *)text;
      break;
    case ARGP_KEY_HELP_POST_DOC:
      //printf("\nARGP_KEY_HELP_POST_DOC text='%s' input='%s'\n", text, (char *)input);
      ret = (char *)text;
      break;
    case ARGP_KEY_HELP_HEADER:
      //printf("\nARGP_KEY_HELP_HEADER text='%s' input='%s'\n", text, (char *)input);
      ret = (char *)text;
      break;
    case ARGP_KEY_HELP_EXTRA:
      //printf("\nARGP_KEY_HELP_EXTRA text='%s' input='%s'\n", text, (char *)input);
      ret = (char *)text;
      break;
    case ARGP_KEY_HELP_DUP_ARGS_NOTE:
      //printf("\nARGP_KEY_HELP_DUP_ARGS_NOTE text='%s' input='%s'\n", text, (char *)input);
      ret = (char *)text;
      break;
    case ARGP_KEY_HELP_ARGS_DOC:
      //printf("\nARGP_KEY_HELP_ARGS_DOC text='%s' input='%s'\n", text, (char *)input);
      ret = strdup("<source> <destination>");
      break;
    default:
      ret = (char *)text;
      break;
  }

  return ret;
}

/* The options we understand. */
static struct argp_option options[] = {
  {"buffer-size", 'b', "BUFFER_SIZE", 0, "Buffer size", 0 },
  {"file-size",   'i', "FILE_SIZE",   0, "File size", 0 },
  {"silent",      's', 0,             0, "Silent", 0 },
  {"very-silent", 'S', 0,             0, "Very silent", 0 },
  {"debug",       'd', 0,             0, "Debug", 0 },
  { 0, 0, 0, 0, 0, 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments {
	size_t buffer_size;
	struct stat st;
	int do_stat;
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

int main(int argc, char *argv[])
{
	int do_stat = 1;
	char *source, *dest;
	int ret;
	int fds, fdd;
	ssize_t nr;
	char *source_map = NULL, *dest_map = NULL;
	size_t buffer_size = 0;
	struct buffer_st *buf = NULL;
	ssize_t sum_r;
	int isxpn = 0, xpnsource = 0, xpndest = 0;
	const char *xpnprefix = "xpn://";
	struct timeval t_ini_trans, t_end_trans, t_ini_total, t_end_total;
	double in_transfer_t, out_transfer_t, total_t, in_transfer_bw, out_transfer_bw, total_bw;
	pthread_t thread_bar, thread_write;
	//int c;
	struct write_args args;
	struct arguments arguments;

	/* Default values. */
	arguments.buffer_size = 0;
	memset(&arguments.st, 0, sizeof(struct stat));
	arguments.do_stat = 1;
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
	memcpy(&st, &arguments.st, sizeof(struct stat));
	do_stat = arguments.do_stat;
	silent= arguments.silent;
	very_silent = arguments.very_silent;
	debug = arguments.debug;
	source = arguments.source;
	dest = arguments.dest;

	if (strncmp(source, xpnprefix, strlen(xpnprefix)) == 0) {
		source = source + strlen(xpnprefix);
		xpnsource = 1;
	}

	if (strncmp(dest, xpnprefix, strlen(xpnprefix)) == 0) {
		dest = dest + strlen(xpnprefix);
		xpndest = 1;
	}

	isxpn = xpnsource | xpndest;

	if (debug) {
		printf("xpnsource=%d, xpndest=%d, isxpn=%d\n", xpnsource, xpndest, isxpn);

		printf("source = '%s'\n", source);
		printf("dest = '%s'\n", dest);

		printf("buffer_size = %zu\n", buffer_size);
	}

	if (isxpn) {
		gettimeofday(&t_ini_total, NULL);
		ret = xpn_init();
		if (ret<0) {
			printf("Error %d while initializing expand\n", ret);
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

	if (st.st_size <= 0) {
		fprintf(stderr, "ERROR: Input file size needed. Provide a working stat() function, or use option '-i' instead.\n");
		exit(-1);
	}

	if (buffer_size <= 0) {
		buffer_size = st.st_size;
	}

	if (debug)
		printf("buffer_size = %d\n", (int)buffer_size);

	if (!very_silent) {
		in_transfer_t = out_transfer_t = 0;
		gettimeofday(&t_ini_transfer, NULL);
	}

	if (!silent)
		if (pthread_create(&thread_bar, NULL, (void * (*)(void *))progression_bar, NULL) != 0)
			fprintf(stderr, "Error in pthread_create of progression_bar\n");

	if (xpnsource)
		fds = xpn_open(source, O_RDONLY);
	else {
		// http://www.linuxquestions.org/questions/programming-9/mmap-tutorial-c-c-511265/
		fds = open(source, O_RDONLY);
		if (fds >= 0) {
			source_map = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fds, 0);
			if (source_map == MAP_FAILED) {
				close(fds);
				perror("Error mmapping the input file");
				exit(EXIT_FAILURE);
			}
		}
	}

	if(fds < 0) {
		perror("Error opening source");
		printf("Error opening source '%s': fd = %d\n", source, fds);
		exit(-1);
	}

	if (xpndest)
		fdd = xpn_open(dest, O_CREAT|O_WRONLY|O_TRUNC, 00644);
	else {
		// http://www.linuxquestions.org/questions/programming-9/mmap-tutorial-c-c-511265/
		fdd = open(dest, O_CREAT|O_RDWR|O_TRUNC|O_NONBLOCK|O_LARGEFILE, 00644);
		if (fdd >= 0) {
			lseek(fdd, st.st_size-1, SEEK_SET);
			ret = write(fdd, "", 1);
			lseek(fdd, 0, SEEK_SET);
			dest_map = mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdd, 0);
			if (dest_map == MAP_FAILED) {
				close(fdd);
				perror("Error mmapping the output file");
				exit(EXIT_FAILURE);
			}
		}
	}

	if(fdd < 0) {
		perror("Error opening dest");
		printf("Error opening dest '%s': fd = %d\n", dest, fdd);
		exit(-1);
	}

	if (xpnsource && xpndest) {
		array_bufs = malloc(2*(st.st_size/buffer_size+1)*sizeof(struct buffer_st));

		args.fdd = fdd;
		args.xpnsource = xpnsource;
		args.xpndest = xpndest;
		args.out_transfer_t = &out_transfer_t;

		pthread_create(&thread_write, NULL, (void * (*)(void *))write_func, &args);
	}

	if ((silent) && (!very_silent))
		printf("Starting transfer...\n");

	sum = 0;
	sum_r = 0;
	do {
		if ((st.st_size > 0) && (st.st_size-sum_r < (ssize_t)buffer_size))
			buffer_size = st.st_size-sum_r;

		if (debug)
			printf("Copying a block of %zu bytes\n", buffer_size);

		if (xpnsource && xpndest) {
			buf = malloc(sizeof(struct buffer_st));
			buf->buffer = (char *)malloc(buffer_size*sizeof(char));
		}

		if (!very_silent)
			gettimeofday(&t_ini_trans, NULL);

		if (xpnsource && xpndest) {
			// Expand is not reentrant
			pthread_mutex_lock(&xpn_mutex);
			nr = xpn_read(fds, buf->buffer, buffer_size);
			pthread_mutex_unlock(&xpn_mutex);
		} else if (xpnsource)
			nr = xpn_read(fds, &dest_map[sum_r], buffer_size);
		else if (xpndest)
			nr = xpn_write(fdd, &source_map[sum_r], buffer_size);
		else { // (!xpnsource && !xpndest)
			memcpy(&dest_map[sum_r], &source_map[sum_r], buffer_size);
			nr = buffer_size;
		}

		if (!very_silent) {
			gettimeofday(&t_end_trans, NULL);
			in_transfer_t += (t_end_trans.tv_sec-t_ini_trans.tv_sec)+(double)(t_end_trans.tv_usec-t_ini_trans.tv_usec)/1000000;
		}

		if (debug) {
				//fprintf(stderr, " read(%zu)    read block %zu (%p) of %zd bytes %u %u %u %u ... %u %u %u %u\n", buffer_size, (unsigned long)last, buf->buffer, nr, (unsigned char)buf->buffer[0], (unsigned char)buf->buffer[1], (unsigned char)buf->buffer[2], (unsigned char)buf->buffer[3], (unsigned char)buf->buffer[nr-4], (unsigned char)buf->buffer[nr-3], (unsigned char)buf->buffer[nr-2], (unsigned char)buf->buffer[nr-1]);
			if (xpnsource && xpndest)
				fprintf(stderr, " read(%lu)    read block %lu (%p) of %zd bytes\n", (unsigned long)buffer_size, (unsigned long)last, buf->buffer, nr);
			else
				fprintf(stderr, " read(%lu)    read block of %zd bytes\n", (unsigned long)buffer_size, nr);
		}

		if (nr > 0) {
			sum_r = sum_r + nr;

			if (xpnsource && xpndest) {
				buf->buffer_length = nr;
				pthread_mutex_lock(&mutex);
				array_bufs[last] = buf;
				last++;
				if (debug)
					printf("Added new item of %zd bytes\n", nr);
				pthread_cond_signal(&cond);
				pthread_mutex_unlock(&mutex);
				buf = NULL;
			} else {
				out_transfer_t += (t_end_trans.tv_sec-t_ini_trans.tv_sec)+(double)(t_end_trans.tv_usec-t_ini_trans.tv_usec)/1000000;
			}
		}
	} while (sum_r < st.st_size);
	//} while (((unsigned int)nr==buffer_size) && (sum_r < st.st_size));
	//last++;

	if (xpnsource)
		xpn_close(fds);
	else {
		if (munmap(source_map, st.st_size) == -1) {
			perror("Error un-mmapping the file");
			/* Decide here whether to close(fd) and exit() or not. Depends... */
		}
		close(fds);
	}

	if ((silent) && (!very_silent))
		printf("Transfer done\n");
	if (!very_silent)
		gettimeofday(&t_ini_trans, NULL);

	if (xpnsource && xpndest)
		pthread_join(thread_write, NULL);

	if ((silent) && (!very_silent))
		printf("Closing output file...");
	if (xpndest)
		xpn_close(fdd);
	else {
		if (munmap(dest_map, st.st_size) == -1) {
			perror("Error un-mmapping the file");
			/* Decide here whether to close(fd) and exit() or not. Depends... */
		}
		close(fdd);
	}
	if (!very_silent) {
		gettimeofday(&t_end_trans, NULL);
		out_transfer_t += (t_end_trans.tv_sec-t_ini_trans.tv_sec)+(double)(t_end_trans.tv_usec-t_ini_trans.tv_usec)/1000000;
	}
	if ((silent) && (!very_silent))
		printf(" done\n");

	if (!silent) {
		pthread_cancel(thread_bar);
		print_bar();
		printf("\n");
	}

	if (!very_silent) {
		in_transfer_bw  = sum/in_transfer_t;
		printf("\n");
		printf("Input transfer time = %.3f s\n", in_transfer_t);
		printf("Input transfer bandwidth = %.3f B/s = %.3f KB/s = %.3f MB/s\n", in_transfer_bw, in_transfer_bw/KB, in_transfer_bw/MB);

		out_transfer_bw = sum/out_transfer_t;
		printf("\n");
		printf("Output transfer time = %.3f s\n", out_transfer_t);
		printf("Output transfer bandwidth = %.3f B/s = %.3f KB/s = %.3f MB/s\n", out_transfer_bw, out_transfer_bw/KB, out_transfer_bw/MB);

		gettimeofday(&t_end_transfer, NULL);
		transfer_t = (t_end_transfer.tv_sec-t_ini_transfer.tv_sec)+(double)(t_end_transfer.tv_usec-t_ini_transfer.tv_usec)/1000000;
		transfer_bw = sum/transfer_t;
		printf("\n");
		printf("Total transfer time = %.3f s\n", transfer_t);
		printf("Total transfer bandwidth = %.3f B/s = %.3f KB/s = %.3f MB/s\n", transfer_bw, transfer_bw/KB, transfer_bw/MB);
	}

	if (isxpn) {
		xpn_destroy();
		if (!very_silent) {
			gettimeofday(&t_end_total, NULL);
			total_t = (t_end_total.tv_sec-t_ini_total.tv_sec)+(double)(t_end_total.tv_usec-t_ini_total.tv_usec)/1000000;
			total_bw = sum/total_t;
			printf("\n");
			printf("Total time = %.3f s\n", total_t);
			printf("Total bandwidth = %.3f B/s = %.3f KB/s = %.3f MB/s\n", total_bw, total_bw/KB, total_bw/MB);
		}
	}

	if (!very_silent)
		printf("\n");

	exit(0);
}

