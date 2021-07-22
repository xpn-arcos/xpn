export XPN_CONF=${HOME}/conf/xpn.conf.xml
export XPN_PROFILE=${HOME}/conf/xpn.profile.xml
export MYSERVER_FILE=${HOME}/conf/xpn.dns
export MYSERVER_HOME=${HOME}/expand/external-utils/myServer_1.0.beta

#parametro:
#id del nodo

I=$1

${MYSERVER_HOME}/expand/exter/myServer.exe -n node$I -p 9999 -f ${MYSERVER_FILE}
