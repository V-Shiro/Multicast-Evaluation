// https://github.com/rust-lang/socket2/blob/master/src/socket.rs
// https://kornel.ski/rust-sys-crate
// https://doc.rust-lang.org/cargo/reference/publishing.html
// https://doc.rust-lang.org/book/ffi.html

use std::net;
use std::net::UdpSocket;

use crate::sys::{self, c_int, getsockopt, setsockopt, Bool};

extern "C" {
    setsockopt(int, int, int, const char*, socklen_t) -> int; 
}
extern "C" {
    setsockopt(int, int, int, const char*, int) -> int; 
}


// const IP_ADD_SOURCE_MEMBERSHIP;

/// Set value for the `SO_REUSEADDR` option on this socket.
///
/// This indicates that further calls to `bind` may allow reuse of local
/// addresses. For IPv4 sockets this means that a socket may bind even when
/// there's a socket already listening on this port.
fn set_reuse_address(&self, reuse: bool) -> io::Result<()> {
    unsafe {
        setsockopt(
            self.as_raw(),
            sys::SOL_SOCKET,
            sys::SO_REUSEADDR,
            reuse as c_int,
        )
    }
}



/// Join a multicast SSM channel using `IP_ADD_SOURCE_MEMBERSHIP` option on this socket.
///
/// This function specifies a new multicast channel for this socket to join.
/// The group must be a valid SSM group address, the source must be the address of the sender
/// and `interface` is the address of the local interface with which the system should join the
/// multicast group. If it's [`Ipv4Addr::UNSPECIFIED`] (`INADDR_ANY`) then
/// an appropriate interface is chosen by the system.
#[cfg(not(any(
    target_os = "dragonfly",
    target_os = "haiku",
    target_os = "hurd",
    target_os = "netbsd",
    target_os = "openbsd",
    target_os = "redox",
    target_os = "fuchsia",
    target_os = "nto",
    target_os = "espidf",
    target_os = "vita",
)))]
fn join_ssm_v4(source: &Ipv4Addr, group: &Ipv4Addr, interface: &Ipv4Addr) -> io::Result<()> {
    let mreqs = sys::IpMreqSource {
        imr_multiaddr: sys::to_in_addr(group),
        imr_interface: sys::to_in_addr(interface),
        imr_sourceaddr: sys::to_in_addr(source),
    };
    unsafe {
        setsockopt(
            self.as_raw(),
            sys::IPPROTO_IP,
            sys::IP_ADD_SOURCE_MEMBERSHIP,
            mreqs,
        )
    }
}

    /// Leave a multicast group using `IP_DROP_SOURCE_MEMBERSHIP` option on this socket.
///
/// For more information about this option, see [`join_ssm_v4`].
///
/// [`join_ssm_v4`]: Socket::join_ssm_v4
#[cfg(not(any(
    target_os = "dragonfly",
    target_os = "haiku",
    target_os = "hurd",
    target_os = "netbsd",
    target_os = "openbsd",
    target_os = "redox",
    target_os = "fuchsia",
    target_os = "nto",
    target_os = "espidf",
    target_os = "vita",
)))]
fn leave_ssm_v4(source: &Ipv4Addr, group: &Ipv4Addr, interface: &Ipv4Addr) -> io::Result<()> {
    let mreqs = sys::IpMreqSource {
        imr_multiaddr: sys::to_in_addr(group),
        imr_interface: sys::to_in_addr(interface),
        imr_sourceaddr: sys::to_in_addr(source),
    };
    unsafe {
        setsockopt(
            self.as_raw(),
            sys::IPPROTO_IP,
            sys::IP_DROP_SOURCE_MEMBERSHIP,
            mreqs,
        )
    }
}

/// Join a multicast SSM channel using `IPV6_ADD_SOURCE_MEMBERSHIP` option on this socket.
///
/// This function specifies a new multicast channel for this socket to join.
/// The group must be a valid SSM group address, the source must be the address of the sender
/// and `interface` is the address of the local interface with which the system should join the
/// multicast group. If it's [`Ipv4Addr::UNSPECIFIED`] (`INADDR_ANY`) then
/// an appropriate interface is chosen by the system.
#[cfg(not(any(
    target_os = "dragonfly",
    target_os = "haiku",
    target_os = "hurd",
    target_os = "netbsd",
    target_os = "openbsd",
    target_os = "redox",
    target_os = "fuchsia",
    target_os = "nto",
    target_os = "espidf",
    target_os = "vita",
)))]
fn join_ssm_v6(&self, source: &Ipv6Addr, group: &Ipv6Addr, interface: &Ipv6Addr) -> io::Result<()> {
    let mreqs = sys::Ipv6MreqSource {
        ipv6mr_multiaddr: sys::to_in6_addr(group),
        ipv6mr_interface: sys::to_in6_addr(interface),
        ipv6mr_sourceaddr: sys::to_in6_addr(source),
    };
    unsafe {
        setsockopt(
            self.as_raw(),
            sys::IPPROTO_IPV6,
            sys::IPV6_ADD_SOURCE_MEMBERSHIP,
            mreqs,
        )
    }
}

    /// Leave a multicast group using `IP_DROP_SOURCE_MEMBERSHIP` option on this socket.
///
/// For more information about this option, see [`join_ssm_v4`].
///
/// [`join_ssm_v4`]: Socket::join_ssm_v4
#[cfg(not(any(
    target_os = "dragonfly",
    target_os = "haiku",
    target_os = "hurd",
    target_os = "netbsd",
    target_os = "openbsd",
    target_os = "redox",
    target_os = "fuchsia",
    target_os = "nto",
    target_os = "espidf",
    target_os = "vita",
)))]
fn leave_ssm_v6(&self, source: &Ipv6Addr, group: &Ipv6Addr, interface: &Ipv6Addr) -> io::Result<()> {
    let mreqs = sys::Ipv6MreqSource {
        ipv6mr_multiaddr: sys::to_in6_addr(group),
        ipv6mr_interface: sys::to_in6_addr(interface),
        ipv6mr_sourceaddr: sys::to_in6_addr(source),
    };
    unsafe {
        setsockopt(
            self.as_raw(),
            sys::IPPROTO_IPV6,
            sys::IPV6_DROP_SOURCE_MEMBERSHIP,
            mreqs,
        )
    }
}

from!(net::UdpSocket, Socket);
from!(Socket, net::UdpSocket);

// test function
fn main() -> std::io::Result<()> {
    //multicast address
    let multicastAddr = Ipv4Addr::new(232, 0, 0, 0);
    let sourceAddr = Ipv4Addr::new(232, 0, 0, 0);

    // Create and bind UDP socket
    let socket = UdpSocket::bind("0.0.0.0:1900")?;

    // add multicast membership
    let netInt = Ipv4Addr::new(0, 0, 0, 0);
    socket.join_ssm_v4(&multicastAddr, &sourceAddr, &netInt)?;

    //receive
    let mut buf = [0; 1024];
    loop {
        let (len, src) = socket.recv_from(&mut buf)?;
        let msg = str::from_utf8(&buf[..len]).expect("Failed to convert message to UTF-8");
        println!("Received {} bytes from {}: {}", len, src, msg);
    }
    // drop membership
    socket.leave_ssm_v4(&multicastAddr, &sourceAddr, &netInt)?;
}