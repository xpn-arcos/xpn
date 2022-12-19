

/*
 *
 */
#include "xpn2.h"
#include "base/time_misc.h"


/*
 *
 */
#define KB (1024)
#define MB (KB*KB)

#if (0)
#undef  DEBUG_AUTOTEST_01
#define NTESTING 1
#define LBUFMIN (8*KB)
#define LBUFMAX (64*KB)
#define TAMFILE (32*MB)
#else
#define DEBUG_AUTOTEST_01
#define NTESTING 1 
//#define LBUFMIN (8*KB-110)
#define LBUFMIN (4*KB-100)
#define LBUFMAX (8*KB)
#define TAMFILE (128*KB)
#endif

#define LFNAME 4*KB



/*
 *
 */
#define T2USEC(mtime) \
        (((float)(mtime).tv_sec*USECPSEC) + ((float)(mtime).tv_usec))

#define T2SEC(mtime) \
        (((float)(mtime).tv_sec) + ((float)(mtime).tv_usec/USECPSEC))

#define BW(mtime,mtamfile) \
        ((float)(mtamfile)/T2USEC(mtime))


/*
 *
 */

/* Read 'tamfile' bytes in blocks of 'lb' bytes. */
void ForwardReading
(
	int cid, 
        int ncid,
	int f, 
        int lb, 
        char *buf, 
        long  tamfile,
	struct timeval *tim
)
{
    int iter ;
    int ret ;
    int offset ;
    struct timeval ti, tf;
#if defined(DEBUG_AUTOTEST_01)
    int i;
#endif

    /* setup work space */
    offset = 0;
    iter= tamfile/lb;

    /* starting chrono */
    TIME_MISC_Timer(&ti);    

    /* reading */
    for ( ; iter>0; iter--)
    {
       if ((ret = xpn2_sread(f,buf,offset,lb))!= lb)
       {
	  printf("[%s:%d]: ForwardReading.xpn2_read: errno = %d\n",
                 __FILE__,__LINE__,ret);
          exit(1);
       }

#if defined(DEBUG_AUTOTEST_01)
#if (0)
       for (i=0; i<(lb-1); i++)
	       if ( buf[i] != 'a' )
	       {
	            printf("file[%d]:buf[%d] -> '%c' != '%c'\n",offset,i,buf[i],'a') ;
		    break ;
	       }
#else
       for (i=0; i<(lb-1); i++)
	       if ( buf[i] != 'a'+(iter%24) )
	       {
	            printf("file[%d]:buf[%d] -> '%c' != '%c'\n",offset,i,buf[i],'a'+(iter%24)) ;
		    break ;
	       }
#endif
#endif

       offset = offset + lb; 
    }

    /* stopping chrono */
    TIME_MISC_Timer(&tf);    

    /* get diffTime */
    TIME_MISC_DiffTime(&ti, &tf, tim);
}

/* Write 'tamfile' bytes in blocks of 'lb' bytes. */
void ForwardWriting
(
	int cid, 
        int ncid,
	int f, 
        int lb, 
        char *buf, 
        long  tamfile,
	struct timeval *tim
)
{
    int iter ;
    int ret ;
    int offset ;
    struct timeval ti, tf;

    /* setup work space */
    offset = 0;
    iter= tamfile/lb;

    /* setup default buffer contents */
    memset(buf, 'a', lb);
    buf[lb-1]='\n';

    /* starting chrono */
    TIME_MISC_Timer(&ti);    

    /* writting */
    for ( ; iter>0; iter--)
    {
#if defined(DEBUG_AUTOTEST_01)
#if (0)
       memset(buf, 'a', lb);
       buf[lb-1]='\n';
#else
       memset(buf, 'a'+(iter%24), lb);
       buf[lb-1]='\n';
#endif
#endif

       if ((ret = xpn2_swrite(f,buf,offset,lb))!= lb)
       {
	  printf("[%s:%d]: ForwardWriting.xpn2_write: errno = %d\n",
                 __FILE__,__LINE__,ret);
          exit(1);
       }

       offset = offset + lb; 
    }

    /* stopping chrono */
    TIME_MISC_Timer(&tf);    

    /* get diffTime */
    TIME_MISC_DiffTime(&ti, &tf, tim);
}

/* Test 1 */
static char *fs_tags[] = { 
                           "xpni_null", 
                           "xpni_ft_raid5outer",
	                   "xpni_ft_raid5inner", 
                           "xpni_log", 
                           NULL } ;

int test_1 ( void )
{
    static char buffer_esc[LBUFMAX];
    static char buffer_lec[LBUFMAX];
    static char fname[LFNAME];

    struct timeval tfw;
    struct timeval tfr;

    int   fd;
    char *dir ;
    char *fs_tag ;
    int   n_srvs ;
    int   l_srvs ;
    char *test_name ;
    int   com_size ;
    int   com_rank ;
    int   lbuffer ; 
    int   fs_tagi ; 


    /* print header */
    printf("\n") ;
    printf("test-01\n") ;

    /* testing... */
    for (fs_tagi = 0; fs_tags[fs_tagi] != NULL; fs_tagi++)
    {
    //for (lbuffer = LBUFMAX; lbuffer >= LBUFMIN; lbuffer/=2)
    for (lbuffer = LBUFMAX; lbuffer >= LBUFMIN; lbuffer=lbuffer-1111)
    {

	    /* test: parameters */
	    dir       = "/PNFS" ;
	    fs_tag    = fs_tags[fs_tagi] ;
	    n_srvs    = 4 ;
	    l_srvs    = 1 ;
	    test_name = "autotest-xpn2" ;
	    com_size  = 1 ;
	    com_rank  = 1 ;

	    /* test: file name */
	    sprintf(fname, 
		    "%s/xpn-%s-%d-%d-%d.%s.%d.%d.%d", 
		    dir, 
		    fs_tag, 
		    n_srvs, 
		    0,
		    8*1024,
		    //lbuffer,
		    test_name,
		    l_srvs,
		    com_size, 
		    com_rank); 

	    /* test: open */
	    fd=xpn2_open(fname,O_CREAT|O_RDWR,0777) ;
	    if (fd < 0)
	    {
		    printf("[%s:%d]: Error creating file '%s'.\n",
			   __FILE__,__LINE__,fname);
		    exit(0);
	    }	

	    /* test: write */
	    printf("Forward writing... (%s,%d)\n",fs_tag,lbuffer);
	    ForwardWriting(com_rank,
			   com_size,
			   fd,
			   lbuffer,
			   buffer_esc,
			   TAMFILE,
			   &tfw) ;

	    /* test: read */
	    printf("Forward reading... (%s,%d)\n",fs_tag,lbuffer);
	    ForwardReading(com_rank,
			   com_size,
			   fd,
			   lbuffer,
			   buffer_lec,
			   TAMFILE,
			   &tfr) ;

	    /* test: close */
	    xpn2_close(fd);

    } /* lbuff */
    } /* fs_tagi */

    return (1);
}

/* Test 2 */
int test_2 ( void )
{
    static char buffer_esc[LBUFMAX];
    static char buffer_lec[LBUFMAX];
    static char fname[LFNAME];

    struct timeval tfw;
    struct timeval tfr;

    int   fd;
    char *dir ;
    char *fs_tag ;
    int   n_srvs ;
    int   l_srvs ;
    char *test_name ;
    int   com_size ;
    int   com_rank ;
    int   lbuffer ; 
    int   fs_tagi ; 
    int   tloop ; 

    float FW_BW_ACC, FR_BW_ACC ;
    float FW_BW, FR_BW ;


    /* print header */
    printf("\n") ;
    printf("test-02\n") ;
    printf("%20s\t%2s\t%2s\t%2s\t%s\t%7s\t%8s\t%8s\t%1s\n",
           "fsi","n_srvs","n_clnt","clnt_id","f_size","s_size","FW_BW","FR_BW","NTEST");

    /* testing... */
    for (fs_tagi = 0; fs_tags[fs_tagi] != NULL; fs_tagi++)
    {
    for (lbuffer = LBUFMAX; lbuffer >= LBUFMIN; lbuffer/=2)
    {

	    /* test: parameters */
	    dir       = "/PNFS" ;
	    fs_tag    = fs_tags[fs_tagi] ;
	    n_srvs    = 4 ;
	    l_srvs    = 1 ;
	    test_name = "autotest-xpn2" ;
	    com_size  = 1 ;
	    com_rank  = 1 ;

	    /* test: file name */
	    sprintf(fname, 
		    "%s/xpn-%s-%d-%d-%d.%s.%d.%d.%d", 
		    dir, 
		    fs_tag, 
		    n_srvs, 
		    0, 
		    8*1024,
		    //lbuffer,
		    test_name,
		    l_srvs,
		    com_size, 
		    com_rank); 

	    /* test: looping */
	    FW_BW_ACC = 0;
	    FR_BW_ACC = 0;

	    for (tloop = 0; tloop < NTESTING; tloop++)
	    {
		    /* test: open */
		    fd=xpn2_open(fname,O_CREAT|O_RDWR,0777) ;
		    if (fd < 0)
		    {
			    printf("[%s:%d]: Error creating file '%s'.\n",
				   __FILE__,__LINE__,fname);
			    exit(0);
		    }	

		    /* test: write */
		    ForwardWriting(com_rank,
				   com_size,
				   fd,
				   lbuffer,
				   buffer_esc,
				   TAMFILE,
				   &tfw) ;
		    FW_BW_ACC += BW(tfw,TAMFILE) ;

		    /* test: read */
		    ForwardReading(com_rank,
				   com_size,
				   fd,
				   lbuffer,
				   buffer_lec,
				   TAMFILE,
				   &tfr) ;
		    FR_BW_ACC += BW(tfr,TAMFILE) ;

		    /* test: close */
		    xpn2_close(fd);
	    }

	    /* test: print results */
	    FW_BW = FW_BW_ACC / NTESTING ;
	    FR_BW = FR_BW_ACC / NTESTING ;

	    printf("%20s\t%2d\t%2d\t%2d\t%ld\t%7d\t%8f\t%8f\t%1d\n",
		    fs_tag, 
		    n_srvs, 
		    com_size, 
		    com_rank, 
		    (long)TAMFILE, 
		    lbuffer,
		    FW_BW,
		    FR_BW,
		    NTESTING); 

    } /* lbuff */
    } /* fs_tagi */

    return (1);
}


/* Main */
int main ( int argc, char **argv )
{
    int ret ; 


    /* unbuffered output */
    setbuf(stdout, NULL);

    /* set up XPN_CONF enviroment variable */
    setenv("XPN_CONF","./",1);

    /* init xpn */
    ret = xpn2_init();
    if(ret < 0)
    {
	    printf("[%s:%d]: Error in Expand init (%d).\n",
                   __FILE__,__LINE__,ret);
	    exit(0);
    }


    /* Test 1 */
    ret = test_1() ;

    /* Test 2 */
    ret = test_2() ;


    /* finalize xpn */
    // xpn2_destroy() ; // BUG in xpn_destroy, not xpn2_destroy...
    exit(0);
}

