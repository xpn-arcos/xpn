#!/bin/sh
set -x

DIR="/export/home/pato11-1/proyectos/xpn/expand-2.0/test/pruebas/"
mpirun -np $4 -machinefile $DIR/machines  -nolocal $DIR/$1 $2 $3 >>  $DIR/resultados/$1.$4.$5.txt
