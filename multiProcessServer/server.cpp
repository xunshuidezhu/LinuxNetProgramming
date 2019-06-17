#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <boost/utility.hpp>
#include <aio.h>


#define BUFFERSIZE 30
#define PORT 12345

using namespace std;
//read the child process
void read_childproc(int sig);
int main()
{
    int serverSock, clientSock;
    sockaddr_in serverAddr, clientAddr;

    pid_t pid;
    struct sigaction act;
    socklen_t adrSize;
    int str_len, state;
    char buf[BUFFERSIZE];
    int opt = 1;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    int opt_len = sizeof(opt);
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, opt_len);

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_family = AF_INET;

    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cout << "serverSock bind error" << endl;
        exit(-1);
    }
    if (listen(serverSock, 5) < 0) {
        cout << "serverSock listen error" << endl;
        exit(-1);
    }

    while (1) {
        adrSize = sizeof(clientAddr);
        clientSock = accept(serverSock, (sockaddr*)&clientAddr, &adrSize);
        if (clientSock == -1) {
            continue;
        } else {
            puts("new client connected...");
        }
        pid = fork();
        if (pid == -1) {
            close(clientSock);
            continue;
        }
        if (pid == 0) {
            close(serverSock);
            while ((str_len = read(clientSock, buf, BUFFERSIZE)) != 0) {
                write(clientSock, buf, str_len);
            }
            close(clientSock);
            puts("client disconnected");
            return 0;
        } else {
            close(clientSock);
        }
    }

    close(serverSock);
    return 0;
}

void read_childproc(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    cout << "remove proc id : " << pid << endl;
}  