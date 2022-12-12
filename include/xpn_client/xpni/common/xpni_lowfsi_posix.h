
#ifndef _XPNI_LOWFSI_POSIX_H
#define _XPNI_LOWFSI_POSIX_H

 #ifdef  __cplusplus
    extern "C" {
 #endif


      #define xpni_lowfsi_init() \
              (1)

      #define xpni_lowfsi_destroy() \
              (1)

      #define xpni_lowfsi_open(mfname,mflag,mmode) \
              open((mfname),(mflag),(mmode))

      #define xpni_lowfsi_creat(mfname,mmode) \
              creat((mfname),(mmode))

      #define xpni_lowfsi_close(mfd) \
              close((mfd))

      #define xpni_lowfsi_pread(mfd,mbuf,moff,msiz) \
              (lseek((mfd),(moff),SEEK_SET),read((mfd),(mbuf),(msiz)))

      #define xpni_lowfsi_pwrite(mfd,mbuf,moff,msiz) \
              (lseek((mfd),(moff),SEEK_SET),write((mfd),(mbuf),(msiz)))

      #define xpni_lowfsi_rename(msrc,mdst) \
              rename((msrc),(mdst))

      #define xpni_lowfsi_unlink(mfname) \
              unlink((mfname))

      #define xpni_lowfsi_lseek(mfd,moffset,mflag) \
              lseek((mfd),(moffset),(mflag))

      #define xpni_lowfsi_opendir(mpath) \
              opendir((mpath))


 #ifdef  __cplusplus
    }
 #endif

#endif

