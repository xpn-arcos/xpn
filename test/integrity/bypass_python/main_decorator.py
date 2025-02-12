
#
#   Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
#
#   This file is part of Expand.
#
#   Expand is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   Expand is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with Expand.  If not, see <http://www.gnu.org/licenses/>.
#

import ctypes
import ctypes.util


# <intercept>
class xpnBypass:
    def __init__(self, args, kwargs, open_before, so_file, xpn_prefix):
        # initial attributes
        self.open_before = open_before
        self.xpn_prefix  = xpn_prefix
        self.is_xpn_file = str(args[0]).startswith(xpn_prefix)
        # load XPN mini-lib
        self.xpn  = ctypes.cdll.LoadLibrary(so_file)
        self.xpn.py_read.restype   = ctypes.POINTER(ctypes.c_char_p)
        self.xpn.py_write.argtypes = (ctypes.c_int, ctypes.c_char_p, ctypes.c_ulong)
        self.libc = ctypes.CDLL(ctypes.util.find_library('c'))
        self.libc.free.argtypes  = (ctypes.c_void_p,)
        # open file
        self.fd = ( self.xpn.py_open(args[0].encode(), args[1].encode()) if self.is_xpn_file else open_before(args[0], args[1]) )
    def write(self, buf):
        if self.is_xpn_file:
             ret = self.xpn.py_write(self.fd, buf, len(buf))
        else:
             ret = self.fd.write(buf)
        return ret
    def read(self):
        if self.is_xpn_file:
             ret  = ""
             _ret = self.xpn.py_read(self.fd)
             if _ret != None:
                ret  = str(ctypes.cast(_ret, ctypes.c_char_p).value)
                self.libc.free(_ret)
        else:
             ret = self.fd.read()
        return ret
    def close(self):
        if self.is_xpn_file:
             ret = self.xpn.py_close(self.fd)
        else:
             ret = self.fd.close()
        return ret

def open_bypass(*args, **kwargs):
    return xpnBypass(args, kwargs, old_open, "./py_xpn.so", "/tmp/expand/P1")

old_open = open
open = open_bypass
# </intercept>


#
# Main
#

# fill buffer with 1024 'a'
buff = []
for i in range(0,1024):
    buff.append('a')
buff.append('\n')
buff_w = ''.join(buff).encode('utf-8')

# write and read buffer
fname = "/tmp/expand/P1/demo.txt"
#fname = "/tmp/demo.txt"

f = open(fname, "w", encoding="utf-8")
f.write(buff_w)
f.close()

f = open(fname, "r", encoding="utf-8")
buff_r = f.read()
f.close()

# print readed buffer
print(buff_r[:25] + "...")

