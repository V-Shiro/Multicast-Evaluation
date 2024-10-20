// https://crates.io/crates/libc
// https://docs.rs/libc/0.2.161/libc/
// https://github.com/rust-lang/socket2/blob/master/

extern crate libc;

use std::net::{UdpSocket, Ipv4Addr};
use std::str;
//use std::os::fd::{AsRawFd, RawFd};
use std::os::unix::io::{AsRawFd, RawFd};

//c datatypes and structs
use libc::{c_int, c_void, ip_mreq_source, in_addr};
//c constants
use libc::{SOL_SOCKET, SO_REUSEADDR, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, IP_DROP_SOURCE_MEMBERSHIP};
//c functions
use libc::{setsockopt, close};


fn to_in_addr(addr: &Ipv4Addr) -> in_addr {
    // `s_addr` is stored as BE on all machines, and the array is in BE order.
    // So the native endian conversion method is used so that it's never
    // swapped.
    in_addr {
        s_addr: u32::from_ne_bytes(addr.octets()),
    }
}

//reuse address function from C
fn activate_reuse_address(socket: i32) -> i32 {
    let yes: i32 = 1;
    let yeslen = std::mem::size_of_val(&yes) as libc::socklen_t;
    unsafe {
        setsockopt(
            socket,                              
            SOL_SOCKET,                          
            SO_REUSEADDR,                        
            &yes as *const _ as *const c_void,   
            yeslen                               
        )
    }
}

//add ssm membership
fn add_source_membership(socket: i32, group: &Ipv4Addr, source: &Ipv4Addr, interface: &Ipv4Addr) -> i32 {
    let mreqs = ip_mreq_source {
        imr_multiaddr: to_in_addr(group),  
        imr_interface: to_in_addr(interface),
        imr_sourceaddr: to_in_addr(source), 
    };
    let mreqslen = std::mem::size_of_val(&mreqs) as libc::socklen_t;
    unsafe {
        setsockopt(
            socket,
            IPPROTO_IP,
            IP_ADD_SOURCE_MEMBERSHIP,
            &mreqs as *const _ as *const c_void,
            mreqslen                       
        )
    }
}

//drop ssm membership
fn drop_source_membership(socket: i32, group: &Ipv4Addr, source: &Ipv4Addr, interface: &Ipv4Addr) -> i32 {
    let mreqs = ip_mreq_source {
        imr_multiaddr: to_in_addr(group), 
        imr_interface: to_in_addr(interface), 
        imr_sourceaddr: to_in_addr(source), 
    };
    let mreqslen = std::mem::size_of_val(&mreqs) as libc::socklen_t;
    unsafe {
        setsockopt(
            socket,
            IPPROTO_IP,
            IP_DROP_SOURCE_MEMBERSHIP,
            &mreqs as *const _ as *const c_void,
            mreqslen                        
        )
    }
}

fn close_socket(socket: i32) {
    unsafe {
        close(socket);
    }
}


fn main() -> std::io::Result<()> {
    //let msgbufsize: usize = 256;
    let group: Ipv4Addr = Ipv4Addr::new(232, 0, 0, 0);
    //let port: u16 = 1900; 
    let source: Ipv4Addr = Ipv4Addr::new(172, 26, 159, 103);
    let interface: Ipv4Addr = Ipv4Addr::new(0, 0, 0, 0);

    /* let _sock = create_udp_socket(); */

    let socket = UdpSocket::bind("0.0.0.0:1900")?;
    let raw_fd: RawFd = socket.as_raw_fd();
    //let raw_fd = socket.as_fd();

    let err1 = activate_reuse_address(raw_fd);
    if err1 < 0 {
        println!("reuse address: {}", err1);
    }

    //add membership
    let err2 = add_source_membership(raw_fd, &group, &source, &interface);
    if err2 < 0 {
        println!("add membership: {}", err2);
    }

    //receive
    let mut buf = [0; 1024];
    //for _n in 1..10 {
    loop {
        let (len, src) = socket.recv_from(&mut buf)?;
        let msg = str::from_utf8(&buf[..len]).expect("Failed to convert message to UTF-8");
        println!("Received {} bytes from {}: {}", len, src, msg);
    }

    //drop membership
    let err3 = drop_source_membership(raw_fd, &group, &source, &interface);
    if err3 < 0 {
        println!("drop membership: {}", err3);
    }
    
    //close
    close_socket(raw_fd);

    Ok(())
}