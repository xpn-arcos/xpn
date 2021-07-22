#ifndef _LIBIOSERVICECLIENT_H_
#define _LIBIOSERVICECLIENT_H_

#include "project/soapStub.h"
#include <unistd.h>             /* defines _POSIX_THREADS if pthreads are available */
#if defined(_POSIX_THREADS) || defined(_SC_THREADS)
#include <pthread.h>
#endif
#include <signal.h>             /* defines SIGPIPE */
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif


/** 
 * Structure that defines a connection with the server
 * This data structure is associated to one communication between the user application and the service.
 */
struct ioservice {
	struct soap *soap;
	char *server;			/**< Server location */
	int useSSL;			/**< Flag used to enable/disable SSL */
	int useGZIP;			/**< Flag used to enable/disable GZIP */
	char *clientCertificate;	/**< Client certificate */
	char *clientPass;		/**< Password to obtain the client certificate*/
	char *clientCacert;		/**< Trusted certificate container*/
	int retries;
	int debug;
};

/**
 * Initializes the environment
 * 
 * @param server Server location where the service is hosted. 
 * @param port Just the port to establish communication with the service
 * @param enableSSL To enable/disable SSL 
 * @param GZIPcompression To enable/disable GZIP compression
 * @param newClientCertificate Client certificate
 * @param newClientPass Client pass certificate 
 * @param newClientCacert Truster client certificates container
 * @return Structure to define a service connection
 */
struct ioservice *posix_ws_init (	char *server,
					int port,
					int GZIPcompression,
					int enableSSL,
					char *newClientCacert,
					char *newClientCertificate,
					char *newClientPass,
					int retries,
					int debug);

void posix_ws_end(struct ioservice *ioservice);

void posix_ws_activate_debug(struct ioservice *ioservice);

void posix_ws_deactivate_debug(struct ioservice *ioservice);

int posix_ws_open(struct ioservice *ioservice, const char *pathname, int flags, ...);

int posix_ws_close(struct ioservice *ioservice, int fd);

ssize_t posix_ws_read(struct ioservice *ioservice, int fd, void *buf, size_t count);

ssize_t posix_ws_write(struct ioservice *ioservice, int fd, const void *buf, size_t count);

off_t posix_ws_lseek(struct ioservice *ioservice, int fd, off_t offset, int whence);

int posix_ws_stat(struct ioservice *ioservice, const char *path, struct stat *buf);

#ifdef __cplusplus
}
#endif

#endif // _LIBIOSERVICECLIENT_H_
