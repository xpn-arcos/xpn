#ifndef _XPN_DEBUG_H
#define _XPN_DEBUG_H

#include <stdio.h>
#include <errno.h>

/*
 * Set debug configuration
 */
extern int xpn_debug; // In src/xpn/xpn_simple/xpncore/xpn_init.c

#define PRINT_TRACE \
	if (xpn_debug) { \
		fprintf (stderr, "[%s:%d]\n", __FILE__, __LINE__); \
	}

#define XPN_DEBUG_COMMON_HEADER fprintf (stderr, "[%s][%s:%d] ", __func__, __FILE__, __LINE__);

#define XPN_DEBUG(format, ...) \
	if (xpn_debug) { \
		XPN_DEBUG_COMMON_HEADER \
		fprintf (stderr, format, ## __VA_ARGS__); \
		fprintf (stderr, "\n"); \
	}

#define XPN_DEBUG_BEGIN_CUSTOM(format, ...) \
	if (xpn_debug) { \
		XPN_DEBUG_COMMON_HEADER \
		fprintf (stderr, "Begin %s", __func__); \
		fprintf (stderr, "("); \
		fprintf (stderr, format, ## __VA_ARGS__); \
		fprintf (stderr, ")"); \
		fprintf (stderr, "\n"); \
	}

#define XPN_DEBUG_END_CUSTOM(format, ...) \
	if (xpn_debug) { \
		XPN_DEBUG_COMMON_HEADER \
		fprintf (stderr, "End   %s", __func__); \
		fprintf (stderr, "("); \
		fprintf (stderr, format, ## __VA_ARGS__); \
		fprintf (stderr, ")"); \
		fprintf (stderr, "=%d, errno=%d", (int)res, errno); \
		fprintf (stderr, "\n"); \
	}

#define XPN_DEBUG_BEGIN XPN_DEBUG("Begin %s()", __func__)
#define XPN_DEBUG_END   XPN_DEBUG("End   %s()=%d, errno=%d", __func__, (int)res, errno)

#define XPN_DEBUG_BEGIN_ARGS1(...) XPN_DEBUG("Begin %s(%s)", __func__, ## __VA_ARGS__)
#define XPN_DEBUG_END_ARGS1(...)   XPN_DEBUG("End   %s(%s)=%d, errno=%d", __func__, ## __VA_ARGS__, (int)res, errno)

#define XPN_DEBUG_BEGIN_ARGS2(...) XPN_DEBUG("Begin %s(%s, %s)", __func__, ## __VA_ARGS__)
#define XPN_DEBUG_END_ARGS2(...)   XPN_DEBUG("End   %s(%s, %s)=%d, errno=%d", __func__, ## __VA_ARGS__, (int)res, errno)

#if defined(DEBUG)
    // base
    // nfi
    #define DEBUG_NFI
    #define DEBUG_MNT
    #define DEBUG_NFS
    #define DEBUG_NFSW
    #define DEBUG_NFS_ERR
    #define DEBUG_NFS_ERRW
    // xpn
    #define XPN_DEBUG
#endif

#endif
