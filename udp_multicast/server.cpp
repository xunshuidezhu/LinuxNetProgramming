#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define BUF_SIZE 3000000

int main()
{
    int recv_sock;
    int str_len;
    char buf[BUF_SIZE];
    sockaddr_in recv_addr;
    ip_mreq join_addr;

    recv_sock = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&recv_addr, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_addr.sin_port = htons(12345);
    int err = bind(recv_sock, (sockaddr*)&recv_addr, sizeof(recv_addr));
    if (err < 0) {
        cout << "bind error" << endl;
        exit(1);
    }
    join_addr.imr_multiaddr.s_addr = inet_addr("224.1.1.2");
    join_addr.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_addr, sizeof(join_addr));

    int cnt = 0;
    while(1) {
        
        str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0);
        if (str_len < 0) 
            break;
        buf[str_len] = 0;
        //fputs(buf, stdout);
        cout << ++cnt << "round" << endl;
        cout << "read msg: " << strlen(buf) << "bytes" << endl;
    }
    close(recv_sock);
    return 0;

}