#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>


#define TTL 64
#define BUF_SIZE 3000000

using namespace std;

int main()
{
    int send_sock;
    sockaddr_in mul_adr;
    int time_live = TTL;
    FILE* fp;
    char buf[BUF_SIZE];
    for (int i = 0; i < 60000; i++) {
        buf[i] = 'a';
    }
    send_sock = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&mul_adr, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr("224.1.1.2");
    mul_adr.sin_port = htons(12345);

    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));
    //fp = fopen("news.txt", "r");
    int cnt = 0;
    while (1) {
        //fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (sockaddr*)&mul_adr, sizeof(mul_adr))
        ;
        cout << ++cnt << "round" << endl;
        cout << "send msg: " <<  strlen(buf) << "bytes" << endl;
        sleep(2);
    }
    fclose(fp);
    close(send_sock);
    return 0;
}