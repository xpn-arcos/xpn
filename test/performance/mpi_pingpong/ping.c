
/* ....................................................................
 *
 * Copyright Alejandro Calderon (1997-1999)
 * <acaldero@laurel.datsi.fi.upm.es>
 * See documentation for more information.
 *
 * permission is hereby granted to copy, modify and redistribute this code
 * in terms of the GNU Library General Public License, Version 2 or later,
 * at your option.
 *
 * .................................................................... */


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "mpi.h"


#define PRUEBAS 		20


int main(int argc, char *argv[])
{
	int             ret;
	char           *buf;
	char            processor_name[MPI_MAX_PROCESSOR_NAME];
	int             namelen;
	double          start_time;
        double          used_time;
        double          avg_time;
        double          us_rate;
        int             max_len, lenbuf;
        int             j;
	int             me, nproc;
	MPI_Status      status;


	setbuf(stdout, NULL);

	// max_length
	max_len = 1024 * 1024;
        if (argc > 1) {
            max_len =atoi(argv[1]);
	}
        if ( (max_len <= 0) || (max_len >= 8*1024*1024) )
        {
                printf("ERROR: max_len = %i\n",max_len);
                printf("ERROR: (max_len <= 0) || (max_len >= 4*1024*1024)\n");
                exit(1) ;
        }

	// Initialization
	ret = MPI_Init(&argc, &argv);	
	if (ret < 0)
	{
		printf("Can't init\n");
		exit(1);
	}

	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	MPI_Get_processor_name(processor_name,&namelen);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	MPI_Barrier(MPI_COMM_WORLD) ;

        buf = (char *) malloc((unsigned) max_len);
        if (buf == NULL)
        {
                perror("Error en malloc");
                exit(1);
        }

#if (0)
	printf("PING ready!\n");
	if (me == 0)
	{
	    ret=MPI_Send(buf,12,MPI_CHAR,1,1, MPI_COMM_WORLD);
	}
	else
	{
	    ret=MPI_Recv(buf,12,MPI_CHAR,0,1, MPI_COMM_WORLD, &status);
	    printf("count = %d\n", status.count);
	}
#endif

	// header (1/2)
	if (0 == me)
	    printf("len_bytes;avg_time_sec;rate_Mbytes_sec\n") ;

	lenbuf = 1 ;
        while (lenbuf <= max_len)
        {
		avg_time = 0.0;
		if (me == 0)
		{
			for(j = 0; j < PRUEBAS; j++)
                	{
				ret=MPI_Recv(buf,lenbuf,MPI_CHAR,1,1,
						MPI_COMM_WORLD, &status);
                                /*
                		if (ret != MPI_SUCCESS)
                        		perror("Error en MPI_Recv");
                                */

				ret=MPI_Send(buf,lenbuf,MPI_CHAR,1,1,
					MPI_COMM_WORLD);
                                /*
                		if (ret != MPI_SUCCESS)
                        		perror("Error en MPI_Send\n");
                                */
			}
		}
		else
		{
			for(j = 0; j < PRUEBAS; j++)
                        {
				start_time = MPI_Wtime();

				ret=MPI_Send(buf,lenbuf,MPI_CHAR,0,1,
                                        MPI_COMM_WORLD);

                                /*
                        	if (ret != MPI_SUCCESS)
                                	perror("Error en MPI_Send\n");
                                */
				ret=MPI_Recv(buf,lenbuf,MPI_CHAR,0,1,
                                                MPI_COMM_WORLD, &status);
                                /*
                        	if (ret != MPI_SUCCESS)
                                	perror("Error en MPI_Recv");
                                */

				used_time = (MPI_Wtime() - start_time);
				avg_time = avg_time + used_time;
			}

                	avg_time =  avg_time / (float)  PRUEBAS;
			if (avg_time > 0)    /* rate is megabytes per second */
                        	us_rate = (double)((nproc * lenbuf)/
					(avg_time*(double)1000000));
                	else
                        	us_rate = 0.0;

	                // values (2/2)
	                if (0 == me)
               	            printf("%e;%e;%e\n", (double)lenbuf, (double)avg_time, (double)us_rate);
               	         // printf("len_bytes=%e avg_time_sec=%e rate_Mbytes_sec=%e\n", (double)lenbuf, (double)avg_time, (double)us_rate);
                }

                lenbuf *= 2;
        }

#if (0)
	if (me != 0)
	    printf("\nclock resolution in seconds: %10.8f\n", MPI_Wtick());
#endif

	MPI_Finalize();
        free(buf);
	exit(0);

}

