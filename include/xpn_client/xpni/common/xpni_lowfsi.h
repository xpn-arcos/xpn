
#ifndef _XPNI_LOWFSI_H
#define _XPNI_LOWFSI_H

 #ifdef  __cplusplus
    extern "C" {
 #endif


   /* ... Include / Inclusion ........................................... */

      #include "all_system.h"


   /* ... Macros / Macros ............................................... */

#if defined(XPNI_LOWFSI_XPN)

      #include "xpni_lowfsi_xpn.h"

#elif defined(XPNI_LOWFSI_POSIX)

      #include "xpni_lowfsi_posix.h"

#elif defined(XPNI_LOWFSI_PRINTF)

      #include "xpni_lowfsi_printf.h"

#else /* by default */

      #include "xpni_lowfsi_xpn.h"

#endif


  /* .................................................................... */


 #ifdef  __cplusplus
    }
 #endif

#endif

