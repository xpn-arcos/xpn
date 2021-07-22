#AM_CPPFLAGS = -I$(top_srcdir)/include -D_FILE_OFFSET_BITS=64 -D_REENTRANT
#LDADD = @enable_fuse@/lib/libfuse.la @libfuse_libs@ $(top_srcdir)/lib/libxpn.a -lrt -ldl
#fuse_expand_LDADD = @enable_fuse@/lib/libfuse.la @enable_fuse@/lib/libulockmgr.la

LDADD = @top_srcdir@/lib/libxpn.a @LIBFUSE_A@ -lfuse -lrt -ldl
bin_PROGRAMS = fuse-expand
