

/*
 *
 */
#include "expand.h"


/*
 *
 */
#ifndef KB
	#define KB (1024)
#endif

#ifndef MB
	#define MB (KB*KB)
#endif

#define LBUFMIN (7*KB)
#define LBUFMAX (8*KB-20)
#define TAMFILE (128*KB)

#define LFNAME 4*KB

#define FILES 1



/* 
 * Main 
 */
int main ( int argc, char **argv )
{
    static char buf[LBUFMAX];
    static char buf2[LBUFMAX];
    static char fname[LFNAME];

    int ret ; 
    int offset, lb ;
    int i ;

    char vowel ;
    int  fi, f[FILES];
    int cont;

    /* unbuffered output */
    setbuf(stdout, NULL);

    /* set up XPN_CONF enviroment variable */
    setenv("XPN_CONF","./",1);

    /* init xpn */
    ret = xpn_init();
    if(ret < 0)
    {
	    printf("[%s:%d]: Error in Expand init (%d).\n",
                   __FILE__,__LINE__,ret);
	    exit(0);
    }

    /* open file */
    
    for (lb=LBUFMIN; lb<=LBUFMAX; lb++)
    {
	    
    	printf("[%s:%d]: Do loop %d\n",
                   __FILE__,__LINE__,lb);
    	for (fi=0; fi< FILES; fi++)
    	{
    	    sprintf(fname, "/PNFS/test-%d",fi); 
	    
	    xpn_unlink(fname) ;
	    /*
	     * printf("[%s:%d]: ForwardWriting.xpn_unlink: %s\n",
				 __FILE__,__LINE__,fname);
	     */
	    f[fi]=xpn_open(fname,O_CREAT|O_RDWR,0777) ;
	  /*			
	    printf("[%s:%d]: ForwardWriting.xpn_open: %s %d\n",
				 __FILE__,__LINE__,fname,f[fi]);
	    */
	    if (f[fi] < 0)
	    {
		    printf("[%s:%d]: Error creating file '%s'.\n",
			   __FILE__,__LINE__,fname);
		    exit(0);
	    }	
    	}

    	/* 3 write */
    	vowel  = 'a' ;
	cont = 0;
    	for (offset=0; offset<=TAMFILE ; offset+=lb)
    	{

	    for (i=0; i<lb; i++)
	    {
		 buf[i]  = vowel + (i % 24) ;
	    }

	    for (fi=0; fi< FILES; fi++)
	    {
		   /*
		    printf("[%s:%d]: ForwardWriting.xpn_write: fd = %d offset = %d lb = %d\n",
				 __FILE__,__LINE__,f[fi],offset,lb);
		    */
		    if ((ret = xpn_pwrite(f[fi],buf,lb,offset))!= lb)
		    {
			  printf("[%s:%d]: ForwardWriting.xpn_write: fd = %d offset = %d lb = %d errno = %d\n",
				 __FILE__,__LINE__,f[fi],offset,lb,ret);
			  exit(1);
		    }
	    }
 	    cont++;
    	    vowel = 'a' + (cont % 24) ;
	}

    	vowel  = 'a' ;
	cont = 0;
    	for (offset=0; offset<=TAMFILE ; offset+=lb)
    	{

	    for (i=0; i<lb; i++)
	    {
		 buf[i]  = vowel + (i % 24) ;
		 buf2[i] = '0' ;
	    }
	    
	    for (fi=0; fi< FILES; fi++)
	    {
		   /*
		    printf("[%s:%d]: ForwardWriting.xpn_write: fd = %d offset = %d lb = %d\n",
				 __FILE__,__LINE__,f[fi],offset,lb);
		    */
		    if ((ret = xpn_pread(f[fi],buf2,lb,offset))!= lb)
		    {
			  printf("[%s:%d]: ForwardWriting.xpn_read: fd = %d offset = %d lb = %d errno = %d\n",
				 __FILE__,__LINE__,f[fi],offset,lb,ret);
			  exit(1);
		    }
		    
	    	    for (i=0; i<lb; i++)
		    {
		      if(buf[i] != buf2[i])
		      {

			  printf("[%s:%d]: (buf[%d]=%c) != (buf2[%d]=%c) offset = %d\n",
				 __FILE__,__LINE__,i,buf[i],i,buf2[i],offset);
			  exit(1);
		      }
		    }
	    }			      
		    
 	    cont++;
    	    vowel = 'a' + (cont % 24) ;
	}
    	for (fi=0; fi< FILES; fi++)
    	{
	    xpn_close(f[fi]) ;
	    /*
	    printf("[%s:%d]: ForwardWriting.xpn_close: %d\n",
				 __FILE__,__LINE__,f[fi]);
    	    */
	    sprintf(fname, "/PNFS/test-%d",fi); 
	    xpn_unlink(fname) ;
	    /*
	    printf("[%s:%d]: ForwardWriting.xpn_unlink: %s\n",
				 __FILE__,__LINE__,fname);
    	    */
	}
    	printf("[%s:%d]: Succesfully loop %d\n",
                   __FILE__,__LINE__,lb);
    }

    /* finalize xpn */
    xpn_destroy() ;
    printf("[%s:%d]: Succesfully test\n",
                   __FILE__,__LINE__);
    exit(0);
}

