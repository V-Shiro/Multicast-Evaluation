use std::net::{UdpSocket, Ipv4Addr, SocketAddrV4};
use std::str;

fn main() -> std::io::Result<()> {
    let multicast_group = "232.0.0.0";
    let port = 1900;
    
    // Bind to the specified port on the local machine
    let socket = UdpSocket::bind(("0.0.0.0", port))?;
    
    // Join the multicast group
    let multicast_addr = multicast_group.parse::<Ipv4Addr>().expect("Invalid multicast address");
    let interface_addr = Ipv4Addr::new(0, 0, 0, 0); // 0.0.0.0 means any interface
    socket.join_multicast_v4(&multicast_addr, &interface_addr)?;
    
    println!("Listening for multicast messages on {}:{}", multicast_group, port);
    
    let mut buf = [0u8; 1500];
    loop {
        match socket.recv_from(&mut buf) {
            Ok((num_bytes, src_addr)) => {
                let msg = str::from_utf8(&buf[..num_bytes]).expect("Failed to convert to string");
                println!("Received {} bytes from {}: {}", num_bytes, src_addr, msg);
            }
            Err(e) => {
                eprintln!("Error receiving data: {}", e);
                break;
            }
        }
    }

    Ok(())
}
