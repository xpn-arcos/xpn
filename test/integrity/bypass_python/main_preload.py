
from ctypes import *
import sys


# get filename
if (len(sys.argv) < 2):
    print("")
    print("Usage:")
    print(" * ./main_preload.py <file name>")
    print("")
    print("Examples:")
    print(" ./main_preload.py /tmp/expand/P1/demo.txt")
    print(" ./main_preload.py /tmp/demo.txt")
    print("")
    sys.exit()

fname = sys.argv[1]

# fill buffer with 1024 'a'
buff = []
for i in range(0,1024):
    buff.append('a')
buff.append('\n')
buff_w = ''.join(buff)

# write and read buffer
try:
    print("fname: " + fname)
    f = open(fname, "w", encoding="utf-8")
    #print("Name of the file: ", fo.name)
    #print("Closed or not : ",   fo.closed)
    #print("Opening mode : ",    fo.mode)
    f.write(str(buff_w))
    f.close()

    print("fname: " + fname)
    f = open(fname, "r", encoding="utf-8")
    buff_r = f.read()
    f.close()
except IOError as e:
    print(e)
    sys.exit()
except:
    print("open failed !")
    sys.exit()

# print readed buffer
print(buff_r[:25] + "...")

