#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>

using namespace std;

int main()
{
    int sock;
    int snd_buf, rcv_buf, state;
    socklen_t len;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if (state < 0) {
        cout << "getsockopt error" << endl;
        exit(-1);
    }

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if (state < 0) {
        cout << "getsockopt error" << endl;
        exit(-1);
    }

    cout << "sendbuf : " << snd_buf << endl;
    cout << "recvbuf : " << rcv_buf << endl;
    
    return 0;
}