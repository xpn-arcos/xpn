
 tcp_server
 ----------

## Compile:
   * To compile tcp_server:
```
make clean
make
```

## Execution:

   * To start one tcp_server:
```
export XPN_CONF=${HOME}/conf/xpn.conf.xml
export XPN_PROFILE=${HOME}/conf/xpn.profile.xml
export MYSERVER_FILE=${HOME}/conf/xpn.dns
export MYSERVER_HOME=${HOME}/expand/src_servers/tcp_server

# argument: node id.
I=$1

${MYSERVER_HOME}/tcp_server.exe -n node$I -p 9999 -f ${MYSERVER_FILE}
```

