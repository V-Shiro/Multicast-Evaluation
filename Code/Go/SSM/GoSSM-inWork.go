// not supported

package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	// Multicast group address with port and source address with port
	multicastAddr := "233.252.0.1:9999"
	sourceAddr := "192.168.1.100"

	// Resolve the multicast address
	udpAddr, err := net.ResolveUDPAddr("udp", multicastAddr)
	if err != nil {
		fmt.Println("Error resolving multicast address:", err)
		os.Exit(1)
	}

	// Create a UDP connection
	conn, err := net.ListenMulticastUDP("udp", nil, udpAddr)
	if err != nil {
		fmt.Println("Error creating UDP connection:", err)
		os.Exit(1)
	}
	defer conn.Close()

	// Join the multicast group from a specific source
	if err := conn.SetControlMessage(net.Flags(1)); err != nil {
		fmt.Println("Error setting control message:", err)
		os.Exit(1)
	}

	for {
		// Read from the connection
		//n, cm, srcAddr, err := conn.ReadMsgUDP(buf, nil)
		buffer := make([]byte, 1024)
		n, srcAddr, err := conn.ReadFromUDP(buffer)
		if err != nil {
			fmt.Println("Error reading message:", err)
			continue
		}

		// Check if the source address matches
		if srcAddr.IP.String() == sourceAddr {
			fmt.Printf("Received %d bytes from %s: %s\n", n, srcAddr.String(), string(buffer[:n]))
		}
	}
}
