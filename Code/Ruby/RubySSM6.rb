# https://github.com/jpignata/blog/blob/master/articles/multicast-in-ruby.md
# https://rubyapi.org/o/socket
# https://www.rubydoc.info/stdlib/socket/Socket

require 'socket'
require 'ipaddr'

MULTICAST_ADDR = "ff05::c" 
PORT = 1900
SOURCE_ADDR = "192.168.1.100"

# Create a UDP socket
socket = UDPSocket.new( :AF_INET6)

# reuse address
socket.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEADDR, true)

# bind
socket.bind("::", PORT)


# SSM
mcast_opt =  IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new("::").hton + IPAddr.new(SOURCE_ADDR).hton
socket.setsockopt(Socket::IPPROTO_IPV6, Socket::IPV6_JOIN_GROUP, mcast_opt)

# receive and check for source
loop do
    msg, info = socket.recvfrom(1024)
    puts msg
end

socket.setsockopt(Socket::IPPROTO_IPV6, Socket::IPV6_LEAVE_GROUP, mcast_opt)
socket.close
