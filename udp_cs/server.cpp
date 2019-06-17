#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// #define ERR_EXIT(m) \
//     {               \
//         perror(m);  \
//         exit(1);    \
//     }               \

void ERR_EXIT(char* msg) {
    perror(msg);
    exit(1);
}
static in_port_t port = 2525;
void str_echo(int sockfd, struct sockaddr_in cliaddr)
{
    printf("str_echo works\n");
    ssize_t n;
    socklen_t len = sizeof(cliaddr);
    char buf[1024] = "i am here";
    n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
    if (n == -1)
        ERR_EXIT("sendto");
    printf("send to client\n");
    memset(buf, 0, sizeof(buf));
    while (1) {
        n = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr*)&cliaddr, &len);
        sendto(sockfd, buf, 1024, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        memset(buf, 0, sizeof(buf));
    }
}

static void* doit(void*);

int main()
{
    int listenfd;
    pthread_t tid;
    socklen_t len;
    struct sockaddr_in cliaddr, servaddr, *tmp;
    //int on=1;
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (listenfd == -1)
        ERR_EXIT("socket");
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = 2525;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(int));
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    //	listen(listenfd,SOMAXCONN);
    int n;
    while (1) {
        char mesg[1024] = { 0 };
        len = sizeof(servaddr);
        tmp = (sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        //	*iptr=accept(listenfd,(struct sockaddr*)&cliaddr,&len);
        n = recvfrom(listenfd, mesg, 1024, 0, (struct sockaddr*)&cliaddr, &len);
        if (n == -1)
            ERR_EXIT("recvfrom");
        printf("get conn\n");
        printf("%s\n", mesg);
        //		fputs(mesg,stdout);
        *tmp = cliaddr;
        pthread_create(&tid, NULL, &doit, tmp);
        memset(mesg, 0, sizeof(mesg));
    }
}

static void* doit(void* arg)
{
    printf("a thread created\n");
    struct sockaddr_in cliaddr = *((struct sockaddr_in*)arg);
    free(arg);
    int connfd;
    connfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = ++port;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(connfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    pthread_detach(pthread_self());
    str_echo(connfd, cliaddr);
    close(connfd);
    return (NULL);
}
