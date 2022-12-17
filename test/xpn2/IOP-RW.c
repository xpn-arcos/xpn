

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "mpi/mpi.h"
#include "base/time_misc.h"
#include "expand.h"
#include "xpn2.h"


#define KB (1024)
#define MB (KB*KB)
#define GB (MB*MB)

#if (1)
	#define LBUFMIN (1*KB)
	#define LBUFMAX (2*MB)
	#define TAMFILE (512 * MB)
        #define FS_TAGS_LIST { "xpni_ft_raid5inner", NULL } 
#elif (1)
	#define LBUFMIN (1*KB)
	#define LBUFMAX (2*MB)
	#define TAMFILE (512 * MB)
        #define FS_TAGS_LIST { "xpni_null", "xpni_ft_raid5outer", "xpni_ft_raid5inner", NULL } 
#else
	#define LBUFMIN (2*KB)
	#define LBUFMAX (4*MB)
	#define TAMFILE (8*MB)
        #define FS_TAGS_LIST { "xpni_ft_raid5inner", "xpni_ft_raid5outer", NULL } 
#endif

#define LFNAME 8*KB

#if !defined(TRUE)
	#define TRUE 1
#endif

#if !defined(FALSE)
	#define FALSE 0
#endif


#define TASA_TRANSF(mtime,mbytes) ((float)(mbytes)/(float) ((mtime).tv_sec * USECPSEC + (mtime).tv_usec))


char buffer_basura[LBUFMAX];
char buffer_esc[LBUFMAX];
char buffer_lec[LBUFMAX];
char str[4*1024];
char *fs_tags[] = FS_TAGS_LIST ;
//int  errno;
int  n_oper = 0;


/*
 *  Get Arguments
 */
static int GetCheckArgs
(
     int    argc, 
     char **argv, 
     char **dir,
     int   *fileSize
)
{
    int max_argc;

    max_argc = 2;
    if (argc < max_argc)
    {
        printf("Uso: %s <directory> [<file size in MB>]\n", argv[0]);
        return (-1) ;
    }

    (*dir) = "/PNFS" ;
    if (argc >= max_argc)
       (*dir) = argv[1]; 

    (*fileSize) = TAMFILE ;
    if (argc >= max_argc+1)
       (*fileSize) = atoi(argv[2]) * MB; 

    return(1);
}


/*
 * Write TAMFILE bytes in blocks of lb bytes.
 */
static void ForwWriting
(
	int comm_rank, 
	int comm_size,
	int fileSize,
	int f, 
	int lb, 
	char *buf, 
	struct timeval *tim
)
{
    int iter,ret;
    struct timeval ti, tf;
    int offset;


    offset = comm_rank*lb;
    iter   = fileSize/lb;
    iter   = iter/comm_size;

    TIME_MISC_Timer(&ti);    

    for ( ; iter>0; iter--)
    {

       //memset(buf, 'a', lb);
       //printf("%d)write %d en %d\n",comm_rank,lb,offset);
	
       if ((ret = xpn2_swrite(f,buf,offset,lb))!= lb)
       {
          sprintf(str, "IOP-RW.ForwWriting: Error en escritura  (errno = %d)\n", ret);
	  printf(str);
          exit(1);
       }

       offset = offset + comm_size*lb; 
       //offset = offset + lb; 
    }

    TIME_MISC_Timer(&tf);    
    TIME_MISC_DiffTime(&ti, &tf, tim);
}

/*
 * Read TAMFILE bytes in blocks of lb bytes and then compares it with
 * Bufe.
 */
static void ForwReading
(
	int comm_rank, 
	int comm_size, 
	int fileSize,
	int f, 
	int lb, 
	char *bufl, 
	char *bufe, 
	struct timeval *tim
)
{
    int iter;
    struct timeval ti, tf;
    int offset;
    int count;

    offset = comm_rank*lb;
    iter   = fileSize/lb;
    iter   = iter/comm_size;

    TIME_MISC_Timer(&ti);   

    for ( ; iter>0; iter--)
    {

        //memset(bufl, 'a', lb);

        if ( (count = xpn2_sread(f,bufl,offset,lb)) != lb)
	{
		printf(str, "IOP-RW.ForwReading: Read %d bytes (%d expected)\n", count, lb);
		exit(1);
	}

        //offset = offset + comm_size*lb;
        offset = offset + lb;
    }

    TIME_MISC_Timer(&tf);
    TIME_MISC_DiffTime(&ti, &tf, tim);
}

/* 
 * This function is similar to ForwReading, but here the reads are
 * beginning in the end of the file.
 */
static void BackwReading
(
	int comm_rank, 
	int comm_size, 
	int fileSize,
	int f, 
	int lb, 
	char *bufl, 
	char *bufe, 
	struct timeval *tim
)
{
    struct timeval ti, tf;
    int iter;
    int offset;

    offset = fileSize;
    iter   = fileSize/lb;
    iter   = iter/comm_size;

    TIME_MISC_Timer(&ti);   

    for(;iter>0;iter--) {

       offset = offset - lb;

       // memset(bufl, 'a', lb);

       if (xpn2_sread(f,bufl,offset,lb)!= lb)
       {
          sprintf(str, "Error IOP-RW.BackReading: Error en lectura  (errno = %d)\n", errno);
	  printf(str);
          exit(1);
       }
    }

    TIME_MISC_Timer(&tf);   
    TIME_MISC_DiffTime(&ti, &tf, tim);
}

/* 
 * This function creats a new file, and is performanced a Writing, 
 * ForwReading and BackReading function.
 */
static int TakeSample
(
	int     comm_rank, 
	int     comm_size,
	int     fileSize,
	int     lbuf, 
	char   *dir, 
	char   *fs_tag, 
	struct timeval *timew, 
	struct timeval *timefr, 
	struct timeval *timebr,
	int     test_type
)
{
    int f;
    char fname[LFNAME];
    int n_srvs, l_srvs ;
    char *test_name ;

 
    /* test: parameters */
    n_srvs    = 8 ;
    l_srvs    = 1 ;
    test_name = "IOP-RW" ;

    /* test: file name */
    sprintf(fname, 
            "%s/xpn-%s-%d-%d-%d.%s.%d.%d.%d", 
            dir, 
            fs_tag, 
            n_srvs, 
            1, 
            lbuf,
            test_name,
            l_srvs,
            comm_size, 
            comm_rank); 

    if (( f=xpn2_open(fname,O_CREAT|O_RDWR,0777)) < 0)
    {
    	sprintf(str, "IOP-RW.TakeSample: failed to create file '%s' (err = %d)\n", fname, f);
    	printf(str);

	return (-1) ;
    }	
    
    // printf("%d > creo %s\n", comm_rank, fname);
    MPI_Barrier(MPI_COMM_WORLD);

    /* Test type */
    timew->tv_sec  = (__time_t)0.0 ;
    timew->tv_usec = (__suseconds_t)0.0 ;

    timefr->tv_sec  = (__time_t)0.0 ;
    timefr->tv_usec = (__suseconds_t)0.0 ;

    timebr->tv_sec  = (__time_t)0.0 ;
    timebr->tv_usec = (__suseconds_t)0.0 ;

    if(test_type == 0){
	   // printf("%d > escribiendo %s\n", comm_rank, fname);
	   ForwWriting(comm_rank, comm_size, fileSize, f, lbuf, buffer_esc, timew);   
    }

    else if(test_type == 1){
 	   // printf("%d > leo %s\n", comm_rank, fname);
	   ForwReading(comm_rank, comm_size, fileSize, f, lbuf, buffer_lec, buffer_esc, timefr);
    }

    else if(test_type == 2){
 	   // printf("%d > leo %s\n", comm_rank, fname);
	   BackwReading(comm_rank, comm_size, fileSize, f, lbuf, buffer_lec, buffer_esc, timefr);
    }

    xpn2_close(f);
    MPI_Barrier(MPI_COMM_WORLD);

    return (1);
}

static void PrintHeader(void)
{
    sprintf(str,
	    "%20s\t%8s\t%7s\t%8s\t%8s\t%8s\t%3s\n", 
	    "FS_TAG","File_Size","L_BUFF","BW_W","BW_FR","BW_BR","OP");
    printf(str);

#if defined(IOPW_FULL_INFO)
    sprintf(str,
	    "%10s\t%3s\t%3s\t%8s\n", 
	    "DIR","C_Size","C_Rank","TOTAL_TIME");
    printf(str);
#endif
}

static void PrintResult
(
     int comm_rank, 
     int comm_size, 
     int fileSize, 
     int lb, 
     char *dir,
     char *fs_tag,
     struct timeval *timet, 
     float trw, 
     float trfr, 
     float trbr, 
     int test_type
)
{
#if defined(IOPW_FULL_INFO)
        float time_in_sec ;
#endif
	char *op ;

	switch (test_type)
	{
		case 0:
	                op = "w" ;
			break;
		case 1:
	                op = "fr" ;
			break;
		case 2:
	                op = "br" ;
			break;
		default:
	                op = "?" ;
			break;
	}

	sprintf(str, 
		"%20s\t%8d\t%7d\t%8f\t%8f\t%8f\t%3s\n", 
		fs_tag, fileSize, lb, trw, trfr, trbr, op);
	printf(str);

#if defined(IOPW_FULL_INFO)
	time_in_sec = (float)timet->tv_sec + (float)timet->tv_usec/USECPSEC ;

        sprintf(str,
    	        "%10s\t%8p\t%3s\t%8s\n", 
	        dir, comm_size, comm_rank, time_in_sec);
        printf(str);
#endif
}


int main (int argc, char **argv)
{
    int lbuf ;
    struct timeval timei, timef, timedif, tini, tfin, tdif;
    char *dir;
    float trw, trfr,  trbr; 
    struct timeval timew, timefr, timebr;
    int  comm_size, comm_rank;
    int  ret;
    int  namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int  fs_tag ;
    int  test_type ;
    int  fileSize ;


    /*
     *
     */
    setbuf(stdout, NULL);


    /*
     *  Init MPI
     */
    ret = MPI_Init(&argc, &argv);
    if (ret < 0) 
    {
	    printf("Error en MPI_Init \n");
	    exit(0);
    }

    MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&comm_rank);
    MPI_Get_processor_name(processor_name,&namelen);

    // printf("PROCESO %s -  %d de %d \n", processor_name, comm_rank, comm_size);

    ret = GetCheckArgs(argc,argv,&dir,&fileSize);
    if (ret < 0)
        exit(ret);

    memset (buffer_basura, '0', LBUFMAX);
    memset (buffer_esc,    '0', LBUFMAX);
    memset (buffer_lec,    '0', LBUFMAX);


    /*
     *  Init XPN
     */
    setenv("XPN_CONF","./",1);
    if((ret = xpn2_init())<0)
    {
	    printf("error en el init %d\n",ret);
	    exit(0);
    }


    /*
     *  Testing...
     */
    if (0 == comm_rank)
        PrintHeader();

    MPI_Barrier(MPI_COMM_WORLD);
    TIME_MISC_Timer(&tini);

    for (test_type=0;  test_type <= 1;          test_type++)
    for (fs_tag=0;     fs_tags[fs_tag] != NULL; fs_tag++)
    for (lbuf=LBUFMAX; lbuf >= LBUFMIN;         lbuf/=2)
    {
       MPI_Barrier(MPI_COMM_WORLD);

       TIME_MISC_Timer(&timei);
       ret = TakeSample(comm_rank, comm_size, fileSize,
		        lbuf, dir, fs_tags[fs_tag], 
		        &timew, &timefr, &timebr, 
		        test_type);
       if (ret < 0) return ret ;
       TIME_MISC_Timer(&timef);
       TIME_MISC_DiffTime(&timei, &timef, &timedif);

       trw=TASA_TRANSF(timew,fileSize); 
       trfr=TASA_TRANSF(timefr,fileSize); 
       trbr=TASA_TRANSF(timebr,fileSize);

       PrintResult(comm_rank, comm_size, fileSize,
		   lbuf, dir, fs_tags[fs_tag], 
		   &timedif, 
		   trw, trfr, trbr, 
		   test_type);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    TIME_MISC_Timer(&tfin);
    TIME_MISC_DiffTime(&tini, &tfin, &tdif);
    

    /*
     *  Finalize XPN
     */
    // xpn2_destroy(); //TODO: xpn_destroy_servers() fail !!!!!!!

    /*
     *  Finalize MPI
     */
    MPI_Finalize(); 

    exit(0);
}

