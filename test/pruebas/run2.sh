#!/bin/sh
set -x

DIR="/export/home/pato11-1/proyectos/xpn/expand-2.0/test/pruebas/"
LBUF="2097152 1048576 524288 262144 131072 65536 32768 16384 8192 4096 2048 1024 512 256 128"
TYPE="tcp tcp.hilos"

PATOS="1 2 4 8 16"
SIZE="64 16 8 1"

for X in $TYPE; do
       for Y in $SIZE; do
		cp $DIR/conf/xpn.$Y.$X.conf $DIR/conf/xpn.conf
		$DIR/delete.sh

		for I in $PATOS; do
			for J in $LBUF; do
				$DIR/sync.sh
				$DIR/lanza.sh IONW /PNFS $J $I xpn.$Y.$X.conf
				#sleep 5
			done
			#sleep 10
			for J in $LBUF; do
				$DIR/sync.sh
				$DIR/lanza.sh IONR /PNFS $J $I xpn.$Y.$X.conf
				#sleep 5
			done
			$DIR/delete.sh
		done
	done
done
