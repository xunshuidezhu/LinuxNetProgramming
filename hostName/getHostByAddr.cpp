#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    hostent* host;
    sockaddr_in addr;
    if (argc != 2) {
        cout << "need the IP address" << endl;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    host = gethostbyaddr((char*)&addr.sin_addr, sizeof(addr.sin_addr), AF_INET);
    cout << "addr size : " << sizeof(addr.sin_addr) << endl;
    cout << "official name :" << " -> " <<host->h_name << endl;
    for (int i = 0; host->h_aliases[i]; i++) {
        cout << "name :" << i << " -> " << host->h_aliases[i] << endl;
    }
    (host->h_addrtype == AF_INET) ? cout << "IPV4" << endl : cout << "IPV6" << endl;
    for (int i = 0; host->h_addr_list[i]; i++) {
        cout << inet_ntoa(*(in_addr*)host->h_addr_list[i]) << endl;
    }
    return 0;

}