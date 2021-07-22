# 1 "prueba_isdir.c"
 



# 1 "../../include/xpn.h" 1



# 1 "../../include/nfi.h" 1



# 1 "../../include/nfi_worker.h" 1



# 1 "../../include/all_system.h" 1





 



	
# 1 "../../include/config.h" 1
 


 



 
 

 
 

 
 

 



 



 
 

 


 


 


 


 


 


 


 


 


 



 



 


 


 


 


 


 
 

 
 

 


 


 
 

 


 


 


 


 


 
 

 


 


 


 



 



 


# 10 "../../include/all_system.h" 2








 



# 36 "../../include/all_system.h"



 


# 1 "/usr/include/stdlib.h" 1 3
 

















 





# 1 "/usr/include/features.h" 1 3
 




















 






























































 




















 





 



 







 
# 142 "/usr/include/features.h" 3


 









 








 



























# 208 "/usr/include/features.h" 3


































 



 


 








 




 
















 


# 1 "/usr/include/sys/cdefs.h" 1 3
 




















 




 






 





 








 



# 65 "/usr/include/sys/cdefs.h" 3


 





 




 









 







 

















 















 







 






 








 








 








 











 










 







 




 


















# 283 "/usr/include/features.h" 2 3



 








 





 

 








# 1 "/usr/include/gnu/stubs.h" 1 3
 



















# 312 "/usr/include/features.h" 2 3




# 25 "/usr/include/stdlib.h" 2 3


 





# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


































typedef unsigned int size_t;






















 




 





























 


























typedef long int wchar_t;

















# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 33 "/usr/include/stdlib.h" 2 3


 




# 91 "/usr/include/stdlib.h" 3


 
typedef struct
  {
    int quot;			 
    int rem;			 
  } div_t;

 

typedef struct
  {
    long int quot;		 
    long int rem;		 
  } ldiv_t;



# 118 "/usr/include/stdlib.h" 3



 



 





 

extern size_t __ctype_get_mb_cur_max (void)  ;


 
extern double atof (__const char *__nptr)    ;
 
extern int atoi (__const char *__nptr)    ;
 
extern long int atol (__const char *__nptr)    ;


 
__extension__ extern long long int atoll (__const char *__nptr)
        ;


 
extern double strtod (__const char *__restrict __nptr,
		      char **__restrict __endptr)  ;










 
extern long int strtol (__const char *__restrict __nptr,
			char **__restrict __endptr, int __base)  ;
 
extern unsigned long int strtoul (__const char *__restrict __nptr,
				  char **__restrict __endptr, int __base)
      ;


 
__extension__
extern long long int strtoq (__const char *__restrict __nptr,
			     char **__restrict __endptr, int __base)  ;
 
__extension__
extern unsigned long long int strtouq (__const char *__restrict __nptr,
				       char **__restrict __endptr, int __base)
      ;



 

 
__extension__
extern long long int strtoll (__const char *__restrict __nptr,
			      char **__restrict __endptr, int __base)  ;
 
__extension__
extern unsigned long long int strtoull (__const char *__restrict __nptr,
					char **__restrict __endptr, int __base)
      ;



# 244 "/usr/include/stdlib.h" 3



 


extern double __strtod_internal (__const char *__restrict __nptr,
				 char **__restrict __endptr, int __group)
      ;
extern float __strtof_internal (__const char *__restrict __nptr,
				char **__restrict __endptr, int __group)
      ;
extern long double __strtold_internal (__const char *__restrict __nptr,
				       char **__restrict __endptr,
				       int __group)  ;

extern long int __strtol_internal (__const char *__restrict __nptr,
				   char **__restrict __endptr,
				   int __base, int __group)  ;



extern unsigned long int __strtoul_internal (__const char *__restrict __nptr,
					     char **__restrict __endptr,
					     int __base, int __group)  ;




__extension__
extern long long int __strtoll_internal (__const char *__restrict __nptr,
					 char **__restrict __endptr,
					 int __base, int __group)  ;



__extension__
extern unsigned long long int __strtoull_internal (__const char *
						   __restrict __nptr,
						   char **__restrict __endptr,
						   int __base, int __group)
      ;




# 377 "/usr/include/stdlib.h" 3




 


extern char *l64a (long int __n)  ;

 
extern long int a64l (__const char *__s)    ;


# 1 "/usr/include/sys/types.h" 1 3
 

















 








 

# 1 "/usr/include/bits/types.h" 1 3
 

















 









# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 29 "/usr/include/bits/types.h" 2 3


 
typedef unsigned char __u_char;
typedef unsigned short __u_short;
typedef unsigned int __u_int;
typedef unsigned long __u_long;

__extension__ typedef unsigned long long int __u_quad_t;
__extension__ typedef long long int __quad_t;
# 48 "/usr/include/bits/types.h" 3

typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;

typedef __quad_t *__qaddr_t;

typedef __u_quad_t __dev_t;		 
typedef __u_int __uid_t;		 
typedef __u_int __gid_t;		 
typedef __u_long __ino_t;		 
typedef __u_int __mode_t;		 
typedef __u_int __nlink_t; 		 
typedef long int __off_t;		 
typedef __quad_t __loff_t;		 
typedef int __pid_t;			 
typedef int __ssize_t;			 
typedef __u_long __rlim_t;		 
typedef __u_quad_t __rlim64_t;		 
typedef __u_int __id_t;			 

typedef struct
  {
    int __val[2];
  } __fsid_t;				 

 
typedef int __daddr_t;			 
typedef char *__caddr_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef long int __swblk_t;		 

typedef long int __clock_t;

 
typedef int __clockid_t;

 
typedef int __timer_t;


 



typedef int __key_t;

 
typedef unsigned short int __ipc_pid_t;


 
typedef long int __blksize_t;

 

 
typedef long int __blkcnt_t;
typedef __quad_t __blkcnt64_t;

 
typedef __u_long __fsblkcnt_t;
typedef __u_quad_t __fsblkcnt64_t;

 
typedef __u_long __fsfilcnt_t;
typedef __u_quad_t __fsfilcnt64_t;

 
typedef __u_quad_t __ino64_t;

 
typedef __loff_t __off64_t;

 
typedef long int __t_scalar_t;
typedef unsigned long int __t_uscalar_t;

 
typedef int __intptr_t;

 
typedef unsigned int __socklen_t;


 

# 1 "/usr/include/bits/pthreadtypes.h" 1 3
 
 
 
 
 
 
 
 
 
 
 
 
 









# 1 "/usr/include/bits/sched.h" 1 3
 



















# 62 "/usr/include/bits/sched.h" 3





 
struct __sched_param
  {
    int __sched_priority;
  };


# 23 "/usr/include/bits/pthreadtypes.h" 2 3


typedef int __atomic_lock_t;

 
struct _pthread_fastlock
{
  long int __status;    
  __atomic_lock_t __spinlock;   

};


 
typedef struct _pthread_descr_struct *_pthread_descr;




 
typedef struct __pthread_attr_s
{
  int __detachstate;
  int __schedpolicy;
  struct __sched_param __schedparam;
  int __inheritsched;
  int __scope;
  size_t __guardsize;
  int __stackaddr_set;
  void *__stackaddr;
  size_t __stacksize;
} pthread_attr_t;


 
typedef struct
{
  struct _pthread_fastlock __c_lock;  
  _pthread_descr __c_waiting;         
} pthread_cond_t;


 
typedef struct
{
  int __dummy;
} pthread_condattr_t;

 
typedef unsigned int pthread_key_t;


 
 

typedef struct
{
  int __m_reserved;                
  int __m_count;                   
  _pthread_descr __m_owner;        
  int __m_kind;                    
  struct _pthread_fastlock __m_lock;  
} pthread_mutex_t;


 
typedef struct
{
  int __mutexkind;
} pthread_mutexattr_t;


 
typedef int pthread_once_t;


# 119 "/usr/include/bits/pthreadtypes.h" 3


# 138 "/usr/include/bits/pthreadtypes.h" 3



 
typedef unsigned long int pthread_t;


# 143 "/usr/include/bits/types.h" 2 3




# 30 "/usr/include/sys/types.h" 2 3




typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;




typedef __loff_t loff_t;



typedef __ino_t ino_t;











typedef __dev_t dev_t;




typedef __gid_t gid_t;




typedef __mode_t mode_t;




typedef __nlink_t nlink_t;




typedef __uid_t uid_t;





typedef __off_t off_t;











typedef __pid_t pid_t;




typedef __id_t id_t;




typedef __ssize_t ssize_t;





typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;





typedef __key_t key_t;









# 1 "/usr/include/time.h" 1 3
 

















 














# 51 "/usr/include/time.h" 3


# 61 "/usr/include/time.h" 3








 
typedef __time_t time_t;










 
typedef __clockid_t clockid_t;










 
typedef __timer_t timer_t;





# 112 "/usr/include/time.h" 3




# 364 "/usr/include/time.h" 3



# 132 "/usr/include/sys/types.h" 2 3


# 143 "/usr/include/sys/types.h" 3



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 146 "/usr/include/sys/types.h" 2 3



 
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;


 

# 180 "/usr/include/sys/types.h" 3


 







typedef int int8_t __attribute__ ((__mode__ (  __QI__ ))) ;
typedef int int16_t __attribute__ ((__mode__ (  __HI__ ))) ;
typedef int int32_t __attribute__ ((__mode__ (  __SI__ ))) ;
typedef int int64_t __attribute__ ((__mode__ (  __DI__ ))) ;


typedef unsigned int u_int8_t __attribute__ ((__mode__ (  __QI__ ))) ;
typedef unsigned int u_int16_t __attribute__ ((__mode__ (  __HI__ ))) ;
typedef unsigned int u_int32_t __attribute__ ((__mode__ (  __SI__ ))) ;
typedef unsigned int u_int64_t __attribute__ ((__mode__ (  __DI__ ))) ;

typedef int register_t __attribute__ ((__mode__ (__word__)));


 






 
# 1 "/usr/include/endian.h" 1 3
 






















 











 
# 1 "/usr/include/bits/endian.h" 1 3
 






# 37 "/usr/include/endian.h" 2 3


 



















# 212 "/usr/include/sys/types.h" 2 3


 
# 1 "/usr/include/sys/select.h" 1 3
 


















 






 


 
# 1 "/usr/include/bits/select.h" 1 3
 

























# 36 "/usr/include/bits/select.h" 3












# 56 "/usr/include/bits/select.h" 3

# 72 "/usr/include/bits/select.h" 3

# 31 "/usr/include/sys/select.h" 2 3


 
# 1 "/usr/include/bits/sigset.h" 1 3
 





















typedef int __sig_atomic_t;

 


typedef struct
  {
    unsigned long int __val[(1024 / (8 * sizeof (unsigned long int))) ];
  } __sigset_t;




 





# 125 "/usr/include/bits/sigset.h" 3

# 34 "/usr/include/sys/select.h" 2 3




typedef __sigset_t sigset_t;


 


# 1 "/usr/include/time.h" 1 3
 

















 














# 51 "/usr/include/time.h" 3


# 61 "/usr/include/time.h" 3



# 72 "/usr/include/time.h" 3



# 84 "/usr/include/time.h" 3



# 96 "/usr/include/time.h" 3








 

struct timespec
  {
    __time_t tv_sec;		 
    long int tv_nsec;		 
  };





# 364 "/usr/include/time.h" 3



# 44 "/usr/include/sys/select.h" 2 3


# 1 "/usr/include/bits/time.h" 1 3
 


















 



# 57 "/usr/include/bits/time.h" 3








 

struct timeval
  {
    __time_t tv_sec;		 
    __suseconds_t tv_usec;	 
  };


# 46 "/usr/include/sys/select.h" 2 3



typedef __suseconds_t suseconds_t;




 
typedef long int __fd_mask;

 




 
typedef struct
  {
     





    __fd_mask __fds_bits[1024  / (8 * sizeof (__fd_mask)) ];


  } fd_set;

 



 
typedef __fd_mask fd_mask;

 




 






 

 




extern int select (int __nfds, fd_set *__restrict __readfds,
		   fd_set *__restrict __writefds,
		   fd_set *__restrict __exceptfds,
		   struct timeval *__restrict __timeout)  ;

# 116 "/usr/include/sys/select.h" 3


 


# 215 "/usr/include/sys/types.h" 2 3


 
# 1 "/usr/include/sys/sysmacros.h" 1 3
 





















 








# 47 "/usr/include/sys/sysmacros.h" 3



# 218 "/usr/include/sys/types.h" 2 3









 


typedef __blkcnt_t blkcnt_t;	  



typedef __fsblkcnt_t fsblkcnt_t;  



typedef __fsfilcnt_t fsfilcnt_t;  


# 254 "/usr/include/sys/types.h" 3








 


# 390 "/usr/include/stdlib.h" 2 3


 



 
extern long int random (void)  ;

 
extern void srandom (unsigned int __seed)  ;

 



extern char *initstate (unsigned int __seed, char *__statebuf,
			size_t __statelen)  ;

 

extern char *setstate (char *__statebuf)  ;



 



struct random_data
  {
    int32_t *fptr;		 
    int32_t *rptr;		 
    int32_t *state;		 
    int rand_type;		 
    int rand_deg;		 
    int rand_sep;		 
    int32_t *end_ptr;		 
  };

extern int random_r (struct random_data *__restrict __buf,
		     int32_t *__restrict __result)  ;

extern int srandom_r (unsigned int __seed, struct random_data *__buf)  ;

extern int initstate_r (unsigned int __seed, char *__restrict __statebuf,
			size_t __statelen,
			struct random_data *__restrict __buf)  ;

extern int setstate_r (char *__restrict __statebuf,
		       struct random_data *__restrict __buf)  ;




 
extern int rand (void)  ;
 
extern void srand (unsigned int __seed)  ;


 
extern int rand_r (unsigned int *__seed)  ;




 

 
extern double drand48 (void)  ;
extern double erand48 (unsigned short int __xsubi[3])  ;

 
extern long int lrand48 (void)  ;
extern long int nrand48 (unsigned short int __xsubi[3])  ;

 
extern long int mrand48 (void)  ;
extern long int jrand48 (unsigned short int __xsubi[3])  ;

 
extern void srand48 (long int __seedval)  ;
extern unsigned short int *seed48 (unsigned short int __seed16v[3])  ;
extern void lcong48 (unsigned short int __param[7])  ;


 


struct drand48_data
  {
    unsigned short int __x[3];	 
    unsigned short int __old_x[3];  
    unsigned short int __c;	 
    unsigned short int __init;	 
    unsigned long long int __a;	 
  };

 
extern int drand48_r (struct drand48_data *__restrict __buffer,
		      double *__restrict __result)  ;
extern int erand48_r (unsigned short int __xsubi[3],
		      struct drand48_data *__restrict __buffer,
		      double *__restrict __result)  ;

 
extern int lrand48_r (struct drand48_data *__restrict __buffer,
		      long int *__restrict __result)  ;
extern int nrand48_r (unsigned short int __xsubi[3],
		      struct drand48_data *__restrict __buffer,
		      long int *__restrict __result)  ;

 
extern int mrand48_r (struct drand48_data *__restrict __buffer,
		      long int *__restrict __result)  ;
extern int jrand48_r (unsigned short int __xsubi[3],
		      struct drand48_data *__restrict __buffer,
		      long int *__restrict __result)  ;

 
extern int srand48_r (long int __seedval, struct drand48_data *__buffer)
      ;

extern int seed48_r (unsigned short int __seed16v[3],
		     struct drand48_data *__buffer)  ;

extern int lcong48_r (unsigned short int __param[7],
		      struct drand48_data *__buffer)  ;







 
extern void *malloc (size_t __size)    ;
 
extern void *calloc (size_t __nmemb, size_t __size)
        ;



 

extern void *realloc (void *__ptr, size_t __size)    ;
 
extern void free (void *__ptr)  ;


 
extern void cfree (void *__ptr)  ;



# 1 "/usr/include/alloca.h" 1 3
 























# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 25 "/usr/include/alloca.h" 2 3


 

 


 
extern void *alloca (size_t __size)  ;





 


# 546 "/usr/include/stdlib.h" 2 3




 
extern void *valloc (size_t __size)    ;








 
extern void abort (void)   __attribute__ ((__noreturn__));


 
extern int atexit (void (*__func) (void))  ;


 

extern int on_exit (void (*__func) (int __status, void *__arg), void *__arg)
      ;


 


extern void exit (int __status)   __attribute__ ((__noreturn__));








 
extern char *getenv (__const char *__name)  ;

 

extern char *__secure_getenv (__const char *__name)  ;


 
 

extern int putenv (char *__string)  ;



 

extern int setenv (__const char *__name, __const char *__value, int __replace)
      ;

 
extern int unsetenv (__const char *__name)  ;



 


extern int clearenv (void)  ;




 



extern char *mktemp (char *__template)  ;

 





extern int mkstemp (char *__template)  ;













 




extern char *mkdtemp (char *__template)  ;



 
extern int system (__const char *__command)  ;










 





extern char *realpath (__const char *__restrict __name,
		       char *__restrict __resolved)  ;



 


typedef int (*__compar_fn_t) (__const void *, __const void *);






 

extern void *bsearch (__const void *__key, __const void *__base,
		      size_t __nmemb, size_t __size, __compar_fn_t __compar);

 

extern void qsort (void *__base, size_t __nmemb, size_t __size,
		   __compar_fn_t __compar);


 
extern int abs (int __x)   __attribute__ ((__const__));
extern long int labs (long int __x)   __attribute__ ((__const__));






 

 
extern div_t div (int __numer, int __denom)
       __attribute__ ((__const__));
extern ldiv_t ldiv (long int __numer, long int __denom)
       __attribute__ ((__const__));








 


 


extern char *ecvt (double __value, int __ndigit, int *__restrict __decpt,
		   int *__restrict __sign)  ;

 


extern char *fcvt (double __value, int __ndigit, int *__restrict __decpt,
		   int *__restrict __sign)  ;

 


extern char *gcvt (double __value, int __ndigit, char *__buf)  ;



 
extern char *qecvt (long double __value, int __ndigit,
		    int *__restrict __decpt, int *__restrict __sign)  ;
extern char *qfcvt (long double __value, int __ndigit,
		    int *__restrict __decpt, int *__restrict __sign)  ;
extern char *qgcvt (long double __value, int __ndigit, char *__buf)  ;


 

extern int ecvt_r (double __value, int __ndigit, int *__restrict __decpt,
		   int *__restrict __sign, char *__restrict __buf,
		   size_t __len)  ;
extern int fcvt_r (double __value, int __ndigit, int *__restrict __decpt,
		   int *__restrict __sign, char *__restrict __buf,
		   size_t __len)  ;

extern int qecvt_r (long double __value, int __ndigit,
		    int *__restrict __decpt, int *__restrict __sign,
		    char *__restrict __buf, size_t __len)  ;
extern int qfcvt_r (long double __value, int __ndigit,
		    int *__restrict __decpt, int *__restrict __sign,
		    char *__restrict __buf, size_t __len)  ;




 

extern int mblen (__const char *__s, size_t __n)  ;
 

extern int mbtowc (wchar_t *__restrict __pwc,
		   __const char *__restrict __s, size_t __n)  ;
 

extern int wctomb (char *__s, wchar_t __wchar)  ;


 
extern size_t mbstowcs (wchar_t *__restrict  __pwcs,
			__const char *__restrict __s, size_t __n)  ;
 
extern size_t wcstombs (char *__restrict __s,
			__const wchar_t *__restrict __pwcs, size_t __n)
      ;



 



extern int rpmatch (__const char *__response)  ;



# 811 "/usr/include/stdlib.h" 3









 






# 842 "/usr/include/stdlib.h" 3


# 852 "/usr/include/stdlib.h" 3



 


extern int getloadavg (double __loadavg[], int __nelem)  ;





 


# 42 "../../include/all_system.h" 2

# 1 "/usr/include/stdio.h" 1 3
 


















 









 



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 34 "/usr/include/stdio.h" 2 3










 
typedef struct _IO_FILE FILE;








 
typedef struct _IO_FILE __FILE;









# 1 "/usr/include/libio.h" 1 3
 






























# 1 "/usr/include/_G_config.h" 1 3
 





 






# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 


# 269 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3











typedef unsigned int  wint_t;




 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 14 "/usr/include/_G_config.h" 2 3










# 1 "/usr/include/wchar.h" 1 3
 

















 











# 46 "/usr/include/wchar.h" 3


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 190 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3














 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 48 "/usr/include/wchar.h" 2 3


# 1 "/usr/include/bits/wchar.h" 1 3
 

























# 50 "/usr/include/wchar.h" 2 3


 













 
typedef struct
{
  int __count;
  union
  {
    wint_t __wch;
    char __wchb[4];
  } __value;		 
} __mbstate_t;




 

# 682 "/usr/include/wchar.h" 3



# 24 "/usr/include/_G_config.h" 2 3


typedef struct
{
  __off_t __pos;
  __mbstate_t __state;
} _G_fpos_t;
typedef struct
{
  __off64_t __pos;
  __mbstate_t __state;
} _G_fpos64_t;








# 1 "/usr/include/gconv.h" 1 3
 

















 








# 1 "/usr/include/wchar.h" 1 3
 

















 











# 46 "/usr/include/wchar.h" 3


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 190 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3














 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 48 "/usr/include/wchar.h" 2 3




 











# 76 "/usr/include/wchar.h" 3




 

# 682 "/usr/include/wchar.h" 3



# 28 "/usr/include/gconv.h" 2 3



# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 


# 269 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 31 "/usr/include/gconv.h" 2 3


 


 
enum
{
  __GCONV_OK = 0,
  __GCONV_NOCONV,
  __GCONV_NODB,
  __GCONV_NOMEM,

  __GCONV_EMPTY_INPUT,
  __GCONV_FULL_OUTPUT,
  __GCONV_ILLEGAL_INPUT,
  __GCONV_INCOMPLETE_INPUT,

  __GCONV_ILLEGAL_DESCRIPTOR,
  __GCONV_INTERNAL_ERROR
};


 
enum
{
  __GCONV_IS_LAST = 0x0001,
  __GCONV_IGNORE_ERRORS = 0x0002
};


 
struct __gconv_step;
struct __gconv_step_data;
struct __gconv_loaded_object;
struct __gconv_trans_data;


 
typedef int (*__gconv_fct) (struct __gconv_step *, struct __gconv_step_data *,
			    __const unsigned char **, __const unsigned char *,
			    unsigned char **, size_t *, int, int);

 
typedef int (*__gconv_init_fct) (struct __gconv_step *);
typedef void (*__gconv_end_fct) (struct __gconv_step *);


 
typedef int (*__gconv_trans_fct) (struct __gconv_step *,
				  struct __gconv_step_data *, void *,
				  __const unsigned char *,
				  __const unsigned char **,
				  __const unsigned char *, unsigned char **,
				  size_t *);

 
typedef int (*__gconv_trans_context_fct) (void *, __const unsigned char *,
					  __const unsigned char *,
					  unsigned char *, unsigned char *);

 
typedef int (*__gconv_trans_query_fct) (__const char *, __const char ***,
					size_t *);

 
typedef int (*__gconv_trans_init_fct) (void **, const char *);
typedef void (*__gconv_trans_end_fct) (void *);

struct __gconv_trans_data
{
   
  __gconv_trans_fct __trans_fct;
  __gconv_trans_context_fct __trans_context_fct;
  __gconv_trans_end_fct __trans_end_fct;
  void *__data;
  struct __gconv_trans_data *__next;
};


 
struct __gconv_step
{
  struct __gconv_loaded_object *__shlib_handle;
  __const char *__modname;

  int __counter;

  char *__from_name;
  char *__to_name;

  __gconv_fct __fct;
  __gconv_init_fct __init_fct;
  __gconv_end_fct __end_fct;

   

  int __min_needed_from;
  int __max_needed_from;
  int __min_needed_to;
  int __max_needed_to;

   
  int __stateful;

  void *__data;		 
};

 

struct __gconv_step_data
{
  unsigned char *__outbuf;     
  unsigned char *__outbufend;  


   
  int __flags;

   

  int __invocation_counter;

   

  int __internal_use;

  __mbstate_t *__statep;
  __mbstate_t __state;	 


   
  struct __gconv_trans_data *__trans;
};


 
typedef struct __gconv_info
{
  size_t __nsteps;
  struct __gconv_step *__steps;
  __extension__ struct __gconv_step_data __data [0] ;
} *__gconv_t;


# 44 "/usr/include/_G_config.h" 2 3

typedef union
{
  struct __gconv_info __cd;
  struct
  {
    struct __gconv_info __cd;
    struct __gconv_step_data __data;
  } __combined;
} _G_iconv_t;

typedef int _G_int16_t __attribute__ ((__mode__ (__HI__)));
typedef int _G_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int _G_uint16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int _G_uint32_t __attribute__ ((__mode__ (__SI__)));




 


















 




 














# 32 "/usr/include/libio.h" 2 3

 

















 

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h" 1 3
 
































































 






typedef void *__gnuc_va_list;



 

# 122 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h" 3




















# 209 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h" 3




# 53 "/usr/include/libio.h" 2 3







# 72 "/usr/include/libio.h" 3


 

















# 103 "/usr/include/libio.h" 3











 

























 



















struct _IO_jump_t;  struct _IO_FILE;

 







typedef void _IO_lock_t;



 

struct _IO_marker {
  struct _IO_marker *_next;
  struct _IO_FILE *_sbuf;
   

   
  int _pos;
# 192 "/usr/include/libio.h" 3

};

 
enum __codecvt_result
{
  __codecvt_ok,
  __codecvt_partial,
  __codecvt_error,
  __codecvt_noconv
};

# 259 "/usr/include/libio.h" 3


struct _IO_FILE {
  int _flags;		 


   
   
  char* _IO_read_ptr;	 
  char* _IO_read_end;	 
  char* _IO_read_base;	 
  char* _IO_write_base;	 
  char* _IO_write_ptr;	 
  char* _IO_write_end;	 
  char* _IO_buf_base;	 
  char* _IO_buf_end;	 
   
  char *_IO_save_base;  
  char *_IO_backup_base;   
  char *_IO_save_end;  

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
  int _blksize;
  __off_t   _old_offset;  


   
  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];

   

  _IO_lock_t *_lock;








  __off64_t   _offset;





  void *__pad1;
  void *__pad2;

  int _mode;
   
  char _unused2[15 * sizeof (int) - 2 * sizeof (void *)];

};


typedef struct _IO_FILE _IO_FILE;


struct _IO_FILE_plus;

extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;











 

 

typedef __ssize_t __io_read_fn (void *__cookie, char *__buf, size_t __nbytes);

 





typedef __ssize_t __io_write_fn (void *__cookie, __const char *__buf,
				 size_t __n);

 





typedef int __io_seek_fn (void *__cookie, __off64_t   *__pos, int __w);

 
typedef int __io_close_fn (void *__cookie);


# 389 "/usr/include/libio.h" 3







extern int __underflow (_IO_FILE *)  ;
extern int __uflow (_IO_FILE *)  ;
extern int __overflow (_IO_FILE *, int)  ;
extern wint_t   __wunderflow (_IO_FILE *)  ;
extern wint_t   __wuflow (_IO_FILE *)  ;
extern wint_t   __woverflow (_IO_FILE *, wint_t  )  ;
























extern int _IO_getc (_IO_FILE *__fp)  ;
extern int _IO_putc (int __c, _IO_FILE *__fp)  ;
extern int _IO_feof (_IO_FILE *__fp)  ;
extern int _IO_ferror (_IO_FILE *__fp)  ;

extern int _IO_peekc_locked (_IO_FILE *__fp)  ;

 



extern void _IO_flockfile (_IO_FILE *)  ;
extern void _IO_funlockfile (_IO_FILE *)  ;
extern int _IO_ftrylockfile (_IO_FILE *)  ;
















extern int _IO_vfscanf (_IO_FILE * __restrict, const char * __restrict,
			__gnuc_va_list , int *__restrict)  ;
extern int _IO_vfprintf (_IO_FILE *__restrict, const char *__restrict,
			 __gnuc_va_list )  ;
extern __ssize_t   _IO_padn (_IO_FILE *, int, __ssize_t  )  ;
extern size_t   _IO_sgetn (_IO_FILE *, void *, size_t  )  ;

extern __off64_t   _IO_seekoff (_IO_FILE *, __off64_t  , int, int)  ;
extern __off64_t   _IO_seekpos (_IO_FILE *, __off64_t  , int)  ;

extern void _IO_free_backup_area (_IO_FILE *)  ;

# 511 "/usr/include/libio.h" 3







# 65 "/usr/include/stdio.h" 2 3


# 76 "/usr/include/stdio.h" 3


 

typedef _G_fpos_t fpos_t;







 





 





 






 







 




 








# 1 "/usr/include/bits/stdio_lim.h" 1 3
 








































# 129 "/usr/include/stdio.h" 2 3



 
extern FILE *stdin;		 
extern FILE *stdout;		 
extern FILE *stderr;		 
 




 
extern int remove (__const char *__filename)  ;
 
extern int rename (__const char *__old, __const char *__new)  ;


 

extern FILE *tmpfile (void)  ;










 
extern char *tmpnam (char *__s)  ;


 

extern char *tmpnam_r (char *__s)  ;




 






extern char *tempnam (__const char *__dir, __const char *__pfx)
        ;



 
extern int fclose (FILE *__stream)  ;
 
extern int fflush (FILE *__stream)  ;


 
extern int fflush_unlocked (FILE *__stream)  ;









 
extern FILE *fopen (__const char *__restrict __filename,
		    __const char *__restrict __modes)  ;
 
extern FILE *freopen (__const char *__restrict __filename,
		      __const char *__restrict __modes,
		      FILE *__restrict __stream)  ;
# 220 "/usr/include/stdio.h" 3










 
extern FILE *fdopen (int __fd, __const char *__modes)  ;


# 249 "/usr/include/stdio.h" 3



 

extern void setbuf (FILE *__restrict __stream, char *__restrict __buf)  ;
 


extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
		    int __modes, size_t __n)  ;


 

extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
		       size_t __size)  ;

 
extern void setlinebuf (FILE *__stream)  ;



 
extern int fprintf (FILE *__restrict __stream,
		    __const char *__restrict __format, ...)  ;
 
extern int printf (__const char *__restrict __format, ...)  ;
 
extern int sprintf (char *__restrict __s,
		    __const char *__restrict __format, ...)  ;

 
extern int vfprintf (FILE *__restrict __s, __const char *__restrict __format,
		     __gnuc_va_list  __arg)  ;
 
extern int vprintf (__const char *__restrict __format, __gnuc_va_list  __arg)
      ;
 
extern int vsprintf (char *__restrict __s, __const char *__restrict __format,
		     __gnuc_va_list  __arg)  ;


 
extern int snprintf (char *__restrict __s, size_t __maxlen,
		     __const char *__restrict __format, ...)
       __attribute__ ((__format__ (__printf__, 3, 4)));

extern int vsnprintf (char *__restrict __s, size_t __maxlen,
		      __const char *__restrict __format, __gnuc_va_list  __arg)
       __attribute__ ((__format__ (__printf__, 3, 0)));


# 321 "/usr/include/stdio.h" 3



 
extern int fscanf (FILE *__restrict __stream,
		   __const char *__restrict __format, ...)  ;
 
extern int scanf (__const char *__restrict __format, ...)  ;
 
extern int sscanf (__const char *__restrict __s,
		   __const char *__restrict __format, ...)  ;

# 347 "/usr/include/stdio.h" 3



 
extern int fgetc (FILE *__stream)  ;
extern int getc (FILE *__stream)  ;

 
extern int getchar (void)  ;

 




 
extern int getc_unlocked (FILE *__stream)  ;
extern int getchar_unlocked (void)  ;



 
extern int fgetc_unlocked (FILE *__stream)  ;



 
extern int fputc (int __c, FILE *__stream)  ;
extern int putc (int __c, FILE *__stream)  ;

 
extern int putchar (int __c)  ;

 




 
extern int fputc_unlocked (int __c, FILE *__stream)  ;



 
extern int putc_unlocked (int __c, FILE *__stream)  ;
extern int putchar_unlocked (int __c)  ;




 
extern int getw (FILE *__stream)  ;

 
extern int putw (int __w, FILE *__stream)  ;



 
extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
      ;







 

extern char *gets (char *__s)  ;


# 437 "/usr/include/stdio.h" 3



 
extern int fputs (__const char *__restrict __s, FILE *__restrict __stream)
      ;







 
extern int puts (__const char *__s)  ;


 
extern int ungetc (int __c, FILE *__stream)  ;


 
extern size_t fread (void *__restrict __ptr, size_t __size,
		     size_t __n, FILE *__restrict __stream)  ;
 
extern size_t fwrite (__const void *__restrict __ptr, size_t __size,
		      size_t __n, FILE *__restrict __s)  ;


 
extern size_t fread_unlocked (void *__restrict __ptr, size_t __size,
			      size_t __n, FILE *__restrict __stream)  ;
extern size_t fwrite_unlocked (__const void *__restrict __ptr, size_t __size,
			       size_t __n, FILE *__restrict __stream)  ;



 
extern int fseek (FILE *__stream, long int __off, int __whence)  ;
 
extern long int ftell (FILE *__stream)  ;
 
extern void rewind (FILE *__stream)  ;

 












 
extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos)
      ;
 
extern int fsetpos (FILE *__stream, __const fpos_t *__pos)  ;
# 520 "/usr/include/stdio.h" 3










 
extern void clearerr (FILE *__stream)  ;
 
extern int feof (FILE *__stream)  ;
 
extern int ferror (FILE *__stream)  ;


 
extern void clearerr_unlocked (FILE *__stream)  ;
extern int feof_unlocked (FILE *__stream)  ;
extern int ferror_unlocked (FILE *__stream)  ;



 
extern void perror (__const char *__s)  ;

 


extern int sys_nerr;
extern __const char *__const sys_errlist[];








 
extern int fileno (FILE *__stream)  ;



 
extern int fileno_unlocked (FILE *__stream)  ;





 
extern FILE *popen (__const char *__command, __const char *__modes)  ;

 
extern int pclose (FILE *__stream)  ;




 
extern char *ctermid (char *__s)  ;









# 602 "/usr/include/stdio.h" 3




 

 
extern void flockfile (FILE *__stream)  ;

 

extern int ftrylockfile (FILE *__stream)  ;

 
extern void funlockfile (FILE *__stream)  ;










 





 




# 43 "../../include/all_system.h" 2

# 1 "/usr/include/errno.h" 1 3
 

















 





 






 

 

# 1 "/usr/include/bits/errno.h" 1 3
 























# 1 "/usr/include/linux/errno.h" 1 3



# 1 "/usr/include/asm/errno.h" 1 3




































































































































# 4 "/usr/include/linux/errno.h" 2 3


# 24 "/usr/include/linux/errno.h" 3



# 25 "/usr/include/bits/errno.h" 2 3


 


 




 
extern int errno;

 
extern int *__errno_location (void)   __attribute__ ((__const__));







 













# 36 "/usr/include/errno.h" 2 3





 

















 



 










# 44 "../../include/all_system.h" 2

# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h" 1 3
 
































































 










 



 

















void va_end (__gnuc_va_list);		 


 



 












 























 
 













# 175 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stdarg.h" 3


 




 

 

 

typedef __gnuc_va_list va_list;
























# 45 "../../include/all_system.h" 2

# 1 "/usr/include/string.h" 1 3
 

















 








 

 


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 33 "/usr/include/string.h" 2 3



 
extern void *memcpy (void *__restrict __dest,
		     __const void *__restrict __src, size_t __n)  ;
 

extern void *memmove (void *__dest, __const void *__src, size_t __n)
      ;

 



extern void *memccpy (void *__restrict __dest, __const void *__restrict __src,
		      int __c, size_t __n)
      ;



 
extern void *memset (void *__s, int __c, size_t __n)  ;

 
extern int memcmp (__const void *__s1, __const void *__s2, size_t __n)
        ;

 
extern void *memchr (__const void *__s, int __c, size_t __n)
         ;

# 73 "/usr/include/string.h" 3



 
extern char *strcpy (char *__restrict __dest, __const char *__restrict __src)
      ;
 
extern char *strncpy (char *__restrict __dest,
		      __const char *__restrict __src, size_t __n)  ;

 
extern char *strcat (char *__restrict __dest, __const char *__restrict __src)
      ;
 
extern char *strncat (char *__restrict __dest, __const char *__restrict __src,
		      size_t __n)  ;

 
extern int strcmp (__const char *__s1, __const char *__s2)
        ;
 
extern int strncmp (__const char *__s1, __const char *__s2, size_t __n)
        ;

 
extern int strcoll (__const char *__s1, __const char *__s2)
        ;
 
extern size_t strxfrm (char *__restrict __dest,
		       __const char *__restrict __src, size_t __n)  ;

# 116 "/usr/include/string.h" 3



 
extern char *strdup (__const char *__s)    ;


 







# 152 "/usr/include/string.h" 3


 
extern char *strchr (__const char *__s, int __c)    ;
 
extern char *strrchr (__const char *__s, int __c)    ;







 

extern size_t strcspn (__const char *__s, __const char *__reject)
        ;
 

extern size_t strspn (__const char *__s, __const char *__accept)
        ;
 
extern char *strpbrk (__const char *__s, __const char *__accept)
        ;
 
extern char *strstr (__const char *__haystack, __const char *__needle)
        ;







 
extern char *strtok (char *__restrict __s, __const char *__restrict __delim)
      ;

 

extern char *__strtok_r (char *__restrict __s,
			 __const char *__restrict __delim,
			 char **__restrict __save_ptr)  ;

extern char *strtok_r (char *__restrict __s, __const char *__restrict __delim,
		       char **__restrict __save_ptr)  ;


# 214 "/usr/include/string.h" 3



 
extern size_t strlen (__const char *__s)    ;









 
extern char *strerror (int __errnum)  ;

 

extern char *strerror_r (int __errnum, char *__buf, size_t __buflen)  ;


 

extern void __bzero (void *__s, size_t __n)  ;


 
extern void bcopy (__const void *__src, void *__dest, size_t __n)  ;

 
extern void bzero (void *__s, size_t __n)  ;

 
extern int bcmp (__const void *__s1, __const void *__s2, size_t __n)
        ;

 
extern char *index (__const char *__s, int __c)    ;

 
extern char *rindex (__const char *__s, int __c)    ;

 

extern int ffs (int __i)   __attribute__ ((__const__));

 









 
extern int strcasecmp (__const char *__s1, __const char *__s2)
        ;

 
extern int strncasecmp (__const char *__s1, __const char *__s2, size_t __n)
        ;


# 289 "/usr/include/string.h" 3



 

extern char *strsep (char **__restrict __stringp,
		     __const char *__restrict __delim)  ;


# 332 "/usr/include/string.h" 3




# 361 "/usr/include/string.h" 3



 


# 46 "../../include/all_system.h" 2

# 1 "/usr/include/stdint.h" 1 3
 

















 








# 1 "/usr/include/bits/wordsize.h" 1 3
 


















# 28 "/usr/include/stdint.h" 2 3


 

 

 
# 46 "/usr/include/stdint.h" 3


 
typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;

typedef unsigned int		uint32_t;





__extension__
typedef unsigned long long int	uint64_t;



 

 
typedef signed char		int_least8_t;
typedef short int		int_least16_t;
typedef int			int_least32_t;



__extension__
typedef long long int		int_least64_t;


 
typedef unsigned char		uint_least8_t;
typedef unsigned short int	uint_least16_t;
typedef unsigned int		uint_least32_t;



__extension__
typedef unsigned long long int	uint_least64_t;



 

 
typedef signed char		int_fast8_t;





typedef int			int_fast16_t;
typedef int			int_fast32_t;
__extension__
typedef long long int		int_fast64_t;


 
typedef unsigned char		uint_fast8_t;





typedef unsigned int		uint_fast16_t;
typedef unsigned int		uint_fast32_t;
__extension__
typedef unsigned long long int	uint_fast64_t;



 








typedef int			intptr_t;


typedef unsigned int		uintptr_t;



 




__extension__
typedef long long int		intmax_t;
__extension__
typedef unsigned long long int	uintmax_t;



 











 

 




 





 






 




 





 






 









 










 











 











 

 


 



 

 








 



 






 

 




 






 



 









 









 











# 47 "../../include/all_system.h" 2


# 1 "/usr/include/dlfcn.h" 1 3
 























 
# 1 "/usr/include/bits/dlfcn.h" 1 3
 






















 





 




 




 


# 64 "/usr/include/bits/dlfcn.h" 3

# 26 "/usr/include/dlfcn.h" 2 3



# 40 "/usr/include/dlfcn.h" 3



 

 

extern void *dlopen (__const char *__file, int __mode)  ;

 

extern int dlclose (void *__handle)  ;

 

extern void *dlsym (void *__restrict __handle,
		    __const char *__restrict __name)  ;









 


extern char *dlerror (void)  ;


# 86 "/usr/include/dlfcn.h" 3


 


# 49 "../../include/all_system.h" 2

# 1 "/usr/include/netdb.h" 1 3
 

















 








# 1 "/usr/include/netinet/in.h" 1 3
 

























 

 
enum
  {
    IPPROTO_IP = 0,	    

    IPPROTO_HOPOPTS = 0,    

    IPPROTO_ICMP = 1,	    

    IPPROTO_IGMP = 2,	    

    IPPROTO_IPIP = 4,	    

    IPPROTO_TCP = 6,	    

    IPPROTO_EGP = 8,	    

    IPPROTO_PUP = 12,	    

    IPPROTO_UDP = 17,	    

    IPPROTO_IDP = 22,	    

    IPPROTO_TP = 29,	    

    IPPROTO_IPV6 = 41,      

    IPPROTO_ROUTING = 43,   

    IPPROTO_FRAGMENT = 44,  

    IPPROTO_RSVP = 46,	    

    IPPROTO_GRE = 47,	    

    IPPROTO_ESP = 50,       

    IPPROTO_AH = 51,        

    IPPROTO_ICMPV6 = 58,    

    IPPROTO_NONE = 59,      

    IPPROTO_DSTOPTS = 60,   

    IPPROTO_MTP = 92,	    

    IPPROTO_ENCAP = 98,	    

    IPPROTO_PIM = 103,	    

    IPPROTO_COMP = 108,	    

    IPPROTO_RAW = 255,	    

    IPPROTO_MAX
  };


 
typedef uint16_t in_port_t;

 
enum
  {
    IPPORT_ECHO = 7,		 
    IPPORT_DISCARD = 9,		 
    IPPORT_SYSTAT = 11,		 
    IPPORT_DAYTIME = 13,	 
    IPPORT_NETSTAT = 15,	 
    IPPORT_FTP = 21,		 
    IPPORT_TELNET = 23,		 
    IPPORT_SMTP = 25,		 
    IPPORT_TIMESERVER = 37,	 
    IPPORT_NAMESERVER = 42,	 
    IPPORT_WHOIS = 43,		 
    IPPORT_MTP = 57,

    IPPORT_TFTP = 69,		 
    IPPORT_RJE = 77,
    IPPORT_FINGER = 79,		 
    IPPORT_TTYLINK = 87,
    IPPORT_SUPDUP = 95,		 


    IPPORT_EXECSERVER = 512,	 
    IPPORT_LOGINSERVER = 513,	 
    IPPORT_CMDSERVER = 514,
    IPPORT_EFSSERVER = 520,

     
    IPPORT_BIFFUDP = 512,
    IPPORT_WHOSERVER = 513,
    IPPORT_ROUTESERVER = 520,

     
    IPPORT_RESERVED = 1024,

     
    IPPORT_USERRESERVED = 5000
  };


 
typedef uint32_t in_addr_t;
struct in_addr
  {
    in_addr_t s_addr;
  };


 



























 

 

 


 

 




 






 
struct in6_addr
  {
    union
      {
	uint8_t	u6_addr8[16];
	uint16_t u6_addr16[8];
	uint32_t u6_addr32[4];
      } in6_u;



  };

extern const struct in6_addr in6addr_any;         
extern const struct in6_addr in6addr_loopback;    






 
# 1 "/usr/include/bits/socket.h" 1 3
 



























# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 29 "/usr/include/bits/socket.h" 2 3


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 1 3
 


 





 
# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/syslimits.h" 1 3
 





# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 1 3
 


 

# 114 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 3



# 1 "/usr/include/limits.h" 1 3
 

















 









 





 

# 113 "/usr/include/limits.h" 3




  




# 140 "/usr/include/limits.h" 3



 
# 1 "/usr/include/bits/posix1_lim.h" 1 3
 

















 









 

 


 


 


 


 


 


 


 



 


 


 


 


 



 


 


 


 


 


 


 


 


 


 


 


 



 


 


 


 


 



 
# 1 "/usr/include/bits/local_lim.h" 1 3
 


















 














 
# 1 "/usr/include/linux/limits.h" 1 3



















# 36 "/usr/include/bits/local_lim.h" 2 3


 




 




 





 

 


 

 


 

 


 



 


 

# 126 "/usr/include/bits/posix1_lim.h" 2 3








 







# 144 "/usr/include/limits.h" 2 3




# 1 "/usr/include/bits/posix2_lim.h" 1 3
 

















 







 


 


 


 


 



 



 


 



 




 




























 



# 148 "/usr/include/limits.h" 2 3






# 117 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 2 3




# 7 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/syslimits.h" 2 3


# 11 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 2 3





 



 



 




 





 



 












 

 




 



 








 



 













 



# 107 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 3




 









# 31 "/usr/include/bits/socket.h" 2 3



 

typedef __socklen_t socklen_t;



 
enum __socket_type
{
  SOCK_STREAM = 1,		 


  SOCK_DGRAM = 2,		 


  SOCK_RAW = 3,			 

  SOCK_RDM = 4,			 

  SOCK_SEQPACKET = 5,		 


  SOCK_PACKET = 10		 



};

 































 































 











 


 
# 1 "/usr/include/bits/sockaddr.h" 1 3
 


















 







 
typedef unsigned short int sa_family_t;

 









# 142 "/usr/include/bits/socket.h" 2 3


 
struct sockaddr
  {
    sa_family_t  sa_family ;	 
    char sa_data[14];		 
  };


 









struct sockaddr_storage
  {
    sa_family_t  ss_family ;	 
    __uint32_t  __ss_align;	 
    char __ss_padding[(128  - (2 * sizeof (__uint32_t ))) ];
  };


 
enum
  {
    MSG_OOB		= 0x01,	 

    MSG_PEEK		= 0x02,	 

    MSG_DONTROUTE	= 0x04,	 






    MSG_CTRUNC		= 0x08,	 

    MSG_PROXY		= 0x10,	 

    MSG_TRUNC		= 0x20,

    MSG_DONTWAIT	= 0x40,  

    MSG_EOR		= 0x80,  

    MSG_WAITALL		= 0x100,  

    MSG_FIN		= 0x200,

    MSG_SYN		= 0x400,

    MSG_CONFIRM		= 0x800,  

    MSG_RST		= 0x1000,

    MSG_ERRQUEUE	= 0x2000,  

    MSG_NOSIGNAL	= 0x4000,  

    MSG_MORE		= 0x8000   

  };


 

struct msghdr
  {
    void *msg_name;		 
    socklen_t msg_namelen;	 

    struct iovec *msg_iov;	 
    int msg_iovlen;		 

    void *msg_control;		 
    socklen_t msg_controllen;	 

    int msg_flags;		 
  };

 
struct cmsghdr
  {
    size_t cmsg_len;		 

    int cmsg_level;		 
    int cmsg_type;		 

    __extension__ unsigned char __cmsg_data [0] ;  

  };

 















extern struct cmsghdr *__cmsg_nxthdr (struct msghdr *__mhdr,
				      struct cmsghdr *__cmsg)  ;
# 280 "/usr/include/bits/socket.h" 3


 

enum
  {
    SCM_RIGHTS = 0x01,		 


    SCM_CREDENTIALS = 0x02,      


    __SCM_CONNECT = 0x03	 
  };

 

struct ucred
{
  pid_t pid;			 
  uid_t uid;			 
  gid_t gid;			 
};

 
# 1 "/usr/include/asm/socket.h" 1 3



# 1 "/usr/include/asm/sockios.h" 1 3



 








# 4 "/usr/include/asm/socket.h" 2 3


 
















 







 






 









 
# 62 "/usr/include/asm/socket.h" 3



# 305 "/usr/include/bits/socket.h" 2 3



 
struct linger
  {
    int l_onoff;		 
    int l_linger;		 
  };


# 212 "/usr/include/netinet/in.h" 2 3



 
struct sockaddr_in
  {
    sa_family_t  sin_family ;
    in_port_t sin_port;			 
    struct in_addr sin_addr;		 

     
    unsigned char sin_zero[sizeof (struct sockaddr) -
			   (sizeof (unsigned short int))  -
			   sizeof (in_port_t) -
			   sizeof (struct in_addr)];
  };

 
struct sockaddr_in6
  {
    sa_family_t  sin6_family ;
    in_port_t sin6_port;	 
    uint32_t sin6_flowinfo;	 
    struct in6_addr sin6_addr;	 
    uint32_t sin6_scope_id;	 
  };

 
struct ipv6_mreq
  {
     
    struct in6_addr ipv6mr_multiaddr;

     
    unsigned int ipv6mr_interface;
  };

 
# 1 "/usr/include/bits/in.h" 1 3
 

















 





 






















 


 




 






 


struct ip_opts
  {
    struct in_addr ip_dst;	 
    char ip_opts[40];		 
  };

 
struct ip_mreq
  {
    struct in_addr imr_multiaddr;	 
    struct in_addr imr_interface;	 
  };

 
struct ip_mreqn
  {
    struct in_addr imr_multiaddr;	 
    struct in_addr imr_address;		 
    int	imr_ifindex;			 
  };

 
struct in_pktinfo
  {
    int ipi_ifindex;			 
    struct in_addr ipi_spec_dst;	 
    struct in_addr ipi_addr;		 
  };

 

























 






 




 



 




# 250 "/usr/include/netinet/in.h" 2 3


 






extern uint32_t ntohl (uint32_t __netlong)   __attribute__ ((__const__));
extern uint16_t ntohs (uint16_t __netshort)
       __attribute__ ((__const__));
extern uint32_t htonl (uint32_t __hostlong)
       __attribute__ ((__const__));
extern uint16_t htons (uint16_t __hostshort)
       __attribute__ ((__const__));



 
# 1 "/usr/include/bits/byteswap.h" 1 3
 

























 





# 43 "/usr/include/bits/byteswap.h" 3






 





 



# 71 "/usr/include/bits/byteswap.h" 3
# 82 "/usr/include/bits/byteswap.h" 3







 

# 99 "/usr/include/bits/byteswap.h" 3


# 113 "/usr/include/bits/byteswap.h" 3



# 270 "/usr/include/netinet/in.h" 2 3


# 291 "/usr/include/netinet/in.h" 3









































 
extern int bindresvport (int __sockfd, struct sockaddr_in *__sock_in)  ;

 
extern int bindresvport6 (int __sockfd, struct sockaddr_in6 *__sock_in)
      ;






















 
struct in6_pktinfo
  {
    struct in6_addr	ipi6_addr;     
    unsigned int	ipi6_ifindex;  
  };

 


# 28 "/usr/include/netdb.h" 2 3



 

# 1 "/usr/include/rpc/netdb.h" 1 3
 
 



























 

 









# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 42 "/usr/include/rpc/netdb.h" 2 3


 

struct rpcent
{
  char *r_name;		 
  char **r_aliases;	 
  int r_number;		 
};

extern void setrpcent (int __stayopen)  ;
extern void endrpcent (void)  ;
extern struct rpcent *getrpcbyname (__const char *__name)  ;
extern struct rpcent *getrpcbynumber (int __number)  ;
extern struct rpcent *getrpcent (void)  ;


extern int getrpcbyname_r (__const char *__name, struct rpcent *__result_buf,
			   char *__buffer, size_t __buflen,
			   struct rpcent **__result)  ;

extern int getrpcbynumber_r (int __number, struct rpcent *__result_buf,
			     char *__buffer, size_t __buflen,
			     struct rpcent **__result)  ;

extern int getrpcent_r (struct rpcent *__result_buf, char *__buffer,
			size_t __buflen, struct rpcent **__result)  ;


 


# 33 "/usr/include/netdb.h" 2 3










# 1 "/usr/include/bits/netdb.h" 1 3
 






















 


struct netent
{
  char *n_name;			 
  char **n_aliases;		 
  int n_addrtype;		 
  uint32_t n_net;		 
};
# 43 "/usr/include/netdb.h" 2 3


 








 

 
extern int h_errno;

 
extern int *__h_errno_location (void)   __attribute__ ((__const__));

# 72 "/usr/include/netdb.h" 3




 




 





















 

extern void herror (__const char *__str)  ;

 
extern __const char *hstrerror (int __err_num)  ;



 
struct hostent
{
  char *h_name;			 
  char **h_aliases;		 
  int h_addrtype;		 
  int h_length;			 
  char **h_addr_list;		 

};

 

extern void sethostent (int __stay_open)  ;

 
extern void endhostent (void)  ;

 

extern struct hostent *gethostent (void)  ;

 

extern struct hostent *gethostbyaddr (__const void *__addr, __socklen_t __len,
				      int __type)  ;

 
extern struct hostent *gethostbyname (__const char *__name)  ;


 


extern struct hostent *gethostbyname2 (__const char *__name, int __af)  ;

 




extern int gethostent_r (struct hostent *__restrict __result_buf,
			 char *__restrict __buf, size_t __buflen,
			 struct hostent **__restrict __result,
			 int *__restrict __h_errnop)  ;

extern int gethostbyaddr_r (__const void *__restrict __addr, __socklen_t __len,
			    int __type,
			    struct hostent *__restrict __result_buf,
			    char *__restrict __buf, size_t __buflen,
			    struct hostent **__restrict __result,
			    int *__restrict __h_errnop)  ;

extern int gethostbyname_r (__const char *__restrict __name,
			    struct hostent *__restrict __result_buf,
			    char *__restrict __buf, size_t __buflen,
			    struct hostent **__restrict __result,
			    int *__restrict __h_errnop)  ;

extern int gethostbyname2_r (__const char *__restrict __name, int __af,
			     struct hostent *__restrict __result_buf,
			     char *__restrict __buf, size_t __buflen,
			     struct hostent **__restrict __result,
			     int *__restrict __h_errnop)  ;



 

extern void setnetent (int __stay_open)  ;

 
extern void endnetent (void)  ;

 

extern struct netent *getnetent (void)  ;

 

extern struct netent *getnetbyaddr (uint32_t __net, int __type)
      ;

 
extern struct netent *getnetbyname (__const char *__name)  ;


 




extern int getnetent_r (struct netent *__restrict __result_buf,
			char *__restrict __buf, size_t __buflen,
			struct netent **__restrict __result,
			int *__restrict __h_errnop)  ;

extern int getnetbyaddr_r (uint32_t __net, int __type,
			   struct netent *__restrict __result_buf,
			   char *__restrict __buf, size_t __buflen,
			   struct netent **__restrict __result,
			   int *__restrict __h_errnop)  ;

extern int getnetbyname_r (__const char *__restrict __name,
			   struct netent *__restrict __result_buf,
			   char *__restrict __buf, size_t __buflen,
			   struct netent **__restrict __result,
			   int *__restrict __h_errnop)  ;



 
struct servent
{
  char *s_name;			 
  char **s_aliases;		 
  int s_port;			 
  char *s_proto;		 
};

 

extern void setservent (int __stay_open)  ;

 
extern void endservent (void)  ;

 

extern struct servent *getservent (void)  ;

 

extern struct servent *getservbyname (__const char *__name,
				      __const char *__proto)  ;

 

extern struct servent *getservbyport (int __port, __const char *__proto)
      ;



 

extern int getservent_r (struct servent *__restrict __result_buf,
			 char *__restrict __buf, size_t __buflen,
			 struct servent **__restrict __result)  ;

extern int getservbyname_r (__const char *__restrict __name,
			    __const char *__restrict __proto,
			    struct servent *__restrict __result_buf,
			    char *__restrict __buf, size_t __buflen,
			    struct servent **__restrict __result)  ;

extern int getservbyport_r (int __port, __const char *__restrict __proto,
			    struct servent *__restrict __result_buf,
			    char *__restrict __buf, size_t __buflen,
			    struct servent **__restrict __result)  ;



 
struct protoent
{
  char *p_name;			 
  char **p_aliases;		 
  int p_proto;			 
};

 

extern void setprotoent (int __stay_open)  ;

 
extern void endprotoent (void)  ;

 

extern struct protoent *getprotoent (void)  ;

 
extern struct protoent *getprotobyname (__const char *__name)  ;

 
extern struct protoent *getprotobynumber (int __proto)  ;



 

extern int getprotoent_r (struct protoent *__restrict __result_buf,
			  char *__restrict __buf, size_t __buflen,
			  struct protoent **__restrict __result)  ;

extern int getprotobyname_r (__const char *__restrict __name,
			     struct protoent *__restrict __result_buf,
			     char *__restrict __buf, size_t __buflen,
			     struct protoent **__restrict __result)  ;

extern int getprotobynumber_r (int __proto,
			       struct protoent *__restrict __result_buf,
			       char *__restrict __buf, size_t __buflen,
			       struct protoent **__restrict __result)  ;



 
extern int setnetgrent (__const char *__netgroup)  ;

 
extern void endnetgrent (void)  ;

 

extern int getnetgrent (char **__restrict __hostp,
			char **__restrict __userp,
			char **__restrict __domainp)  ;


 
extern int innetgr (__const char *__netgroup, __const char *__host,
		    __const char *__user, __const char *domain)  ;

 
extern int getnetgrent_r (char **__restrict __hostp,
			  char **__restrict __userp,
			  char **__restrict __domainp,
			  char *__restrict __buffer, size_t __buflen)  ;




 





extern int rcmd (char **__restrict __ahost, unsigned short int __rport,
		 __const char *__restrict __locuser,
		 __const char *__restrict __remuser,
		 __const char *__restrict __cmd, int *__restrict __fd2p)
      ;

 

extern int rcmd_af (char **__restrict __ahost, unsigned short int __rport,
		    __const char *__restrict __locuser,
		    __const char *__restrict __remuser,
		    __const char *__restrict __cmd, int *__restrict __fd2p,
		    sa_family_t __af)  ;

 




extern int rexec (char **__restrict __ahost, int __rport,
		  __const char *__restrict __name,
		  __const char *__restrict __pass,
		  __const char *__restrict __cmd, int *__restrict __fd2p)
      ;

 

extern int rexec_af (char **__restrict __ahost, int __rport,
		     __const char *__restrict __name,
		     __const char *__restrict __pass,
		     __const char *__restrict __cmd, int *__restrict __fd2p,
		     sa_family_t __af)  ;

 


extern int ruserok (__const char *__rhost, int __suser,
		    __const char *__remuser, __const char *__locuser)  ;

 

extern int ruserok_af (__const char *__rhost, int __suser,
		       __const char *__remuser, __const char *__locuser,
		       sa_family_t __af)  ;

 


extern int rresvport (int *__alport)  ;

 

extern int rresvport_af (int *__alport, sa_family_t __af)  ;



 

 
struct addrinfo
{
  int ai_flags;			 
  int ai_family;		 
  int ai_socktype;		 
  int ai_protocol;		 
  socklen_t ai_addrlen;		 
  struct sockaddr *ai_addr;	 
  char *ai_canonname;		 
  struct addrinfo *ai_next;	 
};

# 438 "/usr/include/netdb.h" 3


 




 




























 

extern int getaddrinfo (__const char *__restrict __name,
			__const char *__restrict __service,
			__const struct addrinfo *__restrict __req,
			struct addrinfo **__restrict __pai)  ;

 
extern void freeaddrinfo (struct addrinfo *__ai)  ;

 
extern __const char *gai_strerror (int __ecode)  ;

 
extern int getnameinfo (__const struct sockaddr *__restrict __sa,
			socklen_t __salen, char *__restrict __host,
			socklen_t __hostlen, char *__restrict __serv,
			socklen_t __servlen, unsigned int __flags)  ;

# 512 "/usr/include/netdb.h" 3



 


# 50 "../../include/all_system.h" 2

# 1 "/usr/include/sys/socket.h" 1 3
 























 

# 1 "/usr/include/sys/uio.h" 1 3
 
























 

 
# 1 "/usr/include/bits/uio.h" 1 3
 

























 



 









 
struct iovec
  {
    void *iov_base;	 
    size_t iov_len;	 
  };
# 29 "/usr/include/sys/uio.h" 2 3



 




extern ssize_t readv (int __fd, __const struct iovec *__vector, int __count)
      ;

 




extern ssize_t writev (int __fd, __const struct iovec *__vector, int __count)
      ;

 


# 27 "/usr/include/sys/socket.h" 2 3


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 29 "/usr/include/sys/socket.h" 2 3



 





 

struct osockaddr
  {
    unsigned short int sa_family;
    unsigned char sa_data[14];
  };


 

enum
{
  SHUT_RD = 0,		 

  SHUT_WR,		 

  SHUT_RDWR		 

};

 








# 94 "/usr/include/sys/socket.h" 3



 


extern int socket (int __domain, int __type, int __protocol)  ;

 



extern int socketpair (int __domain, int __type, int __protocol,
		       int __fds[2])  ;

 
extern int bind (int __fd, __const struct sockaddr *  __addr, socklen_t __len)
      ;

 
extern int getsockname (int __fd, struct sockaddr *__restrict  __addr,
			socklen_t *__restrict __len)  ;

 



extern int connect (int __fd, __const struct sockaddr *  __addr, socklen_t __len)
      ;

 

extern int getpeername (int __fd, struct sockaddr *__restrict  __addr,
			socklen_t *__restrict __len)  ;


 
extern ssize_t send (int __fd, __const void *__buf, size_t __n, int __flags)
      ;

 

extern ssize_t recv (int __fd, void *__buf, size_t __n, int __flags)
      ;

 

extern ssize_t sendto (int __fd, __const void *__buf, size_t __n,
		       int __flags, __const struct sockaddr *  __addr,
		       socklen_t __addr_len)  ;

 



extern ssize_t recvfrom (int __fd, void *__restrict __buf, size_t __n, int __flags,
			 struct sockaddr *__restrict  __addr, socklen_t *__restrict __addr_len)
      ;


 

extern ssize_t sendmsg (int __fd, __const struct msghdr *__message, int __flags)
      ;

 

extern ssize_t recvmsg (int __fd, struct msghdr *__message, int __flags)
      ;


 


extern int getsockopt (int __fd, int __level, int __optname,
		       void *__restrict __optval,
		       socklen_t *__restrict __optlen)  ;

 


extern int setsockopt (int __fd, int __level, int __optname,
		       __const void *__optval, socklen_t __optlen)  ;


 


extern int listen (int __fd, int __n)  ;

 




extern int accept (int __fd, struct sockaddr *__restrict  __addr,
		   socklen_t *__restrict __addr_len)
      ;

 





extern int shutdown (int __fd, int __how)  ;









 


extern int isfdtype (int __fd, int __fdtype)  ;


 


# 51 "../../include/all_system.h" 2

# 1 "/usr/include/sys/stat.h" 1 3
 

















 










# 78 "/usr/include/sys/stat.h" 3


# 94 "/usr/include/sys/stat.h" 3


 

# 1 "/usr/include/bits/stat.h" 1 3
 





















 






 





struct stat
  {
    __dev_t st_dev;			 
    unsigned short int __pad1;

    __ino_t st_ino;			 



    __mode_t st_mode;			 
    __nlink_t st_nlink;			 
    __uid_t st_uid;			 
    __gid_t st_gid;			 
    __dev_t st_rdev;			 
    unsigned short int __pad2;

    __off_t st_size;			 



    __blksize_t st_blksize;		 


    __blkcnt_t st_blocks;		 



    __time_t st_atime;			 
    unsigned long int __unused1;
    __time_t st_mtime;			 
    unsigned long int __unused2;
    __time_t st_ctime;			 
    unsigned long int __unused3;

    unsigned long int __unused4;
    unsigned long int __unused5;



  };

# 102 "/usr/include/bits/stat.h" 3


 



 



 








 





 







# 98 "/usr/include/sys/stat.h" 2 3




















 























 










 





 






 











 





 




 









 
extern int stat (__const char *__restrict __file,
		 struct stat *__restrict __buf)  ;

 

extern int fstat (int __fd, struct stat *__buf)  ;
# 217 "/usr/include/sys/stat.h" 3









 

extern int lstat (__const char *__restrict __file,
		  struct stat *__restrict __buf)  ;
# 239 "/usr/include/sys/stat.h" 3







 


extern int chmod (__const char *__file, __mode_t __mode)  ;

 

extern int fchmod (int __fd, __mode_t __mode)  ;



 

extern __mode_t umask (__mode_t __mask)  ;







 
extern int mkdir (__const char *__path, __mode_t __mode)  ;

 



extern int mknod (__const char *__path, __mode_t __mode, __dev_t __dev)
      ;



 
extern int mkfifo (__const char *__path, __mode_t __mode)  ;

 





















 

extern int __fxstat (int __ver, int __fildes, struct stat *__stat_buf)  ;
extern int __xstat (int __ver, __const char *__filename,
		    struct stat *__stat_buf)  ;
extern int __lxstat (int __ver, __const char *__filename,
		     struct stat *__stat_buf)  ;
# 327 "/usr/include/sys/stat.h" 3










extern int __xmknod (int __ver, __const char *__path, __mode_t __mode,
		     __dev_t *__dev)  ;


 

extern __inline__ int stat (__const char *__path,
			    struct stat *__statbuf)  
{
  return __xstat (3  , __path, __statbuf);
}


extern __inline__ int lstat (__const char *__path,
			     struct stat *__statbuf)  
{
  return __lxstat (3  , __path, __statbuf);
}


extern __inline__ int fstat (int __fd, struct stat *__statbuf)  
{
  return __fxstat (3  , __fd, __statbuf);
}


extern __inline__ int mknod (__const char *__path, __mode_t __mode,
			     __dev_t __dev)  
{
  return __xmknod (1  , __path, __mode, &__dev);
}


# 391 "/usr/include/sys/stat.h" 3




 



# 52 "../../include/all_system.h" 2




 



	
# 1 "/usr/include/dirent.h" 1 3
 

















 








 



# 45 "/usr/include/dirent.h" 3


 














# 1 "/usr/include/bits/dirent.h" 1 3
 





















struct dirent
  {

    __ino_t d_ino;
    __off_t d_off;




    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];		 
  };

# 46 "/usr/include/bits/dirent.h" 3








# 62 "/usr/include/dirent.h" 2 3






 




























 
enum
  {
    DT_UNKNOWN = 0,

    DT_FIFO = 1,

    DT_CHR = 2,

    DT_DIR = 4,

    DT_BLK = 6,

    DT_REG = 8,

    DT_LNK = 10,

    DT_SOCK = 12,

    DT_WHT = 14

  };

 





 

typedef struct __dirstream DIR;

 

extern DIR *opendir (__const char *__name)  ;

 

extern int closedir (DIR *__dirp)  ;

 







extern struct dirent *readdir (DIR *__dirp)  ;













 


extern int readdir_r (DIR *__restrict __dirp,
		      struct dirent *__restrict __entry,
		      struct dirent **__restrict __result)  ;
# 176 "/usr/include/dirent.h" 3









 
extern void rewinddir (DIR *__dirp)  ;




 
extern void seekdir (DIR *__dirp, long int __pos)  ;

 
extern long int telldir (DIR *__dirp)  ;




 
extern int dirfd (DIR *__dirp)  ;






 


 








# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 220 "/usr/include/dirent.h" 2 3


 




extern int scandir (__const char *__restrict __dir,
		    struct dirent ***__restrict __namelist,
		    int (*__selector) (__const struct dirent *),
		    int (*__cmp) (__const void *, __const void *))  ;
# 242 "/usr/include/dirent.h" 3











 

extern int alphasort (__const void *__e1, __const void *__e2)
        ;
# 266 "/usr/include/dirent.h" 3







# 293 "/usr/include/dirent.h" 3


 




extern __ssize_t getdirentries (int __fd, char *__restrict __buf,
				size_t __nbytes,
				__off_t *__restrict __basep)  ;
# 313 "/usr/include/dirent.h" 3










 


# 60 "../../include/all_system.h" 2




	
# 1 "/usr/include/strings.h" 1 3
 




















 


# 63 "/usr/include/strings.h" 3



# 64 "../../include/all_system.h" 2




	



	
# 1 "/usr/include/pthread.h" 1 3
 
 
 
 
 
 
 
 
 
 
 
 
 






# 1 "/usr/include/sched.h" 1 3
 























 



# 1 "/usr/include/time.h" 1 3
 

















 














# 51 "/usr/include/time.h" 3


# 61 "/usr/include/time.h" 3



# 72 "/usr/include/time.h" 3



# 84 "/usr/include/time.h" 3



# 96 "/usr/include/time.h" 3




# 112 "/usr/include/time.h" 3




# 364 "/usr/include/time.h" 3



# 29 "/usr/include/sched.h" 2 3


 
# 1 "/usr/include/bits/sched.h" 1 3
 


























 





 











 
struct sched_param
  {
    int __sched_priority;
  };

 

 

extern int clone (int (*__fn) (void *__arg), void *__child_stack,
		  int __flags, void *__arg)  ;


 



# 73 "/usr/include/bits/sched.h" 3

# 32 "/usr/include/sched.h" 2 3

 



 

 
extern int sched_setparam (__pid_t __pid, __const struct sched_param *__param)
      ;

 
extern int sched_getparam (__pid_t __pid, struct sched_param *__param)  ;

 
extern int sched_setscheduler (__pid_t __pid, int __policy,
			       __const struct sched_param *__param)  ;

 
extern int sched_getscheduler (__pid_t __pid)  ;

 
extern int sched_yield (void)  ;

 
extern int sched_get_priority_max (int __algorithm)  ;

 
extern int sched_get_priority_min (int __algorithm)  ;

 
extern int sched_rr_get_interval (__pid_t __pid, struct timespec *__t)  ;

 


# 20 "/usr/include/pthread.h" 2 3

# 1 "/usr/include/time.h" 1 3
 

















 










 




 


# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 38 "/usr/include/time.h" 2 3


 

# 1 "/usr/include/bits/time.h" 1 3
 


















 







 


 





 


extern long int __sysconf (int);




 

 

 


 






# 73 "/usr/include/bits/time.h" 3

# 42 "/usr/include/time.h" 2 3


 













 
typedef __clock_t clock_t;




# 72 "/usr/include/time.h" 3



# 84 "/usr/include/time.h" 3



# 96 "/usr/include/time.h" 3




# 112 "/usr/include/time.h" 3





 
struct tm
{
  int tm_sec;			 
  int tm_min;			 
  int tm_hour;			 
  int tm_mday;			 
  int tm_mon;			 
  int tm_year;			 
  int tm_wday;			 
  int tm_yday;			 
  int tm_isdst;			 


  long int tm_gmtoff;		 
  __const char *tm_zone;	 




};



 
struct itimerspec
  {
    struct timespec it_interval;
    struct timespec it_value;
  };

 
struct sigevent;











 

extern clock_t clock (void)  ;

 
extern time_t time (time_t *__timer)  ;

 
extern double difftime (time_t __time1, time_t __time0)
       __attribute__ ((__const__));

 
extern time_t mktime (struct tm *__tp)  ;


 


extern size_t strftime (char *__restrict __s, size_t __maxsize,
			__const char *__restrict __format,
			__const struct tm *__restrict __tp)  ;










 

extern struct tm *gmtime (__const time_t *__timer)  ;

 

extern struct tm *localtime (__const time_t *__timer)  ;


 

extern struct tm *gmtime_r (__const time_t *__restrict __timer,
			    struct tm *__restrict __tp)  ;

 

extern struct tm *localtime_r (__const time_t *__restrict __timer,
			       struct tm *__restrict __tp)  ;


 

extern char *asctime (__const struct tm *__tp)  ;

 
extern char *ctime (__const time_t *__timer)  ;


 

 

extern char *asctime_r (__const struct tm *__restrict __tp,
			char *__restrict __buf)  ;

 
extern char *ctime_r (__const time_t *__restrict __timer,
		      char *__restrict __buf)  ;



 
extern char *__tzname[2];	 
extern int __daylight;		 
extern long int __timezone;	 



 
extern char *tzname[2];

 

extern void tzset (void)  ;



extern int daylight;
extern long int timezone;



 

extern int stime (__const time_t *__when)  ;



 






 


 
extern time_t timegm (struct tm *__tp)  ;

 
extern time_t timelocal (struct tm *__tp)  ;

 
extern int dysize (int __year)    __attribute__ ((__const__));




 
extern int nanosleep (__const struct timespec *__requested_time,
		      struct timespec *__remaining)  ;


 
extern int clock_getres (clockid_t __clock_id, struct timespec *__res)  ;

 
extern int clock_gettime (clockid_t __clock_id, struct timespec *__tp)  ;

 
extern int clock_settime (clockid_t __clock_id, __const struct timespec *__tp)
      ;

# 305 "/usr/include/time.h" 3



 
extern int timer_create (clockid_t __clock_id,
			 struct sigevent *__restrict __evp,
			 timer_t *__restrict __timerid)  ;

 
extern int timer_delete (timer_t __timerid)  ;

 
extern int timer_settime (timer_t __timerid, int __flags,
			  __const struct itimerspec *__restrict __value,
			  struct itimerspec *__restrict __ovalue)  ;

 
extern int timer_gettime (timer_t __timerid, struct itimerspec *__value)
      ;

 
extern int timer_getoverrun (timer_t __timerid)  ;



# 349 "/usr/include/time.h" 3


# 359 "/usr/include/time.h" 3



 




# 21 "/usr/include/pthread.h" 2 3



# 1 "/usr/include/signal.h" 1 3
 

















 











 

# 1 "/usr/include/bits/sigset.h" 1 3
 


















# 33 "/usr/include/bits/sigset.h" 3



 





# 125 "/usr/include/bits/sigset.h" 3

# 33 "/usr/include/signal.h" 2 3


 

















# 369 "/usr/include/signal.h" 3


 


# 24 "/usr/include/pthread.h" 2 3

# 1 "/usr/include/bits/pthreadtypes.h" 1 3
 
 
 
 
 
 
 
 
 
 
 
 
 





# 144 "/usr/include/bits/pthreadtypes.h" 3

# 25 "/usr/include/pthread.h" 2 3

# 1 "/usr/include/bits/initspin.h" 1 3
 


















 




 


# 26 "/usr/include/pthread.h" 2 3



 

 

























 

enum
{
  PTHREAD_CREATE_JOINABLE,

  PTHREAD_CREATE_DETACHED

};

enum
{
  PTHREAD_INHERIT_SCHED,

  PTHREAD_EXPLICIT_SCHED

};

enum
{
  PTHREAD_SCOPE_SYSTEM,

  PTHREAD_SCOPE_PROCESS

};

enum
{
  PTHREAD_MUTEX_TIMED_NP,
  PTHREAD_MUTEX_RECURSIVE_NP,
  PTHREAD_MUTEX_ERRORCHECK_NP,
  PTHREAD_MUTEX_ADAPTIVE_NP











};

enum
{
  PTHREAD_PROCESS_PRIVATE,

  PTHREAD_PROCESS_SHARED

};

# 118 "/usr/include/pthread.h" 3




 






 

struct _pthread_cleanup_buffer
{
  void (*__routine) (void *);		   
  void *__arg;				   
  int __canceltype;			   
  struct _pthread_cleanup_buffer *__prev;  
};

 

enum
{
  PTHREAD_CANCEL_ENABLE,

  PTHREAD_CANCEL_DISABLE

};
enum
{
  PTHREAD_CANCEL_DEFERRED,

  PTHREAD_CANCEL_ASYNCHRONOUS

};



 

 


extern int pthread_create (pthread_t *__restrict __thread,
			   __const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg)  ;

 
extern pthread_t pthread_self (void)  ;

 
extern int pthread_equal (pthread_t __thread1, pthread_t __thread2)  ;

 
extern void pthread_exit (void *__retval)
       __attribute__ ((__noreturn__));

 


extern int pthread_join (pthread_t __th, void **__thread_return)  ;

 



extern int pthread_detach (pthread_t __th)  ;


 

 


extern int pthread_attr_init (pthread_attr_t *__attr)  ;

 
extern int pthread_attr_destroy (pthread_attr_t *__attr)  ;

 
extern int pthread_attr_setdetachstate (pthread_attr_t *__attr,
					int __detachstate)  ;

 
extern int pthread_attr_getdetachstate (__const pthread_attr_t *__attr,
					int *__detachstate)  ;

 
extern int pthread_attr_setschedparam (pthread_attr_t *__restrict __attr,
				       __const struct sched_param *__restrict
				       __param)  ;

 
extern int pthread_attr_getschedparam (__const pthread_attr_t *__restrict
				       __attr,
				       struct sched_param *__restrict __param)
      ;

 
extern int pthread_attr_setschedpolicy (pthread_attr_t *__attr, int __policy)
      ;

 
extern int pthread_attr_getschedpolicy (__const pthread_attr_t *__restrict
					__attr, int *__restrict __policy)
      ;

 
extern int pthread_attr_setinheritsched (pthread_attr_t *__attr,
					 int __inherit)  ;

 
extern int pthread_attr_getinheritsched (__const pthread_attr_t *__restrict
					 __attr, int *__restrict __inherit)
      ;

 
extern int pthread_attr_setscope (pthread_attr_t *__attr, int __scope)
      ;

 
extern int pthread_attr_getscope (__const pthread_attr_t *__restrict __attr,
				  int *__restrict __scope)  ;

# 254 "/usr/include/pthread.h" 3


 



extern int pthread_attr_setstackaddr (pthread_attr_t *__attr,
				      void *__stackaddr)  ;

 
extern int pthread_attr_getstackaddr (__const pthread_attr_t *__restrict
				      __attr, void **__restrict __stackaddr)
      ;

# 279 "/usr/include/pthread.h" 3


 


extern int pthread_attr_setstacksize (pthread_attr_t *__attr,
				      size_t __stacksize)  ;

 
extern int pthread_attr_getstacksize (__const pthread_attr_t *__restrict
				      __attr, size_t *__restrict __stacksize)
      ;






 

 

extern int pthread_setschedparam (pthread_t __target_thread, int __policy,
				  __const struct sched_param *__param)
      ;

 
extern int pthread_getschedparam (pthread_t __target_thread,
				  int *__restrict __policy,
				  struct sched_param *__restrict __param)
      ;

















 

 

extern int pthread_mutex_init (pthread_mutex_t *__restrict __mutex,
			       __const pthread_mutexattr_t *__restrict
			       __mutex_attr)  ;

 
extern int pthread_mutex_destroy (pthread_mutex_t *__mutex)  ;

 
extern int pthread_mutex_trylock (pthread_mutex_t *__mutex)  ;

 
extern int pthread_mutex_lock (pthread_mutex_t *__mutex)  ;








 
extern int pthread_mutex_unlock (pthread_mutex_t *__mutex)  ;


 

 

extern int pthread_mutexattr_init (pthread_mutexattr_t *__attr)  ;

 
extern int pthread_mutexattr_destroy (pthread_mutexattr_t *__attr)  ;

 
extern int pthread_mutexattr_getpshared (__const pthread_mutexattr_t *
					 __restrict __attr,
					 int *__restrict __pshared)  ;

 
extern int pthread_mutexattr_setpshared (pthread_mutexattr_t *__attr,
					 int __pshared)  ;

# 383 "/usr/include/pthread.h" 3



 

 

extern int pthread_cond_init (pthread_cond_t *__restrict __cond,
			      __const pthread_condattr_t *__restrict
			      __cond_attr)  ;

 
extern int pthread_cond_destroy (pthread_cond_t *__cond)  ;

 
extern int pthread_cond_signal (pthread_cond_t *__cond)  ;

 
extern int pthread_cond_broadcast (pthread_cond_t *__cond)  ;

 

extern int pthread_cond_wait (pthread_cond_t *__restrict __cond,
			      pthread_mutex_t *__restrict __mutex)  ;

 



extern int pthread_cond_timedwait (pthread_cond_t *__restrict __cond,
				   pthread_mutex_t *__restrict __mutex,
				   __const struct timespec *__restrict
				   __abstime)  ;

 

 
extern int pthread_condattr_init (pthread_condattr_t *__attr)  ;

 
extern int pthread_condattr_destroy (pthread_condattr_t *__attr)  ;

 
extern int pthread_condattr_getpshared (__const pthread_condattr_t *
					__restrict __attr,
					int *__restrict __pshared)  ;

 
extern int pthread_condattr_setpshared (pthread_condattr_t *__attr,
					int __pshared)  ;


# 501 "/usr/include/pthread.h" 3


# 545 "/usr/include/pthread.h" 3



 

 





extern int pthread_key_create (pthread_key_t *__key,
			       void (*__destr_function) (void *))  ;

 
extern int pthread_key_delete (pthread_key_t __key)  ;

 
extern int pthread_setspecific (pthread_key_t __key,
				__const void *__pointer)  ;

 
extern void *pthread_getspecific (pthread_key_t __key)  ;


 

 



extern int pthread_once (pthread_once_t *__once_control,
			 void (*__init_routine) (void))  ;


 

 

extern int pthread_setcancelstate (int __state, int *__oldstate)  ;

 

extern int pthread_setcanceltype (int __type, int *__oldtype)  ;

 
extern int pthread_cancel (pthread_t __thread)  ;

 


extern void pthread_testcancel (void)  ;


 










extern void _pthread_cleanup_push (struct _pthread_cleanup_buffer *__buffer,
				   void (*__routine) (void *),
				   void *__arg)  ;

 





extern void _pthread_cleanup_pop (struct _pthread_cleanup_buffer *__buffer,
				  int __execute)  ;

 


# 644 "/usr/include/pthread.h" 3










 
# 1 "/usr/include/bits/sigthread.h" 1 3
 

























 

 

extern int pthread_sigmask (int __how,
			    __const __sigset_t *__restrict __newmask,
			    __sigset_t *__restrict __oldmask) ;

 
extern int pthread_kill (pthread_t __thread, int __signo)  ;


# 655 "/usr/include/pthread.h" 2 3



 

 










extern int pthread_atfork (void (*__prepare) (void),
			   void (*__parent) (void),
			   void (*__child) (void))  ;

 


extern void pthread_kill_other_threads_np (void)  ;

 


# 72 "../../include/all_system.h" 2




	



	
# 1 "/usr/include/unistd.h" 1 3
 

















 








 

 


 



 


 


 


 



 



 



 



 






 


 




 


 


 



 



 



















































































# 1 "/usr/include/bits/posix_opt.h" 1 3
 





















 


 


 


 


 


 


 


 


 


 


 


 


 



 


 


 


 


 


 



 


 


 


 


 


 


 


 


 




 


 


 


 


 


 


 


 


 


 


 


 



# 175 "/usr/include/unistd.h" 2 3


 




 





 










# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 1 3






 


# 19 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3



 


 





 


# 61 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 





 


















 





 

 

# 131 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 


# 188 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3





 




 

# 271 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


# 283 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3


 

 

# 317 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/stddef.h" 3




 





















# 199 "/usr/include/unistd.h" 2 3


# 236 "/usr/include/unistd.h" 3
















 






 
extern int access (__const char *__name, int __type)  ;








 







 






 





extern __off_t lseek (int __fd, __off_t __offset, int __whence)  ;
# 300 "/usr/include/unistd.h" 3





 
extern int close (int __fd)  ;

 

extern ssize_t read (int __fd, void *__buf, size_t __nbytes)  ;

 
extern ssize_t write (int __fd, __const void *__buf, size_t __n)  ;

# 347 "/usr/include/unistd.h" 3


 



extern int pipe (int __pipedes[2])  ;

 






extern unsigned int alarm (unsigned int __seconds)  ;

 






extern unsigned int sleep (unsigned int __seconds)  ;


 



extern __useconds_t ualarm (__useconds_t __value, __useconds_t __interval)
      ;

 

extern int usleep (__useconds_t __useconds)  ;



 

extern int pause (void)  ;


 
extern int chown (__const char *__file, __uid_t __owner, __gid_t __group)
      ;


 
extern int fchown (int __fd, __uid_t __owner, __gid_t __group)  ;


 

extern int lchown (__const char *__file, __uid_t __owner, __gid_t __group)
      ;



 
extern int chdir (__const char *__path)  ;


 
extern int fchdir (int __fd)  ;


 






extern char *getcwd (char *__buf, size_t __size)  ;









 


extern char *getwd (char *__buf)  ;



 
extern int dup (int __fd)  ;

 
extern int dup2 (int __fd, int __fd2)  ;

 
extern char **__environ;





 

extern int execve (__const char *__path, char *__const __argv[],
		   char *__const __envp[])  ;









 
extern int execv (__const char *__path, char *__const __argv[])  ;

 

extern int execle (__const char *__path, __const char *__arg, ...)  ;

 

extern int execl (__const char *__path, __const char *__arg, ...)  ;

 

extern int execvp (__const char *__file, char *__const __argv[])  ;

 


extern int execlp (__const char *__file, __const char *__arg, ...)  ;



 
extern int nice (int __inc)  ;



 
extern void _exit (int __status) __attribute__ ((__noreturn__));


 


# 1 "/usr/include/bits/confname.h" 1 3
 






















 
enum
  {
    _PC_LINK_MAX,

    _PC_MAX_CANON,

    _PC_MAX_INPUT,

    _PC_NAME_MAX,

    _PC_PATH_MAX,

    _PC_PIPE_BUF,

    _PC_CHOWN_RESTRICTED,

    _PC_NO_TRUNC,

    _PC_VDISABLE,

    _PC_SYNC_IO,

    _PC_ASYNC_IO,

    _PC_PRIO_IO,

    _PC_SOCK_MAXBUF,

    _PC_FILESIZEBITS,

    _PC_REC_INCR_XFER_SIZE,

    _PC_REC_MAX_XFER_SIZE,

    _PC_REC_MIN_XFER_SIZE,

    _PC_REC_XFER_ALIGN,

    _PC_ALLOC_SIZE_MIN,

    _PC_SYMLINK_MAX

  };

 
enum
  {
    _SC_ARG_MAX,

    _SC_CHILD_MAX,

    _SC_CLK_TCK,

    _SC_NGROUPS_MAX,

    _SC_OPEN_MAX,

    _SC_STREAM_MAX,

    _SC_TZNAME_MAX,

    _SC_JOB_CONTROL,

    _SC_SAVED_IDS,

    _SC_REALTIME_SIGNALS,

    _SC_PRIORITY_SCHEDULING,

    _SC_TIMERS,

    _SC_ASYNCHRONOUS_IO,

    _SC_PRIORITIZED_IO,

    _SC_SYNCHRONIZED_IO,

    _SC_FSYNC,

    _SC_MAPPED_FILES,

    _SC_MEMLOCK,

    _SC_MEMLOCK_RANGE,

    _SC_MEMORY_PROTECTION,

    _SC_MESSAGE_PASSING,

    _SC_SEMAPHORES,

    _SC_SHARED_MEMORY_OBJECTS,

    _SC_AIO_LISTIO_MAX,

    _SC_AIO_MAX,

    _SC_AIO_PRIO_DELTA_MAX,

    _SC_DELAYTIMER_MAX,

    _SC_MQ_OPEN_MAX,

    _SC_MQ_PRIO_MAX,

    _SC_VERSION,

    _SC_PAGESIZE,


    _SC_RTSIG_MAX,

    _SC_SEM_NSEMS_MAX,

    _SC_SEM_VALUE_MAX,

    _SC_SIGQUEUE_MAX,

    _SC_TIMER_MAX,


     

    _SC_BC_BASE_MAX,

    _SC_BC_DIM_MAX,

    _SC_BC_SCALE_MAX,

    _SC_BC_STRING_MAX,

    _SC_COLL_WEIGHTS_MAX,

    _SC_EQUIV_CLASS_MAX,

    _SC_EXPR_NEST_MAX,

    _SC_LINE_MAX,

    _SC_RE_DUP_MAX,

    _SC_CHARCLASS_NAME_MAX,


    _SC_2_VERSION,

    _SC_2_C_BIND,

    _SC_2_C_DEV,

    _SC_2_FORT_DEV,

    _SC_2_FORT_RUN,

    _SC_2_SW_DEV,

    _SC_2_LOCALEDEF,


    _SC_PII,

    _SC_PII_XTI,

    _SC_PII_SOCKET,

    _SC_PII_INTERNET,

    _SC_PII_OSI,

    _SC_POLL,

    _SC_SELECT,

    _SC_UIO_MAXIOV,

    _SC_IOV_MAX = _SC_UIO_MAXIOV ,

    _SC_PII_INTERNET_STREAM,

    _SC_PII_INTERNET_DGRAM,

    _SC_PII_OSI_COTS,

    _SC_PII_OSI_CLTS,

    _SC_PII_OSI_M,

    _SC_T_IOV_MAX,


     
    _SC_THREADS,

    _SC_THREAD_SAFE_FUNCTIONS,

    _SC_GETGR_R_SIZE_MAX,

    _SC_GETPW_R_SIZE_MAX,

    _SC_LOGIN_NAME_MAX,

    _SC_TTY_NAME_MAX,

    _SC_THREAD_DESTRUCTOR_ITERATIONS,

    _SC_THREAD_KEYS_MAX,

    _SC_THREAD_STACK_MIN,

    _SC_THREAD_THREADS_MAX,

    _SC_THREAD_ATTR_STACKADDR,

    _SC_THREAD_ATTR_STACKSIZE,

    _SC_THREAD_PRIORITY_SCHEDULING,

    _SC_THREAD_PRIO_INHERIT,

    _SC_THREAD_PRIO_PROTECT,

    _SC_THREAD_PROCESS_SHARED,


    _SC_NPROCESSORS_CONF,

    _SC_NPROCESSORS_ONLN,

    _SC_PHYS_PAGES,

    _SC_AVPHYS_PAGES,

    _SC_ATEXIT_MAX,

    _SC_PASS_MAX,


    _SC_XOPEN_VERSION,

    _SC_XOPEN_XCU_VERSION,

    _SC_XOPEN_UNIX,

    _SC_XOPEN_CRYPT,

    _SC_XOPEN_ENH_I18N,

    _SC_XOPEN_SHM,


    _SC_2_CHAR_TERM,

    _SC_2_C_VERSION,

    _SC_2_UPE,


    _SC_XOPEN_XPG2,

    _SC_XOPEN_XPG3,

    _SC_XOPEN_XPG4,


    _SC_CHAR_BIT,

    _SC_CHAR_MAX,

    _SC_CHAR_MIN,

    _SC_INT_MAX,

    _SC_INT_MIN,

    _SC_LONG_BIT,

    _SC_WORD_BIT,

    _SC_MB_LEN_MAX,

    _SC_NZERO,

    _SC_SSIZE_MAX,

    _SC_SCHAR_MAX,

    _SC_SCHAR_MIN,

    _SC_SHRT_MAX,

    _SC_SHRT_MIN,

    _SC_UCHAR_MAX,

    _SC_UINT_MAX,

    _SC_ULONG_MAX,

    _SC_USHRT_MAX,


    _SC_NL_ARGMAX,

    _SC_NL_LANGMAX,

    _SC_NL_MSGMAX,

    _SC_NL_NMAX,

    _SC_NL_SETMAX,

    _SC_NL_TEXTMAX,


    _SC_XBS5_ILP32_OFF32,

    _SC_XBS5_ILP32_OFFBIG,

    _SC_XBS5_LP64_OFF64,

    _SC_XBS5_LPBIG_OFFBIG,


    _SC_XOPEN_LEGACY,

    _SC_XOPEN_REALTIME,

    _SC_XOPEN_REALTIME_THREADS,


    _SC_ADVISORY_INFO,

    _SC_BARRIERS,

    _SC_BASE,

    _SC_C_LANG_SUPPORT,

    _SC_C_LANG_SUPPORT_R,

    _SC_CLOCK_SELECTION,

    _SC_CPUTIME,

    _SC_THREAD_CPUTIME,

    _SC_DEVICE_IO,

    _SC_DEVICE_SPECIFIC,

    _SC_DEVICE_SPECIFIC_R,

    _SC_FD_MGMT,

    _SC_FIFO,

    _SC_PIPE,

    _SC_FILE_ATTRIBUTES,

    _SC_FILE_LOCKING,

    _SC_FILE_SYSTEM,

    _SC_MONOTONIC_CLOCK,

    _SC_MULTI_PROCESS,

    _SC_SINGLE_PROCESS,

    _SC_NETWORKING,

    _SC_READER_WRITER_LOCKS,

    _SC_SPIN_LOCKS,

    _SC_REGEXP,

    _SC_REGEX_VERSION,

    _SC_SHELL,

    _SC_SIGNALS,

    _SC_SPAWN,

    _SC_SPORADIC_SERVER,

    _SC_THREAD_SPORADIC_SERVER,

    _SC_SYSTEM_DATABASE,

    _SC_SYSTEM_DATABASE_R,

    _SC_TIMEOUTS,

    _SC_TYPED_MEMORY_OBJECTS,

    _SC_USER_GROUPS,

    _SC_USER_GROUPS_R,

    _SC_2_PBS,

    _SC_2_PBS_ACCOUNTING,

    _SC_2_PBS_LOCATE,

    _SC_2_PBS_MESSAGE,

    _SC_2_PBS_TRACK,

    _SC_SYMLOOP_MAX,

    _SC_STREAMS,

    _SC_2_PBS_CHECKPOINT,


    _SC_V6_ILP32_OFF32,

    _SC_V6_ILP32_OFFBIG,

    _SC_V6_LP64_OFF64,

    _SC_V6_LPBIG_OFFBIG,


    _SC_HOST_NAME_MAX,

    _SC_TRACE,

    _SC_TRACE_EVENT_FILTER,

    _SC_TRACE_INHERIT,

    _SC_TRACE_LOG

  };




 
enum
  {
    _CS_PATH,			 


# 492 "/usr/include/bits/confname.h" 3


# 527 "/usr/include/bits/confname.h" 3

# 561 "/usr/include/bits/confname.h" 3


    _CS_V6_WIDTH_RESTRICTED_ENVS

  };

# 500 "/usr/include/unistd.h" 2 3


 
extern long int pathconf (__const char *__path, int __name)  ;

 
extern long int fpathconf (int __fd, int __name)  ;

 
extern long int sysconf (int __name)   __attribute__ ((__const__));


 
extern size_t confstr (int __name, char *__buf, size_t __len)  ;



 
extern __pid_t getpid (void)  ;

 
extern __pid_t getppid (void)  ;

 


extern __pid_t getpgrp (void)  ;








 
extern __pid_t __getpgid (__pid_t __pid)  ;





 


extern int setpgid (__pid_t __pid, __pid_t __pgid)  ;


 











 

extern int setpgrp (void)  ;

# 574 "/usr/include/unistd.h" 3



 


extern __pid_t setsid (void)  ;






 
extern __uid_t getuid (void)  ;

 
extern __uid_t geteuid (void)  ;

 
extern __gid_t getgid (void)  ;

 
extern __gid_t getegid (void)  ;

 


extern int getgroups (int __size, __gid_t __list[])  ;






 



extern int setuid (__uid_t __uid)  ;


 

extern int setreuid (__uid_t __ruid, __uid_t __euid)  ;



 
extern int seteuid (__uid_t __uid)  ;


 



extern int setgid (__gid_t __gid)  ;


 

extern int setregid (__gid_t __rgid, __gid_t __egid)  ;



 
extern int setegid (__gid_t __gid)  ;



 


extern __pid_t fork (void)  ;


 



extern __pid_t vfork (void)  ;



 

extern char *ttyname (int __fd)  ;

 

extern int ttyname_r (int __fd, char *__buf, size_t __buflen)  ;

 

extern int isatty (int __fd)  ;



 

extern int ttyslot (void)  ;



 
extern int link (__const char *__from, __const char *__to)  ;


 
extern int symlink (__const char *__from, __const char *__to)  ;

 


extern int readlink (__const char *__restrict __path, char *__restrict __buf,
		     size_t __len)  ;


 
extern int unlink (__const char *__name)  ;

 
extern int rmdir (__const char *__path)  ;


 
extern __pid_t tcgetpgrp (int __fd)  ;

 
extern int tcsetpgrp (int __fd, __pid_t __pgrp_id)  ;


 
extern char *getlogin (void)  ;

 


extern int getlogin_r (char *__name, size_t __name_len)  ;



 
extern int setlogin (__const char *__name)  ;




 



# 1 "/usr/include/getopt.h" 1 3
 
























 














 





extern char *optarg;

 











extern int optind;

 


extern int opterr;

 

extern int optopt;

# 113 "/usr/include/getopt.h" 3



 

























 


extern int getopt (int ___argc, char *const *___argv, const char *__shortopts);




# 163 "/usr/include/getopt.h" 3

# 172 "/usr/include/getopt.h" 3






 



# 726 "/usr/include/unistd.h" 2 3





 


extern int gethostname (char *__name, size_t __len)  ;




 

extern int sethostname (__const char *__name, size_t __len)  ;

 

extern int sethostid (long int __id)  ;


 


extern int getdomainname (char *__name, size_t __len)  ;
extern int setdomainname (__const char *__name, size_t __len)  ;


 


extern int vhangup (void)  ;

 
extern int revoke (__const char *__file)  ;


 




extern int profil (unsigned short int *__sample_buffer, size_t __size,
		   size_t __offset, unsigned int __scale)  ;


 


extern int acct (__const char *__name)  ;


 
extern char *getusershell (void)  ;
extern void endusershell (void)  ;  
extern void setusershell (void)  ;  


 


extern int daemon (int __nochdir, int __noclose)  ;




 

extern int chroot (__const char *__path)  ;

 

extern char *getpass (__const char *__prompt)  ;




 
extern int fsync (int __fd)  ;





 
extern long int gethostid (void)  ;

 
extern void sync (void)  ;


 

extern int getpagesize (void)    __attribute__ ((__const__));


 

extern int truncate (__const char *__file, __off_t __length)  ;
# 834 "/usr/include/unistd.h" 3





 

extern int ftruncate (int __fd, __off_t __length)  ;













 

extern int getdtablesize (void)  ;






 

extern int brk (void *__addr)  ;

 



extern void *sbrk (intptr_t __delta)  ;




 









extern long int syscall (long int __sysno, ...)  ;





 



 









extern int lockf (int __fd, int __cmd, __off_t __len)  ;














# 933 "/usr/include/unistd.h" 3



 

extern int fdatasync (int __fildes)  ;



 

# 959 "/usr/include/unistd.h" 3



 








 

 









extern int pthread_atfork (void (*__prepare) (void),
			   void (*__parent) (void),
			   void (*__child) (void))  ;


 


# 80 "../../include/all_system.h" 2




	
# 1 "/usr/include/sys/time.h" 1 3
 


























# 1 "/usr/include/bits/time.h" 1 3
 


















 



# 57 "/usr/include/bits/time.h" 3




# 72 "/usr/include/bits/time.h" 3


# 28 "/usr/include/sys/time.h" 2 3










 

# 50 "/usr/include/sys/time.h" 3




 

struct timezone
  {
    int tz_minuteswest;		 
    int tz_dsttime;		 
  };

typedef struct timezone *__restrict __timezone_ptr_t;




 




extern int gettimeofday (struct timeval *__restrict __tv,
			 __timezone_ptr_t __tz)  ;


 

extern int settimeofday (__const struct timeval *__tv,
			 __const struct timezone *__tz)  ;

 



extern int adjtime (__const struct timeval *__delta,
		    struct timeval *__olddelta)  ;



 
enum __itimer_which
  {
     
    ITIMER_REAL = 0,

     
    ITIMER_VIRTUAL = 1,

     

    ITIMER_PROF = 2

  };

 

struct itimerval
  {
     
    struct timeval it_interval;
     
    struct timeval it_value;
  };






typedef int __itimer_which_t;


 

extern int getitimer (__itimer_which_t __which,
		      struct itimerval *__value)  ;

 


extern int setitimer (__itimer_which_t __which,
		      __const struct itimerval *__restrict __new,
		      struct itimerval *__restrict __old)  ;

 

extern int utimes (__const char *__file, __const struct timeval __tvp[2])
      ;



 








# 160 "/usr/include/sys/time.h" 3

# 169 "/usr/include/sys/time.h" 3


 


# 84 "../../include/all_system.h" 2








	
# 1 "/usr/include/rpc/rpc.h" 1 3
 
 




























 









# 1 "/usr/include/rpc/types.h" 1 3
 



























 
 





typedef int bool_t;
typedef int enum_t;
 
typedef unsigned long rpcprog_t;
typedef unsigned long rpcvers_t;
typedef unsigned long rpcproc_t;
typedef unsigned long rpcprot_t;
typedef unsigned long rpcport_t;























# 76 "/usr/include/rpc/types.h" 3








# 1 "/usr/include/sys/param.h" 1 3
 




















# 1 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 1 3
 


 

# 114 "/usr/lib/gcc-lib/i386-linux/2.95.4/include/limits.h" 3







# 22 "/usr/include/sys/param.h" 2 3


# 1 "/usr/include/linux/param.h" 1 3



# 1 "/usr/include/asm/param.h" 1 3
























# 4 "/usr/include/linux/param.h" 2 3



# 24 "/usr/include/sys/param.h" 2 3


 









 







 





 






 




 




# 84 "/usr/include/rpc/types.h" 2 3












# 41 "/usr/include/rpc/rpc.h" 2 3



 
# 1 "/usr/include/rpc/xdr.h" 1 3
 




























 












 


 

 




























 





enum xdr_op {
  XDR_ENCODE = 0,
  XDR_DECODE = 1,
  XDR_FREE = 2
};

 



 











 





typedef struct XDR XDR;
struct XDR
  {
    enum xdr_op x_op;		 
    struct xdr_ops
      {
	bool_t (*x_getlong) (XDR *__xdrs, long *__lp);
	 
	bool_t (*x_putlong) (XDR *__xdrs, __const long *__lp);
	 
	bool_t (*x_getbytes) (XDR *__xdrs, caddr_t __addr, u_int __len);
	 
	bool_t (*x_putbytes) (XDR *__xdrs, __const char *__addr, u_int __len);
	 
	u_int (*x_getpostn) (__const XDR *__xdrs);
	 
	bool_t (*x_setpostn) (XDR *__xdrs, u_int __pos);
	 
	int32_t *(*x_inline) (XDR *__xdrs, u_int __len);
	 
	void (*x_destroy) (XDR *__xdrs);
	 
	bool_t (*x_getint32) (XDR *__xdrs, int32_t *__ip);
	 
	bool_t (*x_putint32) (XDR *__xdrs, __const int32_t *__ip);
	 
      }
     *x_ops;
    caddr_t x_public;		 
    caddr_t x_private;		 
    caddr_t x_base;		 
    u_int x_handy;		 
  };

 








typedef bool_t (*xdrproc_t) (XDR *, void *,...);


 

































































 










struct xdr_discrim
{
  int value;
  xdrproc_t proc;
};

 




















 





















 





extern bool_t xdr_void (void)  ;
extern bool_t xdr_short (XDR *__xdrs, short *__sp)  ;
extern bool_t xdr_u_short (XDR *__xdrs, u_short *__usp)  ;
extern bool_t xdr_int (XDR *__xdrs, int *__ip)  ;
extern bool_t xdr_u_int (XDR *__xdrs, u_int *__up)  ;
extern bool_t xdr_long (XDR *__xdrs, long *__lp)  ;
extern bool_t xdr_u_long (XDR *__xdrs, u_long *__ulp)  ;
extern bool_t xdr_hyper (XDR *__xdrs, quad_t *__llp)  ;
extern bool_t xdr_u_hyper (XDR *__xdrs, u_quad_t *__ullp)  ;
extern bool_t xdr_longlong_t (XDR *__xdrs, quad_t *__llp)  ;
extern bool_t xdr_u_longlong_t (XDR *__xdrs, u_quad_t *__ullp)  ;
extern bool_t xdr_int8_t (XDR *__xdrs, int8_t *__ip)  ;
extern bool_t xdr_uint8_t (XDR *__xdrs, uint8_t *__up)  ;
extern bool_t xdr_int16_t (XDR *__xdrs, int16_t *__ip)  ;
extern bool_t xdr_uint16_t (XDR *__xdrs, uint16_t *__up)  ;
extern bool_t xdr_int32_t (XDR *__xdrs, int32_t *__ip)  ;
extern bool_t xdr_uint32_t (XDR *__xdrs, uint32_t *__up)  ;
extern bool_t xdr_int64_t (XDR *__xdrs, int64_t *__ip)  ;
extern bool_t xdr_uint64_t (XDR *__xdrs, uint64_t *__up)  ;
extern bool_t xdr_bool (XDR *__xdrs, bool_t *__bp)  ;
extern bool_t xdr_enum (XDR *__xdrs, enum_t *__ep)  ;
extern bool_t xdr_array (XDR * _xdrs, caddr_t *__addrp, u_int *__sizep,
			 u_int __maxsize, u_int __elsize, xdrproc_t __elproc)
      ;
extern bool_t xdr_bytes (XDR *__xdrs, char **__cpp, u_int *__sizep,
			 u_int __maxsize)  ;
extern bool_t xdr_opaque (XDR *__xdrs, caddr_t __cp, u_int __cnt)  ;
extern bool_t xdr_string (XDR *__xdrs, char **__cpp, u_int __maxsize)  ;
extern bool_t xdr_union (XDR *__xdrs, enum_t *__dscmp, char *__unp,
			 __const struct xdr_discrim *__choices,
			 xdrproc_t dfault)  ;
extern bool_t xdr_char (XDR *__xdrs, char *__cp)  ;
extern bool_t xdr_u_char (XDR *__xdrs, u_char *__cp)  ;
extern bool_t xdr_vector (XDR *__xdrs, char *__basep, u_int __nelem,
			  u_int __elemsize, xdrproc_t __xdr_elem)  ;
extern bool_t xdr_float (XDR *__xdrs, float *__fp)  ;
extern bool_t xdr_double (XDR *__xdrs, double *__dp)  ;
extern bool_t xdr_reference (XDR *__xdrs, caddr_t *__xpp, u_int __size,
			     xdrproc_t __proc)  ;
extern bool_t xdr_pointer (XDR *__xdrs, char **__objpp,
			   u_int __obj_size, xdrproc_t __xdr_obj)  ;
extern bool_t xdr_wrapstring (XDR *__xdrs, char **__cpp)  ;
extern u_long xdr_sizeof (xdrproc_t, void *)  ;

 




struct netobj
{
  u_int n_len;
  char *n_bytes;
};
typedef struct netobj netobj;
extern bool_t xdr_netobj (XDR *__xdrs, struct netobj *__np)  ;

 




 
extern void xdrmem_create (XDR *__xdrs, __const caddr_t __addr,
			   u_int __size, enum xdr_op __xop)  ;

 
extern void xdrstdio_create (XDR *__xdrs, FILE *__file, enum xdr_op __xop)
      ;

 
extern void xdrrec_create (XDR *__xdrs, u_int __sendsize,
			   u_int __recvsize, caddr_t __tcp_handle,
			   int (*__readit) (char *, char *, int),
			   int (*__writeit) (char *, char *, int))  ;

 
extern bool_t xdrrec_endofrecord (XDR *__xdrs, bool_t __sendnow)  ;

 
extern bool_t xdrrec_skiprecord (XDR *__xdrs)  ;

 
extern bool_t xdrrec_eof (XDR *__xdrs)  ;

 
extern void xdr_free (xdrproc_t __proc, char *__objp)  ;

 


# 45 "/usr/include/rpc/rpc.h" 2 3


 
# 1 "/usr/include/rpc/auth.h" 1 3
 
 




























 















 




 


enum auth_stat {
	AUTH_OK=0,
	 


	AUTH_BADCRED=1,			 
	AUTH_REJECTEDCRED=2,		 
	AUTH_BADVERF=3,			 
	AUTH_REJECTEDVERF=4,		 
	AUTH_TOOWEAK=5,			 
	 


	AUTH_INVALIDRESP=6,		 
	AUTH_FAILED=7			 
};

union des_block {
	struct {
		u_int32_t high;
		u_int32_t low;
	} key;
	char c[8];
};
typedef union des_block des_block;
extern bool_t xdr_des_block (XDR *__xdrs, des_block *__blkp)  ;

 


struct opaque_auth {
	enum_t	oa_flavor;		 
	caddr_t	oa_base;		 
	u_int	oa_length;		 
};

 


typedef struct AUTH AUTH;
struct AUTH {
  struct opaque_auth ah_cred;
  struct opaque_auth ah_verf;
  union des_block ah_key;
  struct auth_ops {
    void (*ah_nextverf) (AUTH *);
    int  (*ah_marshal) (AUTH *, XDR *);		 
    int  (*ah_validate) (AUTH *, struct opaque_auth *);
						 
    int  (*ah_refresh) (AUTH *);		 
    void (*ah_destroy) (AUTH *); 	    	 
  } *ah_ops;
  caddr_t ah_private;
};


 

































extern struct opaque_auth _null_auth;


 



 








extern AUTH *authunix_create (char *__machname, __uid_t __uid, __gid_t __gid,
			      int __len, __gid_t *__aup_gids)  ;
extern AUTH *authunix_create_default (void)  ;
extern AUTH *authnone_create (void)  ;
extern AUTH *authdes_create (const char *__servername, u_int __window,
			     struct sockaddr *__syncaddr, des_block *__ckey)
      ;
extern AUTH *authdes_pk_create (const char *, netobj *, u_int,
				struct sockaddr *, des_block *)  ;











 



extern int getnetname (char *)  ;
extern int host2netname (char *, __const char *, __const char *)  ;
extern int user2netname (char *, __const uid_t, __const char *)  ;
extern int netname2user (__const char *, uid_t *, gid_t *, int *, gid_t *)
      ;
extern int netname2host (__const char *, char *, __const int)  ;

 




extern int key_decryptsession (char *, des_block *)  ;
extern int key_decryptsession_pk (char *, netobj *, des_block *)  ;
extern int key_encryptsession (char *, des_block *)  ;
extern int key_encryptsession_pk (char *, netobj *, des_block *)  ;
extern int key_gendes (des_block *)  ;
extern int key_setsecret (char *)  ;
extern int key_secretkey_is_set (void)  ;
extern int key_get_conv (char *, des_block *)  ;

 


extern bool_t xdr_opaque_auth (XDR *, struct opaque_auth *)  ;

 


# 48 "/usr/include/rpc/rpc.h" 2 3


 
# 1 "/usr/include/rpc/clnt.h" 1 3
 
 




























 












# 1 "/usr/include/sys/un.h" 1 3
 






















 


 

 
struct sockaddr_un
  {
    sa_family_t  sun_family ;
    char sun_path[108];		 
  };





 




 


# 44 "/usr/include/rpc/clnt.h" 2 3


 

 




enum clnt_stat {
	RPC_SUCCESS=0,			 
	 


	RPC_CANTENCODEARGS=1,		 
	RPC_CANTDECODERES=2,		 
	RPC_CANTSEND=3,			 
	RPC_CANTRECV=4,			 
	RPC_TIMEDOUT=5,			 
	 


	RPC_VERSMISMATCH=6,		 
	RPC_AUTHERROR=7,		 
	RPC_PROGUNAVAIL=8,		 
	RPC_PROGVERSMISMATCH=9,		 
	RPC_PROCUNAVAIL=10,		 
	RPC_CANTDECODEARGS=11,		 
	RPC_SYSTEMERROR=12,		 
	RPC_NOBROADCAST = 21,		 
	 


	RPC_UNKNOWNHOST=13,		 
	RPC_UNKNOWNPROTO=17,		 
	RPC_UNKNOWNADDR = 19,		 

	 


	RPC_RPCBFAILURE=14,		 

	RPC_PROGNOTREGISTERED=15,	 
	RPC_N2AXLATEFAILURE = 22,	 
	 


	RPC_FAILED=16,
	RPC_INTR=18,
	RPC_TLIERROR=20,
	RPC_UDERROR=23,
         


        RPC_INPROGRESS = 24,
        RPC_STALERACHANDLE = 25
};


 


struct rpc_err {
  enum clnt_stat re_status;
  union {
    int RE_errno;		 
    enum auth_stat RE_why;	 
    struct {
      u_long low;		 
      u_long high;		 
    } RE_vers;
    struct {			 
      long s1;
      long s2;
    } RE_lb;			 
  } ru;




};


 




typedef struct CLIENT CLIENT;
struct CLIENT {
  AUTH	*cl_auth;		  
  struct clnt_ops {
    enum clnt_stat (*cl_call) (CLIENT *, u_long, xdrproc_t, caddr_t, xdrproc_t,
			       caddr_t, struct timeval);
			       	 
    void (*cl_abort) (void);	 
    void (*cl_geterr) (CLIENT *, struct rpc_err *);
				 
    bool_t (*cl_freeres) (CLIENT *, xdrproc_t, caddr_t);
				 
    void (*cl_destroy) (CLIENT *);  
    bool_t (*cl_control) (CLIENT *, int, char *);
				 
  } *cl_ops;
  caddr_t cl_private;		 
};


 






 















 







 








 









 









 






















 





 








 










 





 





 






extern CLIENT *clntraw_create (__const u_long __prog, __const u_long __vers)
      ;


 









extern CLIENT *clnt_create (__const char *__host, __const u_long __prog,
			    __const u_long __vers, __const char *__prot)
      ;


 










extern CLIENT *clnttcp_create (struct sockaddr_in *__raddr, u_long __prog,
			       u_long __version, int *__sockp, u_int __sendsz,
			       u_int __recvsz)  ;

 




















extern CLIENT *clntudp_create (struct sockaddr_in *__raddr, u_long __program,
			       u_long __version, struct timeval __wait_resend,
			       int *__sockp)  ;
extern CLIENT *clntudp_bufcreate (struct sockaddr_in *__raddr,
				  u_long __program, u_long __version,
				  struct timeval __wait_resend, int *__sockp,
				  u_int __sendsz, u_int __recvsz)  ;




 










extern CLIENT *clntunix_create  (struct sockaddr_un *__raddr, u_long __program,
				 u_long __version, int *__sockp,
				 u_int __sendsz, u_int __recvsz)  ;


extern int callrpc (__const char *__host, __const u_long __prognum,
		    __const u_long __versnum, __const u_long __procnum,
		    __const xdrproc_t __inproc, __const char *__in,
		    __const xdrproc_t __outproc, char *__out)  ;
extern int _rpc_dtablesize (void)  ;

 


extern void clnt_pcreateerror (__const char *__msg)  ;	 
extern char *clnt_spcreateerror(__const char *__msg)  ;	 

 


extern void clnt_perrno (enum clnt_stat __num)  ;		 

 


extern void clnt_perror (CLIENT *__clnt, __const char *__msg)  ;
							 
extern char *clnt_sperror (CLIENT *__clnt, __const char *__msg)  ;
							 

 


struct rpc_createerr {
	enum clnt_stat cf_stat;
	struct rpc_err cf_error;  
};

extern struct rpc_createerr rpc_createerr;



 


extern char *clnt_sperrno (enum clnt_stat __num)  ;	 

 


extern int getrpcport (__const char * __host, u_long __prognum,
		       u_long __versnum, u_int proto)  ;

 



extern void get_myaddress (struct sockaddr_in *)  ;




 


# 51 "/usr/include/rpc/rpc.h" 2 3


 
# 1 "/usr/include/rpc/rpc_msg.h" 1 3
 
 



























 









 









 

 





enum msg_type {
	CALL=0,
	REPLY=1
};

enum reply_stat {
	MSG_ACCEPTED=0,
	MSG_DENIED=1
};

enum accept_stat {
	SUCCESS=0,
	PROG_UNAVAIL=1,
	PROG_MISMATCH=2,
	PROC_UNAVAIL=3,
	GARBAGE_ARGS=4,
	SYSTEM_ERR=5
};

enum reject_stat {
	RPC_MISMATCH=0,
	AUTH_ERROR=1
};

 



 




struct accepted_reply {
	struct opaque_auth	ar_verf;
	enum accept_stat	ar_stat;
	union {
		struct {
			u_long	low;
			u_long	high;
		} AR_versions;
		struct {
			caddr_t	where;
			xdrproc_t proc;
		} AR_results;
		 
	} ru;


};

 


struct rejected_reply {
	enum reject_stat rj_stat;
	union {
		struct {
			u_long low;
			u_long high;
		} RJ_versions;
		enum auth_stat RJ_why;   
	} ru;


};

 


struct reply_body {
	enum reply_stat rp_stat;
	union {
		struct accepted_reply RP_ar;
		struct rejected_reply RP_dr;
	} ru;


};

 


struct call_body {
	u_long cb_rpcvers;	 
	u_long cb_prog;
	u_long cb_vers;
	u_long cb_proc;
	struct opaque_auth cb_cred;
	struct opaque_auth cb_verf;  
};

 


struct rpc_msg {
	u_long			rm_xid;
	enum msg_type		rm_direction;
	union {
		struct call_body RM_cmb;
		struct reply_body RM_rmb;
	} ru;


};




 





extern bool_t	xdr_callmsg (XDR *__xdrs, struct rpc_msg *__cmsg)  ;

 





extern bool_t	xdr_callhdr (XDR *__xdrs, struct rpc_msg *__cmsg)  ;

 





extern bool_t	xdr_replymsg (XDR *__xdrs, struct rpc_msg *__rmsg)  ;

 





extern void	_seterr_reply (struct rpc_msg *__msg, struct rpc_err *__error)
      ;

 


# 54 "/usr/include/rpc/rpc.h" 2 3

# 1 "/usr/include/rpc/auth_unix.h" 1 3
 
 



























 

 





 















 

 


 


 


struct authunix_parms
  {
    u_long aup_time;
    char *aup_machname;
    __uid_t aup_uid;
    __gid_t aup_gid;
    u_int aup_len;
    __gid_t *aup_gids;
  };

extern bool_t xdr_authunix_parms (XDR *__xdrs, struct authunix_parms *__p)
      ;

 




struct short_hand_verf
  {
    struct opaque_auth new_cred;
  };

 


# 55 "/usr/include/rpc/rpc.h" 2 3

# 1 "/usr/include/rpc/auth_des.h" 1 3
 























 

 
enum authdes_namekind
  {
    ADN_FULLNAME,
    ADN_NICKNAME
  };

 

struct authdes_fullname
  {
    char *name;		 
    des_block key;	 
    uint32_t window;	 
  };

 
struct authdes_cred
  {
    enum authdes_namekind adc_namekind;
    struct authdes_fullname adc_fullname;
    uint32_t adc_nickname;
  };

 
struct rpc_timeval
  {
    uint32_t tv_sec;             
    uint32_t tv_usec;            
  };

 
struct authdes_verf
  {
    union
      {
	struct rpc_timeval adv_ctime;	 
	des_block adv_xtime;		 
      }
    adv_time_u;
    uint32_t adv_int_u;
  };

 








 








 
extern int authdes_getucred (__const struct authdes_cred * __adc,
			     uid_t * __uid, gid_t * __gid,
			     short *__grouplen, gid_t * __groups)  ;

 


extern int getpublickey (__const char *__name, char *__key)  ;

 




extern int getsecretkey (__const char *__name, char *__key,
			 __const char *__passwd)  ;

extern int rtime (struct sockaddr_in *__addrp, struct rpc_timeval *__timep,
		  struct rpc_timeval *__timeout)  ;

 



# 56 "/usr/include/rpc/rpc.h" 2 3


 
# 1 "/usr/include/rpc/svc.h" 1 3
 




























 











 

 





















enum xprt_stat {
	XPRT_DIED,
	XPRT_MOREREQS,
	XPRT_IDLE
};

 


typedef struct SVCXPRT SVCXPRT;
struct SVCXPRT {
  int xp_sock;
  u_short xp_port;		 
  const struct xp_ops {
    bool_t	(*xp_recv) (SVCXPRT *__xprt, struct rpc_msg *__msg);
				 
    enum xprt_stat (*xp_stat) (SVCXPRT *__xprt);
				 
    bool_t	(*xp_getargs) (SVCXPRT *__xprt, xdrproc_t __xdr_args,
			       caddr_t args_ptr);  
    bool_t	(*xp_reply) (SVCXPRT *__xprt, struct rpc_msg *__msg);
				 
    bool_t	(*xp_freeargs) (SVCXPRT *__xprt, xdrproc_t __xdr_args,
				caddr_t args_ptr);
				 
    void	(*xp_destroy) (SVCXPRT *__xprt);
				 
  } *xp_ops;
  int		xp_addrlen;	  
  struct sockaddr_in xp_raddr;	  
  struct opaque_auth xp_verf;	  
  caddr_t		xp_p1;		  
  caddr_t		xp_p2;		  
  char		xp_pad [256];	 
};

 




 






































 


struct svc_req {
  rpcprog_t rq_prog;             
  rpcvers_t rq_vers;             
  rpcproc_t rq_proc;             
  struct opaque_auth rq_cred;    
  caddr_t rq_clntcred;           
  SVCXPRT *rq_xprt;              
};



typedef void (*__dispatch_fn_t) (struct svc_req*, SVCXPRT*);


 









extern bool_t svc_register (SVCXPRT *__xprt, rpcprog_t __prog,
			    rpcvers_t __vers, __dispatch_fn_t __dispatch,
			    rpcprot_t __protocol)  ;

 






extern void svc_unregister (rpcprog_t __prog, rpcvers_t __vers)  ;

 





extern void xprt_register (SVCXPRT *__xprt)  ;

 





extern void xprt_unregister (SVCXPRT *__xprt)  ;


 

























extern bool_t	svc_sendreply (SVCXPRT *xprt, xdrproc_t __xdr_results,
			       caddr_t __xdr_location)  ;

extern void	svcerr_decode (SVCXPRT *__xprt)  ;

extern void	svcerr_weakauth (SVCXPRT *__xprt)  ;

extern void	svcerr_noproc (SVCXPRT *__xprt)  ;

extern void	svcerr_progvers (SVCXPRT *__xprt, rpcvers_t __low_vers,
				 rpcvers_t __high_vers)  ;

extern void	svcerr_auth (SVCXPRT *__xprt, enum auth_stat __why)  ;

extern void	svcerr_noprog (SVCXPRT *__xprt)  ;

extern void	svcerr_systemerr (SVCXPRT *__xprt)  ;

 










 




extern struct pollfd *svc_pollfd;
extern int svc_max_pollfd;
extern fd_set svc_fdset;


 



extern void svc_getreq (int __rdfds)  ;
extern void svc_getreq_common (const int __fd)  ;
extern void svc_getreqset (fd_set *__readfds)  ;
extern void svc_getreq_poll (struct pollfd *, const int)  ;
extern void svc_exit (void)  ;
extern void svc_run (void)  ;

 




 



 


extern SVCXPRT *svcraw_create (void)  ;

 


extern SVCXPRT *svcudp_create (int __sock)  ;
extern SVCXPRT *svcudp_bufcreate (int __sock, u_int __sendsz, u_int __recvsz)
      ;

 


extern SVCXPRT *svctcp_create (int __sock, u_int __sendsize, u_int __recvsize)
      ;


 


extern SVCXPRT *svcunix_create (int __sock, u_int __sendsize, u_int __recvsize,
				char *__path)  ;


 


# 59 "/usr/include/rpc/rpc.h" 2 3

# 1 "/usr/include/rpc/svc_auth.h" 1 3
 
 



























 

 











 

 


extern enum auth_stat _authenticate (struct svc_req *__rqst,
				     struct rpc_msg *__msg)  ;

 


# 60 "/usr/include/rpc/rpc.h" 2 3


 




 


 

 
extern fd_set *__rpc_thread_svc_fdset (void) __attribute__ ((__const__));


extern struct rpc_createerr *__rpc_thread_createerr (void)
     __attribute__ ((__const__));

 








extern struct pollfd **__rpc_thread_svc_pollfd (void)
     __attribute__ ((__const__));


extern int *__rpc_thread_svc_max_pollfd (void) __attribute__ ((__const__));


 


# 92 "../../include/all_system.h" 2




	



	



        
# 1 "/usr/include/fcntl.h" 1 3
 

















 








 
 

 

# 1 "/usr/include/bits/fcntl.h" 1 3
 

























 






















 











 


































 


 




 




 














# 134 "/usr/include/bits/fcntl.h" 3


struct flock
  {
    short int l_type;	 
    short int l_whence;	 

    __off_t l_start;	 
    __off_t l_len;	 




    __pid_t l_pid;	 
  };

# 159 "/usr/include/bits/fcntl.h" 3


 









 








# 33 "/usr/include/fcntl.h" 2 3


 















 






 

extern int fcntl (int __fd, int __cmd, ...)  ;

 



extern int open (__const char *__file, int __oflag, ...)  ;












 



extern int creat (__const char *__file, __mode_t __mode)  ;












# 124 "/usr/include/fcntl.h" 3


# 163 "/usr/include/fcntl.h" 3


 


# 104 "../../include/all_system.h" 2




 



# 127 "../../include/all_system.h"



 



 










# 4 "../../include/nfi_worker.h" 2

# 1 "../../include/nfi.h" 1
# 115 "../../include/nfi.h"


# 5 "../../include/nfi_worker.h" 2





enum nfi_work{
	noop 		= -1,
	op_getattr 	= 0,
	op_setattr 	= 1,
	op_open 	= 2,
	op_create 	= 3,
	op_close 	= 4,
	op_remove 	= 5,
	op_rename 	= 6,
	op_read		= 7,
	op_write	= 8,
	op_mkdir	= 9,
	op_rmdir	= 10,
	op_opendir	= 11,
	op_readdir	= 12,
	op_closedir	= 13,
	op_statfs	= 14,
};

struct nfi_worker_io{
	void *buffer;
        off_t offset;
	size_t size;
};

struct nfi_worker_args{
	int operation;
	int result;

	char url[255 ];
	char newurl[255 ];
        struct nfi_fhandle *fh;
        struct nfi_attr *attr;

	struct nfi_info *inf;

	int n_io;
	struct nfi_worker_io *io;
	char *entry;
	unsigned char *type;
};


struct nfi_worker{
	int thread;   	
	pthread_t pth;
	pthread_mutex_t mt;
	pthread_cond_t cnd;
	int ready;
	struct nfi_server *server;
	struct nfi_worker_args arg;
};

int nfi_worker_init(struct nfi_worker *wrk, struct nfi_server *serv, int thread);

int nfi_worker_do_getattr(struct nfi_worker *wrk, 
			struct nfi_fhandle *fh, 
			struct nfi_attr *attr);

int nfi_worker_do_setattr(struct nfi_worker *wrk, 
			struct nfi_fhandle *fh, 
			struct nfi_attr *attr);

int nfi_worker_do_open(struct nfi_worker *wrk, 
			char *url, 
			struct nfi_fhandle *fho);

int nfi_worker_do_create(struct nfi_worker *wrk, 
			char *url,  
			struct nfi_attr *attr, 
			struct nfi_fhandle  *fh);

int nfi_worker_do_close(struct nfi_worker *wrk, 
			struct nfi_fhandle *fh);

int nfi_worker_do_remove(struct nfi_worker *wrk, 
			char *url);

int nfi_worker_do_rename(struct nfi_worker *wrk, 
			char *old_url, 
			char *new_url);

int nfi_worker_do_read(struct nfi_worker *wrk, 
			struct nfi_fhandle *fh, 
		       struct nfi_worker_io *io,
                       int n);

int nfi_worker_do_write(struct nfi_worker *wrk, 
			struct nfi_fhandle *fh, 
		        struct nfi_worker_io *io,
                        int n);

int nfi_worker_do_mkdir(struct nfi_worker *wrk, 
			char *url, 
			struct nfi_attr *attr, 
			struct nfi_fhandle *fh);

int nfi_worker_do_rmdir(struct nfi_worker *wrk, 
			char *url);

int nfi_worker_do_opendir(struct nfi_worker *wrk, 
			char *url, 
			struct nfi_fhandle *fho);

int nfi_worker_do_readdir(struct nfi_worker *wrk, 
			struct nfi_fhandle *fhd, 
			char *entry,
			unsigned char *type
			);

int nfi_worker_do_closedir(struct nfi_worker *wrk, 
			struct nfi_fhandle *fh);

int nfi_worker_do_statfs(struct nfi_worker *wrk, 
			struct nfi_info *inf);


int nfi_worker_wait(struct nfi_worker *wrk);

int nfi_worker_end(struct nfi_worker *wrk);

int nfi_worker_destroy();



# 4 "../../include/nfi.h" 2



 







 







 
struct nfi_ops;
struct nfi_worker;

struct nfi_server{
	int id;			 
	int  protocol;		 
	char *server;		 
	char *url; 		 
				 
				 
	void  *private;		 
	struct nfi_ops *ops;	 
	struct nfi_worker *wrk;	 
};

struct nfi_info{
        u_long at_size;
        u_long at_bsize;
        u_long at_blocks;
        u_long at_bfree;
        u_long at_bavail;
};


struct nfi_attr{
	int 	at_type; 	 	
	mode_t 	at_mode;	 
	uid_t	at_uid;		 
	gid_t	at_gid;		 
	off_t	at_size;		 
	u_long  at_blksize;	 
	u_long  at_blocks;  	 
	time_t  at_atime;   	 
	time_t  at_mtime;   	 
	time_t  at_ctime;   	 
	void	*private;
					     
};


struct nfi_fhandle 
{
	int type;			 
	char *url;			 
	struct nfi_server *server;	 
	void *priv_fh;			 
};




struct nfi_ops 
{
	int (*nfi_reconnect)(struct nfi_server *serv);
	
	int (*nfi_disconnect)(struct nfi_server *serv);
	
	int (*nfi_getattr)(struct nfi_server *serv, struct nfi_fhandle *fh, struct nfi_attr *attr);

	int (*nfi_setattr)(struct nfi_server *serv, struct nfi_fhandle *fh, struct nfi_attr *attr);

	int (*nfi_open)(struct nfi_server *serv, char *url, struct nfi_fhandle *fho); 

	int (*nfi_create)(struct nfi_server *serv, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);

	int (*nfi_close)(struct nfi_server *serv, struct nfi_fhandle *fh);

	int (*nfi_remove)(struct nfi_server *serv, char *url);

	int (*nfi_rename)(struct nfi_server *serv, char *old_url, char *new_url);

	ssize_t (*nfi_read)(struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

	ssize_t (*nfi_write)(struct nfi_server *serv, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

	int (*nfi_mkdir)(struct nfi_server *serv, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

	int (*nfi_rmdir)(struct nfi_server *serv, char *url);

	int (*nfi_opendir)(struct nfi_server *serv, char *url, struct nfi_fhandle *fho);

	int (*nfi_readdir)(struct nfi_server *serv, struct nfi_fhandle *fhd, char *entry, unsigned char *type);

	int (*nfi_closedir)(struct nfi_server *serv, struct nfi_fhandle *fh);

	int (*nfi_statfs)(struct nfi_server *serv, struct nfi_info *inf);

};





# 4 "../../include/xpn.h" 2


# 1 "../../include/nfi_lib.h" 1





# 1 "../../include/string_misc.h" 1


 
 

 




    

      


    

      long   STRING_MISC_StrLen (    char  *str ) ;
      int8_t STRING_MISC_Equal  (    char  *str1,
		                     char  *str2 ) ;
      char  *STRING_MISC_StrDup (   char  *str ) ;

      int hash(char *file,int nServ);

      int ParseURL(   char *url,  
      		      char *protocol, 
		      char *login, 
		      char *passwd, 
		      char *server,  
		      char *port,  
		      char *dir);

       
      int getNameFile(char *file, char *dir);

       
      int getNamePart(char *part, char *dir);

       
      int getDirWithURL(char *url, char *dir);


   


 



 

# 6 "../../include/nfi_lib.h" 2


 
# 1 "../../include/nfi_nfs/nfs_lib.h" 1



# 1 "../../include/nfi_nfs/nfi_nfs.h" 1




# 1 "../../include/nfi_nfs/nfs.h" 1










 


 

 


 

 


 

 



 

 





# 838 "../../include/nfi_nfs/nfs.h"


 
 










typedef char fhandle[32 ];

typedef char *dirpath;

typedef char *name;

struct fhd {
	char fh[32 ];
};
typedef struct fhd fhd;

struct fhstatus {
	u_long status;
	union {
		fhandle directory;
	} fhstatus_u;
};
typedef struct fhstatus fhstatus;

typedef struct mountbody *mountlist;

struct mountbody {
	name ml_hostname;
	dirpath ml_directory;
	mountlist ml_next;
};
typedef struct mountbody mountbody;

typedef struct groupnode *groups;

struct groupnode {
	name gr_name;
	groups gr_next;
};
typedef struct groupnode groupnode;

typedef struct exportnode *exports;

struct exportnode {
	dirpath ex_dir;
	groups ex_groups;
	exports ex_next;
};
typedef struct exportnode exportnode;





typedef char *filename;

typedef char *path;

typedef char nfscookie[4 ];

typedef struct {
	u_long nfsdata_len;
	char *nfsdata_val;
} nfsdata;

enum nfs_stat {
	 
	NFS_OK = 0, 
	  
	NFSERR_PERM = 1,
	NFSERR_NOENT = 2,
	NFSERR_IO = 5,
	NFSERR_NXIO = 6,
	NFSERR_ACCES = 13,
	NFSERR_EXIST = 17,
	NFSERR_NODEV = 19,
	NFSERR_NOTDIR = 20,
	NFSERR_ISDIR = 21,
	NFSERR_FBIG = 27,
	NFSERR_NOSPC = 28,
	NFSERR_ROFS = 30,
	NFSERR_NAMETOOLONG = 63,
	NFSERR_NOTEMPTY = 66,
	NFSERR_DQUOT = 69,
	NFSERR_STALE = 70,
	NFSERR_WFLUSH = 99,
	 
	NFSERR_NULL = -3,
	NFSERR_CONNECT = -7,
	NFSERR_EOFDIR = -47,
	 
	 



































};
typedef enum nfs_stat nfs_stat;

enum ftype {
	NFNON = 0,
	NFREG = 1,
	NFDIR = 2,
	NFBLK = 3,
	NFCHR = 4,
	NFLNK = 5,
};
typedef enum ftype ftype;

struct timevalNfs {
	u_long seconds;
	u_long useconds;
};
typedef struct timevalNfs timevalNfs;

struct fattr {
	ftype type;
	u_long mode;
	u_long nlink;
	u_long uid;
	u_long gid;
	u_long size;
	u_long blocksize;
	u_long rdev;
	u_long blocks;
	u_long fsid;
	u_long fileid;
	timevalNfs atime;
	timevalNfs mtime;
	timevalNfs ctime;
};
typedef struct fattr fattr;

struct sattr {
	u_long mode;
	u_long uid;
	u_long gid;
	u_long size;
	timevalNfs atime;
	timevalNfs mtime;
};
typedef struct sattr sattr;

struct attrstat {
	nfs_stat status;
	union {
		fattr attributes;
	} attrstat_u;
};
typedef struct attrstat attrstat;

struct diropargs {
	fhandle dir;
	filename name;
};
typedef struct diropargs diropargs;

struct diropok {
	fhandle file;
	fattr attributes;
};
typedef struct diropok diropok;

struct diropres {
	nfs_stat status;
	union {
		diropok fhand_attr;
	} diropres_u;
};
typedef struct diropres diropres;

struct sattrargs {
	fhandle file;
	sattr attributes;
};
typedef struct sattrargs sattrargs;

struct readlinkres {
	nfs_stat status;
	union {
		path data;
	} readlinkres_u;
};
typedef struct readlinkres readlinkres;

struct readargs {
	fhandle file;
	u_long offset;
	u_long count;
	u_long totalcount;
};
typedef struct readargs readargs;

struct datosRes {
	fattr attributes;
	nfsdata data;
};
typedef struct datosRes datosRes;

struct readres {
	nfs_stat status;
	union {
		datosRes fich_read;
	} readres_u;
};
typedef struct readres readres;

struct writeargs {
	fhandle file;
	u_long beginoffset;
	u_long offset;
	u_long totalcount;
	nfsdata data;
};
typedef struct writeargs writeargs;

struct createargs {
	diropargs where;
	sattr attributes;
};
typedef struct createargs createargs;

struct renameargs {
	diropargs from;
	diropargs to;
};
typedef struct renameargs renameargs;

struct linkargs {
	fhandle from;
	diropargs to;
};
typedef struct linkargs linkargs;

struct symlinkargs {
	diropargs from;
	path to;
	sattr attributes;
};
typedef struct symlinkargs symlinkargs;

struct readdirargs {
	fhandle dir;
	nfscookie cookie;
	u_long count;
};
typedef struct readdirargs readdirargs;

struct entry {
	u_long fileid;
	filename name;
	nfscookie cookie;
	struct entry *nextentry;
};
typedef struct entry entry;

struct readdirok {
	entry *entries;
	bool_t eof;
};
typedef struct readdirok readdirok;

struct readdirres {
	nfs_stat status;
	union {
		readdirok entradasDir;
	} readdirres_u;
};
typedef struct readdirres readdirres;

struct nfs_info {
	u_long tsize;
	u_long bsize;
	u_long blocks;
	u_long bfree;
	u_long bavail;
};
typedef struct nfs_info nfs_info;

struct statfsres {
	nfs_stat status;
	union {
		struct nfs_info inf;
	} statfsres_u;
};
typedef struct statfsres statfsres;







extern  enum clnt_stat mountproc_null_1(void *, void *, CLIENT *);
extern  bool_t mountproc_null_1_svc(void *, void *, struct svc_req *);

extern  enum clnt_stat mountproc_mnt_1(dirpath *, fhstatus *, CLIENT *);
extern  bool_t mountproc_mnt_1_svc(dirpath *, fhstatus *, struct svc_req *);

extern  enum clnt_stat mountproc_dump_1(void *, mountlist *, CLIENT *);
extern  bool_t mountproc_dump_1_svc(void *, mountlist *, struct svc_req *);

extern  enum clnt_stat mountproc_umnt_1(dirpath *, void *, CLIENT *);
extern  bool_t mountproc_umnt_1_svc(dirpath *, void *, struct svc_req *);

extern  enum clnt_stat mountproc_umntall_1(void *, void *, CLIENT *);
extern  bool_t mountproc_umntall_1_svc(void *, void *, struct svc_req *);

extern  enum clnt_stat mountproc_export_1(void *, exports *, CLIENT *);
extern  bool_t mountproc_export_1_svc(void *, exports *, struct svc_req *);
extern int mountprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

# 1210 "../../include/nfi_nfs/nfs.h"







extern  enum clnt_stat nfsproc_null_2(void *, void *, CLIENT *);
extern  bool_t nfsproc_null_2_svc(void *, void *, struct svc_req *);

extern  enum clnt_stat nfsproc_getattr_2(char *, attrstat *, CLIENT *);
extern  bool_t nfsproc_getattr_2_svc(char *, attrstat *, struct svc_req *);

extern  enum clnt_stat nfsproc_setattr_2(sattrargs *, attrstat *, CLIENT *);
extern  bool_t nfsproc_setattr_2_svc(sattrargs *, attrstat *, struct svc_req *);

extern  enum clnt_stat nfsproc_root_2(void *, void *, CLIENT *);
extern  bool_t nfsproc_root_2_svc(void *, void *, struct svc_req *);

extern  enum clnt_stat nfsproc_lookup_2(diropargs *, diropres *, CLIENT *);
extern  bool_t nfsproc_lookup_2_svc(diropargs *, diropres *, struct svc_req *);

extern  enum clnt_stat nfsproc_readlink_2(char *, readlinkres *, CLIENT *);
extern  bool_t nfsproc_readlink_2_svc(char *, readlinkres *, struct svc_req *);

extern  enum clnt_stat nfsproc_read_2(readargs *, readres *, CLIENT *);
extern  bool_t nfsproc_read_2_svc(readargs *, readres *, struct svc_req *);

extern  enum clnt_stat nfsproc_writecache_2(void *, void *, CLIENT *);
extern  bool_t nfsproc_writecache_2_svc(void *, void *, struct svc_req *);

extern  enum clnt_stat nfsproc_write_2(writeargs *, attrstat *, CLIENT *);
extern  bool_t nfsproc_write_2_svc(writeargs *, attrstat *, struct svc_req *);

extern  enum clnt_stat nfsproc_create_2(createargs *, diropres *, CLIENT *);
extern  bool_t nfsproc_create_2_svc(createargs *, diropres *, struct svc_req *);

extern  enum clnt_stat nfsproc_remove_2(diropargs *, nfs_stat *, CLIENT *);
extern  bool_t nfsproc_remove_2_svc(diropargs *, nfs_stat *, struct svc_req *);

extern  enum clnt_stat nfsproc_rename_2(renameargs *, nfs_stat *, CLIENT *);
extern  bool_t nfsproc_rename_2_svc(renameargs *, nfs_stat *, struct svc_req *);

extern  enum clnt_stat nfsproc_link_2(linkargs *, nfs_stat *, CLIENT *);
extern  bool_t nfsproc_link_2_svc(linkargs *, nfs_stat *, struct svc_req *);

extern  enum clnt_stat nfsproc_symlink_2(symlinkargs *, nfs_stat *, CLIENT *);
extern  bool_t nfsproc_symlink_2_svc(symlinkargs *, nfs_stat *, struct svc_req *);

extern  enum clnt_stat nfsproc_mkdir_2(createargs *, diropres *, CLIENT *);
extern  bool_t nfsproc_mkdir_2_svc(createargs *, diropres *, struct svc_req *);

extern  enum clnt_stat nfsproc_rmdir_2(diropargs *, nfs_stat *, CLIENT *);
extern  bool_t nfsproc_rmdir_2_svc(diropargs *, nfs_stat *, struct svc_req *);

extern  enum clnt_stat nfsproc_readdir_2(readdirargs *, readdirres *, CLIENT *);
extern  bool_t nfsproc_readdir_2_svc(readdirargs *, readdirres *, struct svc_req *);
extern int nfs_program_2_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

extern  enum clnt_stat nfsproc_statfs_2(char *, statfsres *, CLIENT *);
extern  bool_t nfsproc_statfs_2_svc(char *, statfsres *, struct svc_req *);
extern int nfs_program_2_freeresult (SVCXPRT *, xdrproc_t, caddr_t);


# 1331 "../../include/nfi_nfs/nfs.h"


 


extern  bool_t xdr_fhandle (XDR *, fhandle);
extern  bool_t xdr_dirpath (XDR *, dirpath*);
extern  bool_t xdr_name (XDR *, name*);
extern  bool_t xdr_fhd (XDR *, fhd*);
extern  bool_t xdr_fhstatus (XDR *, fhstatus*);
extern  bool_t xdr_mountlist (XDR *, mountlist*);
extern  bool_t xdr_mountbody (XDR *, mountbody*);
extern  bool_t xdr_groups (XDR *, groups*);
extern  bool_t xdr_groupnode (XDR *, groupnode*);
extern  bool_t xdr_exports (XDR *, exports*);
extern  bool_t xdr_exportnode (XDR *, exportnode*);
extern  bool_t xdr_filename (XDR *, filename*);
extern  bool_t xdr_path (XDR *, path*);
extern  bool_t xdr_nfscookie (XDR *, nfscookie);
extern  bool_t xdr_nfsdata (XDR *, nfsdata*);
extern  bool_t xdr_nfs_stat (XDR *, nfs_stat*);
extern  bool_t xdr_ftype (XDR *, ftype*);
extern  bool_t xdr_timevalNfs (XDR *, timevalNfs*);
extern  bool_t xdr_fattr (XDR *, fattr*);
extern  bool_t xdr_sattr (XDR *, sattr*);
extern  bool_t xdr_attrstat (XDR *, attrstat*);
extern  bool_t xdr_diropargs (XDR *, diropargs*);
extern  bool_t xdr_diropok (XDR *, diropok*);
extern  bool_t xdr_diropres (XDR *, diropres*);
extern  bool_t xdr_sattrargs (XDR *, sattrargs*);
extern  bool_t xdr_readlinkres (XDR *, readlinkres*);
extern  bool_t xdr_readargs (XDR *, readargs*);
extern  bool_t xdr_datosRes (XDR *, datosRes*);
extern  bool_t xdr_readres (XDR *, readres*);
extern  bool_t xdr_writeargs (XDR *, writeargs*);
extern  bool_t xdr_createargs (XDR *, createargs*);
extern  bool_t xdr_renameargs (XDR *, renameargs*);
extern  bool_t xdr_linkargs (XDR *, linkargs*);
extern  bool_t xdr_symlinkargs (XDR *, symlinkargs*);
extern  bool_t xdr_readdirargs (XDR *, readdirargs*);
extern  bool_t xdr_entry (XDR *, entry*);
extern  bool_t xdr_readdirok (XDR *, readdirok*);
extern  bool_t xdr_readdirres (XDR *, readdirres*);
extern  bool_t xdr_nfs_info (XDR *, nfs_info*);
extern  bool_t xdr_statfsres (XDR *, statfsres*);

# 1417 "../../include/nfi_nfs/nfs.h"









CLIENT* create_connection_mount(char *name, int type); 

void close_connection_mount(CLIENT *cl);
 
int nfs_mount(char *dir, fhandle fhand, CLIENT *cl );

int nfs_umount(char *path, CLIENT *cl);

int nfs_export(exports* ,CLIENT *cl);




CLIENT* create_connection_nfs(char *name, int type);

void close_connection_nfs(CLIENT *cl);

int nfs_getattr(fhandle fh, fattr *fatt, CLIENT *cl);

int nfs_setattr(fhandle fh, fattr *fatt, CLIENT *cl);

int nfs_lookup(fhandle fhin, char *path , fhandle fhout, fattr *att, CLIENT *cl);

ssize_t nfs_read(fhandle fh, void *data, off_t offset, size_t size, CLIENT *cl);

ssize_t nfs_write(fhandle fh, void *data, off_t offset, size_t size, CLIENT *cl);

int nfs_create(fhandle fhin, char *file, mode_t mode, fhandle fhout, fattr *at, CLIENT *cl);

int nfs_remove(fhandle fh, char *file, CLIENT *cl);

int nfs_rename(fhandle fh, char *name, fhandle fhR, char *nameR, CLIENT *cl);

int nfs_mkdir(fhandle fhin, char *dir, mode_t mode, fhandle fhout, fattr *att, CLIENT *cl);

int nfs_rmdir(fhandle fh, char *dir, CLIENT *cl);

int nfs_readdir(fhandle fh, nfscookie cookie, char *entry, CLIENT *cl);
  
int nfs_statfs(fhandle arg, struct nfs_info *inf, CLIENT *cl);












# 5 "../../include/nfi_nfs/nfi_nfs.h" 2

# 1 "../../include/nfi_nfs/nfi_nfs_err.h" 1





enum nfi_nfs_err{
	NFSERR_PARAM = 0,
	NFSERR_MEMORY = 1,
	NFSERR_URL = 2,
	NFSERR_MNTCONNECTION = 3,
	NFSERR_MOUNT = 4,
	NFSERR_NFSCONNECTION = 5,		
	NFSERR_GETATTR = 6,
	NFSERR_LOOKUP = 7,
	NFSERR_READ = 8,
	NFSERR_WRITE = 9,
	NFSERR_CREATE = 10,
	NFSERR_REMOVE = 11,
	NFSERR_MKDIR = 12,
	NFSERR_READDIR = 13,
	NFSERR_STATFS = 14,
};


void nfs_err(int err);



# 6 "../../include/nfi_nfs/nfi_nfs.h" 2






struct nfi_nfs_server{
	CLIENT *cl;
	fhandle fh;
};

struct nfi_nfs_fhandle{
	int eofdir;
	nfscookie cookie;
	fhandle fh;
};




void NFItoNFSattr(fattr *nfs_att,struct nfi_attr *nfi_att);

void NFStoNFIattr(struct nfi_attr *nfi_att, fattr *nfs_att);

void NFStoNFIInfo(struct nfi_info *nfi_inf, struct nfs_info *nfs_inf);



int nfi_nfs_init(char *url, struct nfi_server *serv);
int nfi_nfs_destroy(struct nfi_server *server);



int nfi_nfs_reconnect(struct nfi_server *server);
int nfi_nfs_disconnect(struct nfi_server *server);

int nfi_nfs_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_nfs_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_nfs_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_nfs_close(struct nfi_server *server, struct nfi_fhandle *fh);

ssize_t nfi_nfs_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

ssize_t nfi_nfs_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

int nfi_nfs_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);

int nfi_nfs_remove(struct nfi_server *server, char *url);

int nfi_nfs_rename(struct nfi_server *server, char *old_url, char *new_url);

int nfi_nfs_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

int nfi_nfs_rmdir(struct nfi_server *server, char *url);

int nfi_nfs_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_nfs_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type);

int nfi_nfs_closedir(struct nfi_server *server, struct nfi_fhandle *fh);

int nfi_nfs_statfs(struct nfi_server *server, struct nfi_info *inf);





# 4 "../../include/nfi_nfs/nfs_lib.h" 2



# 9 "../../include/nfi_lib.h" 2

# 1 "../../include/nfi_local/local_lib.h" 1



# 1 "../../include/nfi_local/nfi_local.h" 1







# 1 "../../include/nfi_local/nfi_local_err.h" 1




enum nfi_local_err{
	LOCALERR_PARAM = 0,
	LOCALERR_MEMORY = 1,
	LOCALERR_URL = 2,
	LOCALERR_MNTCONNECTION = 3,
	LOCALERR_MOUNT = 4,
	LOCALERR_NFSCONNECTION = 5,		
	LOCALERR_GETATTR = 6,
	LOCALERR_LOOKUP = 7,
	LOCALERR_READ = 8,
	LOCALERR_WRITE = 9,
	LOCALERR_CREATE = 10,
	LOCALERR_REMOVE = 11,
	LOCALERR_MKDIR = 12,
	LOCALERR_READDIR = 13,
	LOCALERR_STATFS = 14,
	LOCALERR_NOTDIR = 15,
};


void local_err(int err);



# 8 "../../include/nfi_local/nfi_local.h" 2





struct nfi_local_server{
	char path[255 ];
};

struct nfi_local_fhandle{
	char path[255 ];
	int fd;
	DIR *dir;
};


int nfi_local_init(char *url, struct nfi_server *serv);
int nfi_local_destroy(struct nfi_server *server);



int nfi_local_reconnect(struct nfi_server *server);
int nfi_local_disconnect(struct nfi_server *server);

int nfi_local_getattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_local_setattr(struct nfi_server *server, struct nfi_fhandle *fh, struct nfi_attr *attr);

int nfi_local_open(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_local_close(struct nfi_server *server, struct nfi_fhandle *fh);

ssize_t nfi_local_read(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

ssize_t nfi_local_write(struct nfi_server *server, struct nfi_fhandle *fh, void *buffer, off_t offset, size_t size);

int nfi_local_create(struct nfi_server *server, char *url,  struct nfi_attr *attr, struct nfi_fhandle  *fh);

int nfi_local_remove(struct nfi_server *server, char *url);

int nfi_local_rename(struct nfi_server *server, char *old_url, char *new_url);

int nfi_local_mkdir(struct nfi_server *server, char *url, struct nfi_attr *attr, struct nfi_fhandle *fh);

int nfi_local_rmdir(struct nfi_server *server, char *url);

int nfi_local_opendir(struct nfi_server *server, char *url, struct nfi_fhandle *fho);

int nfi_local_readdir(struct nfi_server *server, struct nfi_fhandle *fhd, char *entry , unsigned char *type);

int nfi_local_closedir(struct nfi_server *server, struct nfi_fhandle *fh);

int nfi_local_statfs(struct nfi_server *server, struct nfi_info *inf);


# 4 "../../include/nfi_local/local_lib.h" 2



# 10 "../../include/nfi_lib.h" 2




# 6 "../../include/xpn.h" 2





 
struct xpn_partition{
	int id;		 
	int nserv; 	 
	int type;	 
	char name[255 ]; 
	int block_size;  
	struct nfi_server *serv;  	
};


 
struct xpn_metadata{
	int sizem;
	int id;
	int version; 	 
	int type; 	 
	int block_size;  
	int nserv;  	 
	struct nfi_attr attr;    
	int type_policy;
	void *policy;
};


enum xpn_policy{
	cicly = 0,
};




struct cicly_policy{
	int first_node;	
};



# 5 "prueba_isdir.c" 2

# 1 "../../include/expand.h" 1



# 1 "../../include/xpn_file.h" 1




# 1 "../../include/xpn_err.h" 1






enum xpn_err_code{
        XPN_OK = 0,
        XPNERR_PARAM = 1,
        XPNERR_MEMORY = 2,
        XPNERR_INVALURL = 3,
        XPNERR_CONF = 4,
	XPNERR_INITSERV = 5, 
        XPNERR_GETATTR = 6,
        XPNERR_LOOKUP = 7,
        XPNERR_READ = 8,
        XPNERR_WRITE = 9,
        XPNERR_CREATE = 10,
        XPNERR_REMOVE = 11,
        XPNERR_MKDIR = 12,
        XPNERR_READDIR = 13,
        XPNERR_STATFS = 14,
	XPNERR_PATH_NOEXIST = 15,
	XPNERR_PART_NOEXIST = 16,
	XPNERR_NOMEMORY = 17,
};




extern int (*__errno_location ()) ;
extern int xpn_errno;

int xpn_err(int err);



# 5 "../../include/xpn_file.h" 2

# 1 "../../include/xpn_policy_init.h" 1





# 1 "../../include/xpn_init.h" 1




# 1 "../../include/xpn_policy_init.h" 1
# 51 "../../include/xpn_policy_init.h"


# 5 "../../include/xpn_init.h" 2

# 1 "../../include/xpn_cwd.h" 1





# 1 "../../include/xpn_policy_cwd.h" 1




# 1 "../../include/xpn_cwd.h" 1
# 19 "../../include/xpn_cwd.h"


# 5 "../../include/xpn_policy_cwd.h" 2



int XpnGetAbsolutePath(char *path, char *path_part);



# 6 "../../include/xpn_cwd.h" 2


struct xpn_cwd{
	char xpn_cwd_path[255 ];	
};

extern struct xpn_cwd xpn_cwddir;

void xpn_init_cwd();
char* xpn_getcwd(char *path, size_t size);

int xpn_chdir(char *path);



# 6 "../../include/xpn_init.h" 2

# 1 "../../include/xpn_file.h" 1
# 67 "../../include/xpn_file.h"


# 7 "../../include/xpn_init.h" 2







extern struct xpn_partition xpn_parttable[128 ];

int xpn_init();

int xpn_destroy();



# 6 "../../include/xpn_policy_init.h" 2




enum type_conf_connection{
	conf_file = 0,
	conf_rpc = 1,
	conf_descriptor = 2,
};

struct conf_connect{
	int type;
	union{
	  FILE *f;
	  CLIENT *cl;
	  int fd;
	 
	}connect_u;
};








struct conf_connect* XpnPartitionOpen();

void XpnPartitionClose(struct conf_connect *fconf);

int XpnGetNextPartition(struct conf_connect *fconf, char *name);

int XpnGetIdPartition(struct conf_connect *fconf, char *name);

int XpnGetInfoPartition(struct conf_connect *fconf, struct xpn_partition *part);

int XpnGetNumServersPartition(struct conf_connect *fconf, struct xpn_partition *part);

struct xpn_partition* XpnSearchPart(int pd);

int XpnGetServer(struct conf_connect *fconf, struct xpn_partition *part, struct nfi_server *serv);

int XpnGetPartition(char *path);



# 6 "../../include/xpn_file.h" 2



 



 



struct xpn_fh{
	int n_nfih;
        struct nfi_fhandle **nfih; 	 
};



struct xpn_attr{
	int 	at_type; 	 	
	mode_t 	at_mode;	 
	uid_t	at_uid;		 
	gid_t	at_gid;		 
	off_t	at_size;		 
	u_long  at_blksize;	 
	u_long  at_blocks;  	 
	time_t  at_atime;   	 
	time_t  at_mtime;   	 
	time_t  at_ctime;   	 
	void	*private;
					     
};


 
struct xpn_filedesc{
        int id;          
	char path[255 ]; 
        int type;        
        int links;       
	mode_t mode;	 
        struct xpn_partition *part;      
 	struct xpn_metadata *mdata;  
        struct xpn_attr attr;    
        off_t offset;        
        int block_size;          
        struct xpn_fh *vfh;  
};



 
extern struct xpn_filedesc *xpn_file_table[128 ];


int xpn_init_file();


int xpn_destroy_file();




# 4 "../../include/expand.h" 2


# 1 "../../include/xpn_open.h" 1





# 1 "../../include/xpn_policy_open.h" 1








enum xpn_work{
        no_xpn_op            = -1,
        op_xpn_getattr      = 0,
	op_xpn_setattr      = 1,
	op_xpn_open         = 2,
	op_xpn_create       = 3,
	op_xpn_close        = 4,
	op_xpn_remove       = 5,
	op_xpn_rename       = 6,
	op_xpn_read         = 7,
	op_xpn_write        = 8,
	op_xpn_mkdir        = 9,
	op_xpn_rmdir        = 10,
	op_xpn_opendir      = 11,
	op_xpn_readdir      = 12,
	op_xpn_closedir     = 13,
	op_xpn_statfs       = 14,
};





void XpnGetURLServer( struct nfi_server *serv, char *abs_path, char *url_serv);

int XpnGetServers(int op, int pd, char *abs_path, int fd, struct nfi_server ***servers);

int XpnCreateMetadata(struct xpn_metadata *mdata, int pd, char *path);

int XpnReadMetadata(    struct xpn_metadata *mdata,
			int nserv,
                        struct nfi_server **servers ,
                        struct xpn_fh *fh,
	                char *path,
			int pd);

int XpnUpdateMetadata(	struct xpn_metadata *mdata,
			int nserv,
                     	struct nfi_server **servers ,
                      	struct xpn_fh *fh,
                      	char *path);

int XpnGetFh(	struct nfi_fhandle **fh, 
		struct nfi_server *servers, 
		char *path);



int XpnGetAtrib(int fd, struct stat *st);


# 6 "../../include/xpn_open.h" 2






int xpn_creat(char *path, mode_t perm);

int xpn_open(char *path, int flags , ...);

int xpn_close(int fd);

int xpn_unlink(char *path);

int xpn_rename(char *path,char *newpath);

int xpn_fstat(int fd, struct stat *sb);

int xpn_stat(char *path, struct stat *sb);

int xpn_chown(char *path, uid_t owner, gid_t group);

int xpn_fchown(int fd, uid_t owner, gid_t group);

int xpn_chmod(char *path, mode_t mode);

int xpn_fchmod(int fd, mode_t mode);

int xpn_ftruncate(int fd, off_t length);

int xpn_dup(int fd);

int xpn_dup2(int fd, int fd2);



# 6 "../../include/expand.h" 2


# 1 "../../include/xpn_rw.h" 1






# 1 "../../include/xpn_policy_rw.h" 1






int XpnGetBlock(int fd, int offset, int *local_offset, int *serv);



# 7 "../../include/xpn_rw.h" 2




ssize_t xpn_read(int fd, void *buffer, size_t size);

ssize_t xpn_write(int fd, void *buffer, size_t size);

ssize_t xpn_pread(int fd, void *buffer, size_t size, off_t offset);

ssize_t xpn_pwrite(int fd, void *buffer, size_t size, off_t offset);

off_t xpn_lseek(int fd, off_t offset, int flag);



# 8 "../../include/expand.h" 2

# 1 "../../include/xpn_dir.h" 1








int xpn_mkdir(char *path, mode_t perm) ;
int xpn_rmdir(char *path) ;



# 9 "../../include/expand.h" 2

# 1 "../../include/xpn_opendir.h" 1







struct __dirstream{
      int fd;
};


DIR* xpn_opendir(char *path);
int xpn_closedir(DIR *dirp);
struct dirent* xpn_readdir(DIR *dirp);
void xpn_rewinddir(DIR *dirp);




# 10 "../../include/expand.h" 2




# 6 "prueba_isdir.c" 2


int main(int argc, char *argv[]) {

	char *path;
	DIR *dirp;
	struct dirent *direntry;
	
	printf("hola\n");
	
	if (argc == 2){
		xpn_init();
		vsprintf(path, "/export/temp/pato11/", argv[1]);
		printf("\nProbando que devuelve S_ISDIR...\n");
		dirp = (DIR *)malloc(sizeof(DIR));
		dirp = xpn_opendir(argv[1]);
		printf("peta 1 antes\n");
		printf("dirp->fd = %d\n", dirp->fd);
		printf("peta 1 despues\n");
		direntry = (struct dirent *)malloc(sizeof(struct dirent));
		direntry = xpn_readdir(dirp);
		printf("peta 2 antes\n");
		printf("El directorio tiene tipo %u\n", direntry->d_type);
		printf("peta 2 despues\n");
		printf("%d\n", ((( ( direntry->d_type ) ) & 0170000 ) == (  0040000  ))  );
	}
	
	printf("\nadios\n");

	return(0);
}
