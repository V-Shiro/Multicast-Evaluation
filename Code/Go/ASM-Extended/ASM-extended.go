// https://pkg.go.dev/golang.org/x/net/ipv4

package main

import (
	"fmt"
	"net"

	"golang.org/x/net/ipv4"
)

func main() {

	en0, err := net.InterfaceByName("en0")
	if err != nil {
		fmt.Println("Interface Error: ", err)
	}
	eth0, err := net.InterfaceByName("eth0")
	if err != nil {
		fmt.Println("Interface Error: ", err)
	}
	group := net.IPv4(232, 0, 0, 0)

	c, err := net.ListenPacket("udp4", "232.0.0.0:1900")
	if err != nil {
		fmt.Println("Listen Error: ", err)
		return
	}
	defer c.Close()

	p := ipv4.NewPacketConn(c)
	if err := p.JoinGroup(en0, &net.UDPAddr{IP: group}); err != nil {
		fmt.Println("JoinGroup Error: ", err)
	}
	if err := p.JoinGroup(eth0, &net.UDPAddr{IP: group}); err != nil {
		fmt.Println("JoinGroup Error: ", err)
	}

	buffer := make([]byte, 1500)
	for {
		n, _, src, err := p.ReadFrom(buffer)
		if err != nil {
			fmt.Println("Read Error: ", err)
			return
		}
		fmt.Println("Received from ", src, string(buffer[:n]))
	}

	if err := p.LeaveGroup(en0, &net.UDPAddr{IP: group}); err != nil {
		fmt.Println("LeaveGroup Error: ", err)
	}
}
