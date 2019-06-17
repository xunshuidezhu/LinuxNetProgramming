#include <iostream>
#include <strings.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <sys/types.h>

using namespace std;

#define IPADDRESS 127.0.0.1


int hostToIp(const char* ip)
{   
    int ipNew;
    ipNew = inet_aton(ip, NULL);
    return ipNew;
}
int main()
{
    int sockfd;
    int sockfd1;
    sockaddr_in sockAddr;
    sockAddr.sin_port = htons(8090);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_family = AF_INET;
    bind(sockfd, (sockaddr*)&sockAddr, sizeof(sockAddr));
    sockaddr_in sockAddr1;
    sockAddr1.sin_family = AF_INET;
    sockAddr1.sin_port = htons(8091);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockfd1 = socket(PF_INET,SOCK_DGRAM, 0);
    if (sockfd < 0)
        cout <<  " sockfd create error " << endl;
    if (sockfd1 < 0)
        cout << " sockfd1 create error " << endl;
    sockfd = bind(sockfd, (sockaddr*)&sockAddr, sizeof(sockAddr));
    union unptest1
    {
        /* data */
        char* c;
        int a;
    };
    unptest1 a;
    int ip = hostToIp("127.0.0.1");
    bzero(a.c, sizeof(char));
    
    cout << ip << endl;
    int res = ntohl(ip);
    cout <<  res << endl;
}