
from ctypes import *


#
# Main
#

# fill buffer with 1024 'a'
buff_w = []
for i in range(0,1024):
    buff_w.append('a')
buff_w.append('\n')

# write and read buffer
fname = "/tmp/expand/P1/demo.txt"

f = open(fname, "w")
f.write(str(buff_w))
f.close()

f = open(fname, "r")
buff_r = f.read()
f.close()

# print readed buffer
print(buff_r[:25] + "...")

