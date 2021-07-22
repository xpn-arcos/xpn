#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "project/xpnuploader.nsmap"
#include "libXpnUploaderClient.h"

void sigpipe_handle(int x) { }

struct ioservice *xpnuploader_ws_init (	char *server,
					int port,
					int enableGZIP,
					int enableSSL,
					char *newClientCacert,
					char *newClientCertificate,
					char *newClientPass,
					int debug) {
	
	int longServer;
	struct ioservice *ioservice = NULL;
	
	if ((server == NULL) || (strlen(server) <= 0)) {
		fprintf(stderr, "server is NULL or has non-positive length");
		return ioservice;
	}
	
	ioservice = (struct ioservice *)malloc(sizeof(struct ioservice));
	memset(ioservice, 0, sizeof(struct ioservice));
	
	ioservice->soap = (struct soap *)malloc(sizeof(struct soap));
	memset(ioservice->soap, 0, sizeof(struct soap));
	
	ioservice->debug = debug;
	
	if (ioservice->debug)
		printf ("Client: posix_ws_init(%s, %d, %d, %d, %s, %s, %s, %d)\n", server, port, enableSSL, enableGZIP, newClientCertificate, newClientPass, newClientCacert, debug);
	
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
	soap_init(ioservice->soap);
	
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
			exit(1);
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

void xpnuploader_ws_end(struct ioservice *ioservice) {
	if (ioservice != NULL) {
		if (ioservice->debug)
			printf ("Ended Web Service client side\n");
		
		if (ioservice->soap != NULL) {
			soap_destroy(ioservice->soap); // dealloc C++ data
			soap_end(ioservice->soap); // dealloc data and clean up
			soap_done(ioservice->soap); // detach soap struct
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

void xpnuploader_ws_activate_debug(struct ioservice *ioservice) {
	ioservice->debug = 1;
}

void xpnuploader_ws_deactivate_debug(struct ioservice *ioservice) {
	ioservice->debug = 0;
}

int xpnuploader_ws_upload(struct ioservice *ioservice, const char *source, const char *dest, const char *partition) {
	int result;
	
	if (ioservice->debug)
		printf ("Client: xpnuploader_ws_upload(%s, %s, %s)", source, dest, partition);
	
	soap_call_ns1__upload(ioservice->soap, ioservice->server, NULL, (char *)source, (char *)dest, (char *)partition, &result);
	
	if (ioservice->debug)
		printf (" = %d\n", result);
	
	if (ioservice->soap->error){
		soap_print_fault(ioservice->soap, stderr);
		exit(-1);
	}
	
	return result;
}
