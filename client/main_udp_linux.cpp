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

void cleanup(int clientSocket)
{
    close(clientSocket);
}

int main(int args, char** argv)
{
    int clientSocket = createTcpSocket();

    const int buffLen = 100; 
    char buffer[buffLen];
    strcpy(buffer, "message from client.");
    
    sockaddr_in clientAddress;
    int clientAddressLen = sizeof(clientAddress);
    std::memset(&clientAddress, 0, sizeof(clientAddress));
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientAddress.sin_port = htons(55555);

    int byteCount = sendto(clientSocket, buffer, buffLen, 0, (sockaddr *)&clientAddress, (socklen_t)clientAddressLen);

    if(byteCount == -1)
    {
        std::cerr<< "Send failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout<< "Message send : "<< buffer << std::endl;


    cleanup(clientSocket);
    return 0;
}