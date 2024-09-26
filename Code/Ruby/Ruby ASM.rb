# https://github.com/jpignata/blog/blob/master/articles/multicast-in-ruby.md
# https://rubyapi.org/o/socket
# https://www.rubydoc.info/stdlib/socket/Socket

require 'socket'
require 'ipaddr'

MULTICAST_ADDR = "225.4.5.6" 
PORT = 5000

# create UDP socket for multicast
s = UDPSocket.new
#s = Socket.new Socket::AF_INET, Socket::SOCK_STREAM

# reuse port
s.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEPORT, true)

# bind to port
s.bind("", PORT)

# ASM
mcast_opt =  IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new("0.0.0.0").hton
s.setsockopt(Socket::IPPROTO_IP, Socket::IP_ADD_MEMBERSHIP, mcast_opt)

# receive
loop do
  msg, info = s.recvfrom(1024)
  puts msg
end
s.close