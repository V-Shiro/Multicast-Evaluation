// https://github.com/dmichael/go-multicast/blob/master/multicast/listener.go
// https://pkg.go.dev/net#UDPConn
// https://pkg.go.dev/net
// guide: https://pkg.go.dev/golang.org/x/net/ipv4

// udp = IPv4 and IPv6, udp4 = only IPv4, udp6 = only IPv6 //https://pkg.go.dev/net#Dial
package GoASM

import (
	"fmt"
	"log"
	"net"
)

// Listen binds to the UDP address and port given and writes packets received
// from that address to a buffer which is passed to a hander
func main(handler func(*net.UDPAddr, int, []byte)) {
	// Parse the string address
	var address string = "232.0.0.0:1900"
	//var address string = "ff05::c:1900"

	addr, err := net.ResolveUDPAddr("udp", address)
	if err != nil {
		log.Fatal(err)
	}

	// Open up a connection (create and bind socket)
	conn, err := net.ListenMulticastUDP("udp", nil, addr)
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()

	const maxDatagramSize = 8192
	conn.SetReadBuffer(1024)

	// receiving
	for {
		buffer := make([]byte, maxDatagramSize)
		numBytes, src, err := conn.ReadFromUDP(buffer)
		if err != nil {
			log.Fatal("ReadFromUDP failed:", err)
		}

		handler(src, numBytes, buffer)
		fmt.Printf("Source:")
	}
}
