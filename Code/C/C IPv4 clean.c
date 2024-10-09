//Source used: https://gist.github.com/hostilefork/f7cae3dc33e7416f2dd25a402857b6c6

#ifdef _WIN32
    #include <Winsock2.h> // before Windows.h, else Winsock 1 conflict
    #include <Ws2tcpip.h> // needed for ip_mreq definition for multicast
    #include <Windows.h>
    #include <io.h> //close
#else
    #include <sys/socket.h> 
    #include <netinet/in.h> 
    #include <arpa/inet.h>
    #include <unistd.h> 
#endif
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 

int main() {

    char* group = "239.255.255.250"; 
    int port = 1900;                

#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(0x0101, &wsaData);
#endif

    // create UDP Socket for Multicast
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    // reuse address
    u_int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes));

    // bind to receive address
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(fd, (struct sockaddr*) &addr, sizeof(addr));

    // join Multicast Group with membership
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq));

    // receive
    while (1) {
        char msgbuf[256];
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
    setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*) &mreq, sizeof(mreq));

    // Clean up
#ifdef _WIN32
    WSACleanup();
#endif
    close(fd);
    return 0;
}