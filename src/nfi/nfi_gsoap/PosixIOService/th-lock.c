#include "project/soapH.h"
/******************************************************************************\
 *
 *      OpenSSL
 *
\******************************************************************************/
#ifdef WITH_OPENSSL
#include <unistd.h> /* defines _POSIX_THREADS if pthreads are available */
#ifdef _POSIX_THREADS
# include <pthread.h>
#endif
#if defined(WIN32)
# define MUTEX_TYPE HANDLE
# define MUTEX_SETUP(x) (x) = CreateMutex(NULL, FALSE, NULL)
# define MUTEX_CLEANUP(x) CloseHandle(x)
# define MUTEX_LOCK(x) WaitForSingleObject((x), INFINITE)
# define MUTEX_UNLOCK(x) ReleaseMutex(x)
# define THREAD_ID GetCurrentThreadID()
#elif defined(_POSIX_THREADS)
# define MUTEX_TYPE pthread_mutex_t
# define MUTEX_SETUP(x) pthread_mutex_init(&(x), NULL)
# define MUTEX_CLEANUP(x) pthread_mutex_destroy(&(x))
# define MUTEX_LOCK(x) pthread_mutex_lock(&(x))
# define MUTEX_UNLOCK(x) pthread_mutex_unlock(&(x))
# define THREAD_ID pthread_self()
#else
# error "You must define mutex operations appropriate for your platform"
# error "See OpenSSL /threads/th-lock.c on how to implement mutex on your platform"
#endif
struct CRYPTO_dynlock_value { MUTEX_TYPE mutex; };
static MUTEX_TYPE *mutex_buf;
static struct CRYPTO_dynlock_value *dyn_create_function(const char *file, int line)
{
   struct CRYPTO_dynlock_value *value;
   value = (struct CRYPTO_dynlock_value*)malloc(sizeof(struct CRYPTO_dynlock_value));
   if (value)
      MUTEX_SETUP(value->mutex);
   return value;
}
static void dyn_lock_function(int mode, struct CRYPTO_dynlock_value *l, const char *file, int line)
{
   if (mode & CRYPTO_LOCK)
      MUTEX_LOCK(l->mutex);
   else
      MUTEX_UNLOCK(l->mutex);
}
static void dyn_destroy_function(struct CRYPTO_dynlock_value *l, const char *file, int line)
{
   MUTEX_CLEANUP(l->mutex);
   free(l);
}
void locking_function(int mode, int n, const char *file, int line)
{
   if (mode & CRYPTO_LOCK)
      MUTEX_LOCK(mutex_buf[n]);
   else
      MUTEX_UNLOCK(mutex_buf[n]);
}
unsigned long id_function()
{
   return (unsigned long)THREAD_ID;
}
int CRYPTO_thread_setup()
{
   int i;
   mutex_buf = (MUTEX_TYPE*)malloc(CRYPTO_num_locks() * sizeof(MUTEX_TYPE));
   if (!mutex_buf)
      return SOAP_EOM;
   for (i = 0; i < CRYPTO_num_locks(); i++)
      MUTEX_SETUP(mutex_buf[i]);
   CRYPTO_set_id_callback(id_function);
   CRYPTO_set_locking_callback(locking_function);
   CRYPTO_set_dynlock_create_callback(dyn_create_function);
   CRYPTO_set_dynlock_lock_callback(dyn_lock_function);
   CRYPTO_set_dynlock_destroy_callback(dyn_destroy_function);
   return SOAP_OK;
}
void CRYPTO_thread_cleanup()
{
   int i;
   if (!mutex_buf)
      return;
   CRYPTO_set_id_callback(NULL);
   CRYPTO_set_locking_callback(NULL);
   CRYPTO_set_dynlock_create_callback(NULL);
   CRYPTO_set_dynlock_lock_callback(NULL);
   CRYPTO_set_dynlock_destroy_callback(NULL);
   for (i = 0; i < CRYPTO_num_locks(); i++)
      MUTEX_CLEANUP(mutex_buf[i]);
   free(mutex_buf);
   mutex_buf = NULL;
}
#endif

/******************************************************************************\
 *
 *      SIGPIPE
 *
\******************************************************************************/

void sigpipe_handle(int x) { }
