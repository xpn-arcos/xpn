#!/bin/bash
set -x

ACLOCAL_FLAGS="-I /usr/share/aclocal/" autoreconf -v -i -s -W all

