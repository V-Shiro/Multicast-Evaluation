# https://github.com/jpignata/blog/blob/master/articles/multicast-in-ruby.md
# https://rubyapi.org/o/socket
# https://www.rubydoc.info/stdlib/socket/Socket
# https://docs.ruby-lang.org/en/3.3/Socket.html

require 'socket'
require 'ipaddr'

MULTICAST_ADDR = "225.0.0.1" 
PORT = 5000

# create UDP socket for multicast
s = UDPSocket.new

# reuse port
s.setsockopt( :SOL_SOCKET,  :SO_REUSEPORT, 1)

# bind to port
s.bind("0.0.0.0", PORT)

# ASM
mcast_opt =  IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new("0.0.0.0").hton
s.setsockopt( :IPPROTO_IP,  :IP_ADD_MEMBERSHIP, mcast_opt)

# receive
loop do
  msg, info = s.recvfrom(1024)
  puts msg
enda
s.setsockopt( :IPPROTO_IP,  :IP_DROP_MEMBERSHIP, mcast_opt)
s.close