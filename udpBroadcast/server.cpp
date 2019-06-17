#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


#define BUF_SIZE 3000000

using namespace std;

int main()
{
    int recv_sock;
    sockaddr_in adr;
    int str_len;
    char buf[BUF_SIZE];
    socklen_t client_adr_size;

    recv_sock = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&adr, sizeof(adr));
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_family = AF_INET;
    adr.sin_port = htons(8888);

    if (bind(recv_sock, (sockaddr*)&adr, sizeof(adr)) < 0) {
        cout << "bind error" << endl;
        exit(1);
    }
        
    while (1) {
        str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0);
        cout << str_len << endl;
        if (str_len < 0) {
            break;
        }
        // buf[str_len] = 0;
        fputs(buf, stdout);
        sleep(1);
        cout << strlen(buf) << endl;
        sleep(1);
    }
    close(recv_sock);
    return 0;
}