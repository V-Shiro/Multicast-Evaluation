// https://pkg.go.dev/golang.org/x/net/ipv4

package main

import (
	"fmt"
	"net"

	"golang.org/x/net/ipv4"
)

func main() {

	// multicast group
	//group := net.IPv4(232, 0, 0, 0)
	group := net.UDPAddr{IP: net.IPv4(232, 0, 0, 0)}
	source := net.UDPAddr{IP: net.IPv4(192, 168, 0, 1)}

	c, err := net.ListenPacket("udp4", "0.0.0.0:1900")
	if err != nil {
		fmt.Println("Listen Error: ", err)
		return
	}
	defer c.Close()

	p := ipv4.NewPacketConn(c)

	if err := p.JoinSourceSpecificGroup(nil, &group, &source); err != nil {
		fmt.Println("JoinGroup Error: ", err)
	}

	buffer := make([]byte, 1500)
	for i := 0; i < 3; i++ { //for {
		n, _, src, err := p.ReadFrom(buffer)
		if err != nil {
			fmt.Println("Read Error: ", err)
			return
		}
		fmt.Println("Received from ", src, string(buffer[:n]))
	}

	if err := p.LeaveSourceSpecificGroup(nil, &group, &source); err != nil {
		fmt.Println("LeaveGroup Error: ", err)
	}
}
