#include <sys/time.h>
#include <stdio.h>

#define USECPSEC		1000000



 void Timer(struct timeval * t);
 void DiffTime(
      struct timeval * to,
      struct timeval * tn,
      struct timeval * dif
);
 void AddTime(
      struct timeval * to,
      struct timeval * tn,
      struct timeval * sum
);
float TimevaltoFloat(struct timeval* timet);
float TimevaltoMicro(struct timeval* timet);

