#!/bin/bash
#set -x

#
#  Copyright 2019-2021 Saul Alonso Monsalve, Felix Garcia Carballeira, Jose Rivadeneira Lopez-Bravo, Alejandro Calderon Mateos,
#
#  This file is part of U20 proyect.
#
#  U20 is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  U20 is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with U20.  If not, see <http://www.gnu.org/licenses/>.
#


# Start RPCbind
/usr/sbin/service rpcbind start
status=$?
if [ $status -ne 0 ]; then
     echo "Failed to start rpcbind: $status"
fi

echo "export LD_LIBRARY_PATH=/opt/mpich/lib:/work/new_bypass:$LD_LIBRARY_PATH" >> ~/.bashrc
echo "export XPN_CONF=/work/xpn.conf.xml" >> ~/.bashrc
source ~/.bashrc

# Start NFS
mkdir /nfs/$(hostname)
echo "/nfs/$(hostname) $(hostname -i)/16(rw,sync,no_root_squash,no_subtree_check,fsid=0)" >> /etc/exports
/usr/sbin/service nfs-kernel-server start
status=$?
if [ $status -ne 0 ]; then
     echo "Failed to start nfs: $status"
     exit $status
fi

# Create xpn.conf
python3 /work/conf_generator.py $(hostname -i) $(hostname)

# Start SSHD
/usr/sbin/sshd -D
status=$?
if [ $status -ne 0 ]; then
     echo "Failed to start sshd: $status"
     exit $status
fi