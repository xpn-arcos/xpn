
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
 *
 *  This file is part of Expand.
 *
 *  Expand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Expand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


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

	// header...
	if (0 == me)
	    printf("len_bytes;avg_time_sec;rate_Mbytes_sec\n") ;

	lenbuf = 1 ;
        while (lenbuf <= max_len)
        {
		avg_time = 0.0;
		if (me == 0)
		{
			for (j = 0; j < PRUEBAS; j++)
                	{
				MPI_Recv(buf,lenbuf,MPI_CHAR,1,1, MPI_COMM_WORLD, &status);
				MPI_Send(buf,lenbuf,MPI_CHAR,1,1, MPI_COMM_WORLD);
			}
		}
		else
		{
			for (j = 0; j < PRUEBAS; j++)
                        {
				start_time = MPI_Wtime();

				MPI_Send(buf,lenbuf,MPI_CHAR,0,1, MPI_COMM_WORLD);
				MPI_Recv(buf,lenbuf,MPI_CHAR,0,1, MPI_COMM_WORLD, &status);

				used_time = (MPI_Wtime() - start_time);
				avg_time = avg_time + used_time;
			}

                	avg_time =  avg_time / (float)  PRUEBAS;
			if (avg_time > 0)    /* rate is megabytes per second */
                             us_rate = (double)((nproc * lenbuf) / (avg_time*(double)1000000));
                	else us_rate = 0.0;

	                // ... and values
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

	return 0;
}

