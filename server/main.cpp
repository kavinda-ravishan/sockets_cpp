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

void bindSocket(SOCKET serverSocket, const char* ip, u_short port)
{
    // bind the socket
    // SOCKADDR_IN struct
    // sin_family : Address family (must be AF_INET)
    // sin_port : IP port
    // sin_addr : IP address
    // sin_zero : Padding to make struct the same size as SOCKADDR.
    // the htons function returns the value in TCP/IP network byte order.
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(ip);
    service.sin_port = htons(port);
    if(bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        std::cerr<< "bind() faild : " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    
    std::cout<< "Socket bind successful, IP : "<<ip<<", PORT : "<<port<<std::endl;
}

void initListen(SOCKET serverSocket)
{
    // listn on the socket
    // s : Descriptor identifying a bound, unconnected socket.
    // backlog : the maximum number of connections allowed  (also OS dependant)
    if(listen(serverSocket, 1) == SOCKET_ERROR)
    {
        std::cerr<< "listen() : Error listening on socket "<< WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    std::cout << "Waiting for connections...\n";
}

SOCKET waitForNewConnection(SOCKET serverSocket)
{
    // accept
    // This is a blocking function
    // s : Descriptor that identifies a socket that has been placed in a listening with the listen() function.
    // addr : Optional struct containing the client address information.
    // Addrlen : Optional size of the address struct (if included)
    SOCKET acceptSocket;
    acceptSocket = accept(serverSocket, NULL, NULL);
    if(acceptSocket == INVALID_SOCKET)
    {
        std::cerr<< "accept faild : "<< WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    std::cout<< "client connected.\n";

    return acceptSocket;
}

void cleanup(SOCKET serverSocket, SOCKET acceptSocket)
{
    // cleanup    
    closesocket(acceptSocket);
    closesocket(serverSocket);
    WSACleanup();
}

int main(int args, char** argv)
{
    loadDll(2, 2);
    SOCKET serverSocket = createTcpSocket();
    bindSocket(serverSocket, "127.0.0.1", 55555);
    initListen(serverSocket);
    SOCKET acceptSocket = waitForNewConnection(serverSocket);

    // receiving
    // S : the descriptor that identifies a connection socket
    // buff : A pointer to the buffer to receive
    // len : buff len in bytes
    // flags : optional set of flags
    // return : no of bytes sent, SOCKET_ERROR if failed.
    const int buffLen = 100; 
    char buffer[buffLen];
    int byteCount = recv(acceptSocket, buffer, buffLen, 0);

    if(byteCount == SOCKET_ERROR)
    {
        std::cout<< "Receive failed : "<< WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout<< "Message received : "<< buffer << std::endl;

    cleanup(serverSocket, acceptSocket);
    system("pause");
    return 0;
}