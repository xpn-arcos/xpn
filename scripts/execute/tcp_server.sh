#!/bin/bash
#set -x

# Header
echo ""
echo " tcp_server.sh"
echo " -------------"
echo ""

# Default configuration
XPN_DIR=$( dirname -- "$0"; )/../../
BASE_DIR=/tmp/xpn
MACHINE_FILE=machinefile
PORT_NUMBER=555

# Check arguments
if [ "$#" != 3 ]; then
    echo ""
    echo " Usage:"
    echo " $0 <base dir> <machine file> <port number>"
    echo " Where:"
    echo " * base dir     = base directory for data and configuration"
    echo " * machine file = list of machines, one per line"
    echo " * port number  = TCP port number"
    echo ""
    exit
fi

# Do request
echo " Begin."

# Configuration
BASE_DIR=$1
MACHINE_FILE=$2
PORT_NUMBER=$3

# cleanup
rm -fr   $BASE_DIR/data/
rm -fr   $BASE_DIR/conf/

# build skeleton
mkdir -p $BASE_DIR/data/
mkdir -p $BASE_DIR/conf/

# (1/3) build machine file...
touch    $BASE_DIR/conf/machinefile
if [ ! -f $MACHINE_FILE ]; then
    hostname > $BASE_DIR/conf/machinefile
else
    cp $MACHINE_FILE $BASE_DIR/conf/machinefile
fi

# (2/3) build xpn.conf.xml file...
touch ${BASE_DIR}/conf/xpn.conf.xml
${XPN_DIR}/scripts/execute/mk_conf.sh --conf        ${BASE_DIR}/conf/xpn.conf.xml \
                                      --machinefile ${BASE_DIR}/conf/machinefile \
                                      --part_size    512k \
                                      --part_name    xpn \
                                      --storage_path ${BASE_DIR}/data

# (3/3) build empty xpn.dns file...
touch    ${BASE_DIR}/conf/xpn.dns

# setup the environment variables
export XPN_PROFILE=${XPN_DIR}/doc/xpn.profile.example.xml
export XPN_CONF=${BASE_DIR}/conf/xpn.conf.xml

# run the tcp_server...
${XPN_DIR}/src/tcp_server/xpn_tcp_server -n localhost -p ${PORT_NUMBER} -ns ${BASE_DIR}/conf/xpn.dns

# Stop
echo " End."

