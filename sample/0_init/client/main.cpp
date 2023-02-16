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
    SOCKET clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(clientSocket == INVALID_SOCKET)
    {
        std::cout<< "Error at socket(): "<< WSAGetLastError() << std::endl;
        WSACleanup();
        return 0; 
    }
    else
    {
        std::cout<< "socket() is OK.\n";
    }

    // connect
    // connect a client to a server
    // within the client binding is performed automatically
    // s : discripter that identifies a socket
    // addr : struct containing server IP address and port.
    // addrlen : size in bytes of addr struct
    // connect will wait 75 seconds for server to respond
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
    u_short port = 55555;
    clientService.sin_port = htons(port);
    if(connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
    {
        std::cout<< "client connect() faild to connect.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 0;
    }
    else
    {
        std::cout<< "client connect() is OK, connected.\n";
    }


    // cleanup    
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}