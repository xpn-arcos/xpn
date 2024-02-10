#!/bin/bash
#set -x

#
#  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos
#
#  This file is part of Expand.
#
#  Expand is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Expand is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
#


# Header
echo ""
echo " sck_server.sh"
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
rm -fr   "${BASE_DIR}/data/"
rm -fr   "${BASE_DIR}/conf/"

# build skeleton
mkdir -p "${BASE_DIR}/data/"
mkdir -p "${BASE_DIR}/conf/"

# (1/3) build machine file...
touch    "${BASE_DIR}/conf/machinefile"
if [ ! -f "$MACHINE_FILE" ]; then
    hostname > "${BASE_DIR}/conf/machinefile"
else
    cp "${MACHINE_FILE}" "${BASE_DIR}/conf/machinefile"
fi

# (2/3) build xpn.conf.xml file...
touch "${BASE_DIR}/conf/xpn.conf.xml"
"${XPN_DIR}"/scripts/execute/mk_conf.sh --conf        "${BASE_DIR}/conf/xpn.conf.xml" \
                                      --machinefile "${BASE_DIR}/conf/machinefile" \
                                      --part_size    512k \
                                      --part_name    xpn \
                                      --storage_path "${BASE_DIR}/data"

# (3/3) build empty xpn.dns file...
touch    "${BASE_DIR}/conf/xpn.dns"

# setup the environment variables
export XPN_PROFILE="${XPN_DIR}/doc/xpn.profile.example.xml"
export XPN_CONF="${BASE_DIR}/conf/xpn.conf.xml"

# run the sck_server...
"${XPN_DIR}"/src/sck_server/xpn_sck_server -n localhost -p "${PORT_NUMBER}" -ns "${BASE_DIR}/conf/xpn.dns"

# Stop
echo " End."
