#ifdef _WIN32
    #include <Winsock2.h> // before Windows.h, else Winsock 1 conflict
    #include <Ws2tcpip.h> // needed for ip_mreq definition for multicast
    #include <Windows.h>
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


    char* group = "239.255.255.250"; 
    int port = 12345; 

    // test message with delay
    //
    const int delay_secs = 1;
    const char *message = "Hello, World!";

#ifdef _WIN32
    
    // Initialize Windows Socket API with given VERSION.
    //
    WSADATA wsaData;
    if (WSAStartup(0x0101, &wsaData)) {
        perror("WSAStartup");
        return 1;
    }
#endif

    // create what looks like an ordinary UDP socket
    //
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    // set up destination address
    //
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(group);
    addr.sin_port = htons(port);

    // now just sendto() our destination!
    //
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