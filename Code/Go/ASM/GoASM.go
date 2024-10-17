// https://github.com/dmichael/go-multicast/blob/master/multicast/listener.go
// https://pkg.go.dev/net#UDPConn
// https://pkg.go.dev/net
// https://pkg.go.dev/golang.org/x/net/ipv4

// udp = IPv4 and IPv6, udp4 = only IPv4, udp6 = only IPv6 //https://pkg.go.dev/net#Dial
package main

import (
	"fmt"
	"log"
	"net"
)

func main() {
	// multicast address and port
	var address string = "232.0.0.0:1900"
	//var address string = "[ff05::c]:1900"

	// multicast address and port to UDP address
	addr, err := net.ResolveUDPAddr("udp", address)
	if err != nil {
		log.Fatal(err)
	}

	// Open up a connection (create and bind socket)
	conn, err := net.ListenMulticastUDP("udp", nil, addr)
	if err != nil {
		log.Fatal(err)
	}
	//clean up
	defer conn.Close()

	// receiving
	conn.SetReadBuffer(1024)
	for {
		buffer := make([]byte, 8192)
		numBytes, src, err := conn.ReadFromUDP(buffer)
		if err != nil {
			log.Fatal("ReadFromUDP failed:", err)
		}
		fmt.Println("source: ", src)
		fmt.Println("numBytes: ", numBytes)

		str1 := string(buffer[:])
		fmt.Println("Message: ", str1)
	}
}
