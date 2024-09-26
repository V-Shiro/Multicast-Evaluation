// Source: https://www.tenouk.com/Module41c.html
// Surce: https://gist.github.com/dksmiffs/d67afe6bda973d67752ae63dc49a7310

#include <arpa/inet.h>  // For inet_addr, htons, etc.
#include <sys/socket.h> // For socket, bind, etc.
#include <netinet/in.h> // For inet_addr, etc.

#include <iostream>     // For std print
#include <unistd.h>     // For close
#include <cstring>      // For memset

int main(){
  // Define the multicast group IP address and port.
    const char* multicastGroupIP = "239.0.0.222"; // Replace with your multicast group IP
    const int multicastPort = 12345; // Replace with your multicast port

    // Create a socket for receiving multicast messages.
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }
  {
    // enable SO_REUSEADDR to allow multiple instances of this application to
    // receive copies of the multicast datagrams.
    int reuse = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) < 0){
      std::cerr << "Failed reuse address" << std::endl;
      close(sockfd);
      return 1;
      } 
  }

  // Bind to the proper port number with the IP address specified as INADDR_ANY
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(multicastPort);
  addr.sin_addr.s_addr = INADDR_ANY;
  // Note from manpage that bind returns 0 on success
  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
      std::cerr << "Failed to bind socket" << std::endl;
      close(sockfd);
      return 1;
  }

  // Join the multicast group
  struct ip_mreq group;
  group.imr_multiaddr.s_addr = inet_addr(multicastGroupIP);
  //group.imr_interface.s_addr = inet_addr("127.0.0.1");
  group.imr_interface.s_addr = htonl(INADDR_ANY);

  if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&group, sizeof(group)) < 0) {
      std::cerr << "Failed to join multicast group" << std::endl;
      close(sockfd);
      return 1;
  }

  // Read from the socket
  char buffer[1024];
  struct sockaddr_in remoteAddr;
  socklen_t addrlen = sizeof(remoteAddr);
  while(1){
    memset(buffer, 0, sizeof(buffer));

    //receive
    int nbytes = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&remoteAddr, &addrlen);
    if (nbytes < 0) {
      std::cerr << "Failed to receive message" << std::endl;
      close(sockfd);
      return 1;
    }          
    if (strcmp("bye", buffer) == 0) {
      break;
    }
    std::cout << "Message from multicast sender: " << buffer << std::endl;
  }
  close(sockfd);
  return 0;
}