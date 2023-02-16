#include <WS2tcpip.h>
#include <winsock.h>
#include <tchar.h>
#include <iostream>

#pragma comment(lib,"Ws2_32")

int main(int args, char** argv)
{
    // load dll
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2); // version 2.2
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    if(wsaerr != 0)
    {
        std::cout<< "The Winsock dll not found!\n";
        return 0;
    }
    else
    {
        std::cout<< "The Winsock dll found!\n";
        std::cout<< "The status : "<< wsaData.szSystemStatus << std::endl;
    }

    // create socket
    // af : The address family specification (AF_INET for UDP or TCP)
    // type : Type specification for the new socket 
    // (SOCK_STREAM for TCP and SOCK_DGRAM for UDP)
    // protocol : the protocol to be used (IPPROTO_TCP for TCP)
    SOCKET serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(serverSocket == INVALID_SOCKET)
    {
        std::cout<< "Error at socket(): "<< WSAGetLastError() << std::endl;
        WSACleanup();
        return 0; 
    }
    else
    {
        std::cout<< "socket() is OK.\n";
    }

    // bind the socket
    // SOCKADDR_IN struct
    // sin_family : Address family (must be AF_INET)
    // sin_port : IP port
    // sin_addr : IP address
    // sin_zero : Padding to make struct the same size as SOCKADDR.
    // the htons function returns the value in TCP/IP network byte order.
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    u_short port = 55555;
    service.sin_port = htons(port);
    if(bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        std::cout<< "bind() faild : " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else
    {
        std::cout<< "bind() is OK.\n";
    }
    
    // listn on the socket
    // s : Descriptor identifying a bound, unconnected socket.
    // backlog : the maximum number of connections allowed  (also OS dependant)
    if(listen(serverSocket, 1) == SOCKET_ERROR)
    {
        std::cout<< "listen() : Error listening on socket "<< WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else
    {
        std::cout << "listen() is OK, Waiting for connections...\n";
    }

    // accept
    // This is a blocking function
    // s : Descriptor that identifies a socket that has been placed in a listening with the listen() function.
    // addr : Optional struct containing the client address information.
    // Addrlen : Optional size of the address struct (if included)
    SOCKET acceptSocket;
    acceptSocket = accept(serverSocket, NULL, NULL);
    if(acceptSocket == INVALID_SOCKET)
    {
        std::cout<< "accept faild : "<< WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }

    std::cout<< "client connected.\n";

    // cleanup    
    closesocket(serverSocket);
    WSACleanup();

    system("pause");
    return 0;
}