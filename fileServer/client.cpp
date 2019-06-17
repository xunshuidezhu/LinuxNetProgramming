#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFERSIZE 30
#define PORT 12345

using namespace std;

int main()
{
    int clientSock;
    FILE* fp;
    fp = fopen("receive.dat", "wb");

    char message[BUFFERSIZE];
    int readCnt; //read count
    sockaddr_in serverAddr;

    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock < 0) {
        cout << "clientSock create error !" << endl;
        exit(-1);
    }

    bzero(&serverAddr, sizeof(clientSock));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr));

    while ((readCnt = read(clientSock, message, BUFFERSIZE)) != 0) {
        fwrite((void*)message, 1, readCnt, fp);
    }
    puts("received server file!");
    write(clientSock, "receive file", 13);
    fclose(fp);
    close(clientSock);
    return 0;
}