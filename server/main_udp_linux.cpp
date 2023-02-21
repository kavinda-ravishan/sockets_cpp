#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

int createTcpSocket()
{
    int serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

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

void cleanup(int serverSocket)
{
    close(serverSocket);
}

int main() {
    
    int serverSocket = createTcpSocket();
    bindSocket(serverSocket, "127.0.0.1", 55555);

    const int buffLen = 100; 
    char buffer[buffLen];

    sockaddr_in clientAddress;
    int clientAddressLen = sizeof(clientAddress);
    std::memset(&clientAddress, 0, sizeof(clientAddress));
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientAddress.sin_port = htons(55555);

    int byteCount = recvfrom(serverSocket, buffer, buffLen, 0, (sockaddr *)&clientAddress, (socklen_t*)&clientAddressLen);

    if(byteCount == -1)
    {
        std::cerr<< "Receive failed."<< std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout<< "Message received : "<< buffer << std::endl;

    cleanup(serverSocket);

    return 0;
}
