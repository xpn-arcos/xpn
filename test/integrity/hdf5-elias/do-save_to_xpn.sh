#!/bin/bash
set -x

# set configuration variables
WORK_DIR=$HOME/
NODE_DIR=/tmp
INSTALL_PATH=$HOME/bin/xpn
SRC_PATH=$HOME/work/xpn/

# (1) clear xpn local directory
#rm -fr   /tmp/work/data
mkdir -p /tmp/work/data
#rm -fr   /tmp/expand/P1
mkdir -p /tmp/expand/P1

echo "localhost" > $WORK_DIR/hostfile

# (2) start expand server
$SRC_PATH/xpn -v \
	      -e sck \
              -w $WORK_DIR -x $NODE_DIR \
              -n 1 \
              -l $WORK_DIR/hostfile start
sleep 2

# (3) launch app with expand client
LD_PRELOAD=$INSTALL_PATH/lib64/xpn_bypass.so:$LD_PRELOAD \
XPN_CONF=./xpn_sck.conf \
XPN_LOCALITY=0 XPN_CONNECTED=1 XPN_SESSION_FILE=1 \
strace -f ./hdf5_test   /tmp/expand/P1/test.h5

# (4) stop expand server
$SRC_PATH/xpn -v -e sck -d $WORK_DIR/hostfile stop
sleep 2
rm -fr $WORK_DIR/hostfile

