
#ifndef _XPNI_FSIT_H
#define _XPNI_FSIT_H

 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Include / Inclusion ........................................... */

      #include "xpn.h"
      #include "xpni/common/xpni_fit.h"
      #include "base/dtable.h"


   /* ... Types / Tipos ................................................. */

      typedef 
      struct struct_xpni_fsi_t xpni_fsi_t ;

      struct struct_xpni_fsi_t
      {

         char     *fsi_name ;

         int     (*fsi_init)       ( ) ;
         int     (*fsi_destroy)    ( ) ;

	 int     (*fsi_register)   ( xpni_fsi_t *fsi ) ;
	 int     (*fsi_unregister) ( xpni_fsi_t *fsi ) ;

	 int     (*fsi_importFile) ( int fd ) ;
	 int     (*fsi_exportFile) ( int fd ) ;

         int     (*fsi_open)       ( int fd ) ;
         int     (*fsi_creat)      ( int fd ) ;
         int     (*fsi_close)      ( int fd ) ;

         ssize_t (*fsi_sread)      ( int fd, void *buffer, off_t offset, size_t size ) ;
         ssize_t (*fsi_swrite)     ( int fd, void *buffer, off_t offset, size_t size ) ;

	 off_t   (*fsi_lseek)      ( int fd, off_t offset, int flag ) ;
	 
	 DIR*	 (*fsi_opendir)	   ( int fd ) ;

         /*#NewInterfaceFunction.AsPointer#*/

      } ;


   /* ... Functions / Funciones ......................................... */

      xpni_fsi_t *xpni_fsit_new       ( ) ;
      int         xpni_fsit_insert    ( xpni_fsi_t *xpni_fsi ) ;
      int         xpni_fsit_insertNew ( ) ;

      int         xpni_fsit_destroy   ( ) ;
      int         xpni_fsit_delete    ( int fd ) ;

      xpni_fsi_t *xpni_fsit_get       ( int fd ) ;
      int         xpni_fsit_getByName ( char *fsi_name ) ;

      int         xpni_fsit_importFile ( int fd ) ;
      int         xpni_fsit_exportFile ( int fd ) ;

      off_t       xpni_fsit_lseek      ( int fd, off_t offset, int flag ) ;
      
      DIR        *xpni_fsit_opendir    ( char *path ) ;
      
      /*#NewInterfaceFunction.AsFunction#*/


   /* ... Macros / Macros ............................................... */

      /* Checking */

      #define xpni_fsit_is_correct(mfd) \
              (NULL != xpni_fsit_get((mfd)))


      /* Access to field */

      #define xpni_fsit_get_FSI_NAME(fd) \
              (xpni_fsit_get((fd))->fsi_name)


      /* Call to a function field from file system management */

      #define xpni_fsit_FSI_INIT(mfd) \
              ((xpni_fsit_get((mfd))->fsi_init)())

      #define xpni_fsit_FSI_DESTROY(mfd) \
              ((xpni_fsit_get((mfd))->fsi_destroy)())

      #define xpni_fsit_FSI_REGISTER(mfd) \
              ((xpni_fsit_get((mfd))->fsi_register)((xpni_fsit_get((mfd)))))

      #define xpni_fsit_FSI_UNREGISTER(mfd) \
              ((xpni_fsit_get((mfd))->fsi_unregister)((xpni_fsit_get((mfd)))))


      /* Call to a function field in general */

      #define xpni_fsit_FSI_IMPORTFILE(mfd) \
              ((xpni_fsit_get(xpni_fit_get_XPNI_FSD((mfd)))->fsi_importFile)((mfd)))

      #define xpni_fsit_FSI_EXPORTFILE(mfd) \
              ((xpni_fsit_get(xpni_fit_get_XPNI_FSD((mfd)))->fsi_exportFile)((mfd)))


      /* Call to a function field from file management */

      #define xpni_fsit_FSI_OPEN(mfd) \
              ((xpni_fsit_get(xpni_fit_get_XPNI_FSD((mfd)))->fsi_open)((mfd)))

      #define xpni_fsit_FSI_CREAT(mfd) \
              ((xpni_fsit_get(xpni_fit_get_XPNI_FSD((mfd)))->fsi_creat)((mfd)))

      #define xpni_fsit_FSI_CLOSE(mfd) \
              ((xpni_fsit_get(xpni_fit_get_XPNI_FSD((mfd)))->fsi_close)((mfd)))

      #define xpni_fsit_FSI_SREAD(mfd,mbuf,moff,msiz) \
              ((xpni_fsit_get(xpni_fit_get_XPNI_FSD((mfd)))->fsi_sread)((mfd),(mbuf),(moff),(msiz)))

      #define xpni_fsit_FSI_SWRITE(mfd,mbuf,moff,msiz) \
              ((xpni_fsit_get(xpni_fit_get_XPNI_FSD((mfd)))->fsi_swrite)((mfd),(mbuf),(moff),(msiz)))

      #define xpni_fsit_FSI_LSEEK(mfd,moffset,mflag) \
	      ((xpni_fsit_get(xpni_fit_get_XPNI_FSD((mfd)))->fsi_lseek)((mfd),(moffset),(mflag)))


      /* Call to a function field from file management */

      #define xpni_fsit_FSI_OPENDIR(mdd) \
              ((xpni_fsit_get(xpni_fit_get_XPNI_FSD((mdd)))->fsi_opendir)((mdd)))


      /* Call to a function field (in general) */
	      
      /*#NewInterfaceFunction.AsMacro1#*/


   /* ................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

