package org.expand.hadoop;

import org.expand.jni.ExpandToPosix;
import org.expand.jni.ExpandFlags;

import java.nio.ByteBuffer;
import java.nio.channels.WritableByteChannel;

public class ExpandOutputChannel implements WritableByteChannel {

	private ExpandToPosix xpn;
	private ExpandFlags flags;

	private int fd;
	private ByteBuffer buf;

	public ExpandOutputChannel (int fd, int bufsize){
		this.xpn = new ExpandToPosix();
		this.flags = xpn.jni_xpn_createFlags();
		this.fd = fd;
		this.buf = ByteBuffer.allocateDirect(bufsize);
	}

	@Override
	public synchronized void close() {

		if (fd < 0) return;

		flush();
		this.buf = null;
		xpn.jni_xpn_close(this.fd);
	}

	@Override
	public synchronized boolean isOpen() {
		return fd >= 0;
	}

	@Override
	public synchronized int write(ByteBuffer src) {

		if (fd < 0) return -1;

		int iniSrcRemaining = src.remaining();
		int srcRemaining = 0;
		int srcPos = 0;
		int srcLimit = 0;
		int bufRemaining = 0;

		while (src.hasRemaining()){
			srcRemaining = src.remaining();
			srcPos = src.position();

			if (!buf.hasRemaining()) flush();

			bufRemaining = buf.remaining();
			srcLimit = src.limit();

			if (srcRemaining > bufRemaining) {
				src.limit(srcPos + bufRemaining);
				buf.put(src);
				src.limit(srcLimit);
			}else buf.put(src);
		}

		return iniSrcRemaining - src.remaining();
	}

	protected void flush() {
		if (fd < 0) return;

		buf.flip();
		if (buf.hasRemaining()){
			long i = xpn.jni_xpn_write(fd, buf, buf.remaining());
		}

		buf.clear();
	}
}
