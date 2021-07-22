
#ifndef _XPNI_FIT_H
#define _XPNI_FIT_H

 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Include / Inclusion ........................................... */

      #include "base/dtable.h"
      #include "xpn.h"
      #include "xpni/common/xpni_fmeta.h"


   /* ... Types / Tipos ................................................. */

      typedef
      struct
      {

         char     *xpn_fname ;
         int       xpn_fd[2] ;
         fmeta_t   xpn_fmeta ;
         int       xpni_fsd ;
         int       xpn_flag ;
         mode_t    xpn_mode ;

      } xpni_fi_t ;


   /* ... Functions / Funciones ......................................... */

      xpni_fi_t  *xpni_fit_new       ( ) ;
      int         xpni_fit_insert    ( xpni_fi_t *xpni_fi ) ;
      int         xpni_fit_insertNew ( ) ;
      int         xpni_fit_delete    ( int fd ) ;

      xpni_fi_t  *xpni_fit_get              ( int fd ) ;
      char       *xpni_fit_get_XPN_FNAME    ( int fd ) ;
      int         xpni_fit_get_XPN_DATA_FD  ( int fd ) ;
      int         xpni_fit_get_XPN_MDATA_FD ( int fd ) ;
      fmeta_t     xpni_fit_get_XPN_FMETA    ( int fd ) ;
      int         xpni_fit_get_XPNI_FSD     ( int fd ) ;
      int         xpni_fit_get_XPN_FLAG     ( int fd ) ;
      mode_t      xpni_fit_get_XPN_MODE     ( int fd ) ;

      int         xpni_fit_set_XPN_FNAME    ( int fd, char *fname ) ;
      int         xpni_fit_set_XPN_DATA_FD  ( int fd, int xpn_fd ) ;
      int         xpni_fit_set_XPN_MDATA_FD ( int fd, int xpn_fd ) ;
      int         xpni_fit_set_XPN_FMETA    ( int fd, fmeta_t *fmeta ) ;
      int         xpni_fit_set_XPNI_FSD     ( int fd, int fsd ) ;
      int         xpni_fit_set_XPN_FLAG     ( int fd, int flag ) ;
      int         xpni_fit_set_XPN_MODE     ( int fd, mode_t mode ) ;


   /* ... Macros / Macros ............................................... */

      /* Checking */

      #define xpni_fit_is_correct(mfd) \
              (NULL != xpni_fit_get((mfd)))


      /* Access to field */

      #define xpni_fit_get_XPN_FNAME(mfd) \
              (xpni_fit_get((mfd))->xpn_fname)

      #define xpni_fit_get_XPN_DATA_FD(mfd) \
              (xpni_fit_get((mfd))->xpn_fd[0])

      #define xpni_fit_get_XPN_MDATA_FD(mfd) \
              (xpni_fit_get((mfd))->xpn_fd[1])

      #define xpni_fit_get_XPN_FMETA(mfd) \
              (xpni_fit_get((mfd))->xpn_fmeta)

      #define xpni_fit_get_XPNI_FSD(mfd) \
              (xpni_fit_get((mfd))->xpni_fsd)

      #define xpni_fit_get_XPN_FLAG(mfd) \
              (xpni_fit_get((mfd))->xpn_flag)

      #define xpni_fit_get_XPN_MODE(mfd) \
              (xpni_fit_get((mfd))->xpn_mode)


      /* Modify a field */

      #define xpni_fit_set_XPN_FNAME(mfd,mfname) \
              ((xpni_fit_get((mfd))->xpn_fname)=(mfname))

      #define xpni_fit_set_XPN_DATA_FD(mfd,mpnfd) \
              ((xpni_fit_get((mfd))->xpn_fd[0])=(mpnfd))

      #define xpni_fit_set_XPN_MDATA_FD(mfd,mpnfd) \
              ((xpni_fit_get((mfd))->xpn_fd[1])=(mpnfd))

      #define xpni_fit_set_XPN_FMETA(mfd,mfmeta) \
              ((xpni_fit_get((mfd))->xpn_fmeta)=(mfmeta))

      #define xpni_fit_set_XPNI_FSD(mfd,mfsd) \
              ((xpni_fit_get((mfd))->xpni_fsd)=(mfsd))

      #define xpni_fit_set_XPN_FLAG(mfd,mflag) \
              ((xpni_fit_get((mfd))->xpn_flag)=(mflag))

      #define xpni_fit_set_XPN_MODE(mfd,mmode) \
              ((xpni_fit_get((mfd))->xpn_mode)=(mmode))


   /* ................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

