#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
    hostent* host;
    host = gethostbyname(argv[1]);
    if (argc != 2) { 
        cout << "need the web name" << endl; 
    }
    if (!host) {
        cout << "get host www.baidu.com error" << endl;
        exit(-1);
    }
    for (int i = 0; host->h_aliases[i]; i++) {
        cout << "name : " << i << " -> " << host->h_aliases[i] << endl;
    }
    (host->h_addrtype == AF_INET) ? cout << "IPV4" << endl : cout << "IPV6" << endl;
    for (int i = 0; host->h_addr_list[i]; i++) {
        cout << inet_ntoa(*(in_addr*)host->h_addr_list[i]) << endl;
    }
    return 0;
}