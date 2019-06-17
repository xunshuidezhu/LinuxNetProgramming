#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <assert.h>


#define PORT 12345
#define BUFFERSIZE 1024
#define IPADDRESS "127.0.0.1"

using namespace std;

void sendmsg(int ip, char* port)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);
    sockaddr_in serverSockAddr;
    sockaddr_in clientSockAddr;
    bzero(&sockfd, sizeof(serverSockAddr));
    inet_pton(AF_INET, IPADDRESS, &serverSockAddr.sin_family);
}
int main()
{
    int clientSock;
    sockaddr_in serverAddr;
    char message[BUFFERSIZE];
    int str_len, recv_len, recv_cnt;

    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock < 0) {
        cout << "create clientSock error" << endl;
        exit(-1);
    }

    bzero(&serverAddr, sizeof(serverAddr));
    //san zhong xie fa 
    //inet_aton(IPADDRESS, &serverAddr.sin_addr);
    //serverAddr.sin_addr.s_addr = inet_addr(IPADDRESS);
    inet_pton(AF_INET, IPADDRESS, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_family = AF_INET;

    int err;
    err = connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (err < 0) {
        cout << "connect error" << endl;
        exit(-1);
    }
    while (1) {
        cout << "inputmessage : ";
        cin >> message;

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        str_len = write(clientSock, message, strlen(message));
        recv_len = 0;
        while (recv_len < str_len) {
            recv_cnt = read(clientSock, &message[recv_len], BUFFERSIZE - 1);
            if (recv_cnt < 0) {
                cout << "read error " << endl;
                exit(-1);
            }
            recv_len += recv_cnt;
        }
        
        message[str_len] = 0;
        cout << "message from server : " << message << endl;
    }
    close(clientSock);
    return 0;


}