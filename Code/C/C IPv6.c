//Source used: https://gist.github.com/hostilefork/f7cae3dc33e7416f2dd25a402857b6c6

#ifdef _WIN32
    #include <Winsock2.h> // before Windows.h, else Winsock 1 conflict
    #include <Ws2tcpip.h> // needed for ip_mreq definition for multicast
    #include <Windows.h>
    #include <io.h> //close
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h> //close
#endif
#include <stdio.h> //printf
#include <string.h> //memset

#define MSGBUFSIZE 256

int main() {
    char* group = "ff05::c"; // e.g. ff05::c for SSDP 
    int port = 1900;        // 0 if error, which is an invalid port

#ifdef _WIN32
    // Initialize Windows Socket API with given VERSION.
    WSADATA wsaData;
    if (WSAStartup(0x0101, &wsaData)) { // mVersionRequired, IpWSAData
        printf("WSAStartup");
        return 1;
    }
#endif

    // create UDP socket (IPv6) for Multicast
    int fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (fd < 0) {
        printf("socket");
        return 1;
    }

    // allow multiple sockets to use the same PORT number
    u_int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) < 0){
        printf("Reusing ADDR failed");
        return 1;
    }

    //set address family to IPv6
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(port);
    addr.sin6_addr = in6addr_any; // accept any IPv6 address

    // Bind the socket to the specified port
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("bind");
        return 1;
    }

    // join Multicast Group with membership
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    
    if (setsockopt(fd, IPPROTO_IPV6, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) < 0){
        printf("add membership");
        return 1;
    }

    // receive loop
    printf("Receiving on port %d...\n", port);
    while (1) {
        char msgbuf[MSGBUFSIZE];
        int addrlen = sizeof(addr);
        int nbytes = recvfrom(fd,msgbuf,MSGBUFSIZE,0,(struct sockaddr *) &addr,&addrlen);
        if (nbytes < 0) {
            printf("recvfrom");
            return 1;
        }
        msgbuf[nbytes] = '\0';
        puts(msgbuf);
    }

    if (setsockopt(fd, IPPROTO_IPV6, IP_DROP_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) < 0){
        printf("drop membership");
        return 1;
    }

    // Clean up
#ifdef _WIN32
    WSACleanup();
#endif
    close(fd);
    return 0;
}
