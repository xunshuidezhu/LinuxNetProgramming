#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#define BUFFERSIZE 30
#define PORT 12345

using namespace std;

int main()
{
    int fd;
    int readCnt; //read count
    fd = open("./file.txt", O_RDWR);
    char message[BUFFERSIZE];
    int serverSock, clientSock;
    sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize;

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        cout << "serverSock create error" << endl;
        exit(-1);
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int err = bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (err < 0) {
        cout << "bind error" << endl;
        exit(-1);
    }

    err = listen(serverSock, 5);
    if (err < 0) {
        cout << "listen error" << endl;
        exit(-1);
    }

    clientAddrSize = sizeof(clientAddr);
    clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrSize);

    while (1) {
        readCnt = read(fd, message, BUFFERSIZE);
        if (readCnt < BUFFERSIZE) {
            write(clientSock, message, readCnt);
            break;
        }
        write(clientSock, message, BUFFERSIZE);
    }
    shutdown(clientSock, SHUT_WR);
    read(clientSock, message, BUFFERSIZE);
    cout << "message from client : " << message << endl;

    close(fd);
    close(clientSock);
    close(serverSock);
    return 0;
}