/*How It Works:

    Multicast Group & Port: The multicast group IP and port are defined as variables at the beginning of the program.
    UDP Address Resolution: The program resolves the multicast address and port into a UDPAddr.
    Multicast Listening: The program uses net.ListenMulticastUDP to start listening for messages on the specified address.
    Receiving Messages: It continuously listens for incoming messages and prints them to the console.

Running the Program:

    Save the code to a .go file, for example multicast_receiver.go.
    Run the program using the Go command:

    bash

go run multicast_receiver.go
*/

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

	fmt.Printf("Listening for multicast messages on %s:%s...\n", multicastGroupIP, multicastPort)

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
