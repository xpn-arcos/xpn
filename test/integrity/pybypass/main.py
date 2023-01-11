
import os
import io
from ctypes import *


# do_write: write /P1/demo.txt
def do_write(fname, buff):
    f = open(fname, "w")
    f.write(str(buff))
    f.close()

# do_read: read /P1/demo.txt
def do_read(fname):
    f = open(fname, "r")
    b = f.read()
    f.close()
    return b

class xpnFile:
    def __init__(self, so_file, args, kwargs):
        self.xpn = CDLL(so_file)
        self.fd = self.xpn.open2(args[0].encode(), args[1].encode())
    def write(self, buf):
        return self.xpn.write2(self.fd, buf, len(buf))
    def read(self):
        return self.xpn.read2(self.fd)
    def close(self):
        return self.xpn.close2(self.fd)


#
# Main
#

# <intercept>
def open_by_xpn(*args, **kwargs):
    return xpnFile("./xpn_py.so", args, kwargs)
open = open_by_xpn
# </intercept>

fname = "/tmp/P1/demo.txt"
if os.path.exists(fname):
   os.remove(fname)

# fill buffer with 1024 'a'
buff_w = []
for i in range(0,1024):
    buff_w.append('a')
buff_w.append('\n')

# write and read buffer
##fname = "/P1/demo.txt"
fname = "demo.txt"
do_write(fname, buff_w)
buff_r = do_read(fname)

# print readed buffer
print(buff_r)

