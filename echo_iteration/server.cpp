#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8091
#define BUFFERSIZE 1024
#define LISTENQUEUE 5

using namespace std;

int main()
{
    int serverSock, clientSock;
    int str_len;
    char message[BUFFERSIZE];
    socklen_t clientAddrLen;
    sockaddr_in serverAddr, clientAddr;

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        cout << "serverSock create error ! " << endl;
        exit(-1);
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_family = AF_INET;

    int err;
    err = bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (err < 0) {
        cout << "bind serverSock error ! " << endl;
        exit(-1);
    }

    listen(serverSock, LISTENQUEUE);
    clientAddrLen = sizeof(clientAddr);
    for (int i = 0; i < 5; i++) {
        
        clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSock < 0) {
            cout << "accept clientSock error ! " << endl;
            exit(-1);
        } 
        cout << i + 1 << "clients connected " << endl;
        while ((str_len = read(clientSock, message, BUFFERSIZE)) != 0) {
            write(clientSock, message, str_len);
        }
        close(clientSock);
    }
    close(serverSock);
    return 0;
}