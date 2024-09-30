#Source: https://stackoverflow.com/questions/603852/how-do-you-udp-multicast-in-python
#Source: https://docs.python.org/3/library/socket.html
#Source: https://stackoverflow.com/questions/16419794/what-is-4sl-format-in-struct-pack-python

# OS: works with windows, linux, mac

import socket
import struct
import sys

# For Python >= 3 "print(...)"
# For Python < 3 "print ..."

MCAST_GRP = '224.1.1.1'
MCAST_PORT = 5007

# create UDP socket for multicast
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

# Allow multiple sockets to use the same port number.
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# listen to MCAST_GRP
s.bind((MCAST_GRP, MCAST_PORT))

# ASM
mreq = struct.pack("4sl", socket.inet_aton(MCAST_GRP), socket.INADDR_ANY)
s.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

while True:
  data = s.recv(10240)
  message = data.decode('utf-8')
  print(message)

s.setsockopt(socket.IPPROTO_IP, socket.IP_DROP_MEMBERSHIP, mreq)

s.close()