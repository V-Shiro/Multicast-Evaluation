// https://github.com/jeroendk/go-tcp-udp/blob/master/udpclient/main.go

package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	// Define the multicast group IP address and port.
	multicastGroupIP := "239.0.0.222" // Replace with your multicast group IP
	multicastPort := "12345"          // Replace with your multicast port

	// Resolve the UDP address.
	addr, err := net.ResolveUDPAddr("udp", net.JoinHostPort(multicastGroupIP, multicastPort))
	if err != nil {
		fmt.Println("Failed to resolve UDP address:", err)
		os.Exit(1)
	}

	// Create a UDP connection for listening.
	conn, err := net.ListenMulticastUDP("udp", nil, addr)
	if err != nil {
		fmt.Println("Failed to listen on multicast address:", err)
		os.Exit(1)
	}
	defer conn.Close()

	// Set the buffer size for receiving messages.
	conn.SetReadBuffer(1024)

	// Start receiving messages in a loop.
	buffer := make([]byte, 1024)
	for {
		// Receive the message.
		n, src, err := conn.ReadFromUDP(buffer)
		if err != nil {
			fmt.Println("Failed to receive message:", err)
			continue
		}

		// Print the received message and the sender's address.
		fmt.Printf("Received message from %s: %s\n", src, string(buffer[:n]))
	}
}
