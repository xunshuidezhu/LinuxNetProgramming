#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 3000000

using namespace std;

int main()
{
    int send_sock;
    sockaddr_in broad_adr;
    FILE* fp;
    char buf[BUF_SIZE];
    int so_brd = 1;

    send_sock = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&broad_adr, sizeof(broad_adr));
    broad_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_port = htons(8888);

    //setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)so_brd, sizeof(so_brd));
    cout << strlen(buf) << endl;
    for (int i = 0; i < 3000000; ++i) {
        buf[i] = 'a';
    }
    cout << strlen(buf) << endl;
    while (1) 
    {
        sendto(send_sock, buf, strlen(buf), 0, (sockaddr*)&broad_adr, sizeof(broad_adr));
        sleep(1);
    }
    close(send_sock);
    return 0;
}