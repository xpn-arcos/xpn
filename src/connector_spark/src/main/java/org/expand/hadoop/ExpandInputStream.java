package org.expand.hadoop;

import java.io.InputStream;
import java.io.Closeable;
import java.nio.ByteBuffer;

import org.expand.jni.ExpandToPosix;
import org.expand.jni.ExpandFlags;
import org.expand.jni.Stat;

public class ExpandInputStream extends InputStream implements Closeable {

	private ExpandToPosix xpn;
	private ExpandFlags flags;
	private ExpandInputChannel in;
	private int fd;
	private String path;
	private long fileSize;

	public ExpandInputStream (String path, int bufsize) {
		this.xpn = new ExpandToPosix();
		this.flags = xpn.jni_xpn_createFlags();
		this.path = path;
		this.fd = xpn.jni_xpn_open(path, flags.O_RDONLY);
		Stat stats = this.xpn.jni_xpn_stat(path.toString());
		this.fileSize = stats.st_size;
		if (fileSize < 0) {
			System.out.println("WRONG FILE SIZE " + path);
		}
		this.in = new ExpandInputChannel(fd, bufsize);
	}

	@Override
	public synchronized int available() {
		return 0;
	}

	@Override
	public synchronized void close() {
		if (this.in == null) return;

		in.close();
		in = null;
	}

	@Override
	public void mark (int readLimit) {}

	@Override
	public boolean markSupported() {
		return false;
	}

	@Override
	public synchronized int read () {
		byte b[] = new byte [1];
		int res = read(b, 0, 1);
		if (res == 1){
			int ret = 0xff & b[0];
			return ret;
		}
		return -1;
	}

	@Override
	public synchronized int read (byte b[]) {
		return read(b, 0, b.length);
	}

	@Override
	public synchronized int read (byte b[], int off, int len) {
		if (in == null) return -1;
		if (len == 0) return 0;
		
		return in.read(ByteBuffer.wrap(b, off, len));
	}

	@Override
	public void reset() {}

	@Override
	public synchronized long skip (long n) {
		if (n < 0) return 0;
		if (in == null) return -1;

		long avail = available();

		if (n > avail) n = avail;

		in.seek(n);
		return n;
	}

	public synchronized void seek (long pos) {
		if (in == null) return;
		if (pos >= fileSize) return;
		
		in.seek(pos);
	}

	public long tell () {
		if (in == null) return -1;

		return in.tell();
	}
}
