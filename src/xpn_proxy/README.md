
# The Expand Proxy (xpn_proxy)


## Execute

As example of how to execute XPN-PROXY, please use the following steps:

  1.  Execute the **Expand server** first:
     ``
   XPN_SCK_IPV=4 ./src/xpn_server/xpn_server -w /tmp/work/data -s sck -t 1 -i 4
   ``
   
  2.  Then, execute the **Expand proxy server** with the Expand configuration file to be used (*XPN_CONF=...*):
      ``
   XPN_CONF=$HOME/xpn_sck.conf \
   XPN_MQTT=0 XPN_LOCALITY=0 XPN_CONNECTED=1 XPN_SESSION_FILE=1 XPN_SCK_IPV=4 ./xpn_proxy_server
   ``
   
  3.  Finally, execute the **Expand proxy client**:
     ``
   XPN_PROXY_ADDR=127.0.0.1:5555     ./proxy-open-write-close /P1/test.txt  12
   ``


