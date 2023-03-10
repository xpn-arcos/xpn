
# tcp_server for XPN


## Compile:

* To compile the tcp_server just execute:
```
make clean
make
```

## Execution:

* In order to start one tcp_server at host "localhost" and port "555", you can use:
```
# cleanup
rm -fr   /tmp/xpn_data/
mkdir -p /tmp/xpn_data/
rm -fr   /tmp/xpn_conf/
mkdir -p /tmp/xpn_conf/
touch    /tmp/xpn_conf/xpn.dns
touch    /tmp/xpn_conf/machinefile
touch    /tmp/xpn_conf/xpn.conf.xml

# build xpn.conf.xml
echo localhost > /tmp/xpn_conf/machinefile

$PREFIX/scripts/execute/mk_conf.sh --conf /tmp/xpn_conf/xpn.conf.xml \
                                   --machinefile /tmp/xpn_conf/machinefile \
                                   --part_size 512k \
                                   --part_name xpn \
                                   --storage_path /tmp/xpn_data

# setup the environment variables
export XPN_PROFILE=${PREFIX}/doc/xpn.profile.example.xml
export XPN_CONF=/tmp/xpn_conf/xpn.conf.xml

# run the tcp_server...
${PREFIX}/src/tcp_server/xpn_tcp_server -n localhost -p 555 -f /tmp/xpn_conf/xpn.dns
```

