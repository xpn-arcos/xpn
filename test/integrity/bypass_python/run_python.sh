#!/bin/bash
#set -x


# Option 2: preload in python with decorator
echo "### DECORATOR ##################################"

echo "action:"
gcc -fPIC -shared -o py_xpn.so py_xpn.c
echo "python3 ./main_decorator.py /tmp/expand/P1/demo.txt"

echo "output:"
      python3 ./main_decorator.py /tmp/expand/P1/demo.txt


# Clean
echo "################################################"
rm -fr py_xpn.so
rm -fr /tmp/demo.txt

