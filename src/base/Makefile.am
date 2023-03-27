
#############
# BASE      #
#############

BASE_HEADER=	@top_srcdir@/include/base/base_lib.h \
				@top_srcdir@/include/base/darray.h \
				@top_srcdir@/include/base/debug_msg.h \
				@top_srcdir@/include/base/debug_tags.h \
				@top_srcdir@/include/base/dtable.h \
				@top_srcdir@/include/base/math_misc.h \
				@top_srcdir@/include/base/string_misc.h \
				@top_srcdir@/include/base/path_misc.h \
				@top_srcdir@/include/base/time_misc.h \
				@top_srcdir@/include/base/trace_msg.h \
				@top_srcdir@/include/base/trace_tags.h \
				@top_srcdir@/include/base/urlstr.h \
				@top_srcdir@/include/base/ns.h \
				@top_srcdir@/include/base/syscall_proxies.h \
				@top_srcdir@/include/base/filesystem.h \
				@top_srcdir@/include/base/workers.h \
				@top_srcdir@/include/base/workers_ondemand.h \
				@top_srcdir@/include/base/workers_pool.h\
				@top_srcdir@/include/base/utils.h

BASE_SOURCE=	@top_srcdir@/src/base/darray.c \
				@top_srcdir@/src/base/dtable.c \
				@top_srcdir@/src/base/math_misc.c \
				@top_srcdir@/src/base/trace_msg.c \
				@top_srcdir@/src/base/debug_msg.c \
				@top_srcdir@/src/base/string_misc.c \
				@top_srcdir@/src/base/path_misc.c \
				@top_srcdir@/src/base/time_misc.c \
				@top_srcdir@/src/base/urlstr.c \
				@top_srcdir@/src/base/ns.c \
				@top_srcdir@/src/base/syscall_proxies.c \
				@top_srcdir@/src/base/filesystem.c \
				@top_srcdir@/src/base/workers.c \
				@top_srcdir@/src/base/workers_ondemand.c \
				@top_srcdir@/src/base/workers_pool.c \
				@top_srcdir@/src/base/utils.c


#############
# LIBRARIES #
#############

OBJEXT = o
lib_LIBRARIES     = libbase.a
libbase_a_SOURCES = $(BASE_SOURCE)

nobase_includedir      = $(includedir)/base
nobase_include_HEADERS = $(BASE_HEADER)

install-data-hook:
	mkdir -p $(includedir)
	[ -d $(includedir)/../../include ] && cp  -a $(includedir)/../../include/*  $(includedir)/
	[ -d $(includedir)/../../include ] && rm -fr $(includedir)/../../include

