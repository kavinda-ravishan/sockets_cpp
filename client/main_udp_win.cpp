#include <WS2tcpip.h>
#include <winsock.h>
#include <tchar.h>
#include <iostream>

#pragma comment(lib,"Ws2_32")

void loadDll(int majorVersion, int minorVersion)
{
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(majorVersion, minorVersion); // version 2.2
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    if(wsaerr != 0)
    {
        std::cerr<< "The Winsock dll not found!\n";
        exit(EXIT_FAILURE);
    }

    std::cout<< "The Winsock dll found, status : "<< wsaData.szSystemStatus << std::endl;
}

SOCKET createTcpSocket()
{
    // create socket
    // af : The address family specification (AF_INET for UDP or TCP)
    // type : Type specification for the new socket 
    // (SOCK_STREAM for TCP and SOCK_DGRAM for UDP)
    // protocol : the protocol to be used (IPPROTO_TCP for TCP)
    SOCKET newSocket = INVALID_SOCKET;
    newSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(newSocket == INVALID_SOCKET)
    {
        std::cerr<< "Error at socket(): "<< WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE); 
    }
    
    std::cout<< "Socket created.\n";

    return newSocket;
}

void cleanup(SOCKET clientSocket)
{
    // cleanup    
    closesocket(clientSocket);
    WSACleanup();
}

int main(int args, char** argv)
{
    loadDll(2, 2);
    SOCKET clientSocket = createTcpSocket();

    // send
    // buf : pointer to buffer
    // len : the length in bytes of buffer
    // flags : Optional set of flags (No routing etc).
    // return : no of bytes sent, SOCKET_ERROR if failed.
    
    // const char* buffer = "message from client.\n";
    // const int buffLen = strlen(buffer) + 1; 

    const int buffLen = 100; 
    char buffer[buffLen];
    strcpy_s(buffer, "message from client.");
    
    sockaddr_in service;
    int serviceLen = sizeof(service);
    std::memset(&service, 0, sizeof(service));
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(55555);

    int byteCount = sendto(clientSocket, buffer, buffLen, 0, (SOCKADDR*)&service, serviceLen);

    if(byteCount == SOCKET_ERROR)
    {
        std::cout<< "Send failed : "<< WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout<< "Message send : "<< buffer << std::endl;


    cleanup(clientSocket);
    system("pause");
    return 0;
}