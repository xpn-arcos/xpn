package org.expand.hadoop;

import java.nio.ByteBuffer;
import java.nio.channels.ReadableByteChannel;

import org.expand.jni.ExpandToPosix;
import org.expand.jni.ExpandFlags;

public class ExpandInputChannel implements ReadableByteChannel {
	private ExpandToPosix xpn;
	private ExpandFlags flags;
	private int fd;
	private int bufsize;
	private ByteBuffer buf;

	public ExpandInputChannel (int fd, int bufsize) {
		this.xpn = new ExpandToPosix();
		this.flags = xpn.jni_xpn_createFlags();
		this.fd = fd;
		this.bufsize = bufsize;
		this.buf = ByteBuffer.allocateDirect(bufsize);
		buf.flip();
	}

	@Override
	public synchronized void close() {
		if (fd < 0) return;
		
		xpn.jni_xpn_close(fd);
		this.fd = -1;
		this.flags = null;
		this.buf.clear();
		this.buf = null;
	}

	@Override
	public synchronized boolean isOpen() {
		return this.fd >= 0;
	}

	@Override
	public synchronized int read (ByteBuffer dst) {
		if (fd < 0) return -1;

		int iniDstRemaining = dst.remaining();
		int dstRemaining = 0;
		int bufRemaining = 0;
		int bufLimit = 0;
		int bufPosition = 0;

		while (dst.hasRemaining()) {
			dstRemaining = dst.remaining();

			if (!buf.hasRemaining()) {
				buf.clear();
				readXPN();
				if (buf.position() == 0) {
					buf.limit(0);
					break;
				}
				buf.flip();
			}
			bufRemaining = buf.remaining();
			bufLimit = buf.limit();
			bufPosition = buf.position();

			if (bufRemaining > dstRemaining) {
				buf.limit(bufPosition + dstRemaining);
				dst.put(buf);
				buf.limit(bufLimit);
			}else{
				dst.put(buf);
			}
		}

		return iniDstRemaining - dst.remaining();
	}

	private synchronized void readXPN() {
		if (fd < 0) return;
		long ret = xpn.jni_xpn_read(fd, buf, bufsize);
		if (ret < 0) return;
		buf.position((int) ret);
	}

	public synchronized void seek (long pos) {
		if (fd < 0) return;

		buf.position(0).limit(0);
		xpn.jni_xpn_lseek(fd, pos, flags.SEEK_SET);
	}

	public synchronized long tell() {
		if (fd < 0) return -1;

		long ret = xpn.jni_xpn_lseek(fd, 0, flags.SEEK_CUR);

		return ret - buf.remaining();
	}
}
