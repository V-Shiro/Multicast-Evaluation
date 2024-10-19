// https://pkg.go.dev/golang.org/x/net/ipv6

package main

import (
	"fmt"
	"net"

	"golang.org/x/net/ipv6"
)

func main() {

	// multicast group
	group := net.UDPAddr{IP: net.ParseIP("ff05::c")}
	source := net.UDPAddr{IP: net.ParseIP("192.168.56.1")}

	c, err := net.ListenPacket("udp6", "[::]:1900")
	if err != nil {
		fmt.Println("Listen Error: ", err)
		return
	}
	defer c.Close()

	p := ipv6.NewPacketConn(c)

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
