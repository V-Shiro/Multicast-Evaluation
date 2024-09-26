# https://github.com/jpignata/blog/blob/master/articles/multicast-in-ruby.md
# https://rubyapi.org/o/socket
# https://www.rubydoc.info/stdlib/socket/Socket

# SSM not supported

require 'socket'
require 'ipaddr'

MULTICAST_ADDR = "225.4.5.6" 
PORT = 5000
SOURCE_ADDR = "192.168.1.100"

# Create a UDP socket
socket = UDPSocket.new

# reuse port
socket.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEPORT, true)

# bind
socket.bind(Socket::INADDR_ANY, PORT)


# SSM (not supported)
mcast_opt =  IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new(INADDR_ANY).hton + IPAddr.new(SOURCE_ADDR).hton
socket.setsockopt(Socket::IPPROTO_IP, Socket::IP_ADD_SOURCE_MEMBERSHIP, mcast_opt)

# receive and check for source
loop do
    msg, info = socket.recvfrom(1024)
  if info[3] == SOURCE_ADDR # Check if the message is from the specified source
    puts msg
  else
    puts "Received message from a different source: #{info[3]}, ignoring..."
  end
end
socket.close
