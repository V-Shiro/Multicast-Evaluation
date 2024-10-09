#Source: https://stackoverflow.com/questions/603852/how-do-you-udp-multicast-in-python
#Source: https://docs.python.org/3/library/socket.html
#Source: https://stackoverflow.com/questions/16419794/what-is-4sl-format-in-struct-pack-python


# OS: works with windows and linux
# IP: works with IP4 IP6

import socket
import struct
import sys

# For Python >= 3 "print(...)"
# For Python < 3 "print ..."

MCAST_GRP = 'ff05::c'
MCAST_PORT = 1900

# create UDP socket for multicast IPv6
s = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

# Allow multiple sockets to use the same port number.
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# listen to MCAST_GRP
s.bind((MCAST_GRP, MCAST_PORT))

# ASM for IPv6
mreq = struct.pack("16sl", socket.inet_aton(MCAST_GRP), socket.INADDR_ANY)
s.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_JOIN_GROUP, mreq)

while True:
  data = s.recv(10240)
  message = data.decode('utf-8')
  print(message)

s.setsockopt(socket.IPPROTO_IPV6, socket.IP_DROP_MEMBERSHIP, mreq)

s.close()