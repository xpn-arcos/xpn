#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "project/posix.nsmap"
#include "libIOServiceClient.h"

void sigpipe_handle(int x) { }

struct ioservice *posix_ws_init (	char *server,
					int port,
					int enableGZIP,
					int enableSSL,
					char *newClientCacert,
					char *newClientCertificate,
					char *newClientPass,
					int retries,
					int debug) {
	
	int longServer;
	struct ioservice *ioservice;
	
	if ((server == NULL) || (strlen(server) <= 0)) {
		fprintf(stderr, "server is NULL or has non-positive length");
		return NULL;
	}
	
	ioservice = (struct ioservice *)malloc(sizeof(struct ioservice));
	memset(ioservice, 0, sizeof(struct ioservice));
	
	//ioservice->soap = (struct soap *)malloc(sizeof(struct soap));
	//memset(ioservice->soap, 0, sizeof(struct soap));
//printf("malloc(%p)\n", ioservice->soap);
	
	ioservice->retries = retries;
	
	ioservice->debug = debug;
	
	if (ioservice->debug)
		printf ("Client: posix_ws_init(%s, %d, %d, %d, %s, %s, %s, %d, %d)\n", server, port, enableSSL, enableGZIP, newClientCertificate, newClientPass, newClientCacert, retries, debug);
	
	if (enableSSL > 0)
		ioservice->useSSL = 1;
	else
		ioservice->useSSL = 0;
	
	if ((enableGZIP >= 1) && (enableGZIP <= 9))
		ioservice->useGZIP = enableGZIP;
	else
		ioservice->useGZIP = 0;
	
#ifdef WITH_OPENSSL
	if (ioservice->useSSL) {
		// Need SIGPIPE handler on Unix/Linux systems to catch broken pipes:
		signal(SIGPIPE, sigpipe_handle);
		soap_ssl_init(); // init OpenSSL (just once)
	}
#endif
	
	//ioservice->soap = soap_new();
	//soap_init(ioservice->soap);
	ioservice->soap = soap_new1(SOAP_ENC_MTOM);
	
#ifdef WITH_OPENSSL
	if (ioservice->useSSL){
		if (newClientCertificate != NULL) {
			//ioservice->clientCertificate = (char *)malloc(strlen(newClientCertificate)+1);
			//strcpy (ioservice->clientCertificate, newClientCertificate);
			ioservice->clientCertificate = strdup(newClientCertificate);
		}
		
		if (newClientPass != NULL) {
			//ioservice->clientPass = (char *)malloc(strlen(newClientPass)+1);
			//strcpy (ioservice->clientPass, newClientPass);
			ioservice->clientPass = strdup(newClientPass);
		}
		
		if (newClientCacert != NULL) {
			//ioservice->clientCacert = (char *)malloc(strlen(newClientCacert)+1);
			//strcpy (ioservice->clientCacert, newClientCacert);
			ioservice->clientCacert = strdup(newClientCacert);
		}
		
		/*
		if (soap_ssl_client_context(	ioservice->soap,
						SOAP_SSL_DEFAULT,
						ioservice->clientCertificate, // keyfile: required only when client must authenticate to server (see SSL docs on how to obtain this file)
						ioservice->clientPass, // password to read the key file
						ioservice->clientCacert, // cacert file to store trusted certificates (needed to verify server)
						NULL, // capath to direcoty with trusted certificates
						NULL)) // if randfile!=NULL: use a file with random data to seed randomness
		*/
		if (soap_ssl_client_context(	ioservice->soap,
						SOAP_SSL_DEFAULT | SOAP_SSL_SKIP_HOST_CHECK, // use SOAP_SSL_DEFAULT in production code, we don't want the host name checks since these will change from machine to machine
						"certs/client.pem"/*NULL*/, // keyfile: required only when client must authenticate to server (see SSL docs on how to obtain this file)
						"password"/*NULL*/, // password to read the keyfile
						"certs/cacert.pem", // optional cacert file to store trusted certificates, use cacerts.pem for all public certificates issued by common CAs
						NULL, // optional capath to directory with trusted certificates
						NULL  // if randfile!=NULL: use a file with random data to seed randomness
		   ))
		{
			
			soap_print_fault(ioservice->soap, stderr);
			return NULL;
		}
	}
#endif
	
#ifdef WITH_GZIP
	if (ioservice->useGZIP > 0) {
		soap_set_omode(ioservice->soap, SOAP_ENC_ZLIB);
		ioservice->soap->z_level = ioservice->useGZIP;
	}
#endif
	
	longServer = strlen(server) + 7;// 7 = ':' + max port length(=5) + '\0'
	ioservice->server = (char *)malloc(longServer);
	bzero (ioservice->server, longServer);
	sprintf(ioservice->server, "%s:%d", server, port);
	
	if (ioservice->debug) {
		printf ("Initialized Web Service client side. DefaultServer = [%s]\n\n", ioservice->server);
#ifdef WITH_GZIP
		if (enableGZIP)
			printf ("\tGZIP is enabled with a compression level of %d\n\n", ioservice->useGZIP);
		else
			printf ("\tGZIP is disabled\n\n");
#endif
#ifdef WITH_OPENSSL
		if (enableSSL) {
			printf ("\tSSL is enabled\n");
			printf ("\t  Client certificate = %s\n", ioservice->clientCertificate);
			printf ("\t  Client password = %s\n", ioservice->clientPass);
			printf ("\t  Client Cacert = %s\n\n", ioservice->clientCacert);
		} else {
			printf ("\tSSL is disabled\n\n");
		}
#endif
	}
	
	return ioservice;
}

void posix_ws_end(struct ioservice *ioservice) {
	if (ioservice != NULL) {
		if (ioservice->debug)
			printf ("Ended Web Service client side\n");
		
		if (ioservice->soap != NULL) {
			soap_destroy(ioservice->soap); // dealloc C++ data
			soap_end(ioservice->soap); // dealloc data and clean up
			soap_done(ioservice->soap); // detach soap struct
//printf("free(%p)\n", ioservice->soap);
			free(ioservice->soap);
		}
		
		if (ioservice->server != NULL)
			free(ioservice->server);
		
		if (ioservice->clientCertificate != NULL)
			free(ioservice->clientCertificate);
		
		if (ioservice->clientPass != NULL)
			free(ioservice->clientPass);
		
		if (ioservice->clientCacert != NULL)
			free(ioservice->clientCacert);
		
		free(ioservice);
	}
}

void posix_ws_activate_debug(struct ioservice *ioservice) {
	ioservice->debug = 1;
}

void posix_ws_deactivate_debug(struct ioservice *ioservice) {
	ioservice->debug = 0;
}

int posix_ws_open(struct ioservice *ioservice, const char *pathname, int flags, ...) {
	va_list ap;
	mode_t mode;
	int retries = 0;
	int result;
	
	mode = 0;
	if ((flags & O_CREAT) > 0) {
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
		if (mode > 0177777) {
			fprintf (stderr, "posix_ws_open: mode = %d invalid\n", mode);
			return -1;
		}
	}
	
	if (ioservice == NULL) {
		fprintf (stderr, "posix_ws_open: ioservice = NULL");
		return -1;
	}

	if (ioservice->soap == NULL) {
		fprintf (stderr, "posix_ws_open: ioservice->soap = NULL");
		return -1;
	}

	if (ioservice->server == NULL) {
		fprintf (stderr, "posix_ws_open: ioservice->server = NULL");
		return -1;
	}

	if (pathname == NULL) {
		fprintf (stderr, "posix_ws_open: pathname = NULL");
		errno = ENOENT;
		return -1;
	}

	if (ioservice->debug)
		printf ("Client: posix_ws_open(%s, %o, %o)", pathname, flags, mode);
	
	do {
		soap_call_ns1__open(ioservice->soap, ioservice->server, NULL, (char *)pathname, flags, mode, &result);
	} while ((retries++ < ioservice->retries) && (ioservice->soap->error));
	
	if (ioservice->soap->error){
		soap_print_fault(ioservice->soap, stderr);
		result = -1;
	} else if (result < 0) {
		fprintf(stderr, "posix_ws_open: Fail open %s in server %s with return code=%d\n", pathname, ioservice->server, result);
		errno = -result;
		result = -1;
	}
	
	if (ioservice->debug)
		printf (" = %d\n", result);
	
	return result;
}

int posix_ws_close(struct ioservice *ioservice, int fd) {
	int retries = 0;
	int result;
	
	if (ioservice->debug)
		printf ("Client: posix_ws_close(%d)", fd);
	
	do {
		soap_call_ns1__close(ioservice->soap, ioservice->server, NULL, fd, &result);
	} while ((retries++ < ioservice->retries) && (ioservice->soap->error));
	
	if (ioservice->soap->error){
		soap_print_fault(ioservice->soap, stderr);
		result = -1;
	}
	
	if (ioservice->debug)
		printf (" = %d\n", result);
	
	return result;
}

ssize_t posix_ws_read(struct ioservice *ioservice, int fd, void *buf, size_t count) {
	int retries = 0;
	struct ns1__readResponse readRes; // FIXME: hay que reservar memoria para readRes.buf.__ptr?
	
	if (ioservice->debug)
		printf ("Client: posix_ws_read(%d, buf, %d)", fd, count);
	
	do {
		soap_call_ns1__read(ioservice->soap, ioservice->server, NULL, fd, count, &readRes);
	} while ((retries++ < ioservice->retries) && (ioservice->soap->error));
	
	if (ioservice->soap->error){
		soap_print_fault(ioservice->soap, stderr);
		readRes.result = -1;
	}
	
	if (ioservice->debug) {
		printf (" = %d\n", readRes.result);
#ifdef WITH_GZIP
		if (ioservice->useGZIP)
			printf("Compression ratio: %f%% (in) %f%% (out)\n", 100*ioservice->soap->z_ratio_in, 100*ioservice->soap->z_ratio_out);
#endif
	}
	
	if (ioservice->soap->error){
		return readRes.result;
	}
	
	memcpy(buf, readRes.buf.xop__Include.__ptr, readRes.buf.xop__Include.__size);
	
	return readRes.result;
}

ssize_t posix_ws_write(struct ioservice *ioservice, int fd, const void *buf, size_t count) {
	int retries = 0;
	int result = -1;
	//struct xsd__base64Binary b64B; // FIXME: (como) hay que reservar memoria para b64B.__ptr?
	ns__t_buf_param mybuf;
	
	if (ioservice->debug)
		printf ("Client: posix_ws_write(%d, buf, %d)", fd, count);
	
	/*
	memset(&b64B, 0, sizeof(struct xsd__base64Binary));
	b64B.__ptr = (unsigned char *)buf;
	//b64B.__ptr = (unsigned char *)soap_malloc(ioservice->soap, count);
	b64B.__size = count;
	//memcpy(b64B.__ptr, buf, count);
	*/
	//b64B.__ptr = NULL;
	//b64B.__size = 0;
	
	//soap_set_mime(ioservice->soap, NULL, NULL);
	//soap_set_dime(ioservice->soap);
	/*
	b64B.id = NULL;
	b64B.type = "application/octet-stream";
	b64B.options = NULL;
	
	soap_set_mime_attachment(ioservice->soap, (char *)buf, count, SOAP_MIME_BINARY, "application/octet-stream", NULL, NULL, NULL);
	*/
	
	memset(&mybuf, 0, sizeof(ns__t_buf_param));
	mybuf.xop__Include.__ptr = (unsigned char*)buf;
	mybuf.xop__Include.__size = count;
	mybuf.xop__Include.id = NULL;
	mybuf.xop__Include.type = "application/octet-stream"; // MIME type
	mybuf.xop__Include.options = NULL;
	mybuf.xmlmime__contentType = "application/octet-stream"; // MIME type
	
	do {
		soap_call_ns1__write(ioservice->soap, ioservice->server, NULL, fd, /*b64B*/mybuf, count, &result);
	} while ((retries++ < ioservice->retries) && (ioservice->soap->error));
	
	if (ioservice->soap->error){
		soap_print_fault(ioservice->soap, stderr);
		result = -1;
	}
	
	if (ioservice->debug) {
		printf (" = %d\n", result);
#ifdef WITH_GZIP
		if (ioservice->useGZIP)
			printf("Compression ratio: %f%% (in) %f%% (out)\n", 100*ioservice->soap->z_ratio_in, 100*ioservice->soap->z_ratio_out);
#endif
	}
	
	return result;
}

off_t posix_ws_lseek(struct ioservice *ioservice, int fd, off_t offset, int whence) {
	int retries = 0;
	long long result;
	
	if (ioservice->debug)
		printf ("Client: posix_ws_lseek(%d, %lld, %d)", fd, offset, whence); // For 32-bits
		//printf ("Client: posix_ws_lseek(%d, %ld, %d)", fd, offset, whence); // For 64-bits
	
	do {
		soap_call_ns1__lseek(ioservice->soap, ioservice->server, NULL, fd, offset, whence, &result);
	} while ((retries++ < ioservice->retries) && (ioservice->soap->error));
	
	if (ioservice->soap->error){
		soap_print_fault(ioservice->soap, stderr);
		result = -1;
	}
	
	if (ioservice->debug)
		printf (" = %lld\n", result);
	
	return result;
}

int posix_ws_stat(struct ioservice *ioservice, const char *path, struct stat *buf) {
	int retries = 0;
	struct ns1__statResponse statRes; // FIXME: hay que reservar memoria para readRes.buf.__ptr?
	
	if (ioservice->debug)
		printf ("Client: posix_ws_stat(%s, buf)", path);
	
	do {
		soap_call_ns1__stat(ioservice->soap, ioservice->server, NULL, (char *)path, &statRes);
	} while ((retries++ < ioservice->retries) && (ioservice->soap->error));
	
	if (ioservice->soap->error){
		soap_print_fault(ioservice->soap, stderr);
		statRes.result = -1;
	}
	
	if (ioservice->debug)
		printf (" = %d\n", statRes.result);
	
	if (ioservice->debug) {
		printf ("sizeof(struct stat) 		= %d\n", sizeof(struct stat));
		printf ("sizeof(struct ns1__structStat)	= %d\n", sizeof(struct ns1__structStat));
	}
	
	if (ioservice->soap->error){
		return statRes.result;
	}
	
	//memcpy(buf, statRes.buf, sizeof(struct stat)); //FIXME: a lo mejor hay que copiar los campos uno por uno
	memset(buf, 0, sizeof(struct stat));
	
	buf->st_dev 	= statRes.buf->st_dev;
	buf->st_ino 	= statRes.buf->st_ino;
	buf->st_mode 	= statRes.buf->st_mode;
	buf->st_nlink 	= statRes.buf->st_nlink;
	buf->st_uid 	= statRes.buf->st_uid;
	buf->st_gid 	= statRes.buf->st_gid;
	buf->st_rdev 	= statRes.buf->st_rdev;
	buf->st_size 	= statRes.buf->st_size;
	buf->st_blksize = statRes.buf->st_blksize;
	buf->st_blocks 	= statRes.buf->st_blocks;
	buf->st_atime 	= statRes.buf->st_atim;
	buf->st_mtime 	= statRes.buf->st_mtim;
	buf->st_ctime 	= statRes.buf->st_ctim;
	
	return statRes.result;
}
