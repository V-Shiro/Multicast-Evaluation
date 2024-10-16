#Source: https://stackoverflow.com/questions/76773374/when-subscribing-to-a-multicast-group-what-is-the-best-way-to-filter-traffic-fr
#Source: https://docs.python.org/3/library/socket.html
#Source: https://stackoverflow.com/questions/16419794/what-is-4sl-format-in-struct-pack-python

# No SSM Support
#Source: https://github.com/python/cpython/issues/89415
#Source: https://github.com/python/cpython/pull/103684

# IS NOT SUPPORTED YET

import socket
import struct
import sys

# For Python >= 3 "print(...)"
# For Python < 3 "print ..."

MCAST_GRP = '232.0.0.0'
MCAST_PORT = 1900
SENDER_IP = '192.168.56.1' 

# create UDP socket for multicast
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

# Allow multiple sockets to use the same port number.
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# listen to MCAST_GRP
s.bind(('', MCAST_PORT))

# SSM 
mreq = struct.pack("4sl4s", socket.inet_aton(MCAST_GRP), socket.INADDR_ANY, socket.inet_aton(SENDER_IP))
s.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_SOURCE_MEMBERSHIP, mreq)

while True:
  data = s.recv(10240)
  message = data.decode('utf-8')
  print(message)

s.close()