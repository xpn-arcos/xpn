# Expand
Expand: XPN Parallel File System

*Licence*: GNU GENERAL PUBLIC LICENSE Version 3</br>
*Authors*: Felix Garcia Carballeira, Luis Miguel Sanchez Garcia, Borja Bergua Guerra, Alejandro Calderon Mateos, Diego Camarmas Alonso, David Garcia Fernandez


## 1. Source structure

The Expand client source code (src/xpn_client) has four main levels:
* xpni:
  eXPaNd Interface provides many enhanced functionality like Fault-Tolerance support, locking, etc.
* xpn:
  eXPaNd implements a Parallel File System using NFI interfaces.
* nfi:
  Network File Interface provides access to several storage protocols (local and network protocols) like NFS, FTP, etc.
* base:
  Base interface provides independent access to operating system resources. 
  Portability is provided in this layer.

