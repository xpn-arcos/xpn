#ifndef _M_STDIO_H
#define	_M_STDIO_H

#define _open xpn_open
#define _creat xpn_creat
#define _close xpn_close
#define _read xpn_read
#define _write xpn_write
#define _lseek xpn_lseek
#define fstat xpn_fstat
#define fflush xpn_fflush

#include <pthread.h>

struct _prefetch_t {
	int do_prefetch;
	pthread_t thread;
	pthread_mutex_t mutex;
	pthread_cond_t cond_main;
	pthread_cond_t cond_th;
};

/*
 * The purpose of these definitions is to preserve the names of the attributes
 * of the FILE structure in the MINIX code.
 * The FILE structure is defined in /usr/include/libio.h as 'struct _IO_FILE'.
 */
#define _count _flags2
#define _fd _fileno
#define _buf _IO_buf_base
#define _ptr _IO_read_ptr
#define _end _IO_buf_end
#define _bufsiz_rel_addr _unused2[0]
#define _bufsiz *_bufsiz_addr
#define _prefetch_rel_addr _unused2[sizeof(int)]
#define _prefetch (*_prefetch_addr)

//#define _isatty(p) 1

#define	PMODE		0666


/*
 * loc_incl.h - local include file for stdio library
 */
/* $Header$ */

#include	<stdio.h>

#define	io_testflag(p,x)	((p)->_flags & (x))

#include	<stdarg.h>

/*
 * stdio.h - input/output definitions
 *
 * (c) copyright 1987 by the Vrije Universiteit, Amsterdam, The Netherlands.
 * See the copyright notice in the ACK home directory, in the file "Copyright".
 */
/* $Header$ */

#ifdef _IOFBF
#undef _IOFBF
#endif

#ifdef _IONBF
#undef _IONBF
#endif

#ifdef _IOLBF
#undef _IOLBF
#endif

#define	_IOFBF		0x000
#define	_IOREAD		0x001
#define	_IOWRITE	0x002
#define	_IONBF		0x004
#define	_IOMYBUF	0x008
#define	_IOEOF		0x010
#define	_IOERR		0x020
#define	_IOLBF		0x040
#define	_IOREADING	0x080
#define	_IOWRITING	0x100
#define	_IOAPPEND	0x200
#define _IOFIFO		0x400

/*
#ifdef stdin
#undef stdin
#endif

#ifdef stdout
#undef stdout
#endif

#ifdef stderr
#undef stderr
#endif

#define	stdin		(&__stdin)
#define	stdout		(&__stdout)
#define	stderr		(&__stderr)
*/

#ifdef BUFSIZ
#undef BUFSIZ
#endif

#define	BUFSIZ		(64*1024)
//#define	NULL		((void *)0)
#define	EOF		(-1)

#define	getchar()	getc(stdin)
#define	putchar(c)	putc(c,stdout)
/*#define	getc(p)		(--(p)->_count >= 0 ? (int) (*(p)->_ptr++) : \
				__fillbuf(p))*/
#define xpn_getc(p)         (--(p)->_count >= 0 ? (unsigned char) (*(p)->_ptr++) : \
                                xpn_fillbuf(p))
#define xpn_getc_nofill(p)  (--(p)->_count >= 0 ? (unsigned char) (*(p)->_ptr++) : \
                                swap_file_ptrs(p))
/*#define	putc(c, p)	(--(p)->_count >= 0 ? \
			 (int) (*(p)->_ptr++ = (c)) : \
			 __flushbuf((c),(p)))*/
#define xpn_putc(c, p)      (--(p)->_count >= 0 ? \
                         (unsigned char) (*(p)->_ptr++ = (c)) : \
                         xpn_flushbuf((c),(p)))

#define	feof(p)		(((p)->_flags & _IOEOF) != 0)
#define	ferror(p)	(((p)->_flags & _IOERR) != 0)
#define clearerr(p)     ((p)->_flags &= ~(_IOERR|_IOEOF))

#define	fileno(stream)		((stream)->_fd)

#endif
