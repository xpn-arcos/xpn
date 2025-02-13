#!/bin/bash
#set -x

echo "mosquis!..."
mosquitto  -c iot-conf/mq.conf

echo "xpn local directories..."
mkdir -p /tmp/work/data

echo "xpn_mq_server..."
../../../src/mq_server/xpn_mq_server  -d /tmp/work/data  -p 3456  -m 0  -ns iot-conf/mq_server.dns

sleep 3

echo "xpn_mq_client..."
XPN_CONF=./iot-conf/xpn.conf  XPN_DNS=./iot-conf/mq_server.dns  XPN_LOCALITY=0  XPN_SESSION=0  XPN_MQTT=1  XPN_QOS_MQTT=0  ./iot-xpn  Madrid-Atocha  ~/work/elias-circs/BIF\ -\ MAD116

