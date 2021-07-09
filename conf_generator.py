#!/usr/bin/python

import sys
import xml.etree.ElementTree as ET

from lockfile import LockFile
lock = LockFile("/work/xpn.conf.xml")
with lock:
  # parsing directly.
  tree = ET.parse('/work/xpn.conf.xml')
  root = tree.getroot()
    
  # partition attributes
  root[0].set('name','xpn1')
  root[0].set('type','NORMAL')
  root[0].set('bsize','64k')
    
  length = len(root[0])
    
  # data nodes creation
  ET.SubElement(root[0], 'data_node')
  root[0][length].set('id', sys.argv[2])
  root[0][length].set('url','nfs3://'+sys.argv[1]+'/nfs/'+sys.argv[2]+'/')
   
  tree.write('/work/xpn.conf.xml')