# Expand
Expand: XPN Parallel File System

*Licence*: GNU GENERAL PUBLIC LICENSE Version 3</br>
*Authors*: Felix Garcia Carballeira, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Alejandro Calderon Mateos, Diego Camarmas Alonso, David Garcia Fernandez


## 1. Source structure

The Expand client source code (src/xpn_client) has four main levels:
* xpni:
  eXPaNd Interface provides many enhanced functionality like Fault-Tolerance support, locking, etc.
* xpn:
  eXPaNd implements a Parallel File System using NFI interfaces.
* nfi:
  Network File Interface provides access to several storage protocols (local and network protocols) like NFS, FTP, etc.
* base:
  Base interface provides independent access to operating system resources. 
  Portability is provided in this layer.


## 2. Testing

### POSIX test suites (Thanks to Borja)

* <http://ltp.sourceforge.net/>
* <http://posixtest.sourceforge.net/>
* <http://www.edos-project.org/xwiki/bin/view/Main/Open_POSIX_Test_Suite>
* <http://www.itl.nist.gov/div897/ctg/posix_form.htm>
* <http://www.ntfs-3g.org/quality.html#testmethods>
* <http://www.ntfs-3g.org/sw/qa/pjd-fstest-20080412.tgz>
* <http://lwn.net/Articles/276617/>
* <http://www.opensubscriber.com/message/linux-fsdevel@vger.kernel.org/8931329.html>
* <http://www.opensubscriber.com/message/linux-fsdevel@vger.kernel.org/8956604.html>
* <http://groups.google.com/group/zfs-fuse/browse_thread/thread/ca726bf276697ff4/3817208dd31c450e?#3817208dd31c450e>
* <http://www.mail-archive.com/ntfs-3g-devel@lists.sourceforge.net/msg00502.html>
* <http://ceph.newdream.net/blog/2008/04/18/posix-file-system-test-suite/>
* <http://www.zip.com.au/~akpm/linux/patches/stuff/ext3-tools.tar.gz>
* <http://www.usenet-forums.com/linux-general/92890-posix-i-o-test-suite.html>
* <http://ftp.freestandards.org/pub/lsb/test_suites/>
* <http://www.bullopensource.org/posix/>
* <http://wiki.xensource.com/xenwiki/XenTest/FunctionalTests>
* <http://www.linuxtestproject.org/>
* <http://kambing.ui.edu/linuxfromscratch/lfs-website/lfs/view/stable/chapter06/glibc.html>
* <http://cross-lfs.org/view/1.0.0/mips64/final-system/glibc-64bit.html>

