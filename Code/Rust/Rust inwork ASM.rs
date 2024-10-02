// https://doc.rust-lang.org/std/net/index.html
// https://bluejekyll.github.io/blog/posts/multicasting-in-rust/
// https://doc.rust-lang.org/std/net/struct.UdpSocket.html#
use std::net::{SocketAddr, UdpSocket};
use std::str;

fn main() -> std::io::Result<()> {
    // Multicast address and port
    let multicast_addr = "224.0.0.1:8080";
    let port = "8080";
    
    // Create a UDP socket
    let socket = UdpSocket::bind("0.0.0.0:8080")?;

    // add multicast membership
    socket.join_multicast_v4(&multicast_addr.parse::<SocketAddr>().unwrap().ip().into(), &"0.0.0.0".parse().unwrap())?;

    let mut buf = [0; 1024];
    loop {
        let (len, src) = socket.recv_from(&mut buf)?;
        let msg = str::from_utf8(&buf[..len]).expect("Failed to convert message to UTF-8");
        println!("Received {} bytes from {}: {}", len, src, msg);
    }
}
