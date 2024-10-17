#https://stackoverflow.com/questions/603852/how-do-you-udp-multicast-in-python
#https://docs.python.org/3/library/socket.html
#https://stackoverflow.com/questions/16419794/what-is-4sl-format-in-struct-pack-python

# OS: works with windows, linux, mac

import socket
import struct
import sys

# For Python >= 3 "print(...)"
# For Python < 3 "print ..."

MCAST_GRP = '232.0.0.0'
MCAST_PORT = 1900

# create UDP socket for multicast
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

# Allow multiple sockets to use the same port number.
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# listen to MCAST_GRP
s.bind(('', MCAST_PORT))

# ASM
mreq = struct.pack("4sl", socket.inet_aton(MCAST_GRP), socket.INADDR_ANY)
s.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

# receive
while True:
  data = s.recv(1024)
  message = data.decode('utf-8')
  print(message)

# drop membership
s.setsockopt(socket.IPPROTO_IP, socket.IP_DROP_MEMBERSHIP, mreq)

# close socket
s.close()