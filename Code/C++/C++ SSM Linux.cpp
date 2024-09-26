// Source: https://www.tenouk.com/Module41c.html
// Source SSM: https://www.ibm.com/docs/en/zvse/6.2?topic=programs-setsockopt

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // For inet_addr

#include <iostream>     // For std print
#include <stdio.h>      // For perror
#include <unistd.h>     // For close
#include <cstring>      // For memset


int port = 4321;
const char* multicastGroupIP = "226.1.1.1";
const char* sourceIP = "225.0.0.1";

int main(){
    /* Create a datagram socket on which to receive. */

    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0){
        perror("Opening datagram socket error");
        return 1;
    }

    /* Enable SO_REUSEADDR to allow multiple instances of this */
    /* application to receive copies of the multicast datagrams. */
    int reuse = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char * ) & reuse, sizeof(reuse)) < 0){
        perror("Setting SO_REUSEADDR error");
        close(sd);
        return 1;
    }

    /* Bind to the proper port number with the IP address */
    /* specified as INADDR_ANY. */
    struct sockaddr_in localSock;
    memset((char * ) & localSock, 0, sizeof(localSock));
    localSock.sin_family = AF_INET;
    localSock.sin_port = htons(port);
    localSock.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr * ) & localSock, sizeof(localSock))){
        perror("Binding datagram socket error");
        close(sd);
        return 1;
    }

    // Join the multicast group

    struct ip_mreq_source mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(multicastGroupIP);  // Multicast group address
    mreq.imr_sourceaddr.s_addr = inet_addr(sourceIP);  // Specific source address
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);  // Use the default network interface

    if (setsockopt(sd, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (char * ) &mreq, sizeof(mreq)) < 0){
        perror("Adding multicast group error");
        close(sd);
        return 1;
    }

    // Read from the socket
    char buffer[1024];
    struct sockaddr_in remoteAddr;
    socklen_t addrlen = sizeof(remoteAddr);
    while(1){
        memset(buffer, 0, sizeof(buffer));

        //receive
        int nbytes = recvfrom(sd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&remoteAddr, &addrlen);
        if (nbytes < 0) {
            perror("Reading datagram message error");
            close(sd);
            return 1;
        }          
        if (strcmp("bye", buffer) == 0) {
            break;
        }
        std::cout << "Message from multicast sender: " << buffer << std::endl;
    }
    close(sd);
        return 0;
}