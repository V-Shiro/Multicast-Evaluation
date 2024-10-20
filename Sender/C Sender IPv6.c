// https://gist.github.com/hostilefork/f7cae3dc33e7416f2dd25a402857b6c6

#ifdef _WIN32
    #include <Winsock2.h> // before Windows.h, else Winsock 1 conflict
    #include <Ws2tcpip.h> // needed for ip_mreq definition for multicast
    #include <Windows.h>
    #include <io.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h> // for sleep()
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    // ASM Range 224.0.0.0 to 239.255.255.255
    //char* group = "239.255.255.250"; //SSDP

    // SSM Range 232.0.0.0 to 232.255.255.255
    //char* group = "232.0.0.0";

    // IPv6 Range ff00::/8
    char* group = "ff05::c";

    int port = 1900; 

    // test message with delay
    const int delay_secs = 1;
    const char *message = "Tobias!";

#ifdef _WIN32
    
    // Initialize Windows Socket API with given VERSION.
    WSADATA wsaData;
    if (WSAStartup(0x0101, &wsaData)) {
        perror("WSAStartup");
        return 1;
    }
#endif

    // create UDP socket for multicast
    int fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    // allow multiple sockets to use the same PORT number
    u_int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) < 0){
        printf("Reusing ADDR failed");
        return 1;
    }

    // set up destination address
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, group, &addr.sin6_addr);
    addr.sin6_port = htons(port);

    // now just sendto() our destination!
    while (1) {
        char ch = 0;
        int nbytes = sendto(fd, message, strlen(message), 0, (struct sockaddr*) &addr, sizeof(addr));
        if (nbytes < 0) {
            perror("sendto");
            #ifdef _WIN32
                WSACleanup();
            #endif
                close(fd);
            return 1;
        }
     #ifdef _WIN32
          Sleep(delay_secs * 1000); // Windows Sleep is milliseconds
     #else
          sleep(delay_secs); // Unix sleep is seconds
     #endif
     }
#ifdef _WIN32
    WSACleanup();
#endif
    close(fd);
    return 0;
}