// https://doc.rust-lang.org/std/net/index.html
// https://bluejekyll.github.io/blog/posts/multicasting-in-rust/
// https://doc.rust-lang.org/std/net/struct.UdpSocket.html

use std::net::{SocketAddr, UdpSocket};
use std::str;

fn main() -> std::io::Result<()> {
    // Multicast address 
    let multicastAddr = Ipv6Addr::new(0xff, 0, 0, 0, 0, 0, 0, 0xc);
    
    // Create a UDP socket
    let socket = UdpSocket::bind("[::]:1900")?;

    // add multicast membership
    let netInt = Ipv6Addr::new(0, 0, 0, 0, 0, 0, 0, 0);
    socket.join_multicast_v6(multicastAddr, netInt)?;

    let mut buf = [0; 1024];
    loop {
        let (len, src) = socket.recv_from(&mut buf)?;
        let msg = str::from_utf8(&buf[..len]).expect("Failed to convert message to UTF-8");
        println!("Received {} bytes from {}: {}", len, src, msg);
    }
    // drop membership
    socket.leave_multicast_v6(multicastAddr, netInt)?;
}
