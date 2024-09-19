/*
How It Works:

    Multicast Group & Port: The multicast group IP and port are defined at the top of the program.
    Socket Creation: The program creates a UDP socket to receive messages.
    Socket Options: The socket is set to allow reuse of the port (SO_REUSEADDR) and joins the multicast group using IP_ADD_MEMBERSHIP.
    Receiving Messages: The program continuously listens for incoming messages, prints them to the console, and provides the sender's IP address.

Compilation and Running:

    Compile the program using a C++ compiler, such as g++:

    bash

g++ -o multicast_receiver multicast_receiver.cpp

Run the compiled program:

bash

./multicast_receiver
*/

#include <iostream>
#include <cstring>      // For memset
#include <arpa/inet.h>  // For inet_addr, htons, etc.
#include <sys/socket.h> // For socket, bind, etc.
#include <unistd.h>     // For close

int main() {
    // Define the multicast group IP address and port.
    const char* multicastGroupIP = "239.0.0.222"; // Replace with your multicast group IP
    const int multicastPort = 12345; // Replace with your multicast port

    // Create a socket for receiving multicast messages.
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Allow multiple sockets to use the same port number.
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) < 0) {
        std::cerr << "Failed to set socket options" << std::endl;
        close(sockfd);
        return 1;
    }

    // Bind the socket to the multicast port.
    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(multicastPort);

    if (bind(sockfd, (struct sockaddr*)&localAddr, sizeof(localAddr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(sockfd);
        return 1;
    }

    // Join the multicast group.
    struct ip_mreq group;
    group.imr_multiaddr.s_addr = inet_addr(multicastGroupIP);
    group.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&group, sizeof(group)) < 0) {
        std::cerr << "Failed to join multicast group" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Listening for multicast messages on " << multicastGroupIP << ":" << multicastPort << "..." << std::endl;

    // Receive messages in a loop.
    char buffer[1024];
    struct sockaddr_in remoteAddr;
    socklen_t addrlen = sizeof(remoteAddr);

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        // Receive the message.
        int nbytes = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&remoteAddr, &addrlen);
        if (nbytes < 0) {
            std::cerr << "Failed to receive message" << std::endl;
            close(sockfd);
            return 1;
        }

        // Display the received message and the sender's address.
        std::cout << "Received message from " << inet_ntoa(remoteAddr.sin_addr) << ": " << buffer << std::endl;
    }

    // Close the socket.
    close(sockfd);
    return 0;
}