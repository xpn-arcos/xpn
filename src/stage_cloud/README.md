sudo apt-get install python3-boto3 python3-paramiko

S3_CONFIG_FILE=<path_to_configs3.json>/configS3.json \
XPN_CONF=<path_to_xpn.conf>/xpn_sck.conf XPN_LOCALITY=0 XPN_SESSION_FILE=1 XPN_MQTT=0 XPN_CONNECTED=1 \
LD_PRELOAD=<path_to_xpn>/xpn/src/bypass/xpn_bypass.so:$LD_PRELOAD \
python3 launcher_S3.py upload /tmp/expand/P1/kk.txt testS3.txt 8 ./machinefile


S3_CONFIG_FILE=<path_to_configs3.json>/configS3.json \
XPN_CONF=<path_to_xpn.conf>/xpn_sck.conf XPN_LOCALITY=0 XPN_SESSION_FILE=1 XPN_MQTT=0 XPN_CONNECTED=1 \
LD_PRELOAD=<path_to_xpn>/xpn/src/bypass/xpn_bypass.so:$LD_PRELOAD \
python3 launcher_S3.py download testS3.txt /tmp/expand/P1/kk2.txt 8 ./machinefile