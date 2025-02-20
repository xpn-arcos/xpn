#!/bin/bash
set -x

XPN_CONF=./test/integrity/xpn-iot/conf/xpn.conf XPN_LOCALITY=0 XPN_SESSION=0 XPN_MQTT=1 XPN_QOS_MQTT=0 ./test/integrity/xpn-iot/iot-xpn ~/elias-circs/bif6

