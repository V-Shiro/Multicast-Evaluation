// https://github.com/dmichael/go-multicast/blob/master/multicast/listener.go
// https://pkg.go.dev/net#UDPConn
// https://pkg.go.dev/net
// guide: https://pkg.go.dev/golang.org/x/net/ipv4

// udp = IPv4 and IPv6, udp4 = only IPv4, udp6 = only IPv6 //https://pkg.go.dev/net#Dial
package main

import (
	"fmt"
	"log"
	"net"
)

// Listen binds to the UDP address and port given and writes packets received
// from that address to a buffer which is passed to a hander
func main() {
	// Parse the string address
	var address string = "[ff05::c]:1900"

	addr, err := net.ResolveUDPAddr("udp6", address)
	if err != nil {
		log.Fatal(err)
	}

	// Open up a connection (create and bind socket)
	conn, err := net.ListenMulticastUDP("udp6", nil, addr)
	if err != nil {
		log.Fatal(err)
	}

	/* if err := conn.SetReuseAddr(true); err != nil {
		log.Fatal(err)
	}
	if err := conn.SetControlMessage(net.FlagInterface, true); err != nil {
		log.Fatal(err)
	} */

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
		message := bytesToString(buffer)
		fmt.Println(numBytes)
		fmt.Println()
		fmt.Println(src)
	}
}
