#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

int createTcpSocket()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    return serverSocket;
}

void bindSocket(int serverSocket, const char* ip, uint16_t port)
{
    sockaddr_in serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ip);
    serverAddress.sin_port = htons(port);

    int bindStatus = bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress));

    if (bindStatus == -1) {
        std::cerr << "Error binding socket\n";
        exit(EXIT_FAILURE);
    }
}

void initListen(int serverSocket)
{
    int listenStatus = listen(serverSocket, 1);

    if (listenStatus == -1) {
        std::cerr << "Error listening on socket\n";
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Listening for incoming connections...\n";
}

int waitForNewConnection(int serverSocket)
{
    sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

    if (clientSocket == -1) {
        std::cerr << "Error accepting connection\n";
        return 1;
    }

    std::cout << "Connection accepted from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << "\n";
    return clientSocket;
}

void cleanup(int serverSocket, int acceptSocket)
{
    close(acceptSocket);
    close(serverSocket);
}

int main() {
    
    int serverSocket = createTcpSocket();
    bindSocket(serverSocket, "127.0.0.1", 55555);
    initListen(serverSocket);
    int acceptSocket = waitForNewConnection(serverSocket);

    const int buffLen = 100; 
    char buffer[buffLen];
    int byteCount = recv(acceptSocket, buffer, buffLen, 0);

    if(byteCount == -1)
    {
        std::cerr<< "Receive failed."<< std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout<< "Message received : "<< buffer << std::endl;

    cleanup(serverSocket, acceptSocket);

    return 0;
}
