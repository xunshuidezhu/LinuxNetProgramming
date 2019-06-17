#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 12345
#define BUFFERSIZE 1024
#define OPSZ 4

using namespace std;

int calculate(int opNum, int opnds[], char op);
int main()
{
    int serverSock, clientSock;
    char opInfo[BUFFERSIZE];
    int result, opnd_cnt, i;
    int recv_cnt, recv_len;
    sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize;

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        cout << "serverSock create error !" << endl;
        exit(-1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int err = bind(serverSock, (sockaddr*)(&serverAddr), sizeof(serverAddr));
    if (err < 0) {
        cout << "bind error ! " << endl;
        exit(-1);
    }

    err = listen(serverSock, 5);
    if (err < 0) {
        cout << "listen error" << endl;
        exit(-1);
    }

    clientAddrSize = sizeof(clientAddr);

    for (i = 0; i < 5; i++) {
        opnd_cnt = 0;
        clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrSize);
        read(clientSock, &opnd_cnt, 1);

        recv_len = 0;
        while ((opnd_cnt * OPSZ + 1) > recv_len) {
            recv_cnt = read(clientSock, &opInfo[recv_len], BUFFERSIZE - 1);
            recv_len += recv_cnt;
        }
        result = calculate(opnd_cnt, (int*)opInfo, opInfo[recv_len - 1]);
        write(clientSock, (char*)&result, sizeof(result));
        close(clientSock);
    }
    close(serverSock);
    return 0;
}

int calculate(int opNum, int opnds[], char op)
{
    int result = opnds[0], i;
    switch (op) {
    case '+':
        for (i = 1; i < opNum; i++)
            result += opnds[i];
        break;
    case '-':
        for (i = 1; i < opNum; i++)
            result -= opnds[i];
        break;
    case '*':
        for (i = 1; i < opNum; i++)
            result *= opnds[i];
        break;
    default:
        break;
    }
    return result;
}