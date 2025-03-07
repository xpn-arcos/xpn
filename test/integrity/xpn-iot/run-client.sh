#!/bin/bash
set -x

XPN_CONF=./test/integrity/xpn-iot/conf/xpn_iot.conf XPN_LOCALITY=0 XPN_CONNECTED=1 XPN_SESSION_FILE=1 XPN_MQTT=1 XPN_QOS_MQTT=0 ./test/integrity/xpn-iot/iot-xpn  Origen-Destino  ~/user-circs/bif6

