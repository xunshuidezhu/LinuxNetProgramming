#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>
#define ERR_EXIT(m) \
    {               \
        perror(m);  \
        exit(1);    \
    }               \

void* copyto(void*);
static int sockfd;
static FILE* fp;
static struct sockaddr serv;
void str_cli(FILE* fp_arg, int sock_arg, struct sockaddr* servaddr, socklen_t servlen)
{
    char recvline[1024] = "hello";
    pthread_t tid;
    sockfd = sock_arg;
    fp = fp_arg;
    socklen_t len = sizeof(struct sockaddr_in);
    sendto(sockfd, recvline, strlen(recvline), 0, servaddr, sizeof(struct sockaddr));
    memset(recvline, 0, sizeof(recvline));
    recvfrom(sockfd, recvline, 1024, 0, &serv, &len);
    fputs(recvline, stdout);
    printf("\n");
    //serv=servaddr;
    pthread_create(&tid, NULL, copyto, &serv);
    memset(recvline, 0, sizeof(recvline));
    while (recvfrom(sockfd, recvline, 1024, 0, &serv, &len) > 0) {
        fputs(recvline, stdout);
        memset(recvline, 0, sizeof(recvline));
    }
}

void* copyto(void* arg)
{
    struct sockaddr* servaddr = (struct sockaddr*)arg;
    char sendline[1024] = { 0 };
    while (fgets(sendline, 1024, fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, servaddr, sizeof(*servaddr));
        memset(sendline, 0, sizeof(sendline));
    }
    //shutdown(sockfd,SHUT_WR);
    return (NULL);
}

int main()
{
    struct sockaddr_in servaddr;
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
        ERR_EXIT("socket");
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = 2525;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //      connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    str_cli(stdin, sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    close(sockfd);
    return 0;
}