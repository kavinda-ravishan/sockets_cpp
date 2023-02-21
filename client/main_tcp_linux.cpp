#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

void connectSocket(int clientSocket, const char* ip, uint16_t port)
{
    struct sockaddr_in clientService;
    memset(&clientService, 0, sizeof(clientService));
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(ip);
    clientService.sin_port = htons(port);

    if (connect(clientSocket, (struct sockaddr *)&clientService, sizeof(clientService)) < 0) {
        std::cerr <<"client failed to connect.\n";
        exit(EXIT_FAILURE);
    }
    
    std::cerr << "client connected.\n";
}

void cleanup(int clientSocket)
{
    close(clientSocket);
}

int main(int args, char** argv)
{
    int clientSocket = createTcpSocket();
    connectSocket(clientSocket, "127.0.0.1", 55555);

    const int buffLen = 100; 
    char buffer[buffLen];
    strcpy(buffer, "message from client.");
    
    int byteCount = send(clientSocket, buffer, buffLen, 0);

    if(byteCount == -1)
    {
        std::cerr<< "Send failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout<< "Message send : "<< buffer << std::endl;


    cleanup(clientSocket);
    return 0;
}