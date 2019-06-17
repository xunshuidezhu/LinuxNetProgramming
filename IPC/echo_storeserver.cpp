#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>


#define BUFSIZE 100
#define PORT 8091

using namespace std;

void handle_error(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(-1);
}
//signal callback func remove the child pid
void handle_child_proc(int sig)
{
    pid_t pid;
    int state;
    pid = waitpid(-1, &state, 0);
    cout << "state" << state << endl;
    cout << "remove proc id " << pid << endl;
}

int main()
{
    int serverSock, clientSock;
    sockaddr_in serverAddr, clientAddr;
    int fds[2];
    int state;
    int str_len;
    char buf[BUFSIZE];

    pid_t pid;
    socklen_t clientAddrSize;
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = handle_child_proc;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, 0);

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        cerr << "serverSock create error" << endl;
        exit(1);
    }

    int opt = 1;
    int optlen = sizeof(opt);
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, optlen);

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(12345);
    

    int err = bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (err < 0) {
        cerr << "bind error" << endl;
        exit(-1);
    }

    err = listen(serverSock, 5);
    if (err < 0) {
        cerr << "listen error" << endl;
        exit(-1);
    }

    pipe(fds);
    pid = fork();
    if (pid == 0) {
        //child proc store the msg from the pipe
        FILE* fp = fopen("./echomsg.txt", "wt");
        char message[BUFSIZE];
        while (1) {
            int len = read(fds[0], message, BUFSIZE);
            fwrite((void*)message, 1, len, fp);
        }
        fclose(fp);
        return 0;


    }

    while (1) {
        clientAddrSize = sizeof(clientAddr);
        clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSock < 0) {
            continue;
        } else {
            puts("new client connect");
        }
        pid_t pid;
        pid = fork();
        if (pid == 0) {
            close(serverSock);
            while ((str_len = read(clientSock, buf, BUFSIZE)) != 0) {
                write(clientSock, buf, str_len);
                write(fds[1], buf, str_len);
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
