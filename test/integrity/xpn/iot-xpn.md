
# xpn_server with mosquitto XPN


## Pre-requisites:
* In order to use mosquitto on mq_server module, you have to install the following packages:
```
sudo apt-get install mosquitto mosquitto-clients mosquitto-dev libmosquitto-dev
```

* Then, we need to stop the Mosquitto MQTT service:
```
sudo systemctl stop mosquitto
```


## Compile:

* To compile the examples you just need to execute:
```
make clean
make
```


## Execution:

### Mosquitto
* Create the mosquitto configuration file:
```
touch mosquitto.conf
```

* Insert the following parameters:
```
max_queued_messages 0
memory_limit 0
allow_anonymous true
listener 1883
max_keepalive 0
max_connections -1
```

* Start the mosquitto server:
```
mosquitto  -c mosquitto.conf
```

### Expand server
* Create the DNS server file with one line per server IP:
```
echo localhost > server.dns
```

* Create the local server directory:
```
mkdir -p /tmp/work/data
```

* In order to start one xpn_server at port "7555", you can use:
```
../../../src/xpn_server/xpn_server -d /tmp/work/data  -p 3456  -m 0  -ns server.dns
```

### Expand client
* Create the Expand configuration file:
```
touch xpn.conf
```

* Insert the following parameters:
```
[partition]
bsize = 512k
replication_level = 0
partition_name = P1
server_url = mq_server://localhost/tmp/work/data
```

* In order to start the client, just execute:
```
XPN_CONF=xpn.conf  XPN_DNS=server.dns  XPN_LOCALITY=0  XPN_SESSION=0  XPN_MQTT=1  XPN_QOS_MQTT=0  ./iot-xpn  Provincia-Estacion circulaciones/circulacion
```


