#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFERSIZE 30

using namespace std;

void read_routine(int sock, char* buf);
void write_routine(int sock, char* buf);

int main()
{
    int sock;
    pid_t pid;
    char buf[BUFFERSIZE];
    sockaddr_in serverAddr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cout << "sock create error" << endl;
    }
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_family = AF_INET;

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cout << "connect error" << endl;
        exit(-1);
    }

    pid = fork();
    if (pid == 0) {
        write_routine(sock, buf);
    } else {
        read_routine(sock, buf);
    }
    close(sock);
    return 0;
}

void read_routine(int sock, char* buf)
{
    while (1) {
        int str_len = read(sock, buf, BUFFERSIZE);
        if (str_len == 0) {
            return;
        }
        buf[str_len] = 0;
        cout << "message from server" << buf << endl;
    }
}

void write_routine(int sock, char* buf)
{
    while (1) {
        fgets(buf, BUFFERSIZE, stdin);
        if (!strcmp(buf, "q\n")) {
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, strlen(buf));
    }
}