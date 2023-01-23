
from ctypes import *


#
# Main
#

# fill buffer with 1024 'a'
buff = []
for i in range(0,1024):
    buff.append('a')
buff.append('\n')
buff_w = ''.join(buff)

# write and read buffer
fname = "/tmp/expand/P1/demo.txt"
#fname = "/tmp/demo.txt"

f = open(fname, "w", encoding="utf-8")
f.write(str(buff_w))
f.close()

f = open(fname, "r", encoding="utf-8")
buff_r = f.read()
f.close()

# print readed buffer
print(buff_r[:25] + "...")

