#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFERSIZE 3
#define PORT 8090

using namespace std;

int main()
{
    int clientSock;
    char message[BUFFERSIZE];
    int str_len;
    socklen_t adrSize;
    sockaddr_in serverAddr, fromAddr;

    clientSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSock < 0) {
        cout << "clientSock create error ! " << endl;
        exit(-1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    inet_pton(AF_INET, "192.168.14.255", &serverAddr.sin_addr);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    while (1) {
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message, "q\n")) {
            break;
        }
        sendto(clientSock, message, strlen(message), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
        adrSize = sizeof(serverAddr);
        str_len = recvfrom(clientSock, message, BUFFERSIZE, 0, (sockaddr*)&serverAddr, &adrSize);
        message[str_len] = 0;

        cout << "server ip : " << inet_ntoa(serverAddr.sin_addr) << "server PORT :" << ntohs(serverAddr.sin_port) << endl;
        cout << "message from server : " << message << endl;
    }

    close(clientSock);
    return 0;
}