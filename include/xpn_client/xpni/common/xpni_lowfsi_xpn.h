
#ifndef _XPNI_LOWFSI_XPN_H
#define _XPNI_LOWFSI_XPN_H

 #ifdef  __cplusplus
    extern "C" {
 #endif


      #define xpni_lowfsi_init() \
              xpn_init()

      #define xpni_lowfsi_destroy() \
              xpn_destroy()

      #define xpni_lowfsi_open(mfname,mflag,mmode) \
              xpn_open((mfname),(mflag),(mmode))

      #define xpni_lowfsi_creat(mfname,mmode) \
              xpn_creat((mfname),(mmode))

      #define xpni_lowfsi_close(mfd) \
              xpn_close((mfd))

      #define xpni_lowfsi_pread(mfd,mbuf,moff,msiz) \
              xpn_pread((mfd),(mbuf),(moff),(msiz))

      #define xpni_lowfsi_pwrite(mfd,mbuf,moff,msiz) \
              xpn_pwrite((mfd),(mbuf),(moff),(msiz))

      #define xpni_lowfsi_rename(msrc,mdst) \
              xpn_rename((msrc),(mdst))

      #define xpni_lowfsi_unlink(mfname) \
              xpn_unlink((mfname))

      #define xpni_lowfsi_lseek(mfd,moffset,mflag) \
              xpn_lseek((mfd),(moffset),(mflag))

      #define xpni_lowfsi_opendir(mpath) \
              xpn_opendir((mpath))


 #ifdef  __cplusplus
    }
 #endif

#endif

