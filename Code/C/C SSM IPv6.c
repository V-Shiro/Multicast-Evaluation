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
#define GROUP "ff05::c"       // Example multicast group (SSM range 232.0.0.0/8)
#define PORT 1900               // Example port number
#define SOURCE "172.26.159.103"  // Example source address (SSM source)

int main() {
    // Initialize Windows Socket API with given VERSION.
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(0x0101, &wsaData)) { // mVersionRequired, IpWSAData 
        printf("WSAStartup");
        return 1;
    }
#endif

    // create UDP Socket for Multicast
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

    // Bind to the specified port
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(PORT);
    addr.sin6_addr = in6addr_any;

    if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        printf("bind");
        return 1;
    }

    // Join the multicast group with a specific source (SSM)
    struct ip_mreq_source mreq;
    inet_pton(AF_INET6, GROUP, &mreq.imr_multiaddr);   // Multicast group address
    inet_pton(AF_INET6, SOURCE, &mreq.imr_sourceaddr); // Specific source address
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);  // Use the default network interface

    int error = setsockopt(fd, IPPROTO_IPV6, IP_ADD_SOURCE_MEMBERSHIP, (char*) &mreq, sizeof(mreq));
    if (error < 0) {
        printf("add membership %d", error);
        return 1;
    }

    // receive loop
    printf("Receiving on port %d...\n", PORT);
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

    // drop membership
    setsockopt(fd, IPPROTO_IPV6, IP_DROP_SOURCE_MEMBERSHIP, (char*) &mreq, sizeof(mreq));
    // Clean up
#ifdef _WIN32
    WSACleanup();
#endif
    close(fd);
    return 0;
}
