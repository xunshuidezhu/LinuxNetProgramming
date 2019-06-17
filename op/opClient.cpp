#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFERSIZE 1024
#define PORT 12345
#define OPSZ 4
#define RLT_SIZE 4

using namespace std;

int main()
{
    int clientSock;
    char opmsg[BUFFERSIZE];
    int result, opnd_cnt, i;
    sockaddr_in serverAddr;

    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock < 0) {
        cout << "clientSock create error" << endl;
        exit(-1);
    }
    
    bzero(&serverAddr, sizeof(serverAddr));
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if ((connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)) {
        cout << "connect serverAddr error" << endl;
        exit(-1);
    } else {
        cout << "connected ...." << endl;
    }
    fputs("operand count :", stdout);
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;
    
    for (i = 0; i < opnd_cnt; i++) {
        cout << "operand " << i + 1 << endl;
        scanf("%d", (int*)&opmsg[i * OPSZ + 1]);
    }

    fgetc(stdin);
    fputs("OperatorL ", stdout);
    scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]);
    write(clientSock, opmsg, opnd_cnt * OPSZ + 1);
    read(clientSock, &result, RLT_SIZE);

    cout << "result: " << result << endl;
    close(clientSock);
    return 0;
    
}