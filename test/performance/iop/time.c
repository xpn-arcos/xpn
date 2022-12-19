#include <sys/time.h>
#include "time.h"
#include <stdio.h>




void Timer(struct timeval * t)
{
     int i=0;
     
     while (gettimeofday(t, 0)==-1)
	i++;
     if (i!=0)
	printf("intentos:%d\n",i+1);
}


/* dig = tn -to                       */
 void DiffTime(
      struct timeval * to,
      struct timeval * tn,
      struct timeval * dif
)
{
     long aux;

     aux = (tn->tv_usec - to->tv_usec) + USECPSEC;
     dif->tv_usec = aux % USECPSEC;
     dif->tv_sec = (tn->tv_sec - to->tv_sec) - (1 - (aux / USECPSEC));
    /* if(dif->tv_sec<0){
	printf("segundos:%d\n",dif->tv_sec);
	exit(0);
     }*/
}


 void AddTime(
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

/* convierte un tiempo timet segundos */
float TimevaltoFloat(struct timeval* timet)
{
   return( (float)(timet->tv_sec + (float)timet->tv_usec/ (float)USECPSEC));
}

float TimevaltoMicro(struct timeval* timet)
{
   return( (float)timet->tv_sec * (float)USECPSEC + (float)timet->tv_usec);
}


