//Source: https://medium.com/@jinscott/c-winsock2-multicasting-d17ba2d088b3
//Source SSM: https://www.ibm.com/docs/en/zvse/6.2?topic=programs-setsockopt

#include <iostream>
#include <string>
#include <Winsock2.h> 
#include <Ws2tcpip.h> 

#pragma comment(lib,"WS2_32") //block comment warning

#define UDP_MAX_SIZE 65535

int Port = 8910;
const char* multicastGroupIP = "226.1.1.1";
const char* sourceIP = "225.0.0.1";

int main(){

    //WSAStartup
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        std::cout << "WSAStartup function failed with error: <" << iResult << ">\n";
        WSACleanup();
        return -1;
    }
    // Create socket
    SOCKET Socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);
    if (Socket == INVALID_SOCKET){
        std::cout << "Can not create socket: <" << WSAGetLastError() << ">\n";
        closesocket(Socket);
        WSACleanup();
        return -1;
    }

    // Allow reuse of port
    int optval = 1;
    if (setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) < 0){
        std::cout << "Socket set SO_REUSEADDR fail\n";
        closesocket(Socket);
        WSACleanup();
        return -1;
    }

    // Allow any address
    sockaddr_in AllowAddr;
    memset(&AllowAddr, 0, sizeof(AllowAddr));
    AllowAddr.sin_family = AF_INET;
    AllowAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    AllowAddr.sin_port = htons(Port);

    // Bind socket
    if (bind(Socket, (struct sockaddr*)&AllowAddr, sizeof(AllowAddr)) < 0) {
        std::cout << "Multicast failed to bind socket\n";
        closesocket(Socket);
        WSACleanup();
        return -1;
    }

    // Join the multicast group with a specific source (SSM)
    struct ip_mreq_source JoinReq;
    JoinReq.imr_multiaddr.s_addr = inet_addr(multicastGroupIP);  // Multicast group address
    JoinReq.imr_sourceaddr.s_addr = inet_addr(sourceIP);  // Specific source address
    JoinReq.imr_interface.s_addr = htonl(INADDR_ANY);  // Use the default network interface


    // Join membership
    if ((setsockopt(Socket, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (char*)&JoinReq, sizeof(JoinReq))) < 0) {
        std::cout << "Multicast join membership fail. Error code: <" << WSAGetLastError() << ">\n";
        closesocket(Socket);
        WSACleanup();
        return -1;
    }

    //Receive
    char ReceivedIP[46] = { 0 };
    char ReceivedMessage[UDP_MAX_SIZE + 1] = { 0 };
    while (1) {
        sockaddr_in ClientAddr;
        int addrlen = sizeof(ClientAddr);
        int nbytes = recvfrom(Socket,ReceivedMessage,UDP_MAX_SIZE,0,(struct sockaddr*)&ClientAddr,&addrlen);
        if (nbytes < 0) {
            std::cout << "Receive fail. Error code: <" << WSAGetLastError() << ">\n";
            closesocket(Socket);
            WSACleanup();
            return -1;
        }
        inet_ntop(AF_INET, &ClientAddr.sin_addr, (PSTR)ReceivedIP, 46);
        std::cout << "Received from: " << ReceivedIP << ", " << ntohs(ClientAddr.sin_port) << "\n";
        std::cout << ReceivedMessage << "\n";
        if (strcmp("bye", ReceivedMessage) == 0) {
            break;
        }
        memset(ReceivedMessage, 0, UDP_MAX_SIZE);
    }
    closesocket(Socket);
    WSACleanup();

    return 0;
}

