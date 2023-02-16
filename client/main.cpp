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
    newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(newSocket == INVALID_SOCKET)
    {
        std::cerr<< "Error at socket(): "<< WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE); 
    }
    
    std::cout<< "Socket created.\n";

    return newSocket;
}

void connectSocket(SOCKET clientSocket, const char* ip, u_short port)
{
    // connect
    // connect a client to a server
    // within the client binding is performed automatically
    // s : discripter that identifies a socket
    // addr : struct containing server IP address and port.
    // addrlen : size in bytes of addr struct
    // connect will wait 75 seconds for server to respond
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(ip);
    clientService.sin_port = htons(port);
    if(connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
    {
        std::cout<< "client faild to connect.\n";
        closesocket(clientSocket);
        WSACleanup();
        exit(EXIT_FAILURE); 
    }
    std::cerr<< "Connected.\n";
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
    connectSocket(clientSocket, "127.0.0.1", 55555);

    // send
    // buf : pointer to buffer
    // len : the length in bytes of buffer
    // flags : Optional set of flags (No routing etc).
    // return : no of bytes sent, SOCKET_ERROR if failed.
    
    // const char* buffer = "message from client.\n";
    // const int buffLen = strlen(buffer) + 1; 

    const int buffLen = 100; 
    char buffer[buffLen];
    strcpy_s(buffer, "message from client.\n");
    
    int byteCount = send(clientSocket, buffer, buffLen, 0);

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