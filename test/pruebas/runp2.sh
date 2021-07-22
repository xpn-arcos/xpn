#!/bin/sh
set -x

DIR="/export/home/pato11-1/proyectos/xpn/expand-2.0/test/pruebas/"
LBUF="2097152 1048576 524288 262144 131072 65536 32768 16384 8192 4096 2048 1024 512 256 128"
#TYPE="file file.hilos udp udp.hilos tcp tcp.hilos"
PATOS="16 8 4 2 1"

$DIR/delete.sh
$DIR/umountdirs.sh
$DIR/mountdirsudp.sh
for I in $PATOS; do
	for J in $LBUF; do
		$DIR/sync.sh
		$DIR/lanza1.sh IOPsW /export/temp/pato23/ $J $I udp
		#sleep 5
	done
	
	for J in $LBUF; do
		$DIR/sync.sh
		$DIR/lanza1.sh IOPsR /export/temp/pato23/ $J $I udp
		#sleep 5
	done

	#for J in $LBUF; do
	#	$DIR/sync.sh
	#	$DIR/lanza1.sh IOPsRe /export/temp/pato23/ $J $I udp
		#sleep 5
	#done
	$DIR/delete.sh
done

$DIR/delete.sh
$DIR/umountdirs.sh
$DIR/mountdirstcp.sh
for I in $PATOS; do
	for J in $LBUF; do
		$DIR/sync.sh
		$DIR/lanza1.sh IOPsW /export/temp/pato23/ $J $I tcp
		#sleep 5
	done
	
	for J in $LBUF; do
		$DIR/sync.sh
		$DIR/lanza1.sh IOPsR /export/temp/pato23/ $J $I tcp
		#sleep 5
	done

	#for J in $LBUF; do
	#	$DIR/sync.sh
	#	$DIR/lanza1.sh IOPsRe /export/temp/pato23/ $J $I tcp
		#sleep 5
	#done
	$DIR/delete.sh
done
