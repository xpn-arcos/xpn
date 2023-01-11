
import os
import io
from ctypes import *


# <intercept>
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

def open_by_xpn(*args, **kwargs):
    return xpnFile("./xpn_py.so", args, kwargs)

open = open_by_xpn
# </intercept>


#
# Main
#

# fill buffer with 1024 'a'
buff_w = []
for i in range(0,1024):
    buff_w.append('a')
buff_w.append('\n')

# write and read buffer
fname = "demo.txt"

f = open(fname, "w")
f.write(str(buff_w))
f.close()

f = open(fname, "r")
buff_r = f.read()
f.close()

# print readed buffer
print(buff_r[:25] + "...")

