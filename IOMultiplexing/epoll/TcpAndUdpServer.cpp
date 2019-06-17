#include <sys/socket.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#define MAX_EVENT_NUMBER 1024
#define TCP_BUFFER_SIZE 512
#define UDP_BUFFER_SIZE 1024

int set_non_blocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void add_fd(int epollfd, int fd)
{
    epoll_event event;
    event.data.fd = epollfd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    set_non_blocking(fd);
}

int main(int argc, char* argv[])
{
    assert(argc >= 2);
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int ret = 0;
    sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_addr.sin_addr);
    server_addr.sin_port = htons(port);

    //TCP socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    ret = bind(listenfd, (sockaddr*)&server_addr, sizeof(server_addr));
    assert(ret >= 0);

    ret = listen(listenfd, 5);
    assert(ret >= 0);

    //UDP socket
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_addr.sin_addr);
    server_addr.sin_port = htons(port);
    int udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(udp_fd >= 0);

    ret = bind(udp_fd, (sockaddr*)&server_addr, sizeof(server_addr));
    assert(ret >= 0);

    struct epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    add_fd(epollfd, listenfd);
    add_fd(epollfd, udp_fd);

    while (1) {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if (number < 0) {
            printf("epoll failure \n");
            break;
        }

        for (int i = 0; i < number; ++i) {
            int sock_fd = events[i].data.fd;
            if (sock_fd == listenfd) {
                sockaddr_in client_addr;
                socklen_t client_addrsize = sizeof(client_addr);
                int connect_fd = accept(listenfd, (sockaddr*)(&client_addr), &client_addrsize);
                add_fd(epollfd, connect_fd);
            }
            else if (sock_fd == udp_fd) {
                char buf[UDP_BUFFER_SIZE];
                bzero(buf, UDP_BUFFER_SIZE);
                sockaddr_in client_addr;
                socklen_t client_addr_size = sizeof(client_addr);
                ret = recvfrom(udp_fd, buf, UDP_BUFFER_SIZE - 1, 0, (sockaddr*)&client_addr, &client_addr_size);
                if (ret > 0) {
                    sendto(udp_fd, buf, UDP_BUFFER_SIZE - 1, 0, (sockaddr*)&client_addr, client_addr_size);
                }
            }
            else if (events[i].events & EPOLLIN) {
                char buf[TCP_BUFFER_SIZE];
                while (1) {
                    bzero(buf, TCP_BUFFER_SIZE);
                    ret = recv(sock_fd, buf, TCP_BUFFER_SIZE - 1, 0);
                    if (ret < 0) {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
                        {
                            break;
                        }
                        close(sock_fd);
                        break;
                    }
                    else if (ret == 0) {
                        close(sock_fd);
                    }
                    else {
                        send(sock_fd, buf, ret, 0);
                    }
                }
            }
            else {
                printf("something else happened \n");
            }
        }
        close(listenfd);
        return 0;
    }





}