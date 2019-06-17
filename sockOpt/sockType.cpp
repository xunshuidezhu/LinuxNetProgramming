#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main()
{
    int tcpSock, udpSock;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);
    tcpSock = socket(AF_INET, SOCK_STREAM, 0);
    udpSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (tcpSock < 0 || udpSock < 0) {
        cout << "tcp/udp socket create error!" << endl;
        exit(-1);
    }    

    state = getsockopt(tcpSock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if (state < 0) {
        cout << "get the socket option error" << endl;
        exit(-1);
    }
    (sock_type == SOCK_STREAM) ? cout << "SOCK_STREAM" << endl : cout << "UNKONOWN" << endl;
    state = getsockopt(udpSock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if (state < 0) {
        cout << "get the socket option error" << endl;
        exit(-1);
    }
    (sock_type == SOCK_DGRAM) ? cout << "SOCK_DGRAM" << endl : cout << "UNkonwn" << endl;
    return 0;


}