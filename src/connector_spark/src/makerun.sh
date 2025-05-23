#!/bin/bash
set -x

export JAVA_INC=/usr/lib/jvm/java-11-openjdk-amd64/include
export XPN_INC=../../../include
export XPN_SRC=../../../src
export CLASSPATH=$CLASSPATH:./src/main/java
export MPI_PATH=/home/lab/bin/mpich
export XPN_PATH=/home/lab/bin/xpn
export MQTT_PATH=/home/lab/bin/mqtt

ar -x $XPN_PATH/lib/libxpn.a
gcc -shared $XPN_PATH/lib/*.o -o $XPN_PATH/lib/libxpn.so
rm $XPN_PATH/lib/*.o

javac -h . main/java/org/expand/jni/ExpandToPosix.java

cc -c -fPIC -I$JAVA_INC -I$JAVA_INC/linux -I$XPN_INC/xpn_client/ -I/usr/include/ -O2 -Wall -D_REENTRANT -DPOSIX_THREADS -DHAVE_CONFIG_H org_expand_jni_ExpandToPosix.c

cc -shared -o libexpandtoposix.so org_expand_jni_ExpandToPosix.o -L$XPN_SRC/base/ -L$XPN_SRC/xpn_client -L$MPI_PATH/lib -L$XPN_PATH/lib -L$MQTT_PATH/lib -lxpn -lmpi -lpthread -ldl -lmosquitto

rm *.o

