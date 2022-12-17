
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include "mpi.h"


#define USECPSEC 1000000

#define LFNAME 128
#define KB 1024
#define MB (KB*KB)

#define LBUFMIN (128)
#define LBUFMAX (2*MB)
//int LBufMax = 2*MB;
#define TAMFILE (10*MB)
#define TRUE 1
#define FALSE 0

#define TASA_TRANSF(t) (t)

char *  TAKE_SAMPLE_DIR;
char buffer_basura[512*KB];
char buffer_esc[LBUFMAX];
//char *buffer_esc;
char buffer_lec[LBUFMAX];
//char *buffer_lec;
int errno;
char str[200];
int n_oper = 0;



//#define MACHINE "cacau2.hww.de"
#define MAQUINA "172.18.9.206"
#define PUERTO    9224
int fdsocket;



MPI_File fh;
MPI_Status status;

/*
 * If you want, you can use this function to get the parameters:
 *                    -dir : path of the work file.
 *                    -miid
 *                    -n_cli: number of virtual clients.
 *                    -niter: numero of iterations.
 */
static int GetCheckArgs(int argc, char **argv, char *dir, int *cid, int *ncid)
{
    if (argc==2)
    {
       strcpy(dir,argv[1]);
       (*cid)=0;
       (*ncid)=1;
       return(1);
    }
    else if (argc==4)
    {
       strcpy(dir,argv[1]);
       (*cid)=atoi(argv[2]);
       (*ncid)=atoi(argv[3]);
       return(1);
    }

    printf("Uso: %s Dir; tu das %d\n", argv[0], argc );
    exit(1);
}



void open_socket(){
	int flag =1, flag_len = sizeof(flag);
    struct hostent *hp;
	struct sockaddr_in server_addr;
	int ret;	
	
	
	fdsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (setsockopt (fdsocket, IPPROTO_TCP,
		        TCP_NODELAY, &flag, sizeof(flag)) == -1)
		return;
	
	
	hp = gethostbyname (MAQUINA);
    if(hp == NULL){
		fprintf(stderr,"error gethostbyname (%s,%d)\n",
						               MAQUINA, PUERTO);
		return;
	}

    bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
    memcpy (&(server_addr.sin_addr), hp->h_addr, hp->h_length);
	
	server_addr.sin_port = htons(PUERTO);

	printf("antes de connect\n");
	ret = connect(fdsocket, (struct sockaddr *) &server_addr,
							sizeof(server_addr));

	if(ret == -1){
		fprintf(stderr,"error connect (%s,%d)\n",
                                       MAQUINA, PUERTO);
	}
	printf("despues de connect\n");
	
}

void close_socket(){
	printf("close_socket\n");
	close(fdsocket);
}

void send_socket(char *name){
	printf("send_socket\n");
	write(fdsocket, name, strlen(name)+1);
}

void recive_socket(){
	char buff[10];
	printf("recive_socket\n");
	read(fdsocket, buff, 5);
}


/*
 * Write TAMFILE bytes in blocks of lb bytes.
 */
static void ForwWriting
(
	int cid, int ncid,
	int f, int lb, char *buf,
	double *tim
)
{
    int iter,ret;
    double ti, tf;
    int offset;

	char s[1024],fname2[LFNAME];


    offset = 0;
    offset=cid*lb;
    iter= TAMFILE/lb;
    iter= iter/ncid;
	//printf("iter = %d ncid = %d\n",iter, ncid);
    //Timer(&ti);
	ti = MPI_Wtime();
    for ( ; iter>0; iter--)
    {

#ifdef XPN
       if (xpn_lseek(f,offset,SEEK_SET) < 0)
	       perror("ERROR EN XPN_LSEEK\n");
#elif MPI
       MPI_File_seek(fh, offset, MPI_SEEK_SET);
#else
       lseek(f,offset,SEEK_SET)
#endif

       memset(buf, 'a', lb);
       ret = -1;
#ifdef XPN
       if ((ret = xpn_write(f,buf,lb))!= lb)
#elif MPI
       if(MPI_File_write(fh, buf, lb, MPI_CHAR, &status) <0)
#else
       if ((ret = write(f,buf,lb))!= lb)
#endif
       {
          sprintf(str, "IOC.ForwWriting: Error en escritura  errno = %d\n", ret);
	  		printf(str);
          exit(1);
       }

//	   printf("%d> write off %d size %d \n",cid,offset,lb);
       offset = offset + ncid*lb;
       //offset = offset + lb;

    }

#ifdef RECOLECT		
   		MPI_Barrier(MPI_COMM_WORLD);
		if(cid == 0){
			/* Recopilacion de los datos */
			sprintf (fname2, "%s.%d.%d", "IOP", ncid, lb);
			//sprintf(s,"ssh cacau2.hww.de /cacau/HLRS/HLRS/hpcluis/XPN/rec/gather.exe /PNFS/%s /mscratch/ws/hpcluis-SimulatesomeThing-0/%s &>/cacau/HLRS/HLRS/hpcluis/RESULTS/SALIDA-gather.txt",fname2,fname2);
			//printf("begin %s\n",);
			send_socket(fname2);
			recive_socket();
			//printf("end %s\n",s);
			
		}
		MPI_Barrier(MPI_COMM_WORLD);
#endif		
	
    //Timer(&tf);
	tf = MPI_Wtime();
    //DiffTime(&ti, &tf, tim);
	*tim = tf - ti;
}

/*
 * Read TAMFILE bytes in blocks of lb bytes and then compares it with
 * Bufe.
 */
static void ForwReading
(
	int cid, int ncid,
	int f, int lb, char *bufl, char *bufe,
	double *tim
)
{
    int iter;
    double ti, tf;
    int offset;
    int count;

    offset = 0;
    offset=cid*lb;
    iter= TAMFILE/lb;
    iter= iter/ncid;

    //Timer(&ti);
	ti = MPI_Wtime();
    for ( ; iter>0; iter--)
    {

#ifdef XPN
       if (xpn_lseek(f,offset,SEEK_SET) < 0)
	       perror("ERROR EN XPN_LSEEK\n");
#elif MPI
       MPI_File_seek(fh, offset, MPI_SEEK_SET);
#else
       lseek(f,offset,SEEK_SET)
#endif

        memset(bufl, 'a', lb);
        ret = -1;
	count = 0;
#ifdef XPN
	if ( (count = xpn_read(f, bufl, lb)) != lb )
#elif MPI
	if ( MPI_File_read(fh, bufl, lb, MPI_CHAR, &status) <0 )
#else
	if ( (count = read(f, bufl, lb)) != lb )
#endif
	{
		printf(str, "IOC.ForwReading: Read %d bytes (%d expected)\n", count, lb);
		exit(1);
	}
//		printf("%d> read off %d size %d \n",cid,offset,lb);
        offset = offset + ncid*lb;
        //offset = offset + lb;
    }

    //Timer(&tf);
	tf = MPI_Wtime();
    //DiffTime(&ti, &tf, tim);
	*tim = tf - ti;
}

/*
 * This function is similar to ForwReading, but here the reads are
 * beginning in the end of the file.
 */
static void BackwReading
(
	int cid, int ncid,
	int f, int lb, char *bufl, char *bufe,
	double *tim
)
{
  /*
	double ti, tf;
    int iter;
    int offset;

    offset=TAMFILE;
    iter= TAMFILE/lb;

    Timer(&ti);

    for(;iter>0;iter--){

       offset = offset - lb;

       xpn_lseek(f,offset,SEEK_SET) ;

       memset(bufl, 'a', lb);

       if (xpn_read(f,bufl,lb)!= lb)
       {
          sprintf(str, "error IOC.BackReading: Error en lectura  errno = %d\n", errno);
	  printf(str);
          exit(1);
       }
    }

    Timer(&tf);
    DiffTime(&ti, &tf, tim);
*/
}

/*
 * This function creats a new file, and is performanced a Writing,
 * ForwReading and BackReading function.
 */
static void TakeSample
(
	int cid, int ncid,
	int lbuf, char *dir,
	double *timew,
	double *timefr,
	double *timebr,
	int type
)
{
    int f;
    char fname[LFNAME];
	MPI_Info info;
	

    dir=TAKE_SAMPLE_DIR;
	
    //sprintf (fname, "xpn:/%s/%s.%d.%d.%d", dir, "IOP", ncid, cid, lbuf);
    //sprintf (fname, "xpn:/%s/%s.%d.%d", dir, "IOP", ncid, lbuf);

	sprintf (fname, "%s/%s.%d.%d", dir, "IOP", ncid, lbuf);
    //if (( f=xpn_open(fname,O_CREAT|O_RDWR,0777)) < 0)

   /* lo abren todos */
   if (type == 0)
   {
    	//printf ("antes del open -*%s*-",fname);

	f = -1;
#ifdef XPN
	if ((f=xpn_open(fname,O_CREAT|O_RDWR,0777)) < 0)
#elif MPI
   	if (MPI_File_open(MPI_COMM_SELF, fname, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh) <0)
#else
	if ((f=open(fname,O_CREAT|O_RDWR,0777)) < 0)
#endif
   	{
    		sprintf(str, "IOC.TakeSample: failed to create file %s\n", fname);
    		printf(str);
    	}
    	printf("%d > creo %s\n", cid, fname);

	printf("%d > escribiendo %s\n", cid, fname);
	ForwWriting(cid, ncid, f, lbuf, buffer_esc, timew);
	MPI_Barrier(MPI_COMM_WORLD);
   }
/*
   if(type == 1){
    printf("%d > antes de open %s\n", cid, fname);
#ifdef XPN
    if (( f=xpn_open(fname,O_RDWR,0777)) < 0)
#elif MPI
    if (MPI_File_open(MPI_COMM_SELF, fname, MPI_MODE_RDWR,
			                   MPI_INFO_NULL, &fh) <0)
#else
    if (( f=open(fname,O_RDWR,0777)) < 0)
#endif
    {
    	sprintf(str, "IOC.TakeSample: failed to create file %s\n", fname);
    	printf(str);
    }	
    printf("%d > open %s\n", cid, fname);
	printf("%d > leo %s\n", cid, fname);
	ForwReading(cid, ncid, f, lbuf, buffer_lec, buffer_esc, timefr);
	MPI_Barrier(MPI_COMM_WORLD);
   }
*/

   MPI_Barrier(MPI_COMM_WORLD);
#ifdef XPN
   xpn_close(f);
#elif MPI
   MPI_File_close(&fh);
#else
   close(f);
#endif

   if((type == 1)&&(cid == 0)){
   		printf("%d>borro %s\n",cid,fname);
#ifdef XPN
		xpn_unlink(fname);
#elif MPI
   		MPI_File_delete(fname, info);
#else
		unlink(fname);
#endif
   }

   MPI_Barrier(MPI_COMM_WORLD);
   MPI_Barrier(MPI_COMM_WORLD);
}

static void PrintHeader(void)
{
    printf("L_BUF N_BUF TOT(MB) E(MB/s) L_S(MB/s) L_NS(MB/s) T.TOTAL\n");
    printf("----------------------------------------------------------\n");
}

static void PrintResult(int cid, int lb, double timet, float trw, float trfr, float trbr, int flag)
{

	if(flag == 0){		
		sprintf(str, "%d\t%6d\t%f\t%f\tIOPwrite\t\n", cid, lb, trw, timet);
	}
	if(flag == 1){
	    sprintf(str, "%d\t%6d\t%f\t%f\tIOPread\t\n", cid, lb, trfr, timet);
	}
	printf(str);
}
/*
static void PrintSummary(struct timeval *ttot, int n, float med_w, float med_fr, float med_br)
{
  int n_users;

    printf("==================================================\n");
    sprintf(str, "Bandwidth. (Write):  %f MB/s \n",med_w/n);
    printf(str);
    sprintf(str, "Bandwidth. (Sec. Read):  %f MB/s \n",med_fr/n);
    printf(str);
    sprintf(str, "Bandwidth. (Rand. Read):  %f MB/s \n",med_br/n);
    printf(str);
    sprintf(str, "Average Bandwidth:  %f MB/s \n",(med_w+med_fr+med_br)/(3*n));
    printf(str);
    sprintf(str, "Total time:  %f s. \n\n",((float)ttot->tv_sec +
		(float)ttot->tv_usec/USECPSEC));
    printf(str);

    sprintf(str, "BW %2.4f; BSR %2.4f; BRR %2.4f;  AVG %2.4f \n",
        n_users * (med_w/n), n_users * (med_fr/n), n_users * (med_br/n),
       n_users * (med_w+med_fr+med_br)/(3*n));
    printf(str);
    sprintf(str,"T. Time %2.4f \n", ((float)ttot->tv_sec +
		(float)ttot->tv_usec/USECPSEC));
    printf(str);
    sprintf(str, "BW %2.4f; BSR %2.4f; BRR %2.4f;  AVG %2.4f \n",
        (med_w/n), (med_fr/n), (med_br/n), (med_w+med_fr+med_br)/(3*n));
    printf(str);


}
*/
int main(int argc, char **argv)
{
    int lbuf, nit=0;
    double timei, timef, timedif, tini, tfin, tdif;
    char dir[LFNAME];
    float trw, trfr,  trbr;
    float trw_med=0, trfr_med=0,  trbr_med=0;
    double timew, timefr, timebr;
    int ncid, cid;
    int ret;
    int  namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    char **endptr;


    //setenv("XPN_CONF","/cacau/HLRS/HLRS/hpcluis/BENCH/IOP/",1);
    //setenv("XPN_CONF","/cacau/HLRS/HLRS/hpcluis/BENCH/IOP/",0);
    //setenv("XPN_FILE_TRANSLATOR","/cacau/HLRS/HLRS/hpcluis/DNS.txt",1);
    //setenv("XPN_FILE_TRANSLATOR","/cacau/HLRS/HLRS/hpcluis/DNS.txt",0);
		

    ret = MPI_Init(&argc, &argv);
	
	
#ifdef RECOLECT		
	open_socket();
#endif
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
	if (ret < 0) {
	    printf("Error en MPI_Init \n");
	    exit(0);
    }

	/*
	if (argc >= 3) {
		LBufMax = strtol(argv[2], endptr, 10);
	}
	printf("LBufMax = %d\n",LBufMax);
	buffer_esc = malloc(LBufMax*sizeof(char));
	buffer_lec = malloc(LBufMax*sizeof(char));
	*/

#ifdef XPN
    ret = xpn_init();
    if (ret < 0) {
	printf("Error en xpn_init\n");
	exit(0);
    }
#endif

    MPI_Comm_size(MPI_COMM_WORLD,&ncid);
    MPI_Comm_rank(MPI_COMM_WORLD,&cid);
    MPI_Get_processor_name(processor_name,&namelen);
    printf("PROCESO(%s) %s -  %d de %d \n", argv[0] , processor_name, cid, ncid);

    MPI_Barrier(MPI_COMM_WORLD);

    lbuf = LBUFMAX;
    //lbuf = LBufMax;
    memset (buffer_esc, '7', LBUFMAX);
    //memset (buffer_esc, '7', LBufMax);
    TAKE_SAMPLE_DIR = argv[1];

    //Timer(&tini);
    tini = MPI_Wtime();

    for ( ; lbuf>=LBUFMIN; lbuf/=2)
    {
      //sleep(2);
      MPI_Barrier(MPI_COMM_WORLD);
      //Timer(&timei);
      timei = MPI_Wtime();
      TakeSample(cid, ncid, lbuf, dir, &timew, &timefr, &timebr, 0);
      //Timer(&timef);
      timef = MPI_Wtime();
      //DiffTime(&timei, &timef, &timedif);
      timedif = timef - timei;
      trw=TASA_TRANSF(timew);
      trfr=TASA_TRANSF(timefr);
      trbr=TASA_TRANSF(timebr);
      trw_med+=trw; trfr_med+=trfr; trbr_med+=trbr;
      PrintResult(cid, lbuf, timedif, trw, trfr, trbr,0);
      nit++;
    }

    lbuf = LBUFMAX;
    //lbuf = LBufMax;
    MPI_Barrier(MPI_COMM_WORLD);

    for ( ; lbuf>=LBUFMIN; lbuf/=2)
    {
      //sleep(2);
      MPI_Barrier(MPI_COMM_WORLD);
      //Timer(&timei);
      timei = MPI_Wtime();
      TakeSample(cid, ncid, lbuf, dir, &timew, &timefr, &timebr, 1);
      //Timer(&timef);
      timef = MPI_Wtime();
      //DiffTime(&timei, &timef, &timedif);
      timedif = timef - timei;
      trw=TASA_TRANSF(timew);
      trfr=TASA_TRANSF(timefr);
      trbr=TASA_TRANSF(timebr);
      trw_med+=trw; trfr_med+=trfr; trbr_med+=trbr;
      PrintResult(cid, lbuf, timedif, trw, trfr, trbr,1);
      nit++;
    }

    //Timer(&tfin);
    //DiffTime(&tini, &tfin, &tdif);
    MPI_Barrier(MPI_COMM_WORLD);

    //PrintSummary(&tdif, nit, trw_med, trfr_med, trbr_med);
    //

#ifdef RECOLECT		
	close_socket();
#endif

#ifdef XPN
    ret = xpn_destroy();
    if (ret < 0) {
	printf("Error en xpn_destroy\n");
	exit(0);
    }
#endif

    MPI_Finalize();

    exit(0);
}

