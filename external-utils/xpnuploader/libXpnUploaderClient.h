#ifndef _LIBXPNUPLOADERCLIENT_H_
#define _LIBXPNUPLOADERCLIENT_H_

#include "soapStub.h"
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
struct ioservice *xpnuploader_ws_init (	char *server,
					int port,
					int GZIPcompression,
					int enableSSL,
					char *newClientCacert,
					char *newClientCertificate,
					char *newClientPass,
					int debug);

void xpnuploader_ws_end(struct ioservice *ioservice);

void xpnuploader_ws_activate_debug(struct ioservice *ioservice);

void xpnuploader_ws_deactivate_debug(struct ioservice *ioservice);

int xpnuploader_ws_upload(struct ioservice *ioservice, const char *source, const char *dest, const char *partition);

#ifdef __cplusplus
}
#endif

#endif // _LIBXPNUPLOADERCLIENT_H_
