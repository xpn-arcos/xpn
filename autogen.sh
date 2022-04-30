#!/bin/bash
set -x

#libtoolize
#aclocal
#autoconf
#autoheader
#automake --add-missing

ACLOCAL_FLAGS="-I /usr/share/aclocal/" autoreconf -v -i -s -W all -m

