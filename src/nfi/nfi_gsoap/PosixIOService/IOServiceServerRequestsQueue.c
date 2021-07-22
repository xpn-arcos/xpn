#include "soapH.h"
#include <pthread.h>

/*** BEGIN ***/
#include "th-lock.h"
#include <signal.h>             /* defines SIGPIPE */
/*** END ***/


#define BACKLOG (100) // Max. request backlog
#define MAX_THR (16) // Size of thread pool
#define MAX_QUEUE (1600) // Max. size of request queue

SOAP_SOCKET queue[MAX_QUEUE]; // The global request queue of sockets
int head = 0, tail = 0; // Queue head and tail

void *process_queue(void*);
int enqueue(SOAP_SOCKET);
SOAP_SOCKET dequeue();
pthread_mutex_t queue_cs;
pthread_cond_t queue_cv;

/*** BEGIN ***/
extern int debug;

void showUsage () {
   printf ("Usage: IOServiceServerRequestsQueue -p PortNumber [-z 1-9] [-s] [-a contenedor] [-c certificado] [-k clave] [v] [h]\n\n");
   printf ("\t-p: Puerto donde el servidor escucha peticiones de los clientes.\n");
   printf ("\t-z [1-9]: Activa la compresión gZip con nivel de compresión [1-9]\n");
   printf ("\t-s: Activa la autenticación y cifrado mediante SSL.\n");
   printf ("\t-a: Ruta al contenedor de certificados fiables (cacert.pem).\n");
   printf ("\t-c: Ruta al certificado del servidor (server.pem)\n");
   printf ("\t-k: Clave para acceder al certificado\n");
   printf ("\t-v: Mostrar mensajes durante la ejecución\n");
   printf ("\t-h: Este mensaje de ayuda\n");
   printf ("\n");
   exit (0);
}
/*** END ***/

int main(int argc, char **argv)
{
/*** BEGIN ***/
   int c;
   int port;			/** Port where the server will listen the request */
   int enableSSL;		/** Enables SSL auntenthication and encryption */
   int enableGZIP;		/** Enables GZIP compression */
   int compressionLevel;	/** GZIP compression level */
   int serverCertificateFlag;
   int serverPassFlag;
   int serverCacertFlag;
   char* serverCacert;		/** Trusted certificates container. "CA/cacert.pem" */
   char* serverCertificate;	/** Server certificate. "CA/server.pem" */
   char* serverPass;		/** Server pass certificate. "password" */
   struct soap soap;

   debug = opterr = port =  enableGZIP = enableSSL = compressionLevel = 0;
   serverCacertFlag = serverCertificateFlag = serverPassFlag = 0;
   serverCacert = serverCertificate = serverPass = NULL;

   while ((c = getopt (argc, argv, "p:z:sa:c:k:vh")) != -1)

      switch (c) {

         // Ayuda
         case 'h':
            showUsage();
            break;

         // Puerto de escucha
         case 'p':
            port = atoi (optarg);
            if ((port < 0) || (port > 65535)) {
               showUsage();
               exit(-1);
            }
            break;

         // Activa la compresión
         case 'z':
            enableGZIP = 1;
            compressionLevel = atoi (optarg);
            if ((compressionLevel < 1) || (compressionLevel > 9)) {
               showUsage();
               exit(-1);
            }
            break;

         // Activa el SSL
         case 's':
            enableSSL = 1;
            break;

         // Ruta al contenedor de certificados fiables
         case 'a':
            serverCacertFlag = 1;
            if (optarg != NULL) {
               serverCacert = (char*) malloc (strlen (optarg) +1);
               strcpy (serverCacert, optarg);
            }
            break;

         // Ruta al certificado del servidor
         case 'c':
            serverCertificateFlag = 1;
            if (optarg != NULL) {
               serverCertificate = (char*) malloc (strlen (optarg) +1);
               strcpy (serverCertificate, optarg);
            }
            break;

         // Clave para acceder al certificado del servidor
         case 'k':
            serverPassFlag = 1;
            if (optarg != NULL) {
               serverPass = (char*) malloc (strlen (optarg) +1);
               strcpy (serverPass, optarg);
            }
            break;

         case 'v':
            debug = 1;
            break;

         case '?':
            if (isprint (optopt)) {
               fprintf (stderr, "Unknown option `-%c'.\n", optopt);
               showUsage();
            } else {
               fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
               showUsage();
            }
            return 1;

         default:
            abort ();
      }

#ifdef WITH_OPENSSL	
   if (enableSSL) {
      // Need SIGPIPE handler on Unix/Linux systems to catch broken pipes:
      signal(SIGPIPE, sigpipe_handle);
      soap_ssl_init();
      if (CRYPTO_thread_setup()) {
         fprintf(stderr, "Cannot setup thread mutex\n");
         exit(1);
      }
   }
#endif

   soap_init2(&soap, SOAP_ENC_MTOM, SOAP_ENC_MTOM);
   //soap_init2(&soap, SOAP_IO_KEEPALIVE, SOAP_IO_KEEPALIVE);
   //signal(SIGPIPE, sigpipe_handle);
/*** END ***/
   //struct soap soap;
   //soap_init(&soap);
   if (argc < 2) // no args: assume this is a CGI application
   {
      soap_serve(&soap); // serve request, one thread, CGI style
      soap_destroy(&soap); // dealloc C++ data
      soap_end(&soap); // dealloc data and clean up
   }
   else
   {
/*** BEGIN ***/
      soap.send_timeout = 60; // 60 seconds
      soap.recv_timeout = 60; // 60 seconds
      //soap.accept_timeout = 3600; // server stops after 1 hour of inactivity
      soap.accept_timeout = 0; // no timeout
      soap.max_keep_alive = 100; // max keep-alive sequence
      void *process_request(void*);
      struct soap *tsoap = NULL;
#ifdef WITH_OPENSSL
      if (enableSSL) {
         /*
         if (soap_ssl_server_context(&soap,
                                     SOAP_SSL_DEFAULT,
                                     serverCertificate, // keyfile: required when server must authenticate to clients (see SSL docs on now to obtain this file)
                                     serverPass,        // password to read the key file
                                     serverCacert,      // optional cacert file to store trusted certificates
                                     NULL,              // optional capath to directory with trusted certificates
                                     NULL,              // DH file, if NULL use RSA
                                     NULL,              // if randfile!=NULL: use a file with random data to seed randomness
                                     NULL))             // optional server identification to enable SSL session cache (must be a unique name)
         */
         if (soap_ssl_server_context(&soap,
                                     SOAP_SSL_DEFAULT,   // use SOAP_SSL_REQUIRE_CLIENT_AUTHENTICATION to verify clients: client must provide a key file e.g. "client.pem" and "password"
                                     "certs/server.pem", // keyfile: see SSL docs on how to obtain this file
                                     "password",         // password to read the key file
                                     "certs/cacert.pem", // cacert file to store trusted certificates (to authenticate clients)
                                     NULL,               // capath
                                     "certs/dh512.pem",  // DH file, if NULL use RSA
                                     NULL,               // if randfile!=NULL: use a file with random data to seed randomness
                                     "sslserver"         // server identification for SSL session cache (unique server name, e.g. use argv[0])
            ))
         {
            soap_print_fault(&soap, stderr);
            exit(1);
         }
      }
#endif

#ifdef WITH_GZIP
      if (enableGZIP) {
         soap_set_omode (&soap, SOAP_ENC_ZLIB);
         soap.z_level = compressionLevel;
      }
#endif
/*** END ***/
      struct soap *soap_thr[MAX_THR]; // each thread needs a runtime environment
      pthread_t tid[MAX_THR];
      //int port = atoi(argv[1]); // first command-line arg is port
      SOAP_SOCKET m, s;
      int i;
      m = soap_bind(&soap, NULL, port, BACKLOG);
/*** BEGIN ***/
      while (!soap_valid_socket(m)) {
         fprintf(stderr, "Retrying bind on port %d\n", port);
         sleep(1);
         m = soap_bind(&soap, NULL, port, BACKLOG);
      }
/*** END ***/
      //if (!soap_valid_socket(m))
      //   exit(1);
      fprintf(stderr, "Socket connection successful %d\n", m);
/*** BEGIN ***/
      printf ("\nServer running and listening on port %d\n", port);

      if (enableGZIP)
         printf ("\tGZIP is enabled with a compression level of %d\n\n", compressionLevel);
      else
         printf ("\tGZIP is disabled\n\n");

      if (enableSSL){
         printf ("\tSSL is enabled\n");
         printf ("\t  Server certificate = %s\n", serverCertificate);
         printf ("\t  Server password = %s\n", serverPass);
         printf ("\t  Server Cacert = %s\n", serverCacert);
      } else
         printf ("\tSSL is disabled\n");
/*** END ***/
      pthread_mutex_init(&queue_cs, NULL);
      pthread_cond_init(&queue_cv, NULL);
      for (i = 0; i < MAX_THR; i++)
      {
         soap_thr[i] = soap_copy(&soap);
         fprintf(stderr, "Starting thread %d\n", i);
/*** BEGIN ***/
#ifdef WITH_OPENSSL
         if (enableSSL) {
            if (soap_ssl_accept(tsoap)) {
               soap_print_fault(tsoap, stderr);
               soap_free(tsoap);
               continue; /* when soap ssl accept fails, we should just go on */
            }
         }
#endif
/*** END ***/
         pthread_create(&tid[i], NULL, (void*(*)(void*))process_queue, (void*)soap_thr[i]);
      }
      for (;;)
      {
         s = soap_accept(&soap);
         if (!soap_valid_socket(s))
         {
            if (soap.errnum)
            {
               soap_print_fault(&soap, stderr);
               continue; // retry
            }
            else
            {
               fprintf(stderr, "Server timed out\n");
               break;
            }
         }
         if (debug) fprintf(stderr, "Thread %d accepts socket %d connection from IP %lu.%lu.%lu.%lu\n", i, s, (soap.ip >> 24)&0xFF, (soap.ip >> 16)&0xFF, (soap.ip >> 8)&0xFF, soap.ip&0xFF);
         while (enqueue(s) == SOAP_EOM)
            sleep(1);
      }
      for (i = 0; i < MAX_THR; i++)
      {
         while (enqueue(SOAP_INVALID_SOCKET) == SOAP_EOM)
            sleep(1);
      }
      for (i = 0; i < MAX_THR; i++)
      {
         fprintf(stderr, "Waiting for thread %d to terminate... ", i);
         pthread_join(tid[i], NULL);
         fprintf(stderr, "terminated\n");
         soap_done(soap_thr[i]);
         free(soap_thr[i]);
      }
      pthread_mutex_destroy(&queue_cs);
      pthread_cond_destroy(&queue_cv);
   }
   soap_done(&soap);
#ifdef WITH_OPENSSL	
   if (enableSSL) {
      CRYPTO_thread_cleanup();
   }
#endif
   return 0;
}

void *process_queue(void *soap)
{
   struct soap *tsoap = (struct soap*)soap;
   for (;;)
   {
      tsoap->socket = dequeue();
      if (!soap_valid_socket(tsoap->socket))
         break;
      soap_serve(tsoap);
      soap_destroy(tsoap);
      soap_end(tsoap);
      if (debug) fprintf(stderr, "served\n");
   }
   return NULL;
}

int enqueue(SOAP_SOCKET sock)
{
   int status = SOAP_OK;
   int next;
   pthread_mutex_lock(&queue_cs);
   next = tail + 1;
   if (next >= MAX_QUEUE)
      next = 0;
   if (next == head)
      status = SOAP_EOM;
   else
   {
      queue[tail] = sock;
      tail = next;
   }
   pthread_cond_signal(&queue_cv);
   pthread_mutex_unlock(&queue_cs);
   return status;
}

SOAP_SOCKET dequeue()
{
   SOAP_SOCKET sock;
   pthread_mutex_lock(&queue_cs);
   while (head == tail)       pthread_cond_wait(&queue_cv, &queue_cs);
   sock = queue[head++];
   if (head >= MAX_QUEUE)
      head = 0;
   pthread_mutex_unlock(&queue_cs);
   return sock;
}
