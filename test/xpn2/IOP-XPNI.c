

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


#define KB 1024
#define MB (KB*KB)

#if (0)
        #define LBUF    (32*KB)
	#define TAMFILE (100 * MB)
        #define FS_SIZE_LIST { 32*KB, 512*KB, 2*MB, 16*MB, 64*MB, 128*MB, 256*MB, -1 } ;
        #define FS_TAGS_LIST { "xpni_ft_raid5outer", "xpni_ft_raid5inner", NULL } ;
#else
        #define LBUF    (32*KB)
	#define TAMFILE (100 * MB)
        #define FS_SIZE_LIST { 32*KB, 512*KB, 2*MB, 16*MB, 64*MB, 128*MB, 256*MB, -1 } ;
        #define FS_TAGS_LIST { "xpni_ft_raid5inner", "xpni_ft_raid5outer", NULL } ;
#endif

#define LFNAME 4*KB

#if !defined(TRUE)
	#define TRUE 1
#endif

#if !defined(FALSE)
	#define FALSE 0
#endif


char  str[4*1024];
int   fs_size[] = FS_SIZE_LIST ;
char *fs_tags[] = FS_TAGS_LIST ;


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
static void FileFill
(
	int comm_rank, 
	int comm_size,
	int fileSize,
	int lb,
	int f, 
	struct timeval *tim
)
{
    int    iter,ret;
    struct timeval ti, tf;
    int    offset;
    static char buf[LBUF]; 


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
          sprintf(str, "IOP-XPNI.ForwWriting: Error en escritura  (errno = %d)\n", ret);
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
 * Change Expand file system interface
 */
static void FileChange
(
	int    comm_rank, 
	int    comm_size, 
	int    fileSize,
	int    lb, 
	int    f, 
	char  *fs_tag, 
	struct timeval *tim
)
{
    struct timeval ti, tf;
    int ret ;


    TIME_MISC_Timer(&ti);   

    ret = xpn2_ioctl(f,XPN2_IOCTL_SETXPN2INTERFACE,fs_tag) ;
    if (ret < 0)
	    fprintf(stderr,"XPN2_IOCTL_SETXPN2INTERFACE fail!");

    TIME_MISC_Timer(&tf);
    TIME_MISC_DiffTime(&ti, &tf, tim);
}


/* 
 * This function creats a new file, and is performanced a Writing, 
 * ForwReading and BackReading function.
 */
static void TakeSample
(
	int     comm_rank, 
	int     comm_size,
	int     fileSize,
	int     lbuf, 
	char   *dir, 
	char   *fs_tag, 
	struct timeval *time_w,
	struct timeval *time_xch
)
{
    int f;
    char fname[LFNAME];
    int n_srvs, l_srvs ;
    char *test_name ;
    char *fs_tag1 ;

 
    /* test: parameters */
    n_srvs    = 8 ;
    l_srvs    = 1 ;
    test_name = "IOP-XPNI" ;
    fs_tag1   = "xpni_null" ;

    /* test: file name */
    sprintf(fname, 
            "%s/xpn-%s-%d-%d-%d.%s.%d.%d.%d", 
            dir, 
            fs_tag1, 
            n_srvs, 
            1, 
            lbuf,
            test_name,
            l_srvs,
            comm_size, 
            comm_rank); 

    if (( f=xpn2_open(fname,O_CREAT|O_RDWR,0777)) < 0)
    {
    	sprintf(str, "IOP-XPNI.TakeSample: failed to create file '%s' (err = %d)\n", fname, f);
    	printf(str);
    }	
    
    // printf("%d > creo %s\n", comm_rank, fname);
    MPI_Barrier(MPI_COMM_WORLD);

    /* Test type */
    time_w->tv_sec  = (__time_t)0.0 ;
    time_w->tv_usec = (__suseconds_t)0.0 ;

    time_xch->tv_sec  = (__time_t)0.0 ;
    time_xch->tv_usec = (__suseconds_t)0.0 ;

    FileFill   (comm_rank, comm_size, fileSize, lbuf, f,         time_w);   
    FileChange (comm_rank, comm_size, fileSize, lbuf, f, fs_tag, time_xch);   

    xpn2_close(f);
    MPI_Barrier(MPI_COMM_WORLD);
}

static void PrintHeader(void)
{
    sprintf(str,
	    "%10s\t%10s\t%20s\t%10s\t%8s\t%8s\n", 
	    "Comm_Size","Comm_Rank","FS_TAG","File_Size","T_W_SEC","T_XCH_SEC");
    printf(str);
}

static void PrintResult
(
     int    comm_rank, 
     int    comm_size, 
     int    fileSize, 
     int    buferSize, 
     char  *fs_tag,
     float  time_w, 
     float  time_xch
)
{
	sprintf(str, 
		"%10d\t%10d\t%20s\t%10d\t%8f\t%8f\n", 
		comm_size, comm_rank, fs_tag, fileSize, time_w, time_xch);
	printf(str);
}


int main (int argc, char **argv)
{
    int lbuf ;
    struct timeval timei, timef, timedif, tini, tfin, tdif;
    char *dir;
    struct timeval time_w, time_xch ;
    float          TotalTime_w, TotalTime_xch ;
    int comm_size, comm_rank;
    int ret;
    int  namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int fs_tag ;
    int fileSize ;


    /*
     *
     */
    setbuf(stdout, NULL);

    ret = GetCheckArgs(argc,argv,&dir,&fileSize);
    if (ret < 0)
        exit(ret);


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
    PrintHeader();

    MPI_Barrier(MPI_COMM_WORLD);
    TIME_MISC_Timer(&tini);

    lbuf = LBUF ;
    for (fs_tag=0;     fs_tags[fs_tag]   != NULL; fs_tag++)
    for (fileSize=0;   fs_size[fileSize] != (-1); fileSize++)
    {
       MPI_Barrier(MPI_COMM_WORLD);

       TIME_MISC_Timer(&timei);
       TakeSample(comm_rank, comm_size, fs_size[fileSize],
		  lbuf, dir, fs_tags[fs_tag], 
		  &time_w,&time_xch);
       TIME_MISC_Timer(&timef);
       TIME_MISC_DiffTime(&timei, &timef, &timedif);

       TotalTime_w   = (float)(time_w.tv_sec)   + (float)time_w.tv_usec   / USECPSEC;
       TotalTime_xch = (float)(time_xch.tv_sec) + (float)time_xch.tv_usec / USECPSEC;

       PrintResult(comm_rank, comm_size, fs_size[fileSize],
		   lbuf, fs_tags[fs_tag], 
		   TotalTime_w, TotalTime_xch);
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

