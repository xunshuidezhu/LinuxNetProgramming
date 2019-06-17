#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <glog/logging.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFERSIZE 4
#define EPOLLSIZE 50
#define PORT 12345
#define IPADDRESS "127.0.0.1"

void handle_error(char* msg);
int main()
{
    int serverSock, clientSock;
    sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize;
    char msg[BUFFERSIZE];
    int str_len, i;

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        handle_error("serverSock create error");
    }
    epoll_event* ep_events;
    epoll_event event;
    int epfd, event_cnt;
    //serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    int err = bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (err < 0) {
        handle_error("bind error");
    }

    err = listen(serverSock, 5);
    if (err < 0) {
        handle_error("listen error");
    }
    //return epollfd
    epfd = epoll_create(EPOLLSIZE);
    epfd = epoll_create(EPOLLSIZE);
    ep_events = (epoll_event*)malloc((sizeof(struct epoll_event) * EPOLLSIZE));

    event.events = EPOLLIN;
    event.data.fd = serverSock;
    //correct return 0, else -1
    epoll_ctl(epfd, EPOLL_CTL_ADD, serverSock, &event);
    while (1) {
        //return epoll wait nums, EPOLLSIZE:the max nums of save the epollfd
        //ep_events:the addr of the structs which save the epollfd
        //-1 : the time to wait, -1 is wait always
        event_cnt = epoll_wait(epfd, ep_events, EPOLLSIZE, -1);
        if (event_cnt == -1) {
            puts("epoll_wait() error");
            break;
        }

        puts("return epoll_wait");
        for (i = 0; i < event_cnt; i++) {
            if (ep_events[i].data.fd == serverSock) {
                clientAddrSize = sizeof(clientAddr);
                clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrSize);
                //set noblocking
                int flag = fcntl(clientSock, F_GETFL, 0);
                fcntl(clientSock, F_SETFL, flag | O_NONBLOCK);
                //set ET
                event.events = EPOLLIN|EPOLLET;
                event.data.fd = clientSock;
                //epoll control
                epoll_ctl(epfd, EPOLL_CTL_ADD, clientSock, &event);
                printf("connect client : %d \n", clientSock);
            } else {
                while (1) {
                    str_len = read(ep_events[i].data.fd, msg, BUFFERSIZE);
                    if (str_len == 0) {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("closed client: %d", ep_events[i].data.fd);
                        break;
                    } else if (str_len < 0) {
                        if (errno == EAGAIN) {
                            break;
                        }
                    }
                    else {
                        write(ep_events[i].data.fd, msg, str_len);
                    }
                }
            }
        }
    }
    close(serverSock);
    close(epfd);
    return 0;
}
void handle_error(char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}
