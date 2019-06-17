#include <arpa/inet.h>
#include <cstring>
#include <deque>
#include <iostream>
#include <list>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define BUFFERSIZE 3
#define PORT 8090

using namespace std;

int main()
{
    list<int> l = { 1, 2, 3, 4, 5 };
    list<int>::iterator i = l.begin();
    deque<int> q = { 1, 2, 3, 4 };
    deque<int>::iterator i = q.begin();
    vector<int> v = { 1, 3, 4 };
    vector<int>::iterator i = v.begin();
    int serverSock;
    sockaddr_in serverAddr, clientAddr;
    int str_len;
    socklen_t clientAddrSize;
    char message[BUFFERSIZE];

    serverSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSock < 0) {
        cout << "serverSock create error" << endl;
        exit(-1);
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    int err = bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (err < 0) {
        cout << "bind error !" << endl;
        exit(-1);
    }

    while (1) {
        clientAddrSize = sizeof(clientAddr);
        str_len = recvfrom(serverSock, message, BUFFERSIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
        sendto(serverSock, message, str_len, 0, (sockaddr*)&clientAddr, clientAddrSize);
        char mes[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, mes, sizeof(mes));
        // mes = inet_ntoa(clientAddr.sin_addr);
        cout << "client ip : " << mes << " PROT :" << ntohs(clientAddr.sin_port) << endl;
    }
    close(serverSock);
    return 0;
}