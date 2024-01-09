
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Luis Miguel Sanchez Garcia, Borja Bergua Guerra
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


   /* ... Include / Inclusion ........................................... */

      #include "base/time_misc.h"


   /* ... Functions / Funciones ......................................... */

    /**
     * Get a timestamp 't' at current time.
     * @param t the timestamp.
     * @return nothing.
     */
     void TIME_MISC_Timer
     (
         struct timeval * t
     )
     {
	     int i=0;
	     
	     while (gettimeofday(t, 0)==-1)
		i++;

	     if (i!=0)
		printf("WARNING: fail to gettimeofday in '%d' times.\n",i+1);
     }


    /**
     * Compute 'dig = tn - to'.
     * @param to initial instant.
     * @param tn final instant.
     * @param dif the substaction value ('tn - to').
     * @return nothing.
     */
     void TIME_MISC_DiffTime
     (
         struct timeval * to,
         struct timeval * tn,
         struct timeval * dif
     )
     {
	     long aux;

	     aux = (tn->tv_usec - to->tv_usec) + USECPSEC;
	     dif->tv_usec = aux % USECPSEC;
	     dif->tv_sec = (tn->tv_sec - to->tv_sec) - (1 - (aux / USECPSEC));
	     /* 
                if(dif->tv_sec<0)
                {
	  	   printf("segundos:%d\n",dif->tv_sec);
		   exit(0);
	        }
             */
     }


    /**
     * Compute 'sum = tn + to'.
     * @param to initial instant.
     * @param tn final instant.
     * @param sum the addition value ('tn + to').
     * @return nothing.
     */
     void TIME_MISC_AddTime
     (
           struct timeval * to,
           struct timeval * tn,
           struct timeval * sum
     )
     {
	     long aux;

	     aux = tn->tv_usec + to->tv_usec;
	     sum->tv_usec = aux % USECPSEC;
	     sum->tv_sec = (tn->tv_sec + to->tv_sec) + (aux / USECPSEC);
     }


    /**
     * Return a timestamp as seconds.
     * @param timet the timestamp.
     * @return timestamp as seconds.
     */
     float TIME_MISC_TimevaltoFloat(struct timeval* timet)
     {
         return( (float)(timet->tv_sec + (float)timet->tv_usec/ (float)USECPSEC));
     }


    /**
     * Return a timestamp as microseconds.
     * @param timet the timestamp.
     * @return timestamp as microseconds.
     */
     float TIME_MISC_TimevaltoMicro(struct timeval* timet)
     {
         return( (float)timet->tv_sec * (float)USECPSEC + (float)timet->tv_usec);
     }


   /* ................................................................... */

