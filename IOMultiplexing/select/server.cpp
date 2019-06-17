#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8091
#define BUFFERSIZE 100

using namespace std;

void error_handling(char* msg)
{
    puts(msg);
    exit(-1);
}

int main()
{
    int serverSock, clientSock;
    struct timeval timeout;
    sockaddr_in serverAddr, clientAddr;
    fd_set reads, temps;

    socklen_t clientAddrSize;
    int fd_max, str_len, fd_num, i;
    char buf[BUFFERSIZE];
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        error_handling("serverSock create error");
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    int err = bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (err < 0) {
        error_handling("bind error");
    }

    err = listen(serverSock, 5);
    if (err < 0) {
        error_handling("listen error");
    }

    FD_ZERO(&reads);
    FD_SET(serverSock, &reads);
    fd_max = serverSock;

    while (1) {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if ((fd_num = select(fd_max + 1, &temps, 0, 0, &timeout)) == -1) {
            puts("select error");
            break;
        }
        if (fd_num == 0) {
            continue;
        }

        for (i = 0; i < fd_max + 1; i++) {
            if (FD_ISSET(i, &temps)) {
                if (i == serverSock) {
                    //connection  request
                    clientAddrSize = sizeof(clientAddr);
                    clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrSize);
                    if (clientSock < 0) {
                        error_handling("clientSock create error");
                    }
                    FD_SET(clientSock, &reads);
                    if (fd_max < clientSock) {
                        fd_max = clientSock;
                    }
                    cout << "connect client" << clientSock << endl;
                } else {
                    //read message
                    str_len = read(i, buf, BUFFERSIZE);
                    if (str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        cout << "close client id :" << "i" << endl;
                    } else {
                        //echo the msg
                        write(i, buf, str_len);
                    }
                }
            }
        }
    }
    close(serverSock);
    return 0;
}