# https://github.com/jpignata/blog/blob/master/articles/multicast-in-ruby.md
# https://rubyapi.org/o/socket
# https://www.rubydoc.info/stdlib/socket/Socket
# https://docs.ruby-lang.org/en/3.3/Socket.html

require 'socket'
require 'ipaddr'

MULTICAST_ADDR = "232.0.0.0" 
# MULTICAST_ADDR = "ff05::c" 

PORT = 1900

# create UDP socket for multicast
s = UDPSocket.new

# reuse address
s.setsockopt( :SOL_SOCKET,  :SO_REUSEADDR, true)
# s.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEADDR, true)

# bind to port
s.bind("0.0.0.0", PORT)

# ASM
mcast_opt =  IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new("0.0.0.0").hton
s.setsockopt( :IPPROTO_IP,  :IP_ADD_MEMBERSHIP, mcast_opt)
# s.setsockopt(Socket::IPPROTO_IP, Socket::IP_ADD_MEMBERSHIP, mcast_opt)

# receive
loop do
  msg, info = s.recvfrom(1024)
  puts msg
end
s.setsockopt( :IPPROTO_IP,  :IP_DROP_MEMBERSHIP, mcast_opt)
# s.setsockopt(Socket::IPPROTO_IP, Socket::IP_DROP_MEMBERSHIP, mcast_opt)
s.close